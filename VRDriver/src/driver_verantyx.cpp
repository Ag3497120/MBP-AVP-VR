#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <openvr_driver.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void LogDriver(const char* msg) {
    FILE* f = fopen("Z:\\Users\\motonishikoudai\\Verantyx_VR_Drive\\verantyx_driver.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

#pragma pack(push, 1)
struct SharedHands {
    double poseTimestamp;
    double renderedTimestamp;
    float headTransform[16];
    float leftTransform[16];
    float rightTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
    uint32_t rightButtons;
    uint32_t leftButtons;
    float rightStickX;
    float rightStickY;
    float leftStickX;
    float leftStickY;
    float rightVelocity[3];
    float leftVelocity[3];
};
#pragma pack(pop)

static HANDLE hMapFile = NULL;
static SharedHands* pSharedHands = NULL;

static void InitSharedMemoryDriver() {
    if (hMapFile) return;
    
    const int mapSize = 16 + 4096 * 4096 * 4 + 194;
    
    hMapFile = OpenFileMappingA(FILE_MAP_READ, FALSE, "VerantyxVRSharedHands");
    if (!hMapFile) {
        HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, mapSize, "VerantyxVRSharedHands");
            CloseHandle(hFile);
        }
    }
    
    if (hMapFile) {
        void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, mapSize);
        if (pBuf) {
            pSharedHands = (SharedHands*)((uint8_t*)pBuf + 16 + 4096 * 4096 * 4);
        }
    }
}

static vr::HmdQuaternion_t ExtractQuaternion(const float* m) {
    vr::HmdQuaternion_t q;
    float t = m[0] + m[5] + m[10];
    if (t > 0.0f) {
        float S = sqrtf(1.0f + t) * 2.0f;
        q.w = 0.25f * S;
        q.x = (m[6] - m[9]) / S;
        q.y = (m[8] - m[2]) / S;
        q.z = (m[1] - m[4]) / S;
    } else if ((m[0] > m[5]) && (m[0] > m[10])) {
        float S = sqrtf(1.0f + m[0] - m[5] - m[10]) * 2.0f;
        q.w = (m[6] - m[9]) / S;
        q.x = 0.25f * S;
        q.y = (m[1] + m[4]) / S;
        q.z = (m[8] + m[2]) / S;
    } else if (m[5] > m[10]) {
        float S = sqrtf(1.0f + m[5] - m[0] - m[10]) * 2.0f;
        q.w = (m[8] - m[2]) / S;
        q.x = (m[1] + m[4]) / S;
        q.y = 0.25f * S;
        q.z = (m[6] + m[9]) / S;
    } else {
        float S = sqrtf(1.0f + m[10] - m[0] - m[5]) * 2.0f;
        q.w = (m[1] - m[4]) / S;
        q.x = (m[8] + m[2]) / S;
        q.y = (m[6] + m[9]) / S;
        q.z = 0.25f * S;
    }
    return q;
}

class CVerantyxDirectMode : public vr::IVRDriverDirectModeComponent {
public:
    virtual void CreateSwapTextureSet(uint32_t unPid, const vr::IVRDriverDirectModeComponent::SwapTextureSetDesc_t *pSwapTextureSetDesc, vr::IVRDriverDirectModeComponent::SwapTextureSet_t *pOutSwapTextureSet) override {}
    virtual void DestroyAllSwapTextureSets(uint32_t unPid) override {}
    virtual void GetNextSwapTextureSetIndex(vr::SharedTextureHandle_t sharedTextureHandles[2], uint32_t (*pIndices)[2]) override {}
    virtual void SubmitLayer(const vr::IVRDriverDirectModeComponent::SubmitLayerPerEye_t (&perEye)[2]) override {}
    virtual void Present(vr::SharedTextureHandle_t syncTexture) override {}
    virtual void PostPresent(const vr::IVRDriverDirectModeComponent::Throttling_t *pThrottling) override {}
    virtual void GetFrameTiming(vr::DriverDirectMode_FrameTiming *pFrameTiming) override {}
};

#include <chrono>
#include <thread>

class CVerantyxVirtualDisplay : public vr::IVRVirtualDisplay {
public:
    virtual void Present(const vr::PresentInfo_t *pPresentInfo, uint32_t unPresentInfoSize) override {}
    
    virtual void WaitForPresent() override {
        std::this_thread::sleep_for(std::chrono::milliseconds(11));
    }
    
    virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override {
        static auto startTime = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - startTime).count();
        double frameDuration = 1.0 / 90.0;
        *pulFrameCounter = (uint64_t)(elapsed / frameDuration);
        *pfSecondsSinceLastVsync = (float)(elapsed - (*pulFrameCounter * frameDuration));
        return true;
    }
};

class CVerantyxDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent {
public:
    CVerantyxDirectMode m_directMode;
    CVerantyxVirtualDisplay m_virtualDisplay;

    vr::TrackedDeviceIndex_t m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
    vr::PropertyContainerHandle_t m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;
    vr::VRInputComponentHandle_t m_proximityComponent = 0;

    virtual vr::EVRInitError Activate(uint32_t unObjectId) override {
        LogDriver("[Verantyx] CVerantyxDeviceDriver Activated!");
        m_unObjectId = unObjectId;
        m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);
        
        vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ManufacturerName_String, "Verantyx");
        vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ModelNumber_String, "VerantyxHMD");
        vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, vr::Prop_HasVirtualDisplayComponent_Bool, true);
        vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_CurrentUniverseId_Uint64, 1);
        vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, vr::Prop_ContainsProximitySensor_Bool, false);
        vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, vr::Prop_EdidVendorID_Int32, 0);
        vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, vr::Prop_EdidProductID_Int32, 0);
        
        // Fetch default DXGI Adapter LUID
        HMODULE hDXGI = LoadLibraryA("dxgi.dll");
        if (hDXGI) {
            typedef HRESULT(WINAPI* CreateDXGIFactory_t)(REFIID, void**);
            CreateDXGIFactory_t pCreateDXGIFactory = (CreateDXGIFactory_t)GetProcAddress(hDXGI, "CreateDXGIFactory");
            if (pCreateDXGIFactory) {
                IDXGIFactory* pFactory = nullptr;
                if (SUCCEEDED(pCreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
                    IDXGIAdapter* pAdapter = nullptr;
                    if (SUCCEEDED(pFactory->EnumAdapters(0, &pAdapter))) {
                        DXGI_ADAPTER_DESC desc;
                        if (SUCCEEDED(pAdapter->GetDesc(&desc))) {
                            uint64_t luid = ((uint64_t)desc.AdapterLuid.HighPart << 32) | desc.AdapterLuid.LowPart;
                            // DO NOT SET LUID! 
                            // Setting a D3DMetal LUID will cause DXVK in vrcompositor to crash because it can't find the adapter.
                            // vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_GraphicsAdapterLuid_Uint64, luid);
                        }
                        pAdapter->Release();
                    }
                    pFactory->Release();
                }
            }
        }
        
        vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, vr::Prop_DeviceProvidesBatteryStatus_Bool, false);
        vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_UserIpdMeters_Float, 0.063f);
        vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_UserHeadToEyeDepthMeters_Float, 0.f);
        vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_DisplayFrequency_Float, 90.0f);
        vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_SecondsFromVsyncToPhotons_Float, 0.011f);
        
        /*
        IDXGIFactory* pFactory = nullptr;
        if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
            IDXGIAdapter* pAdapter = nullptr;
            if (SUCCEEDED(pFactory->EnumAdapters(0, &pAdapter))) {
                DXGI_ADAPTER_DESC desc;
                if (SUCCEEDED(pAdapter->GetDesc(&desc))) {
                    uint64_t luid = 0;
                    memcpy(&luid, &desc.AdapterLuid, sizeof(luid));
                    vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_GraphicsAdapterLuid_Uint64, luid);
                    LogDriver("[Verantyx] Adapter LUID set!");
                }
                pAdapter->Release();
            }
            pFactory->Release();
        } else {
            LogDriver("[Verantyx] CreateDXGIFactory failed!");
        }
        */

        vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/proximity", &m_proximityComponent);
        return vr::VRInitError_None;
    }
    virtual void Deactivate() override {}
    virtual void EnterStandby() override {}
    virtual void* GetComponent(const char* pchComponentNameAndVersion) override {
        if (!strcmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)) {
            return (vr::IVRDisplayComponent*)this;
        }
        if (!strcmp(pchComponentNameAndVersion, vr::IVRVirtualDisplay_Version)) {
            return &m_virtualDisplay;
        }
        return nullptr;
    }
    virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override {}
#if defined(__GNUC__) && !defined(__clang__)
    virtual vr::DriverPose_t* GetPose(vr::DriverPose_t* ret) override {
#else
    virtual vr::DriverPose_t GetPose() override {
#endif
        vr::DriverPose_t pose = { 0 };
        pose.poseIsValid = true;
        pose.result = vr::TrackingResult_Running_OK;
        pose.deviceIsConnected = true;
        pose.qWorldFromDriverRotation = { 1, 0, 0, 0 };
        pose.qDriverFromHeadRotation = { 1, 0, 0, 0 };
        pose.qRotation = { 1, 0, 0, 0 };
        InitSharedMemoryDriver();
        if (pSharedHands && pSharedHands->headTransform[0] != 0.0f) {
            pose.qRotation = ExtractQuaternion(pSharedHands->headTransform);
            pose.vecPosition[0] = pSharedHands->headTransform[12];
            pose.vecPosition[1] = pSharedHands->headTransform[13];
            pose.vecPosition[2] = pSharedHands->headTransform[14];
        }

#if defined(__GNUC__) && !defined(__clang__)
        if (ret) *ret = pose;
        return ret;
    }
#else
        return pose;
    }
#endif
    virtual void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override {
        *pnX = 0; *pnY = 0; *pnWidth = 3840; *pnHeight = 2160;
    }
    virtual bool IsDisplayOnDesktop() override { return false; }
    virtual bool IsDisplayRealDisplay() override { return false; }
    virtual void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) override {
        *pnWidth = 1920; *pnHeight = 2160;
    }
    virtual void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override {
        *pnY = 0; *pnWidth = 1920; *pnHeight = 2160;
        if (eEye == vr::Eye_Left) *pnX = 0; else *pnX = 1920;
    }
    virtual void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) override {
        *pfLeft = -1.0f; *pfRight = 1.0f; *pfTop = -1.0f; *pfBottom = 1.0f;
    }
    virtual vr::DistortionCoordinates_t* ComputeDistortion(vr::DistortionCoordinates_t* pRet, vr::EVREye eEye, float fU, float fV) override {
        pRet->rfBlue[0] = fU; pRet->rfBlue[1] = fV;
        pRet->rfGreen[0] = fU; pRet->rfGreen[1] = fV;
        pRet->rfRed[0] = fU; pRet->rfRed[1] = fV;
        return pRet;
    }
    virtual bool ComputeInverseDistortion( vr::HmdVector2_t *pResult, vr::EVREye eEye, uint32_t unChannel, float fU, float fV ) override {
        return false;
    }
};

class CVerantyxControllerDriver : public vr::ITrackedDeviceServerDriver {
public:
    vr::TrackedDeviceIndex_t m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
    vr::PropertyContainerHandle_t m_ulPropertyContainer = vr::k_ulInvalidPropertyContainer;
    bool m_bIsLeft = false;
    
    vr::VRInputComponentHandle_t m_compTrigger = 0;
    vr::VRInputComponentHandle_t m_compA = 0;

    CVerantyxControllerDriver(bool isLeft) : m_bIsLeft(isLeft) {}

    virtual vr::EVRInitError Activate(uint32_t unObjectId) override {
        m_unObjectId = unObjectId;
        m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);

        vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ModelNumber_String, m_bIsLeft ? "Oculus Rift S (Left Controller)" : "Oculus Rift S (Right Controller)");
        vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_RenderModelName_String, m_bIsLeft ? "oculus_rifts_controller_left" : "oculus_rifts_controller_right");
        
        // This is strictly required for SteamVR to process inputs!
        vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_InputProfilePath_String, "{oculus}/input/touch_profile.json");
        
        vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, vr::Prop_ControllerRoleHint_Int32, m_bIsLeft ? vr::TrackedControllerRole_LeftHand : vr::TrackedControllerRole_RightHand);
        vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, vr::Prop_DeviceClass_Int32, vr::TrackedDeviceClass_Controller);
        
        // SteamVR Input requires at least a basic input profile to be valid for games like Alyx
        vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/trigger/click", &m_compTrigger);
        vr::VRDriverInput()->CreateBooleanComponent(m_ulPropertyContainer, "/input/a/click", &m_compA);
        
        return vr::VRInitError_None;
    }
    virtual void Deactivate() override {}
    virtual void EnterStandby() override {}
    virtual void* GetComponent(const char* pchComponentNameAndVersion) override { return nullptr; }
    virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override {}
#if defined(__GNUC__) && !defined(__clang__)
    virtual vr::DriverPose_t* GetPose(vr::DriverPose_t* ret) override {
#else
    virtual vr::DriverPose_t GetPose() override {
#endif
        vr::DriverPose_t pose = { 0 };
        pose.poseIsValid = true;
        pose.result = vr::TrackingResult_Running_OK;
        pose.deviceIsConnected = true;
        pose.qWorldFromDriverRotation = { 1, 0, 0, 0 };
        pose.qDriverFromHeadRotation = { 1, 0, 0, 0 };
        pose.qRotation = { 1, 0, 0, 0 };
        
        pose.vecPosition[0] = m_bIsLeft ? -0.2 : 0.2;
        pose.vecPosition[1] = -0.2;
        pose.vecPosition[2] = -0.5;

        InitSharedMemoryDriver();
        float* srcTransform = m_bIsLeft ? (pSharedHands ? pSharedHands->leftTransform : nullptr) : (pSharedHands ? pSharedHands->rightTransform : nullptr);
        if (srcTransform && srcTransform[0] != 0.0f) {
            pose.qRotation = ExtractQuaternion(srcTransform);
            pose.vecPosition[0] = srcTransform[12];
            pose.vecPosition[1] = srcTransform[13];
            pose.vecPosition[2] = srcTransform[14];
        } else if (pSharedHands && pSharedHands->headTransform[0] != 0.0f) {
            // Fallback to head tracking
            pose.qRotation = ExtractQuaternion(pSharedHands->headTransform);
            pose.vecPosition[0] = pSharedHands->headTransform[12] + (m_bIsLeft ? -0.2f : 0.2f);
            pose.vecPosition[1] = pSharedHands->headTransform[13] - 0.2f;
            pose.vecPosition[2] = pSharedHands->headTransform[14] - 0.3f;
        }

#if defined(__GNUC__) && !defined(__clang__)
        if (ret) *ret = pose;
        return ret;
    }
#else
        return pose;
    }
#endif
};

class CVerantyxServerProvider : public vr::IServerTrackedDeviceProvider {
public:
    CVerantyxDeviceDriver* m_pHmd = nullptr;
    CVerantyxControllerDriver* m_pControllerLeft = nullptr;
    CVerantyxControllerDriver* m_pControllerRight = nullptr;

    virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override { 
        VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
        LogDriver("[Verantyx] ServerProvider Init called!");
        
        m_pHmd = new CVerantyxDeviceDriver();
        vr::VRServerDriverHost()->TrackedDeviceAdded("verantyx_hmd", vr::TrackedDeviceClass_HMD, m_pHmd);

        m_pControllerLeft = new CVerantyxControllerDriver(true);
        vr::VRServerDriverHost()->TrackedDeviceAdded("verantyx_ctrl_l", vr::TrackedDeviceClass_Controller, m_pControllerLeft);
        
        m_pControllerRight = new CVerantyxControllerDriver(false);
        vr::VRServerDriverHost()->TrackedDeviceAdded("verantyx_ctrl_r", vr::TrackedDeviceClass_Controller, m_pControllerRight);

        return vr::VRInitError_None; 
    }
    virtual void Cleanup() override {
        if (m_pHmd) { delete m_pHmd; m_pHmd = nullptr; }
        if (m_pControllerLeft) { delete m_pControllerLeft; m_pControllerLeft = nullptr; }
        if (m_pControllerRight) { delete m_pControllerRight; m_pControllerRight = nullptr; }
    }
    virtual const char* const* GetInterfaceVersions() override { return vr::k_InterfaceVersions; }
    virtual void RunFrame() override {
        if (m_pHmd && m_pHmd->m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
            vr::DriverPose_t pose;
#if defined(__GNUC__) && !defined(__clang__)
            m_pHmd->GetPose(&pose);
#else
            pose = m_pHmd->GetPose();
#endif
            vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_pHmd->m_unObjectId, pose, sizeof(vr::DriverPose_t));
        }
        
        if (m_pControllerLeft && m_pControllerLeft->m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
            vr::DriverPose_t pose;
#if defined(__GNUC__) && !defined(__clang__)
            m_pControllerLeft->GetPose(&pose);
#else
            pose = m_pControllerLeft->GetPose();
#endif
            vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_pControllerLeft->m_unObjectId, pose, sizeof(vr::DriverPose_t));
        }

        if (m_pControllerRight && m_pControllerRight->m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
            vr::DriverPose_t pose;
#if defined(__GNUC__) && !defined(__clang__)
            m_pControllerRight->GetPose(&pose);
#else
            pose = m_pControllerRight->GetPose();
#endif
            vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_pControllerRight->m_unObjectId, pose, sizeof(vr::DriverPose_t));
        }
    }
    virtual bool ShouldBlockStandbyMode() override { return true; }
    virtual void EnterStandby() override {}
    virtual void LeaveStandby() override {}
};

CVerantyxServerProvider g_provider;
extern "C" __declspec(dllexport) void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
    LogDriver(pInterfaceName);
    if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName)) {
        return &g_provider;
    }
    if (pReturnCode)
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
    return nullptr;
}
