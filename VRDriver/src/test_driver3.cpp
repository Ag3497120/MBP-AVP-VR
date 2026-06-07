#include <windows.h>
#include <d3d11.h>
#include <openvr_driver.h>

class CVerantyxServerProvider : public vr::IServerTrackedDeviceProvider {
public:
    virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext) override { 
        GUID g = __uuidof(ID3D11Texture2D);
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
