#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <stdio.h>
#include <stdint.h>
#include "MinHook.h"

void LogProxy(const char* msg) {
    FILE* fp = fopen("Z:\\Users\\motonishikoudai\\Verantyx_VR_Drive\\proxy_log.txt", "a");
    if (fp) {
        fprintf(fp, "%s\n", msg);
        fclose(fp);
    }
}

typedef HRESULT (WINAPI *PFN_D3D11_CREATE_DEVICE)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
typedef HRESULT (WINAPI *PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

PFN_D3D11_CREATE_DEVICE g_pOriginalD3D11CreateDevice = NULL;
PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN g_pOriginalD3D11CreateDeviceAndSwapChain = NULL;

PFN_D3D11_CREATE_DEVICE pDxvkCreateDevice = NULL;
PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN pDxvkCreateDeviceAndSwapChain = NULL;

bool isCompositor = false;
bool dxvkTried = false;

void LazyLoadDXVK() {
    if (dxvkTried) return;
    dxvkTried = true;
    
    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    
    char dxvkPath[MAX_PATH];
    lstrcpyA(dxvkPath, exePath);
    char* lastSlash = strrchr(dxvkPath, '\\');
    if (!lastSlash) lastSlash = strrchr(dxvkPath, '/');
    if (lastSlash) {
        *lastSlash = '\0';
        lstrcatA(dxvkPath, "\\dxvk\\d3d11.dll");
        
        char logMsg[512];
        sprintf(logMsg, "[Verantyx] Lazy loading DXVK from: %s", dxvkPath);
        LogProxy(logMsg);
        
        HMODULE hDxvk = LoadLibraryExA(dxvkPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (hDxvk) {
            pDxvkCreateDevice = (PFN_D3D11_CREATE_DEVICE)GetProcAddress(hDxvk, "D3D11CreateDevice");
            pDxvkCreateDeviceAndSwapChain = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetProcAddress(hDxvk, "D3D11CreateDeviceAndSwapChain");
            if (pDxvkCreateDevice) {
                LogProxy("[Verantyx] DXVK loaded SUCCESSFULLY and function pointers obtained.");
            } else {
                LogProxy("[Verantyx] Loaded DXVK DLL but D3D11CreateDevice export not found!");
            }
        } else {
            sprintf(logMsg, "[Verantyx] Failed to lazy load DXVK! Error: %lu", GetLastError());
            LogProxy(logMsg);
        }
    }
}

HRESULT STDMETHODCALLTYPE Hooked_D3D11CreateDevice(
    IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, 
    const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, 
    ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) 
{
    LogProxy("[Verantyx] Hooked_D3D11CreateDevice called!");
    
    if (isCompositor) {
        LazyLoadDXVK();
        if (pDxvkCreateDevice) {
            LogProxy("[Verantyx] Routing D3D11CreateDevice to DXVK...");
            // DXVK crashes if pAdapter is a D3DMetal DXGI adapter, so we nullify it
            if (pAdapter != nullptr) {
                pAdapter = nullptr;
                DriverType = D3D_DRIVER_TYPE_HARDWARE;
            }
            return pDxvkCreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
        }
        LogProxy("[Verantyx] WARNING: DXVK missing for compositor, falling back to D3DMetal native (may crash!).");
    }
    
    LogProxy("[Verantyx] Routing D3D11CreateDevice to D3DMetal native...");
    return g_pOriginalD3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
}

HRESULT STDMETHODCALLTYPE Hooked_D3D11CreateDeviceAndSwapChain(
    IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, 
    const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, 
    const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, 
    ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) 
{
    LogProxy("[Verantyx] Hooked_D3D11CreateDeviceAndSwapChain called!");
    
    if (isCompositor) {
        LazyLoadDXVK();
        if (pDxvkCreateDeviceAndSwapChain) {
            LogProxy("[Verantyx] Routing D3D11CreateDeviceAndSwapChain to DXVK...");
            if (pAdapter != nullptr) {
                pAdapter = nullptr;
                DriverType = D3D_DRIVER_TYPE_HARDWARE;
            }
            return pDxvkCreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
        }
    }
    
    LogProxy("[Verantyx] Routing D3D11CreateDeviceAndSwapChain to D3DMetal native...");
    return g_pOriginalD3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
}

void InitHook() {
    static bool bInit = false;
    if (bInit) return;
    bInit = true;
    
    char exePath[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    
    if (strstr(exePath, "vrcompositor.exe")) {
        isCompositor = true;
        LogProxy("[Verantyx] Proxy loaded into vrcompositor.exe (Will use DXVK)");
    } else {
        isCompositor = false;
        LogProxy("[Verantyx] Proxy loaded into non-compositor process (Will use D3DMetal)");
    }
    
    MH_Initialize();
    
    char sysDir[MAX_PATH];
    GetSystemDirectoryA(sysDir, MAX_PATH);
    lstrcatA(sysDir, "\\d3d11.dll");
    
    HMODULE hD3D11 = LoadLibraryA(sysDir);
    if (hD3D11) {
        void* pD3D11CreateDevice = (void*)GetProcAddress(hD3D11, "D3D11CreateDevice");
        void* pD3D11CreateDeviceAndSwapChain = (void*)GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain");
        if (pD3D11CreateDevice) MH_CreateHook(pD3D11CreateDevice, (void*)Hooked_D3D11CreateDevice, (LPVOID*)&g_pOriginalD3D11CreateDevice);
        if (pD3D11CreateDeviceAndSwapChain) MH_CreateHook(pD3D11CreateDeviceAndSwapChain, (void*)Hooked_D3D11CreateDeviceAndSwapChain, (LPVOID*)&g_pOriginalD3D11CreateDeviceAndSwapChain);
        MH_EnableHook(MH_ALL_HOOKS);
        LogProxy("[Verantyx] MinHook applied to system d3d11.dll (D3DMetal)");
    } else {
        LogProxy("[Verantyx] Failed to load system d3d11.dll!");
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
