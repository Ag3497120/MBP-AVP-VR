#include <openvr.h>
#include <windows.h>
#include <stdio.h>

typedef uint32_t (*VR_InitInternal2_t)(vr::EVRInitError* peError, vr::EVRApplicationType eType, const char* pStartupInfo);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    HMODULE hOpenVR = LoadLibraryA("openvr_api.dll");
    if (hOpenVR) {
        VR_InitInternal2_t pInit2 = (VR_InitInternal2_t)GetProcAddress(hOpenVR, "VR_InitInternal2");
        if (pInit2) {
            vr::EVRInitError peError;
            // Launch as VRApplication_Background just like the real one did in the logs
            uint32_t token = pInit2(&peError, vr::VRApplication_Background, "");
            
            if (token != 0) {
                printf("[Verantyx Dummy Compositor] Connected to vrserver successfully!\n");
                // Keep the IPC connection alive FOREVER
                while (true) {
                    Sleep(10000);
                }
            } else {
                printf("[Verantyx Dummy Compositor] Failed to connect: %d\n", peError);
            }
        }
    }
    return 0;
}
