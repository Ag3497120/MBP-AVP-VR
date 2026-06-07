import re

with open('openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Add MinHook include
if '#include "minhook/include/MinHook.h"' not in content:
    content = content.replace('#include <d3d11.h>', '#include <d3d11.h>\n#include "minhook/include/MinHook.h"\n#include <stdio.h>')

# Define the hook logic to append
hook_logic = """
// ==========================================
// D3D11 MULTITHREADING PROTECTION HOOK
// ==========================================

typedef HRESULT (STDMETHODCALLTYPE *PFN_D3D11_CREATE_DEVICE)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
typedef HRESULT (STDMETHODCALLTYPE *PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

PFN_D3D11_CREATE_DEVICE g_pOriginalD3D11CreateDevice = nullptr;
PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN g_pOriginalD3D11CreateDeviceAndSwapChain = nullptr;

static void LogHook(const char* msg) {
    FILE* f = fopen("vr_emulator_log.txt", "a");
    if (f) {
        fprintf(f, "%s\\n", msg);
        fclose(f);
    }
}

static void SetupHooks_Emulator(ID3D11Device* pDevice) {
    // We don't need the elaborate texture hooks of the proxy right now,
    // we just need the D3D11 multithread protection.
}

HRESULT STDMETHODCALLTYPE Hooked_D3D11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    char logMsg[256];
    sprintf(logMsg, "[Verantyx] Emulator Hooked_D3D11CreateDevice: pAdapter=%p, DriverType=%d, Flags=%d", pAdapter, DriverType, Flags);
    LogHook(logMsg);

    if (pAdapter != nullptr) {
        pAdapter = nullptr;
        DriverType = D3D_DRIVER_TYPE_HARDWARE;
    }

    Flags &= ~(D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS);

    HRESULT hr = g_pOriginalD3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
    
    sprintf(logMsg, "[Verantyx] Emulator Hooked_D3D11CreateDevice Result: hr=%x", hr);
    LogHook(logMsg);

    if (SUCCEEDED(hr) && ppDevice && *ppDevice) {
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED((*ppDevice)->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt))) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
            LogHook("[Verantyx] Emulator ID3D11Multithread Protection ENABLED!");
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE Hooked_D3D11CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    char logMsg[256];
    sprintf(logMsg, "[Verantyx] Emulator Hooked_D3D11CreateDeviceAndSwapChain: pAdapter=%p, DriverType=%d, Flags=%x", pAdapter, DriverType, Flags);
    LogHook(logMsg);

    if (pAdapter != nullptr) {
        pAdapter = nullptr;
        DriverType = D3D_DRIVER_TYPE_HARDWARE;
    }
    Flags &= ~(D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS);

    HRESULT hr = g_pOriginalD3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
    
    sprintf(logMsg, "[Verantyx] Emulator Hooked_D3D11CreateDeviceAndSwapChain Result: hr=%x", hr);
    LogHook(logMsg);

    if (SUCCEEDED(hr) && ppDevice && *ppDevice) {
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED((*ppDevice)->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt))) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
            LogHook("[Verantyx] Emulator ID3D11Multithread Protection ENABLED!");
        }
    }
    return hr;
}

void InitD3D11Hook() {
    MH_Initialize();
    HMODULE hD3D11 = LoadLibraryA("d3d11.dll");
    if (hD3D11) {
        void* pD3D11CreateDevice = (void*)GetProcAddress(hD3D11, "D3D11CreateDevice");
        void* pD3D11CreateDeviceAndSwapChain = (void*)GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain");
        if (pD3D11CreateDevice) MH_CreateHook(pD3D11CreateDevice, (void*)Hooked_D3D11CreateDevice, (LPVOID*)&g_pOriginalD3D11CreateDevice);
        if (pD3D11CreateDeviceAndSwapChain) MH_CreateHook(pD3D11CreateDeviceAndSwapChain, (void*)Hooked_D3D11CreateDeviceAndSwapChain, (LPVOID*)&g_pOriginalD3D11CreateDeviceAndSwapChain);
        MH_EnableHook(MH_ALL_HOOKS);
        LogHook("[Verantyx] Emulator D3D11 Hooks Injected Successfully.");
    } else {
        LogHook("[Verantyx] Emulator ERROR: Could not load d3d11.dll to inject hooks.");
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) { 
        LogHook("[Verantyx] openvr_api.dll (Emulator) Loaded. Process ATTACH.");
        DisableThreadLibraryCalls(hinstDLL); 
        InitD3D11Hook(); 
    }
    else if (fdwReason == DLL_PROCESS_DETACH) { 
        MH_Uninitialize(); 
    }
    return TRUE;
}
"""

if 'InitD3D11Hook' not in content:
    content += "\n" + hook_logic

with open('openvr_emulator.cpp', 'w') as f:
    f.write(content)

print("Injected D3D11 multithread protection hook into openvr_emulator.cpp")
