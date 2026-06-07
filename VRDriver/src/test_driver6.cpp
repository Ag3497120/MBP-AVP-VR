#include <windows.h>
#include <d3d11.h>
#include <openvr_driver.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


using namespace vr;

void LogDriver(const char* msg) {
    FILE* f = fopen("C:\\verantyx_driver.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
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

class CVerantyxVirtualDisplay : public vr::IVRVirtualDisplay {
    void* m_pSharedMap = nullptr;
    uint32_t m_seq = 0;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pContext = nullptr;
    ID3D11Texture2D* m_pStaging = nullptr;
public:
    virtual void Present(const vr::PresentInfo_t *pPresentInfo, uint32_t unPresentInfoSize) override {}
    virtual void WaitForPresent() override {}
    virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override { return true; }
};

class CVerantyxDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent {
    CVerantyxDirectMode m_directMode;
    CVerantyxVirtualDisplay m_virtualDisplay;
    vr::VRInputComponentHandle_t m_proximityComponent = 0;
public:
    vr::TrackedDeviceIndex_t m_unObjectId;
    vr::PropertyContainerHandle_t m_ulPropertyContainer;

    CVerantyxDeviceDriver() {}
    virtual ~CVerantyxDeviceDriver() {}
    virtual EVRInitError Activate(uint32_t unObjectId) override { return vr::VRInitError_None; }
    virtual void Deactivate() override {}
    virtual void EnterStandby() override {}
    virtual void* GetComponent(const char* pchComponentNameAndVersion) override { return nullptr; }
    virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override {}
    virtual vr::DriverPose_t GetPose() override { vr::DriverPose_t pose={0}; return pose; }
    virtual void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override {}
    virtual bool IsDisplayOnDesktop() override { return false; }
    virtual bool IsDisplayRealDisplay() override { return false; }
    virtual void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) override {}
    virtual void GetEyeOutputViewport(EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override {}
    virtual void GetProjectionRaw(EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) override {}
    virtual DistortionCoordinates_t* ComputeDistortion(DistortionCoordinates_t* pRet, EVREye eEye, float fU, float fV) override { return pRet; }
    virtual bool ComputeInverseDistortion( vr::HmdVector2_t *pResult, vr::EVREye eEye, uint32_t unChannel, float fU, float fV ) override { return false; }
};

class CVerantyxServerProvider : public vr::IServerTrackedDeviceProvider {
public:
    virtual EVRInitError Init(vr::IVRDriverContext* pDriverContext) override { return vr::VRInitError_None; }
    virtual void Cleanup() override {}
    virtual const char* const* GetInterfaceVersions() override { return vr::k_InterfaceVersions; }
    virtual void RunFrame() override {}
    virtual bool ShouldBlockStandbyMode() override { return true; }
    virtual void EnterStandby() override {}
    virtual void LeaveStandby() override {}
    CVerantyxDeviceDriver* m_pHmd = nullptr;
};

CVerantyxServerProvider g_serverDriverNull;

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec( dllexport )
#else
#define HMD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
#endif

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
    if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName)) {
        return &g_serverDriverNull;
    }
    if (pReturnCode)
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
    return nullptr;
}
