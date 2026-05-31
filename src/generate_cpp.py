with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write("""#include "fake_openvr.h"
#include <windows.h>
#include <string.h>
#include <d3d11.h>
#include <math.h>

using namespace vr;

// Helper to create a basic HmdMatrix34_t (identity)
HmdMatrix34_t Identity34() {
    HmdMatrix34_t m;
    memset(&m, 0, sizeof(m));
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f;
    return m;
}

// Helper to create a basic HmdMatrix44_t (projection)
HmdMatrix44_t CreateProjection(float fov, float aspect, float nearZ, float farZ) {
    HmdMatrix44_t m;
    memset(&m, 0, sizeof(m));
    float f = 1.0f / tanf(fov * 3.1415926535f / 360.0f);
    m.m[0][0] = f / aspect;
    m.m[1][1] = f;
    m.m[2][2] = farZ / (nearZ - farZ);
    m.m[2][3] = (nearZ * farZ) / (nearZ - farZ);
    m.m[3][2] = -1.0f;
    return m;
}

class MyFakeVRSystem : public FakeIVRSystem {
public:
    virtual void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) override {
        *pnWidth = 1920;
        *pnHeight = 1080;
    }

    virtual HmdMatrix44_t GetProjectionMatrix( EVREye eEye, float fNearZ, float fFarZ ) override {
        return CreateProjection(90.0f, 1920.0f/1080.0f, fNearZ, fFarZ);
    }

    virtual HmdMatrix34_t GetEyeToHeadTransform( EVREye eEye ) override {
        HmdMatrix34_t m = Identity34();
        // Slight offset for left/right eye
        if (eEye == Eye_Left) m.m[0][3] = -0.03f;
        else m.m[0][3] = 0.03f;
        return m;
    }

    virtual void GetDeviceToAbsoluteTrackingPose( ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount ) override {
        for(uint32_t i=0; i<unTrackedDevicePoseArrayCount; ++i) {
            pTrackedDevicePoseArray[i].bPoseIsValid = true;
            pTrackedDevicePoseArray[i].bDeviceIsConnected = true;
            pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking = Identity34();
        }
    }
};

class MyFakeVRCompositor : public FakeIVRCompositor {
public:
    virtual void SetTrackingSpace( ETrackingUniverseOrigin eOrigin ) override {}
    
    virtual EVRCompositorError WaitGetPoses( VR_ARRAY_COUNT(unRenderPoseArrayCount) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT(unGamePoseArrayCount) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount ) override {
        if(pRenderPoseArray && unRenderPoseArrayCount > 0) {
            for(uint32_t i=0; i<unRenderPoseArrayCount; ++i) {
                pRenderPoseArray[i].bPoseIsValid = true;
                pRenderPoseArray[i].bDeviceIsConnected = true;
                pRenderPoseArray[i].mDeviceToAbsoluteTracking = Identity34();
            }
        }
        if(pGamePoseArray && unGamePoseArrayCount > 0) {
            for(uint32_t i=0; i<unGamePoseArrayCount; ++i) {
                pGamePoseArray[i].bPoseIsValid = true;
                pGamePoseArray[i].bDeviceIsConnected = true;
                pGamePoseArray[i].mDeviceToAbsoluteTracking = Identity34();
            }
        }
        return VRCompositorError_None;
    }

    virtual EVRCompositorError Submit( EVREye eEye, const Texture_t *pTexture, const VRTextureBounds_t* pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default ) override {
        if (!pTexture || pTexture->eType != TextureType_DirectX) return VRCompositorError_None;
        
        ID3D11Texture2D* pTex = (ID3D11Texture2D*)pTexture->handle;
        if (!pTex) return VRCompositorError_None;

        static int frameCount = 0;
        if (eEye == Eye_Left && frameCount % 60 == 0) {
            FILE* f = fopen("C:\\\\vr_emulator_frames.txt", "a");
            if (f) {
                fprintf(f, "Received Left Eye Frame %d! Texture ptr: %p\\n", frameCount, pTex);
                fclose(f);
            }
        }
        if (eEye == Eye_Left) frameCount++;

        return VRCompositorError_None;
    }
};

class MyFakeVRSettings : public FakeIVRSettings {
public:
    virtual void GetString( const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError = 0L ) override {
        if (peError) *peError = VRSettingsError_None;
        if (pchValue && unValueLen > 0) pchValue[0] = '\\0';
    }
    virtual int32_t GetInt32( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = 0L ) override {
        if (peError) *peError = VRSettingsError_None;
        return 0;
    }
    virtual float GetFloat( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = 0L ) override {
        if (peError) *peError = VRSettingsError_None;
        return 0.0f;
    }
    virtual bool GetBool( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = 0L ) override {
        if (peError) *peError = VRSettingsError_None;
        return false;
    }
};

MyFakeVRSystem g_VRSystem;
MyFakeVRCompositor g_VRCompositor;
MyFakeVRSettings g_VRSettings;

class MyFakeIVRApplications : public FakeIVRApplications {};
MyFakeIVRApplications g_IVRApplications;
class MyFakeIVRChaperone : public FakeIVRChaperone {};
MyFakeIVRChaperone g_IVRChaperone;
class MyFakeIVRChaperoneSetup : public FakeIVRChaperoneSetup {};
MyFakeIVRChaperoneSetup g_IVRChaperoneSetup;
class MyFakeIVRHeadsetView : public FakeIVRHeadsetView {};
MyFakeIVRHeadsetView g_IVRHeadsetView;
class MyFakeIVRNotifications : public FakeIVRNotifications {};
MyFakeIVRNotifications g_IVRNotifications;
class MyFakeIVROverlay : public FakeIVROverlay {};
MyFakeIVROverlay g_IVROverlay;
class MyFakeIVROverlayView : public FakeIVROverlayView {};
MyFakeIVROverlayView g_IVROverlayView;
class MyFakeIVRRenderModels : public FakeIVRRenderModels {};
MyFakeIVRRenderModels g_IVRRenderModels;
class MyFakeIVRExtendedDisplay : public FakeIVRExtendedDisplay {};
MyFakeIVRExtendedDisplay g_IVRExtendedDisplay;
class MyFakeIVRTrackedCamera : public FakeIVRTrackedCamera {};
MyFakeIVRTrackedCamera g_IVRTrackedCamera;
class MyFakeIVRScreenshots : public FakeIVRScreenshots {};
MyFakeIVRScreenshots g_IVRScreenshots;
class MyFakeIVRResources : public FakeIVRResources {};
MyFakeIVRResources g_IVRResources;
class MyFakeIVRDriverManager : public FakeIVRDriverManager {};
MyFakeIVRDriverManager g_IVRDriverManager;
class MyFakeIVRInput : public FakeIVRInput {};
MyFakeIVRInput g_IVRInput;
class MyFakeIVRIOBuffer : public FakeIVRIOBuffer {};
MyFakeIVRIOBuffer g_IVRIOBuffer;
class MyFakeIVRSpatialAnchors : public FakeIVRSpatialAnchors {};
MyFakeIVRSpatialAnchors g_IVRSpatialAnchors;
class MyFakeIVRDebug : public FakeIVRDebug {};
MyFakeIVRDebug g_IVRDebug;
class MyFakeIVRIPCResourceManagerClient : public FakeIVRIPCResourceManagerClient {};
MyFakeIVRIPCResourceManagerClient g_IVRIPCResourceManagerClient;

extern "C" {

__declspec(dllexport) uint32_t VR_InitInternal(EVRInitError *peError, EVRApplicationType eType) {
    if (peError) *peError = VRInitError_None;
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_InitInternal called!\\n"); fclose(f); }
    return 1;
}

__declspec(dllexport) uint32_t VR_InitInternal2(EVRInitError *peError, EVRApplicationType eType, const char* pStartupInfo) {
    if (peError) *peError = VRInitError_None;
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_InitInternal2 called!\\n"); fclose(f); }
    return 1;
}

__declspec(dllexport) void VR_ShutdownInternal() {}
__declspec(dllexport) bool VR_IsHmdPresent() { return true; }
__declspec(dllexport) bool VR_IsRuntimeInstalled() { return true; }

__declspec(dllexport) void *VR_GetGenericInterface(const char *pchInterfaceVersion, EVRInitError *peError) {
    if (peError) *peError = VRInitError_None;
    
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_GetGenericInterface: %s\\n", pchInterfaceVersion); fclose(f); }

    if (strncmp(pchInterfaceVersion, "IVRSystem", 9) == 0) return &g_VRSystem;
    if (strncmp(pchInterfaceVersion, "IVRApplications", 15) == 0) return &g_IVRApplications;
    if (strncmp(pchInterfaceVersion, "IVRSettings", 11) == 0) return &g_VRSettings;
    if (strncmp(pchInterfaceVersion, "IVRChaperone", 12) == 0) return &g_IVRChaperone;
    if (strncmp(pchInterfaceVersion, "IVRChaperoneSetup", 17) == 0) return &g_IVRChaperoneSetup;
    if (strncmp(pchInterfaceVersion, "IVRCompositor", 13) == 0) return &g_VRCompositor;
    if (strncmp(pchInterfaceVersion, "IVRHeadsetView", 14) == 0) return &g_IVRHeadsetView;
    if (strncmp(pchInterfaceVersion, "IVRNotifications", 16) == 0) return &g_IVRNotifications;
    if (strncmp(pchInterfaceVersion, "IVROverlay", 10) == 0) return &g_IVROverlay;
    if (strncmp(pchInterfaceVersion, "IVROverlayView", 14) == 0) return &g_IVROverlayView;
    if (strncmp(pchInterfaceVersion, "IVRRenderModels", 15) == 0) return &g_IVRRenderModels;
    if (strncmp(pchInterfaceVersion, "IVRExtendedDisplay", 18) == 0) return &g_IVRExtendedDisplay;
    if (strncmp(pchInterfaceVersion, "IVRTrackedCamera", 16) == 0) return &g_IVRTrackedCamera;
    if (strncmp(pchInterfaceVersion, "IVRScreenshots", 14) == 0) return &g_IVRScreenshots;
    if (strncmp(pchInterfaceVersion, "IVRResources", 12) == 0) return &g_IVRResources;
    if (strncmp(pchInterfaceVersion, "IVRDriverManager", 16) == 0) return &g_IVRDriverManager;
    if (strncmp(pchInterfaceVersion, "IVRInput", 8) == 0) return &g_IVRInput;
    if (strncmp(pchInterfaceVersion, "IVRIOBuffer", 11) == 0) return &g_IVRIOBuffer;
    if (strncmp(pchInterfaceVersion, "IVRSpatialAnchors", 17) == 0) return &g_IVRSpatialAnchors;
    if (strncmp(pchInterfaceVersion, "IVRDebug", 8) == 0) return &g_IVRDebug;
    if (strncmp(pchInterfaceVersion, "IVRIPCResourceManagerClient", 27) == 0) return &g_IVRIPCResourceManagerClient;
    
    if (peError) *peError = VRInitError_Init_InterfaceNotFound;
    return nullptr;
}

__declspec(dllexport) const char * VR_GetVRInitErrorAsSymbol( EVRInitError error ) { return "VRInitError_None"; }
__declspec(dllexport) const char * VR_GetVRInitErrorAsEnglishDescription( EVRInitError error ) { return "No Error"; }
__declspec(dllexport) uint32_t VR_GetInitToken() { return 1; }

__declspec(dllexport) bool VR_IsInterfaceVersionValid( const char *pchInterfaceVersion ) { return true; }
__declspec(dllexport) bool VR_GetRuntimePath( char *pchPathBuffer, uint32_t unBufferSize, uint32_t *punRequiredBufferSize ) {
    const char* path = "C:\\\\Program Files (x86)\\\\Steam\\\\steamapps\\\\common\\\\SteamVR";
    if (punRequiredBufferSize) *punRequiredBufferSize = strlen(path) + 1;
    if (pchPathBuffer && unBufferSize > strlen(path)) strcpy(pchPathBuffer, path);
    return true;
}
__declspec(dllexport) const char *VR_RuntimePath() {
    return "C:\\\\Program Files (x86)\\\\Steam\\\\steamapps\\\\common\\\\SteamVR";
}
__declspec(dllexport) const char *VR_GetStringForHmdError( EVRInitError error ) { return "No Error"; }
__declspec(dllexport) void *VRControlPanel() { return nullptr; }
__declspec(dllexport) IVRHeadsetView *VRHeadsetView() { return nullptr; }
__declspec(dllexport) void *VRPaths() { return nullptr; }
__declspec(dllexport) void *VRVirtualDisplay() { return nullptr; }
__declspec(dllexport) void *LiquidVR() { return nullptr; }

} // extern "C"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        FILE* f = fopen("C:\\\\vr_emulator.log", "w");
        if (f) { fprintf(f, "OpenVR Emulator Loaded!\\n"); fclose(f); }
    }
    return TRUE;
}
""")
