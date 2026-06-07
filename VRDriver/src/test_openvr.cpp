#include <openvr.h>
#include <stdio.h>
#include <windows.h>

int main() {
    printf("Starting Dummy VR Game (No D3D11)...\n");
    
    HMODULE hOpenVR = LoadLibraryA("openvr_api.dll");
    if (!hOpenVR) {
        printf("Failed to load openvr_api.dll!\n");
        return 1;
    }
    
    typedef void* (*VR_GetGenericInterface_t)(const char* pchInterfaceVersion, vr::EVRInitError* peError);
    VR_GetGenericInterface_t pVR_GetGenericInterface = (VR_GetGenericInterface_t)GetProcAddress(hOpenVR, "VR_GetGenericInterface");
    
    vr::EVRInitError eError = vr::VRInitError_None;
    vr::IVRCompositor* pCompositor = (vr::IVRCompositor*)pVR_GetGenericInterface(vr::IVRCompositor_Version, &eError);
    if (!pCompositor) {
        printf("Failed to get IVRCompositor! Error: %d\n", eError);
        return 1;
    }
    printf("Got IVRCompositor: %p\n", pCompositor);
    
    Sleep(5000); // Keep alive
    return 0;
}
