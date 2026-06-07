#include <windows.h>

__declspec(dllexport) void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
    OutputDebugStringA("VERANTYX_DRIVER_LOG: HmdDriverFactory called!\n");
    return NULL;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        OutputDebugStringA("VERANTYX_DRIVER_LOG: DllMain PROCESS_ATTACH!\n");
    }
    return TRUE;
}
