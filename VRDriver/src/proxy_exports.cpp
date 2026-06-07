#include <windows.h>
#include <stdio.h>

HMODULE LoadRealOpenVR() {
    static HMODULE hReal = nullptr;
    if (hReal) return hReal;
    char path[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("openvr_api.dll"), path, MAX_PATH);
    char* p = strrchr(path, '\\');
    if (p) {
        strcpy(p + 1, "openvr_api_real.dll");
        hReal = LoadLibraryA(path);
    }
    if (!hReal) hReal = LoadLibraryA("openvr_api_real.dll");
    return hReal;
}

extern "C" __declspec(dllexport) void* LiquidVR() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "LiquidVR");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "LiquidVR"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VRControlPanel() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VRControlPanel");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VRControlPanel"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VRHeadsetView() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VRHeadsetView");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VRHeadsetView"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VRPaths() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VRPaths");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VRPaths"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VRVirtualDisplay() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VRVirtualDisplay");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VRVirtualDisplay"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_GetGenericInterface() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_GetGenericInterface");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_GetGenericInterface"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_GetInitToken() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_GetInitToken");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_GetInitToken"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_GetRuntimePath() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_GetRuntimePath");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_GetRuntimePath"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_GetStringForHmdError() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_GetStringForHmdError");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_GetStringForHmdError"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_GetVRInitErrorAsEnglishDescription() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_GetVRInitErrorAsEnglishDescription");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_GetVRInitErrorAsEnglishDescription"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_GetVRInitErrorAsSymbol() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_GetVRInitErrorAsSymbol");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_GetVRInitErrorAsSymbol"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_InitInternal() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_InitInternal");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_InitInternal"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_InitInternal2() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_InitInternal2");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_InitInternal2"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_IsHmdPresent() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_IsHmdPresent");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_IsHmdPresent"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_IsInterfaceVersionValid() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_IsInterfaceVersionValid");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_IsInterfaceVersionValid"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_IsRuntimeInstalled() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_IsRuntimeInstalled");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_IsRuntimeInstalled"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_RuntimePath() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_RuntimePath");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_RuntimePath"))
    );
    return nullptr;
}
extern "C" __declspec(dllexport) void* VR_ShutdownInternal() {
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {
        FARPROC proc = GetProcAddress(hReal, "VR_ShutdownInternal");
        if (proc) goto jump;
    }
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "VR_ShutdownInternal"))
    );
    return nullptr;
}
