#include <windows.h>

extern void InitHook();
extern void UninitHook();

HINSTANCE hRealDll = NULL;
void LoadRealDll() {
    if (!hRealDll) {
        hRealDll = LoadLibraryA("dxgi_real.dll");
    }
}

extern "C" __declspec(dllexport) void* CreateDXGIFactory(void* a, void* b) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "CreateDXGIFactory");
    return func ? func(a, b) : nullptr;
}

extern "C" __declspec(dllexport) void* CreateDXGIFactory1(void* a, void* b) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "CreateDXGIFactory1");
    return func ? func(a, b) : nullptr;
}

extern "C" __declspec(dllexport) void* CreateDXGIFactory2(void* a, void* b, void* c) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "CreateDXGIFactory2");
    return func ? func(a, b, c) : nullptr;
}

extern "C" __declspec(dllexport) void* DXGID3D10CreateDevice(void* a, void* b, void* c, void* d, void* e, void* f) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*, void*, void*, void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "DXGID3D10CreateDevice");
    return func ? func(a, b, c, d, e, f) : nullptr;
}

extern "C" __declspec(dllexport) void* DXGID3D10RegisterLayers(void* a, void* b) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "DXGID3D10RegisterLayers");
    return func ? func(a, b) : nullptr;
}

extern "C" __declspec(dllexport) void* DXGIDeclareAdapterRemovalSupport() {
    LoadRealDll();
    typedef void* (*PFunc)();
    PFunc func = (PFunc)GetProcAddress(hRealDll, "DXGIDeclareAdapterRemovalSupport");
    return func ? func() : nullptr;
}

extern "C" __declspec(dllexport) void* DXGIGetDebugInterface1(void* a, void* b, void* c) {
    LoadRealDll();
    typedef void* (*PFunc)(void*, void*, void*);
    PFunc func = (PFunc)GetProcAddress(hRealDll, "DXGIGetDebugInterface1");
    return func ? func(a, b, c) : nullptr;
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
