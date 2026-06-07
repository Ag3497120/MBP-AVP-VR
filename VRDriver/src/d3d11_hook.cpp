#include <windows.h>
#include <d3d11.h>
#include <d3d11_4.h>
#include <dxgi.h>
#include <stdio.h>

void LogProxy(const char* msg) {
    FILE* f = fopen("C:\\proxy_log.txt", "a");
    if (f) { 
        char exePath[MAX_PATH] = {0};
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        const char* exeName = strrchr(exePath, '\\');
        exeName = exeName ? exeName + 1 : exePath;
        fprintf(f, "[%s:%lu] %s\n", exeName, GetCurrentProcessId(), msg); 
        fclose(f); 
    }
}

typedef HRESULT (WINAPI *PFN_D3D11_CREATE_DEVICE)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
typedef HRESULT (WINAPI *PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

PFN_D3D11_CREATE_DEVICE g_pRealD3D11CreateDevice = NULL;
PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN g_pRealD3D11CreateDeviceAndSwapChain = NULL;
HMODULE g_hRealD3D11 = NULL;
IDXGIAdapter* g_LastGoodAdapter = NULL;

void InitHook() {
    static bool bInit = false;
    if (bInit) return;
    bInit = true;

    LogProxy("[Verantyx] D3D11 Proxy Initializing (DXVK Clean Mode)...");

    if (g_hRealD3D11 == nullptr) {
        char systemDir[MAX_PATH];
        GetSystemDirectoryA(systemDir, MAX_PATH);
        strcat(systemDir, "\\d3d11.dll");
        g_hRealD3D11 = LoadLibraryA(systemDir);
        if (g_hRealD3D11) {
            g_pRealD3D11CreateDevice = (PFN_D3D11_CREATE_DEVICE)GetProcAddress(g_hRealD3D11, "D3D11CreateDevice");
            g_pRealD3D11CreateDeviceAndSwapChain = (PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN)GetProcAddress(g_hRealD3D11, "D3D11CreateDeviceAndSwapChain");
            LogProxy("[Verantyx] Successfully loaded system d3d11.dll!");
        } else {
            LogProxy("[Verantyx] FAILED to load system d3d11.dll!");
        }
    }
}

extern "C" __declspec(dllexport) HRESULT WINAPI D3D11CreateDevice(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    if (!g_pRealD3D11CreateDevice) return E_FAIL;
    
    char logMsg[256];
    sprintf(logMsg, "[Verantyx] Call D3D11CreateDevice: pAdapter=%p, DriverType=%d, Flags=0x%x, FeatureLevels=%d, pFeatureLevels=%p, SDK=%d", pAdapter, DriverType, Flags, FeatureLevels, pFeatureLevels, SDKVersion);
    LogProxy(logMsg);

    if (pAdapter != nullptr && DriverType == D3D_DRIVER_TYPE_UNKNOWN) {
        if (!g_LastGoodAdapter) {
            g_LastGoodAdapter = pAdapter;
            g_LastGoodAdapter->AddRef();
            LogProxy("[Verantyx] Saved good adapter!");
        }
    }
    
    if (pAdapter == nullptr && DriverType == D3D_DRIVER_TYPE_HARDWARE) {
        if (g_LastGoodAdapter) {
            pAdapter = g_LastGoodAdapter;
            DriverType = D3D_DRIVER_TYPE_UNKNOWN;
            LogProxy("[Verantyx] Replaced NULL adapter with saved good adapter!");
        } else {
            LogProxy("[Verantyx] Attempting to dynamically query DXGI adapter...");
            HMODULE hDxgi = LoadLibraryA("dxgi.dll");
            if (hDxgi) {
                typedef HRESULT(WINAPI *PFN_CREATE_DXGI_FACTORY)(REFIID, void**);
                PFN_CREATE_DXGI_FACTORY pCreateDXGIFactory = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(hDxgi, "CreateDXGIFactory1");
                if (pCreateDXGIFactory) {
                    IDXGIFactory1* pFactory = nullptr;
                    HRESULT hrDxgi = pCreateDXGIFactory(__uuidof(IDXGIFactory1), (void**)&pFactory);
                    if (SUCCEEDED(hrDxgi) && pFactory) {
                        IDXGIAdapter* pDynAdapter = nullptr;
                        HRESULT hrEnum = pFactory->EnumAdapters(0, &pDynAdapter);
                        if (SUCCEEDED(hrEnum) && pDynAdapter) {
                            pAdapter = pDynAdapter;
                            DriverType = D3D_DRIVER_TYPE_UNKNOWN;
                            g_LastGoodAdapter = pAdapter;
                            LogProxy("[Verantyx] SUCCESS: Replaced NULL adapter with DYNAMICALLY queried DXGI adapter!");
                        } else {
                            char err[128]; sprintf(err, "[Verantyx] FAILED EnumAdapters, hr=%lx", hrEnum); LogProxy(err);
                        }
                        pFactory->Release();
                    } else {
                        char err[128]; sprintf(err, "[Verantyx] FAILED CreateDXGIFactory1, hr=%lx", hrDxgi); LogProxy(err);
                    }
                } else {
                    LogProxy("[Verantyx] FAILED to get CreateDXGIFactory1 proc address!");
                }
            } else {
                LogProxy("[Verantyx] FAILED to load dxgi.dll!");
            }
        }
    }
    
    // Remove thread-unsafe flags that crash Wine/DXVK on Mac
    Flags &= ~0x1; // D3D11_CREATE_DEVICE_SINGLETHREADED
    Flags &= ~0x8; // D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
    
    if (FeatureLevels == 0 || pFeatureLevels == nullptr) {
        static const D3D_FEATURE_LEVEL defaultFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        pFeatureLevels = defaultFeatureLevels;
        FeatureLevels = 1;
        LogProxy("[Verantyx] Forced valid default feature levels array!");
    }

    HRESULT hr = E_FAIL;
    if (g_pRealD3D11CreateDevice) {
        hr = g_pRealD3D11CreateDevice(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, ppDevice, pFeatureLevel, ppImmediateContext);
    }
    
    sprintf(logMsg, "[Verantyx] D3D11CreateDevice native return: %lx", hr);
    LogProxy(logMsg);
    
    if (SUCCEEDED(hr) && ppDevice && *ppDevice) {
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED((*ppDevice)->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt))) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
        }
    }
    
    return hr;
}

extern "C" __declspec(dllexport) HRESULT WINAPI D3D11CreateDeviceAndSwapChain(IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags, const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain, ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    if (!g_pRealD3D11CreateDeviceAndSwapChain) return E_FAIL;
    
    char logMsg[256];
    sprintf(logMsg, "[Verantyx] Call D3D11CreateDeviceAndSwapChain: pAdapter=%p, DriverType=%d, Flags=0x%x, FeatureLevels=%d", pAdapter, DriverType, Flags, FeatureLevels);
    LogProxy(logMsg);

    if (pAdapter != nullptr && DriverType == D3D_DRIVER_TYPE_UNKNOWN) {
        if (!g_LastGoodAdapter) {
            g_LastGoodAdapter = pAdapter;
            g_LastGoodAdapter->AddRef();
            LogProxy("[Verantyx] Saved good adapter!");
        }
    }
    
    if (pAdapter == nullptr && DriverType == D3D_DRIVER_TYPE_HARDWARE) {
        if (g_LastGoodAdapter) {
            pAdapter = g_LastGoodAdapter;
            DriverType = D3D_DRIVER_TYPE_UNKNOWN;
            LogProxy("[Verantyx] Replaced NULL adapter with saved good adapter!");
        } else {
            LogProxy("[Verantyx] Attempting to dynamically query DXGI adapter...");
            HMODULE hDxgi = LoadLibraryA("dxgi.dll");
            if (hDxgi) {
                typedef HRESULT(WINAPI *PFN_CREATE_DXGI_FACTORY)(REFIID, void**);
                PFN_CREATE_DXGI_FACTORY pCreateDXGIFactory = (PFN_CREATE_DXGI_FACTORY)GetProcAddress(hDxgi, "CreateDXGIFactory1");
                if (pCreateDXGIFactory) {
                    IDXGIFactory1* pFactory = nullptr;
                    HRESULT hrDxgi = pCreateDXGIFactory(__uuidof(IDXGIFactory1), (void**)&pFactory);
                    if (SUCCEEDED(hrDxgi) && pFactory) {
                        IDXGIAdapter* pDynAdapter = nullptr;
                        HRESULT hrEnum = pFactory->EnumAdapters(0, &pDynAdapter);
                        if (SUCCEEDED(hrEnum) && pDynAdapter) {
                            pAdapter = pDynAdapter;
                            DriverType = D3D_DRIVER_TYPE_UNKNOWN;
                            g_LastGoodAdapter = pAdapter;
                            LogProxy("[Verantyx] SUCCESS: Replaced NULL adapter with DYNAMICALLY queried DXGI adapter!");
                        } else {
                            char err[128]; sprintf(err, "[Verantyx] FAILED EnumAdapters, hr=%lx", hrEnum); LogProxy(err);
                        }
                        pFactory->Release();
                    } else {
                        char err[128]; sprintf(err, "[Verantyx] FAILED CreateDXGIFactory1, hr=%lx", hrDxgi); LogProxy(err);
                    }
                } else {
                    LogProxy("[Verantyx] FAILED to get CreateDXGIFactory1 proc address!");
                }
            } else {
                LogProxy("[Verantyx] FAILED to load dxgi.dll!");
            }
        }
    }
    
    Flags &= ~0x1; // D3D11_CREATE_DEVICE_SINGLETHREADED
    Flags &= ~0x8; // D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS
    
    if (FeatureLevels == 0 || pFeatureLevels == nullptr) {
        static const D3D_FEATURE_LEVEL defaultFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        pFeatureLevels = defaultFeatureLevels;
        FeatureLevels = 1;
        LogProxy("[Verantyx] Forced valid default feature levels array!");
    }

    HRESULT hr = E_FAIL;
    if (g_pRealD3D11CreateDeviceAndSwapChain) {
        hr = g_pRealD3D11CreateDeviceAndSwapChain(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, ppDevice, pFeatureLevel, ppImmediateContext);
    }

    sprintf(logMsg, "[Verantyx] D3D11CreateDeviceAndSwapChain native return: %lx", hr);
    LogProxy(logMsg);

    if (SUCCEEDED(hr) && ppDevice && *ppDevice) {
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED((*ppDevice)->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt))) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
        }
    }
    
    return hr;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        InitHook();
    }
    return TRUE;
}
