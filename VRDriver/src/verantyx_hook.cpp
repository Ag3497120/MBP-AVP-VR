#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <stdio.h>
#include "MinHook.h"

void LogProxy(const char* msg) {
    FILE* f = fopen("C:\\proxy_log.txt", "a");
    if (f) { fprintf(f, "%s\n", msg); fclose(f); }
}

typedef HRESULT (WINAPI *PFN_D3D11_CREATE_DEVICE)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
typedef HRESULT (WINAPI *PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

PFN_D3D11_CREATE_DEVICE g_pOriginalD3D11CreateDevice = NULL;
PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN g_pOriginalD3D11CreateDeviceAndSwapChain = NULL;

HRESULT STDMETHODCALLTYPE Hooked_D3D11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    char logMsg[256];
    sprintf(logMsg, "[Verantyx] AppInit Hooked_D3D11CreateDevice: pAdapter=%p, DriverType=%d, Flags=%x", pAdapter, DriverType, Flags);
    LogProxy(logMsg);

    if (pAdapter != nullptr) {
        pAdapter = nullptr;
        DriverType = D3D_DRIVER_TYPE_HARDWARE;
    }
    Flags &= ~(D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS);

    HRESULT hr = g_pOriginalD3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
    
    sprintf(logMsg, "[Verantyx] AppInit Hooked_D3D11CreateDevice Result: hr=%x", hr);
    LogProxy(logMsg);
    return hr;
}

HRESULT STDMETHODCALLTYPE Hooked_D3D11CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    char logMsg[256];
    sprintf(logMsg, "[Verantyx] AppInit Hooked_D3D11CreateDeviceAndSwapChain: pAdapter=%p, DriverType=%d, Flags=%x", pAdapter, DriverType, Flags);
    LogProxy(logMsg);

    if (pAdapter != nullptr) {
        pAdapter = nullptr;
        DriverType = D3D_DRIVER_TYPE_HARDWARE;
    }
    Flags &= ~(D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS);

    HRESULT hr = g_pOriginalD3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
    
    sprintf(logMsg, "[Verantyx] AppInit Hooked_D3D11CreateDeviceAndSwapChain Result: hr=%x", hr);
    LogProxy(logMsg);
    return hr;
}

void InitHook() {
    static bool bInit = false;
    if (bInit) return;
    bInit = true;

    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    if (!strstr(exePath, "vrcompositor.exe") && !strstr(exePath, "hlvr.exe")) {
        return;
    }

    LogProxy("[Verantyx] AppInit InitHook called by:");
    LogProxy(exePath);

    MH_Initialize();

    HMODULE hD3D11 = GetModuleHandleA("d3d11.dll");
    if (!hD3D11) hD3D11 = LoadLibraryA("d3d11.dll");
    
    if (hD3D11) {
        LogProxy("[Verantyx] Successfully loaded system d3d11.dll for hooking!");
        void* pD3D11CreateDevice = (void*)GetProcAddress(hD3D11, "D3D11CreateDevice");
        void* pD3D11CreateDeviceAndSwapChain = (void*)GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain");
        if (pD3D11CreateDevice) {
            MH_CreateHook(pD3D11CreateDevice, (void*)Hooked_D3D11CreateDevice, (LPVOID*)&g_pOriginalD3D11CreateDevice);
            LogProxy("[Verantyx] Hooked D3D11CreateDevice");
        }
        if (pD3D11CreateDeviceAndSwapChain) {
            MH_CreateHook(pD3D11CreateDeviceAndSwapChain, (void*)Hooked_D3D11CreateDeviceAndSwapChain, (LPVOID*)&g_pOriginalD3D11CreateDeviceAndSwapChain);
            LogProxy("[Verantyx] Hooked D3D11CreateDeviceAndSwapChain");
        }
        MH_EnableHook(MH_ALL_HOOKS);
    } else {
        LogProxy("[Verantyx] FAILED to load system d3d11.dll!");
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) { 
        DisableThreadLibraryCalls(hinstDLL); 
        InitHook(); 
    }
    else if (fdwReason == DLL_PROCESS_DETACH) { 
        MH_Uninitialize(); 
    }
    return TRUE;
}
