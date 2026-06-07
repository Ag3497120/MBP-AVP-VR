#include <openvr_driver.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

void LogMsg(const char* msg) {
    FILE* f = fopen("Z:\\Users\\motonishikoudai\\Verantyx_VR_Drive\\verantyx_driver.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            LogMsg("DllMain: DLL_PROCESS_ATTACH");
            break;
    }
    return TRUE;
}

class CVerantyxDeviceDriver : public vr::ITrackedDeviceServerDriver {
public:
    virtual vr::EVRInitError Activate(uint32_t unObjectId) override { return vr::VRInitError_None; }
    virtual void Deactivate() override {}
    virtual void EnterStandby() override {}
    virtual void* GetComponent(const char* pchComponentNameAndVersion) override { return nullptr; }
    virtual void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override {}
    virtual vr::DriverPose_t GetPose() override {
        vr::DriverPose_t pose = { 0 };
        pose.poseIsValid = true;
        pose.result = vr::TrackingResult_Running_OK;
        pose.deviceIsConnected = true;
        return pose;
    }
};

class CVerantyxServerProvider : public vr::IServerTrackedDeviceProvider {
    CVerantyxDeviceDriver* m_pHmd;
public:
    virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override {
        LogMsg("Init() called!");
        VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);
        m_pHmd = new CVerantyxDeviceDriver();
        vr::VRServerDriverHost()->TrackedDeviceAdded("verantyx_hmd", vr::TrackedDeviceClass_HMD, m_pHmd);
        return vr::VRInitError_None;
    }
    virtual void Cleanup() override { }
    virtual const char* const* GetInterfaceVersions() override { return vr::k_InterfaceVersions; }
    virtual void RunFrame() override { }
    virtual bool ShouldBlockStandbyMode() override { return false; }
    virtual void EnterStandby() override { }
    virtual void LeaveStandby() override { }
};

CVerantyxServerProvider g_provider;

extern "C" __declspec(dllexport) void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
    char buf[256];
    sprintf(buf, "HmdDriverFactory called with: %s", pInterfaceName);
    LogMsg(buf);
    
    if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName)) {
        return &g_provider;
    }
    if (pReturnCode) {
        *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
    }
    return NULL;
}
