#include "openvr.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace vr;

typedef void* (*VR_GetGenericInterface_t)(const char *pchInterfaceVersion, EVRInitError *peError);

static HMODULE g_hReal = NULL;
static VR_GetGenericInterface_t g_realGetGenericInterface = NULL;

static void LoadReal() {
    if (g_hReal) return;
    g_hReal = LoadLibraryA("openvr_api_real.dll");
    if (g_hReal) {
        g_realGetGenericInterface = (VR_GetGenericInterface_t)GetProcAddress(g_hReal, "VR_GetGenericInterface");
    }
}

static void Log(const char* msg) {
    FILE* f = fopen("vr_emulator_log.txt", "a");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
    }
}

// ----------------------------------------------------------------------------
// Hooks
// ----------------------------------------------------------------------------

typedef uint32_t (__thiscall *GetStringTrackedDeviceProperty_t)(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError);
static GetStringTrackedDeviceProperty_t g_origGetStringTrackedDeviceProperty = nullptr;

uint32_t __fastcall Hooked_GetStringTrackedDeviceProperty(void* _this, void* _edx, vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError) {
    Log("Called: Hooked_GetStringTrackedDeviceProperty");
    
    // We override specific properties for the controllers
    if (unDeviceIndex == 1 || unDeviceIndex == 2) {
        const char* s = nullptr;
        if (prop == vr::Prop_RenderModelName_String) {
            if (unDeviceIndex == 1) s = "{indexcontroller}valve_controller_knu_1_0_left";
            else if (unDeviceIndex == 2) s = "{indexcontroller}valve_controller_knu_1_0_right";
        }
        else if (prop == vr::Prop_ControllerType_String) s = "knuckles";
        else if (prop == vr::Prop_TrackingSystemName_String) s = "lighthouse";
        else if (prop == vr::Prop_ManufacturerName_String) s = "Valve";
        else if (prop == vr::Prop_ModelNumber_String) s = "Valve Index";
        
        if (s) {
            if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\0'; }
            if(pError) *pError = vr::TrackedProp_Success;
            return (uint32_t)strlen(s) + 1;
        }
    }
    
    // For everything else (including headset), call the original
    return g_origGetStringTrackedDeviceProperty(_this, unDeviceIndex, prop, pchValue, unBufferSize, pError);
}

typedef ETrackedDeviceClass (__thiscall *GetTrackedDeviceClass_t)(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex);
static GetTrackedDeviceClass_t g_origGetTrackedDeviceClass = nullptr;

ETrackedDeviceClass __fastcall Hooked_GetTrackedDeviceClass(void* _this, void* _edx, vr::TrackedDeviceIndex_t unDeviceIndex) {
    if (unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller;
    return g_origGetTrackedDeviceClass(_this, unDeviceIndex);
}

typedef bool (__thiscall *IsTrackedDeviceConnected_t)(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex);
static IsTrackedDeviceConnected_t g_origIsTrackedDeviceConnected = nullptr;

bool __fastcall Hooked_IsTrackedDeviceConnected(void* _this, void* _edx, vr::TrackedDeviceIndex_t unDeviceIndex) {
    if (unDeviceIndex == 1 || unDeviceIndex == 2) return true;
    return g_origIsTrackedDeviceConnected(_this, unDeviceIndex);
}


static bool g_bIVRSystemHooked = false;

extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, EVRInitError *peError) {
    LoadReal();
    if (!g_realGetGenericInterface) {
        if(peError) *peError = VRInitError_Init_Internal;
        return nullptr;
    }
    
    std::string iface(pchInterfaceVersion);
    std::string msg = "VR_GetGenericInterface: " + iface;
    Log(msg.c_str());
    
    void* pReal = g_realGetGenericInterface(pchInterfaceVersion, peError);
    if (!pReal) {
        Log(" -> Returned MOCK (Real was null)");
        static class UniversalMock {
        public:
            virtual void* Dummy0() { return nullptr; } virtual void* Dummy1() { return nullptr; }
            virtual void* Dummy2() { return nullptr; } virtual void* Dummy3() { return nullptr; }
            virtual void* Dummy4() { return nullptr; } virtual void* Dummy5() { return nullptr; }
            virtual void* Dummy6() { return nullptr; } virtual void* Dummy7() { return nullptr; }
            virtual void* Dummy8() { return nullptr; } virtual void* Dummy9() { return nullptr; }
            virtual void* Dummy10() { return nullptr; } virtual void* Dummy11() { return nullptr; }
            virtual void* Dummy12() { return nullptr; } virtual void* Dummy13() { return nullptr; }
            virtual void* Dummy14() { return nullptr; } virtual void* Dummy15() { return nullptr; }
            virtual void* Dummy16() { return nullptr; } virtual void* Dummy17() { return nullptr; }
            virtual void* Dummy18() { return nullptr; } virtual void* Dummy19() { return nullptr; }
            virtual void* Dummy20() { return nullptr; } virtual void* Dummy21() { return nullptr; }
            virtual void* Dummy22() { return nullptr; } virtual void* Dummy23() { return nullptr; }
            virtual void* Dummy24() { return nullptr; } virtual void* Dummy25() { return nullptr; }
            virtual void* Dummy26() { return nullptr; } virtual void* Dummy27() { return nullptr; }
            virtual void* Dummy28() { return nullptr; } virtual void* Dummy29() { return nullptr; }
            virtual void* Dummy30() { return nullptr; } virtual void* Dummy31() { return nullptr; }
            virtual void* Dummy32() { return nullptr; } virtual void* Dummy33() { return nullptr; }
            virtual void* Dummy34() { return nullptr; } virtual void* Dummy35() { return nullptr; }
            virtual void* Dummy36() { return nullptr; } virtual void* Dummy37() { return nullptr; }
            virtual void* Dummy38() { return nullptr; } virtual void* Dummy39() { return nullptr; }
            virtual void* Dummy40() { return nullptr; } virtual void* Dummy41() { return nullptr; }
            virtual void* Dummy42() { return nullptr; } virtual void* Dummy43() { return nullptr; }
            virtual void* Dummy44() { return nullptr; } virtual void* Dummy45() { return nullptr; }
            virtual void* Dummy46() { return nullptr; } virtual void* Dummy47() { return nullptr; }
            virtual void* Dummy48() { return nullptr; } virtual void* Dummy49() { return nullptr; }
            virtual void* Dummy50() { return nullptr; }
        } mock;
        return &mock;
    }
    
    Log(" -> Returned REAL");
    
    // Hook IVRSystem_021
    if (iface == "IVRSystem_021" && !g_bIVRSystemHooked) {
        g_bIVRSystemHooked = true;
        void** vtable = *(void***)pReal;
        
        DWORD oldProtect;
        VirtualProtect(vtable, sizeof(void*) * 100, PAGE_EXECUTE_READWRITE, &oldProtect);
        
        // Index 28 is GetStringTrackedDeviceProperty
        g_origGetStringTrackedDeviceProperty = (GetStringTrackedDeviceProperty_t)vtable[28];
        vtable[28] = (void*)Hooked_GetStringTrackedDeviceProperty;
        
        // Index 20 is GetTrackedDeviceClass
        g_origGetTrackedDeviceClass = (GetTrackedDeviceClass_t)vtable[20];
        vtable[20] = (void*)Hooked_GetTrackedDeviceClass;
        
        // Index 21 is IsTrackedDeviceConnected
        g_origIsTrackedDeviceConnected = (IsTrackedDeviceConnected_t)vtable[21];
        vtable[21] = (void*)Hooked_IsTrackedDeviceConnected;
        
        VirtualProtect(vtable, sizeof(void*) * 100, oldProtect, &oldProtect);
        Log(" -> Hooked IVRSystem_021!");
    }
    
    return pReal;
}

// Forward other exports
extern "C" __declspec(dllexport) uint32_t VR_GetInitToken() {
    LoadReal();
    typedef uint32_t (*Func)();
    Func f = (Func)GetProcAddress(g_hReal, "VR_GetInitToken");
    return f ? f() : 0;
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsEnglishDescription(EVRInitError error) {
    LoadReal();
    typedef const char* (*Func)(EVRInitError);
    Func f = (Func)GetProcAddress(g_hReal, "VR_GetVRInitErrorAsEnglishDescription");
    return f ? f(error) : "";
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsSymbol(EVRInitError error) {
    LoadReal();
    typedef const char* (*Func)(EVRInitError);
    Func f = (Func)GetProcAddress(g_hReal, "VR_GetVRInitErrorAsSymbol");
    return f ? f(error) : "";
}

extern "C" __declspec(dllexport) bool VR_IsHmdPresent() {
    LoadReal();
    typedef bool (*Func)();
    Func f = (Func)GetProcAddress(g_hReal, "VR_IsHmdPresent");
    return f ? f() : true;
}

extern "C" __declspec(dllexport) bool VR_IsRuntimeInstalled() {
    LoadReal();
    typedef bool (*Func)();
    Func f = (Func)GetProcAddress(g_hReal, "VR_IsRuntimeInstalled");
    return f ? f() : true;
}

extern "C" __declspec(dllexport) bool VR_IsInterfaceVersionValid(const char* pchInterfaceVersion) {
    LoadReal();
    typedef bool (*Func)(const char*);
    Func f = (Func)GetProcAddress(g_hReal, "VR_IsInterfaceVersionValid");
    return f ? f(pchInterfaceVersion) : true;
}

extern "C" __declspec(dllexport) bool VR_GetRuntimePath(char *pchPathBuffer, uint32_t unBufferSize, uint32_t *punRequiredBufferSize) {
    LoadReal();
    typedef bool (*Func)(char*, uint32_t, uint32_t*);
    Func f = (Func)GetProcAddress(g_hReal, "VR_GetRuntimePath");
    if (f) return f(pchPathBuffer, unBufferSize, punRequiredBufferSize);
    if (punRequiredBufferSize) *punRequiredBufferSize = 1;
    if (pchPathBuffer && unBufferSize > 0) pchPathBuffer[0] = '\0';
    return true;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal(EVRInitError *peError, EVRApplicationType eType) {
    LoadReal();
    typedef uint32_t (*Func)(EVRInitError*, EVRApplicationType);
    Func f = (Func)GetProcAddress(g_hReal, "VR_InitInternal");
    return f ? f(peError, eType) : 0;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal2(EVRInitError *peError, EVRApplicationType eType, const char *pStartupInfo) {
    LoadReal();
    typedef uint32_t (*Func)(EVRInitError*, EVRApplicationType, const char*);
    Func f = (Func)GetProcAddress(g_hReal, "VR_InitInternal2");
    return f ? f(peError, eType, pStartupInfo) : 0;
}

extern "C" __declspec(dllexport) void VR_ShutdownInternal() {
    LoadReal();
    typedef void (*Func)();
    Func f = (Func)GetProcAddress(g_hReal, "VR_ShutdownInternal");
    if (f) f();
}

extern "C" __declspec(dllexport) const char* VR_GetStringForHmdError(vr::EVRInitError error) {
    LoadReal();
    typedef const char* (*Func)(vr::EVRInitError);
    Func f = (Func)GetProcAddress(g_hReal, "VR_GetStringForHmdError");
    return f ? f(error) : "";
}

extern "C" __declspec(dllexport) void* VRControlPanel() {
    LoadReal();
    typedef void* (*Func)();
    Func f = (Func)GetProcAddress(g_hReal, "VRControlPanel");
    return f ? f() : nullptr;
}

