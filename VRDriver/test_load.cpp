#include <windows.h>
#include <stdio.h>

int main() {
    HMODULE h = LoadLibraryA("Z:\\Users\\motonishikoudai\\Verantyx_VR_Drive\\SteamVR_Prefix\\drive_c\\Program Files (x86)\\Steam\\steamapps\\common\\SteamVR\\drivers\\null\\bin\\win64\\driver_null.dll");
    if (!h) {
        printf("Failed to load DLL! Error: %lu\n", GetLastError());
        return 1;
    }
    printf("Successfully loaded DLL!\n");
    void* factory = (void*)GetProcAddress(h, "HmdDriverFactory");
    if (!factory) {
        printf("Failed to find HmdDriverFactory! Error: %lu\n", GetLastError());
        return 1;
    }
    printf("Successfully found HmdDriverFactory!\n");
    return 0;
}
