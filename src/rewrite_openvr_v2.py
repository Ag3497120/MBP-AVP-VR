import sys

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write("""#include "fake_openvr_generated.h"
#include <windows.h>
#include <stdio.h>

class MyVRSystem : public FakeSystem {
public:
    virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        if(pnWidth) *pnWidth = 1920;
        if(pnHeight) *pnHeight = 1080;
    }
    virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) override {
        static bool first = true;
        if (first && uncbVREvent >= sizeof(VREvent_t)) {
            first = false;
            pEvent->eventType = VREvent_TrackedDeviceActivated;
            pEvent->trackedDeviceIndex = k_unTrackedDeviceIndex_Hmd;
            return true;
        }
        return false;
    }
    virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        return unDeviceIndex == k_unTrackedDeviceIndex_Hmd;
    }
    virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        if (unDeviceIndex == k_unTrackedDeviceIndex_Hmd) return TrackedDeviceClass_HMD;
        return TrackedDeviceClass_Invalid;
    }
    virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override {
        for(uint32_t i=0; i<unTrackedDevicePoseArrayCount; ++i) {
            pTrackedDevicePoseArray[i].bPoseIsValid = true;
            pTrackedDevicePoseArray[i].bDeviceIsConnected = true;
            pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking = Identity34_Gen();
        }
    }
};

class MyVRCompositor : public FakeCompositor {
public:
    virtual EVRCompositorError WaitGetPoses(VR_ARRAY_COUNT(unRenderPoseArrayCount) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT(unGamePoseArrayCount) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        Sleep(11); // Sleep for ~11ms to simulate 90FPS and prevent thread starvation
        if(pRenderPoseArray && unRenderPoseArrayCount > 0) {
            for(uint32_t i=0; i<unRenderPoseArrayCount; ++i) {
                pRenderPoseArray[i].bPoseIsValid = true;
                pRenderPoseArray[i].bDeviceIsConnected = true;
                pRenderPoseArray[i].mDeviceToAbsoluteTracking = Identity34_Gen();
            }
        }
        if(pGamePoseArray && unGamePoseArrayCount > 0) {
            for(uint32_t i=0; i<unGamePoseArrayCount; ++i) {
                pGamePoseArray[i].bPoseIsValid = true;
                pGamePoseArray[i].bDeviceIsConnected = true;
                pGamePoseArray[i].mDeviceToAbsoluteTracking = Identity34_Gen();
            }
        }
        return VRCompositorError_None;
    }
};

class MyVRSettings : public FakeSettings {
public:
    virtual const char *GetString(const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError = 0L) override {
        if (peError) *peError = VRSettingsError_None;
        if (pchValue && unValueLen > 0) pchValue[0] = '\\0';
        return "";
    }
    virtual int32_t GetInt32(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = 0L) override {
        if (peError) *peError = VRSettingsError_None;
        return 0;
    }
    virtual bool GetBool(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = 0L) override {
        if (peError) *peError = VRSettingsError_None;
        return false;
    }
};

class MyVRInput : public FakeInput {
public:
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        if (pHandle) *pHandle = 1;
        return VRInputError_None;
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        if (pHandle) *pHandle = 2;
        return VRInputError_None;
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputDigitalActionData_t)) {
            memset(pActionData, 0, sizeof(InputDigitalActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputAnalogActionData_t)) {
            memset(pActionData, 0, sizeof(InputAnalogActionData_t));
        }
        return VRInputError_None;
    }
};

class MyVRRenderModels : public FakeRenderModels {
public:
    virtual EVRRenderModelError LoadRenderModel_Async(const char *pchRenderModelName, RenderModel_t **ppRenderModel) override {
        return VRRenderModelError_NotSupported;
    }
};

MyVRSystem g_VRSystem;
MyVRCompositor g_VRCompositor;
MyVRSettings g_VRSettings;
MyVRInput g_VRInput;
MyVRRenderModels g_VRRenderModels;

FakeApplications g_IVRApplications;
FakeChaperone g_IVRChaperone;
FakeChaperoneSetup g_IVRChaperoneSetup;
FakeOverlay g_IVROverlay;

extern "C" {
__declspec(dllexport) uint32_t VR_InitInternal(EVRInitError *peError, EVRApplicationType eType) {
    if (peError) *peError = VRInitError_None;
    return 1;
}
__declspec(dllexport) uint32_t VR_InitInternal2(EVRInitError *peError, EVRApplicationType eType, const char* pStartupInfo) {
    if (peError) *peError = VRInitError_None;
    return 1;
}
__declspec(dllexport) void VR_ShutdownInternal() {}

__declspec(dllexport) void *VR_GetGenericInterface(const char *pchInterfaceVersion, EVRInitError *peError) {
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_GetGenericInterface: %s\\n", pchInterfaceVersion); fclose(f); }

    if (strncmp(pchInterfaceVersion, "IVRSystem_", 10) == 0) return &g_VRSystem;
    if (strncmp(pchInterfaceVersion, "IVRApplications_", 16) == 0) return &g_IVRApplications;
    if (strncmp(pchInterfaceVersion, "IVRSettings_", 12) == 0) return &g_VRSettings;
    if (strncmp(pchInterfaceVersion, "IVRChaperone_", 13) == 0) return &g_IVRChaperone;
    if (strncmp(pchInterfaceVersion, "IVRChaperoneSetup_", 18) == 0) return &g_IVRChaperoneSetup;
    if (strncmp(pchInterfaceVersion, "IVRCompositor_", 14) == 0) return &g_VRCompositor;
    if (strncmp(pchInterfaceVersion, "IVROverlay_", 11) == 0) return &g_IVROverlay;
    if (strncmp(pchInterfaceVersion, "IVRRenderModels_", 16) == 0) return &g_VRRenderModels;
    if (strncmp(pchInterfaceVersion, "IVRInput_", 9) == 0) return &g_VRInput;

    if (peError) *peError = VRInitError_Init_InterfaceNotFound;
    return nullptr;
}

__declspec(dllexport) const char * VR_GetVRInitErrorAsSymbol( EVRInitError error ) { return "VRInitError_None"; }
__declspec(dllexport) const char * VR_GetVRInitErrorAsEnglishDescription( EVRInitError error ) { return "No Error"; }
__declspec(dllexport) uint32_t VR_GetInitToken() { return 1; }
__declspec(dllexport) bool VR_IsInterfaceVersionValid( const char *pchInterfaceVersion ) { return true; }
__declspec(dllexport) bool VR_GetRuntimePath( char *pchPathBuffer, uint32_t unBufferSize, uint32_t *punRequiredBufferSize ) {
    if (punRequiredBufferSize) *punRequiredBufferSize = 256;
    if (pchPathBuffer && unBufferSize > 0) {
        strncpy(pchPathBuffer, "C:\\\\openvr", unBufferSize);
    }
    return true;
}
}
""")

print("Rewritten openvr_emulator.cpp with correct names")
