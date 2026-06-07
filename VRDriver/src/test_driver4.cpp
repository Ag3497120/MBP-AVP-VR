#include <windows.h>
#include <stdio.h>
#include <openvr_driver.h>

void LogDriver(const char* msg) {
    FILE* f = fopen("C:\\verantyx_driver.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

class CVerantyxServerProvider : public vr::IServerTrackedDeviceProvider {
public:
    virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override { 
        LogDriver("Test");
        return vr::VRInitError_None; 
    }
    virtual void Cleanup() override {}
    virtual const char* const* GetInterfaceVersions() override { return vr::k_InterfaceVersions; }
    virtual void RunFrame() override {}
    virtual bool ShouldBlockStandbyMode() override { return true; }
    virtual void EnterStandby() override {}
    virtual void LeaveStandby() override {}
};
CVerantyxServerProvider g_provider;
extern "C" __declspec(dllexport) void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
    return &g_provider;
}
