#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <d3d11.h>

// Forward declaration of VR interfaces
namespace vr {
    enum EVREye { Eye_Left = 0, Eye_Right = 1 };
    enum ETextureType { TextureType_Invalid = -1, TextureType_DirectX = 0, TextureType_OpenGL = 1, TextureType_Vulkan = 2, TextureType_IOSurface = 3, TextureType_DirectX12 = 4, TextureType_DXGISharedHandle = 5, TextureType_Metal = 6 };
    enum EColorSpace { ColorSpace_Auto = 0, ColorSpace_Gamma = 1, ColorSpace_Linear = 2 };
    struct Texture_t { void* handle; ETextureType eType; EColorSpace eColorSpace; };
    struct VRTextureBounds_t { float uMin, vMin; float uMax, vMax; };
    enum EVRSubmitFlags { Submit_Default = 0x00, Submit_LensDistortionAlreadyApplied = 0x01, Submit_GlRenderBuffer = 0x02, Submit_Reserved = 0x04, Submit_TextureWithPose = 0x08, Submit_TextureWithDepth = 0x10, Submit_FrameDiscontinuty = 0x20 };
    enum EVRCompositorError { VRCompositorError_None = 0 };
    enum EVRInitError { VRInitError_None = 0 };
    enum EVRApplicationType { VRApplication_Other = 0 };
}

HMODULE g_hOriginalOpenVR = NULL;
void LoadOriginalDLL() {
    if (!g_hOriginalOpenVR) {
        g_hOriginalOpenVR = LoadLibraryA("openvr_api_real.dll");
    }
}

void* g_pOriginalCompositor = nullptr;
void** g_pCompositorVTable = nullptr;
void** g_pOriginalCompositorVTable = nullptr;

// Type of the original Submit function
typedef vr::EVRCompositorError (__fastcall *Submit_t)(void* thisptr, vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags);
Submit_t g_pOriginalSubmit = nullptr;

// File mapping for D3D11 device (we can get the device from the texture)
ID3D11Device* g_pD3DDevice = nullptr;
ID3D11DeviceContext* g_pD3DContext = nullptr;
ID3D11Texture2D* g_pStagingTexture = nullptr;

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
                        // TODO: Save to disk or send over UDP
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
    
    // Call the original Submit (if compositor is dead, this might return an error, but we can ignore it)
    vr::EVRCompositorError err = g_pOriginalSubmit(thisptr, eEye, pTexture, pBounds, nSubmitFlags);
    
    // Always return None to the game so it keeps submitting!
    return vr::VRCompositorError_None;
}

void HookCompositor(void* pCompositor) {
    if (!pCompositor || g_pCompositorVTable) return;
    
    g_pOriginalCompositor = pCompositor;
    g_pOriginalCompositorVTable = *(void***)pCompositor;
    
    // Allocate new VTable (approx 60 functions)
    g_pCompositorVTable = new void*[100];
    memcpy(g_pCompositorVTable, g_pOriginalCompositorVTable, sizeof(void*) * 100);
    
    // Submit is at index 5 for IVRCompositor_022
    g_pOriginalSubmit = (Submit_t)g_pCompositorVTable[5];
    g_pCompositorVTable[5] = (void*)Hooked_Submit;
    
    // Replace the VTable pointer in the object
    // Need to unprotect memory if it's protected, but usually the object is in heap so it's fine.
    *(void***)pCompositor = g_pCompositorVTable;
}

extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    LoadOriginalDLL();
    typedef void* (*VR_GetGenericInterface_t)(const char*, vr::EVRInitError*);
    VR_GetGenericInterface_t pOrig = (VR_GetGenericInterface_t)GetProcAddress(g_hOriginalOpenVR, "VR_GetGenericInterface");
    
    void* pInterface = pOrig(pchInterfaceVersion, peError);
    if (pInterface && strncmp(pchInterfaceVersion, "IVRCompositor_", 14) == 0) {
        HookCompositor(pInterface);
    }
    return pInterface;
}

// Forward all other exports
#define FORWARD_EXPORT(name) \
    extern "C" __declspec(dllexport) void* name(...) { \
        LoadOriginalDLL(); \
        void* pOrig = (void*)GetProcAddress(g_hOriginalOpenVR, #name); \
        if (pOrig) { \
            return ((void*(*)(...))pOrig)(); \
        } \
        return nullptr; \
    }

// This macro is not valid for stdcall/fastcall forwarding with arguments without assembly,
// but OpenVR C-API uses cdecl. Wait, VR_InitInternal takes arguments.
// We should properly forward them.
extern "C" __declspec(dllexport) intptr_t VR_InitInternal(vr::EVRInitError *peError, vr::EVRApplicationType eType) {
    LoadOriginalDLL();
    typedef intptr_t (*VR_InitInternal_t)(vr::EVRInitError*, vr::EVRApplicationType);
    VR_InitInternal_t pOrig = (VR_InitInternal_t)GetProcAddress(g_hOriginalOpenVR, "VR_InitInternal");
    return pOrig(peError, eType);
}

extern "C" __declspec(dllexport) intptr_t VR_InitInternal2(vr::EVRInitError *peError, vr::EVRApplicationType eType, const char* pStartupInfo) {
    LoadOriginalDLL();
    typedef intptr_t (*VR_InitInternal2_t)(vr::EVRInitError*, vr::EVRApplicationType, const char*);
    VR_InitInternal2_t pOrig = (VR_InitInternal2_t)GetProcAddress(g_hOriginalOpenVR, "VR_InitInternal2");
    return pOrig(peError, eType, pStartupInfo);
}

extern "C" __declspec(dllexport) void VR_ShutdownInternal() {
    LoadOriginalDLL();
    typedef void (*VR_ShutdownInternal_t)();
    VR_ShutdownInternal_t pOrig = (VR_ShutdownInternal_t)GetProcAddress(g_hOriginalOpenVR, "VR_ShutdownInternal");
    pOrig();
}

extern "C" __declspec(dllexport) bool VR_IsHmdPresent() {
    LoadOriginalDLL();
    typedef bool (*VR_IsHmdPresent_t)();
    VR_IsHmdPresent_t pOrig = (VR_IsHmdPresent_t)GetProcAddress(g_hOriginalOpenVR, "VR_IsHmdPresent");
    return pOrig();
}

extern "C" __declspec(dllexport) bool VR_IsRuntimeInstalled() {
    LoadOriginalDLL();
    typedef bool (*VR_IsRuntimeInstalled_t)();
    VR_IsRuntimeInstalled_t pOrig = (VR_IsRuntimeInstalled_t)GetProcAddress(g_hOriginalOpenVR, "VR_IsRuntimeInstalled");
    return pOrig();
}

extern "C" __declspec(dllexport) const char* VR_RuntimePath() {
    LoadOriginalDLL();
    typedef const char* (*VR_RuntimePath_t)();
    VR_RuntimePath_t pOrig = (VR_RuntimePath_t)GetProcAddress(g_hOriginalOpenVR, "VR_RuntimePath");
    return pOrig();
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsSymbol(vr::EVRInitError error) {
    LoadOriginalDLL();
    typedef const char* (*VR_GetVRInitErrorAsSymbol_t)(vr::EVRInitError);
    VR_GetVRInitErrorAsSymbol_t pOrig = (VR_GetVRInitErrorAsSymbol_t)GetProcAddress(g_hOriginalOpenVR, "VR_GetVRInitErrorAsSymbol");
    return pOrig(error);
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsEnglishDescription(vr::EVRInitError error) {
    LoadOriginalDLL();
    typedef const char* (*VR_GetVRInitErrorAsEnglishDescription_t)(vr::EVRInitError);
    VR_GetVRInitErrorAsEnglishDescription_t pOrig = (VR_GetVRInitErrorAsEnglishDescription_t)GetProcAddress(g_hOriginalOpenVR, "VR_GetVRInitErrorAsEnglishDescription");
    return pOrig(error);
}

extern "C" __declspec(dllexport) void* VR_GetInitToken() {
    LoadOriginalDLL();
    typedef void* (*VR_GetInitToken_t)();
    VR_GetInitToken_t pOrig = (VR_GetInitToken_t)GetProcAddress(g_hOriginalOpenVR, "VR_GetInitToken");
    return pOrig();
}

extern "C" __declspec(dllexport) bool VR_IsInterfaceVersionValid(const char *pchInterfaceVersion) {
    LoadOriginalDLL();
    typedef bool (*VR_IsInterfaceVersionValid_t)(const char*);
    VR_IsInterfaceVersionValid_t pOrig = (VR_IsInterfaceVersionValid_t)GetProcAddress(g_hOriginalOpenVR, "VR_IsInterfaceVersionValid");
    return pOrig(pchInterfaceVersion);
}
