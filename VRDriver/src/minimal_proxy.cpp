#include <windows.h>
#include <d3d11.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/openvr.h"

// Shared memory for Hand Tracking and Video
struct SharedFrame {
    uint32_t sequenceNumber;
    uint32_t width;
    uint32_t height;
    uint32_t format;
};

static HANDLE hMapFile = NULL;
static void* pBuf = NULL;
static SharedFrame* pHeader = NULL;
static uint8_t* pPixelData = NULL;
static ID3D11Texture2D* pStagingTexture = NULL;
static uint32_t frameSeq = 1;

static void InitSharedMemory() {
    if (hMapFile) return;
    const int mapSize = 16 + 1920 * 1080 * 4 + 130;
    HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, mapSize, NULL);
        if (hMapFile) {
            pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, mapSize);
            if (pBuf) {
                pHeader = (SharedFrame*)pBuf;
                pPixelData = (uint8_t*)pBuf + sizeof(SharedFrame);
            }
        }
    }
}

static void Log(const char* msg) {
    FILE* f = fopen("vr_proxy_log.txt", "a");
    if (f) { fprintf(f, "%s\n", msg); fclose(f); }
}

// Function pointers for real OpenVR
typedef void* (*VR_GetGenericInterface_t)(const char*, vr::EVRInitError*);
typedef uint32_t (*VR_InitInternal_t)(vr::EVRInitError*, vr::EVRApplicationType);
typedef uint32_t (*VR_InitInternal2_t)(vr::EVRInitError*, vr::EVRApplicationType, const char*);
typedef void (*VR_ShutdownInternal_t)();
typedef bool (*VR_IsHmdPresent_t)();

static HMODULE g_hReal = NULL;
static void LoadReal() {
    if (g_hReal) return;
    g_hReal = LoadLibraryA("openvr_api_real.dll");
}

// Hooked WaitGetPoses
typedef vr::EVRCompositorError (*WaitGetPoses_t)(void*, vr::TrackedDevicePose_t*, uint32_t, vr::TrackedDevicePose_t*, uint32_t);
static WaitGetPoses_t g_realWaitGetPoses = nullptr;

vr::EVRCompositorError Hooked_WaitGetPoses(void* _this, vr::TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, vr::TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) {
    // Populate poses using the REAL IVRSystem directly
    LoadReal();
    if (g_hReal) {
        VR_GetGenericInterface_t f = (VR_GetGenericInterface_t)GetProcAddress(g_hReal, "VR_GetGenericInterface");
        if (f) {
            vr::EVRInitError err;
            vr::IVRSystem* sys = (vr::IVRSystem*)f(vr::IVRSystem_Version, &err);
            if (sys) {
                if (pRenderPoseArray && unRenderPoseArrayCount > 0) {
                    sys->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, pRenderPoseArray, unRenderPoseArrayCount);
                }
                if (pGamePoseArray && unGamePoseArrayCount > 0) {
                    sys->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, pGamePoseArray, unGamePoseArrayCount);
                }
            }
        }
    }
    return vr::VRCompositorError_None;
}

// Hooked Submit
typedef vr::EVRCompositorError (*Submit_t)(void*, vr::EVREye, const vr::Texture_t*, const vr::VRTextureBounds_t*, vr::EVRSubmitFlags);
static Submit_t g_realSubmit = nullptr;

vr::EVRCompositorError Hooked_Submit(void* _this, vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags) {
    InitSharedMemory();
    if (eEye == vr::Eye_Left && pTexture && pTexture->handle && pHeader && pPixelData) {
        ID3D11Texture2D* pTex = (ID3D11Texture2D*)pTexture->handle;
        ID3D11Device* pDevice = nullptr;
        pTex->GetDevice(&pDevice);
        if (pDevice) {
            ID3D11DeviceContext* pContext = nullptr;
            pDevice->GetImmediateContext(&pContext);
            if (pContext) {
                D3D11_TEXTURE2D_DESC desc;
                pTex->GetDesc(&desc);
                if (!pStagingTexture) {
                    desc.Usage = D3D11_USAGE_STAGING;
                    desc.BindFlags = 0;
                    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                    desc.MiscFlags = 0;
                    pDevice->CreateTexture2D(&desc, nullptr, &pStagingTexture);
                }
                if (pStagingTexture) {
                    pContext->CopyResource(pStagingTexture, pTex);
                    D3D11_MAPPED_SUBRESOURCE mapped;
                    if (SUCCEEDED(pContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mapped))) {
                        pHeader->width = desc.Width;
                        pHeader->height = desc.Height;
                        pHeader->format = desc.Format;
                        
                        uint8_t* dst = pPixelData;
                        uint8_t* src = (uint8_t*)mapped.pData;
                        uint32_t bytesPerRow = desc.Width * 4;
                        for(uint32_t y = 0; y < desc.Height; ++y) {
                            memcpy(dst + y * bytesPerRow, src + y * mapped.RowPitch, bytesPerRow);
                        }
                        pHeader->sequenceNumber = ++frameSeq;
                        pContext->Unmap(pStagingTexture, 0);
                    }
                }
                pContext->Release();
            }
            pDevice->Release();
        }
    }
    return vr::VRCompositorError_None;
}

// Hooked PostPresentHandoff
typedef void (*PostPresentHandoff_t)(void*);
static PostPresentHandoff_t g_realPostPresentHandoff = nullptr;
void Hooked_PostPresentHandoff(void* _this) {
    // do nothing to avoid blocking
}


extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    LoadReal();
    if (!g_hReal) return nullptr;
    
    VR_GetGenericInterface_t realFunc = (VR_GetGenericInterface_t)GetProcAddress(g_hReal, "VR_GetGenericInterface");
    if (!realFunc) return nullptr;

    char msg[256];
    sprintf(msg, "Requested Interface: %s", pchInterfaceVersion);
    Log(msg);

    void* pReal = realFunc(pchInterfaceVersion, peError);
    if (!pReal) return nullptr;

    // VTable Hooking for IVRCompositor
    if (strstr(pchInterfaceVersion, "IVRCompositor")) {
        static bool s_compositorHooked = false;
        if (!s_compositorHooked) {
            void** vtable = *(void***)pReal;
            DWORD oldProtect;
            if (VirtualProtect(&vtable[2], sizeof(void*) * 6, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                g_realWaitGetPoses = (WaitGetPoses_t)vtable[2];
                vtable[2] = (void*)&Hooked_WaitGetPoses;

                g_realSubmit = (Submit_t)vtable[5];
                vtable[5] = (void*)&Hooked_Submit;

                g_realPostPresentHandoff = (PostPresentHandoff_t)vtable[7];
                vtable[7] = (void*)&Hooked_PostPresentHandoff;

                VirtualProtect(&vtable[2], sizeof(void*) * 6, oldProtect, &oldProtect);
                s_compositorHooked = true;
                Log("Successfully hooked IVRCompositor VTable!");
            } else {
                Log("Failed to VirtualProtect IVRCompositor VTable!");
            }
        }
    }

    return pReal;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal(vr::EVRInitError *peError, vr::EVRApplicationType eType) {
    LoadReal();
    if (!g_hReal) return 0;
    VR_InitInternal_t realFunc = (VR_InitInternal_t)GetProcAddress(g_hReal, "VR_InitInternal");
    // ALWAYS force VRApplication_Other to avoid the 306 IPC Compositor error!
    if (realFunc) return realFunc(peError, vr::VRApplication_Other);
    return 0;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal2(vr::EVRInitError *peError, vr::EVRApplicationType eType, const char *pStartupInfo) {
    LoadReal();
    if (!g_hReal) return 0;
    VR_InitInternal2_t realFunc = (VR_InitInternal2_t)GetProcAddress(g_hReal, "VR_InitInternal2");
    // ALWAYS force VRApplication_Other to avoid the 306 IPC Compositor error!
    if (realFunc) return realFunc(peError, vr::VRApplication_Other, pStartupInfo);
    return 0;
}

extern "C" __declspec(dllexport) void VR_ShutdownInternal() {
    LoadReal();
    if (g_hReal) {
        VR_ShutdownInternal_t realFunc = (VR_ShutdownInternal_t)GetProcAddress(g_hReal, "VR_ShutdownInternal");
        if (realFunc) realFunc();
    }
}

extern "C" __declspec(dllexport) bool VR_IsHmdPresent() {
    LoadReal();
    if (g_hReal) {
        VR_IsHmdPresent_t realFunc = (VR_IsHmdPresent_t)GetProcAddress(g_hReal, "VR_IsHmdPresent");
        if (realFunc) return realFunc();
    }
    return true; // Assume true if we can't check
}

extern "C" __declspec(dllexport) uint32_t VR_GetInitToken() {
    LoadReal();
    if (g_hReal) {
        typedef uint32_t (*Func_t)();
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_GetInitToken");
        if (f) return f();
    }
    return 1;
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsEnglishDescription(vr::EVRInitError error) {
    LoadReal();
    if (g_hReal) {
        typedef const char* (*Func_t)(vr::EVRInitError);
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_GetVRInitErrorAsEnglishDescription");
        if (f) return f(error);
    }
    return "Unknown Error";
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsSymbol(vr::EVRInitError error) {
    LoadReal();
    if (g_hReal) {
        typedef const char* (*Func_t)(vr::EVRInitError);
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_GetVRInitErrorAsSymbol");
        if (f) return f(error);
    }
    return "VRInitError_Unknown";
}

extern "C" __declspec(dllexport) bool VR_IsInterfaceVersionValid(const char *pchInterfaceVersion) {
    LoadReal();
    if (g_hReal) {
        typedef bool (*Func_t)(const char*);
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_IsInterfaceVersionValid");
        if (f) return f(pchInterfaceVersion);
    }
    return true;
}

extern "C" __declspec(dllexport) bool VR_IsRuntimeInstalled() {
    LoadReal();
    if (g_hReal) {
        typedef bool (*Func_t)();
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_IsRuntimeInstalled");
        if (f) return f();
    }
    return true;
}

extern "C" __declspec(dllexport) bool VR_GetRuntimePath(char *pchPathBuffer, uint32_t unBufferSize, uint32_t *punRequiredBufferSize) {
    LoadReal();
    if (g_hReal) {
        typedef bool (*Func_t)(char*, uint32_t, uint32_t*);
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_GetRuntimePath");
        if (f) return f(pchPathBuffer, unBufferSize, punRequiredBufferSize);
    }
    if (punRequiredBufferSize) *punRequiredBufferSize = 1;
    if (pchPathBuffer && unBufferSize > 0) pchPathBuffer[0] = '\0';
    return true;
}

extern "C" __declspec(dllexport) const char *VR_GetStringForHmdError(vr::EVRInitError error) {
    return VR_GetVRInitErrorAsEnglishDescription(error);
}

extern "C" __declspec(dllexport) void* VRControlPanel() { return nullptr; }
