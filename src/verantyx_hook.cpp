#include <windows.h>
#include <stdio.h>
#include <d3d11.h>
#include "MinHook/include/MinHook.h"
#include "openvr.h"

// Function pointer types
typedef void* (*VR_GetGenericInterface_t)(const char *pchInterfaceVersion, vr::EVRInitError *peError);
typedef vr::EVRCompositorError (__fastcall *Submit_t)(void* thisptr, vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags);

// Original function pointers
VR_GetGenericInterface_t fpVR_GetGenericInterface = NULL;
Submit_t g_pOriginalSubmit = NULL;

// Global interfaces
vr::IVRCompositor* g_pOriginalCompositor = nullptr;
void** g_pCompositorVTable = nullptr;
void** g_pOriginalCompositorVTable = nullptr;

// D3D11 pointers
ID3D11Device* g_pD3DDevice = nullptr;
ID3D11DeviceContext* g_pD3DContext = nullptr;
ID3D11Texture2D* g_pStagingTexture = nullptr;

// Our hooked Submit function
vr::EVRCompositorError __fastcall Hooked_Submit(void* thisptr, vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags) {
    if (pTexture && pTexture->eType == vr::TextureType_DirectX) {
        ID3D11Texture2D* pTex2D = (ID3D11Texture2D*)pTexture->handle;
        if (pTex2D) {
            if (!g_pD3DDevice) {
                pTex2D->GetDevice(&g_pD3DDevice);
                if (g_pD3DDevice) {
                    g_pD3DDevice->GetImmediateContext(&g_pD3DContext);
                }
            }
            
            if (g_pD3DDevice && g_pD3DContext) {
                D3D11_TEXTURE2D_DESC desc;
                pTex2D->GetDesc(&desc);
                
                if (!g_pStagingTexture) {
                    desc.Usage = D3D11_USAGE_STAGING;
                    desc.BindFlags = 0;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                    desc.MiscFlags = 0;
                    g_pD3DDevice->CreateTexture2D(&desc, nullptr, &g_pStagingTexture);
                }

                if (g_pStagingTexture) {
                    g_pD3DContext->CopyResource(g_pStagingTexture, pTex2D);
                    D3D11_MAPPED_SUBRESOURCE mapped;
                    if (SUCCEEDED(g_pD3DContext->Map(g_pStagingTexture, 0, D3D11_MAP_READ, 0, &mapped))) {
                        static int frameCount = 0;
                        if (frameCount < 10) {
                            char filename[256];
                            sprintf(filename, "C:\\vr_frame_%d_eye%d.rgba", frameCount, eEye);
                            FILE* fp = fopen(filename, "wb");
                            if (fp) {
                                fwrite(mapped.pData, 1, mapped.RowPitch * desc.Height, fp);
                                fclose(fp);
                            }
                            frameCount++;
                        }
                        g_pD3DContext->Unmap(g_pStagingTexture, 0);
                    }
                }
            }
        }
    }
    
    // We ignore the error and return Success so the game continues running without crashing.
    if (g_pOriginalSubmit) {
        g_pOriginalSubmit(thisptr, eEye, pTexture, pBounds, nSubmitFlags);
    }
    return vr::VRCompositorError_None;
}

// Hook Compositor VTable
void HookCompositor(void* pCompositor) {
    if (!pCompositor || g_pCompositorVTable) return;
    
    g_pOriginalCompositor = (vr::IVRCompositor*)pCompositor;
    g_pOriginalCompositorVTable = *(void***)pCompositor;
    
    g_pCompositorVTable = new void*[100];
    memcpy(g_pCompositorVTable, g_pOriginalCompositorVTable, sizeof(void*) * 100);
    
    // Submit is at index 5
    g_pOriginalSubmit = (Submit_t)g_pCompositorVTable[5];
    g_pCompositorVTable[5] = (void*)Hooked_Submit;
    
    // Overwrite the object's VTable pointer
    *(void***)pCompositor = g_pCompositorVTable;
}

// Our hooked VR_GetGenericInterface
void* Hooked_VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    void* pInterface = fpVR_GetGenericInterface(pchInterfaceVersion, peError);
    if (pInterface && strncmp(pchInterfaceVersion, "IVRCompositor_", 14) == 0) {
        HookCompositor(pInterface);
    }
    return pInterface;
}

// Background thread to wait for openvr_api.dll to load
DWORD WINAPI InjectionThread(LPVOID lpParam) {
    HMODULE hOpenVR = NULL;
    while (!hOpenVR) {
        hOpenVR = GetModuleHandleA("openvr_api.dll");
        Sleep(100);
    }
    
    void* pVR_GetGenericInterface = (void*)GetProcAddress(hOpenVR, "VR_GetGenericInterface");
    if (pVR_GetGenericInterface) {
        MH_Initialize();
        MH_CreateHook(pVR_GetGenericInterface, (void*)Hooked_VR_GetGenericInterface, (reinterpret_cast<void**>(&fpVR_GetGenericInterface)));
        MH_EnableHook(pVR_GetGenericInterface);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, InjectionThread, NULL, 0, NULL);
    }
    return TRUE;
}
