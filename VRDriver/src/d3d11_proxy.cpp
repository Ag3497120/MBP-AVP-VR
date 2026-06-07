#include <windows.h>

extern void InitHook();
extern void UninitHook();

HINSTANCE hRealDll = NULL;
void LoadRealDll() {
    if (!hRealDll) {
        char path[MAX_PATH];
        GetSystemDirectoryA(path, MAX_PATH);
        strcat(path, "\\d3d11.dll");
        hRealDll = LoadLibraryA(path);
    }
}

extern "C" __declspec(dllexport) void* D3D11CreateDevice(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "D3D11CreateDevice");
    return func ? func(a, b, c, d, e, f, g, h, i, j) : nullptr;
}

extern "C" __declspec(dllexport) void* D3D11CreateDeviceAndSwapChain(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "D3D11CreateDeviceAndSwapChain");
    return func ? func(a, b, c, d, e, f, g, h, i, j, k, l) : nullptr;
}

extern "C" __declspec(dllexport) void* D3D11On12CreateDevice(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*, void*, void*, void*, void*, void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "D3D11On12CreateDevice");
    return func ? func(a, b, c, d, e, f, g, h) : nullptr;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(hinstDLL); 
        InitHook(); 
    }
    else if (fdwReason == DLL_PROCESS_DETACH) { 
        UninitHook(); 
    }
    return TRUE;
}
