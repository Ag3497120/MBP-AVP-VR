exports = [
    "LiquidVR",
    "VRControlPanel",
    "VRHeadsetView",
    "VRPaths",
    "VRVirtualDisplay",
    "VR_GetGenericInterface",
    "VR_GetInitToken",
    "VR_GetRuntimePath",
    "VR_GetStringForHmdError",
    "VR_GetVRInitErrorAsEnglishDescription",
    "VR_GetVRInitErrorAsSymbol",
    "VR_InitInternal",
    "VR_InitInternal2",
    "VR_IsHmdPresent",
    "VR_IsInterfaceVersionValid",
    "VR_IsRuntimeInstalled",
    "VR_RuntimePath",
    "VR_ShutdownInternal"
]

print('''#include <windows.h>
#include <stdio.h>

HMODULE LoadRealOpenVR() {
    static HMODULE hReal = nullptr;
    if (hReal) return hReal;
    char path[MAX_PATH];
    GetModuleFileNameA(GetModuleHandleA("openvr_api.dll"), path, MAX_PATH);
    char* p = strrchr(path, '\\\\');
    if (p) {
        strcpy(p + 1, "openvr_api_real.dll");
        hReal = LoadLibraryA(path);
    }
    if (!hReal) hReal = LoadLibraryA("openvr_api_real.dll");
    return hReal;
}
''')

for name in exports:
    print(f'''extern "C" __declspec(dllexport) void* {name}() {{
    HMODULE hReal = LoadRealOpenVR();
    if (hReal) {{
        FARPROC proc = GetProcAddress(hReal, "{name}");
        if (proc) goto jump;
    }}
    return nullptr;
jump:
    __asm__ __volatile__ (
        "jmp *%0"
        :
        : "r" (GetProcAddress(hReal, "{name}"))
    );
    return nullptr;
}}''')
