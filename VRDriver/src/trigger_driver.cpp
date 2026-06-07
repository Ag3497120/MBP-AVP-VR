#include <openvr.h>
#include <stdio.h>

int main() {
    printf("Calling VR_Init...\n");
    vr::EVRInitError eError = vr::VRInitError_None;
    vr::IVRSystem *pSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);
    if (eError != vr::VRInitError_None) {
        printf("VR_Init failed: %d\n", eError);
        return 1;
    }
    printf("VR_Init succeeded! System interface: %p\n", pSystem);
    vr::VR_Shutdown();
    return 0;
}
