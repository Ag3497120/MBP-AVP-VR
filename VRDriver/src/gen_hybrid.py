import re
import os

header_path = "../include/openvr.h"
with open(header_path, "r") as f:
    content = f.read()

# Remove C++ comments
content = re.sub(r'//.*', '', content)
content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

interfaces = {}
class_matches = re.finditer(r'class\s+(IVR[A-Za-z0-9_]+)\s*\{([^}]*)\};', content)

for cm in class_matches:
    iface_name = cm.group(1)
    body = cm.group(2)
    interfaces[iface_name] = []
    
    method_matches = re.finditer(r'virtual\s+(.*?[\s\*&])(\w+)\s*\((.*?)\)\s*(const)?\s*=\s*0\s*;', body, re.DOTALL)
    for mm in method_matches:
        ret_type = mm.group(1).strip()
        name = mm.group(2).strip()
        args = mm.group(3).strip()
        args = re.sub(r'\s+', ' ', args)
        is_const = bool(mm.group(4))
        interfaces[iface_name].append({
            'ret_type': ret_type,
            'name': name,
            'args': args,
            'is_const': is_const
        })

out = """#include "openvr.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <d3d11.h>

struct SharedFrame {
    uint32_t sequenceNumber;
    uint32_t width;
    uint32_t height;
    uint32_t format;
};

#pragma pack(push, 1)
struct SharedHands {
    float leftTransform[16];
    float rightTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
};
#pragma pack(pop)

static HANDLE hMapFile = NULL;
static void* pBuf = NULL;
static SharedFrame* pHeader = NULL;
static uint8_t* pPixelData = NULL;
static SharedHands* pSharedHands = NULL;
static uint32_t frameSeq = 1;
static ID3D11Texture2D* pStagingTexture = NULL;

static void InitSharedMemory() {
    if (hMapFile) return;
    const int mapSize = 16 + 1920 * 1080 * 4 + 130;
    HANDLE hFile = CreateFileA("C:\\\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, mapSize, NULL);
        if (hMapFile) {
            pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, mapSize);
            if (pBuf) {
                pHeader = (SharedFrame*)pBuf;
                pPixelData = (uint8_t*)pBuf + sizeof(SharedFrame);
                pSharedHands = (SharedHands*)((uint8_t*)pBuf + 16 + 1920 * 1080 * 4);
            }
        }
    }
}

static HMODULE g_hReal = NULL;
static void LoadReal() {
    if (g_hReal) return;
    g_hReal = LoadLibraryA("openvr_api_real.dll");
}

class UniversalMock {
public:
"""
for i in range(100):
    out += f"    virtual void* Dummy{i}() {{ return nullptr; }}\n"
out += """} g_universalMock;

using namespace vr;

"""

found_interfaces = []

for iface, methods in interfaces.items():
    found_interfaces.append(iface)
    out += f"class Mock_{iface} : public vr::{iface} {{\n"
    out += "public:\n"
    for m in methods:
        params_def = m['args']
        const_str = " const" if m['is_const'] else ""
        out += f"    virtual {m['ret_type']} {m['name']}({params_def}){const_str} override {{\n"
        out += f"        FILE* f_{m['name']} = fopen(\"vr_emulator_log.txt\", \"a\"); if(f_{m['name']}) {{ fprintf(f_{m['name']}, \"Called: {iface}::{m['name']}\\n\"); fclose(f_{m['name']}); }}\n"
        
        if m['name'] == 'WaitGetPoses':
            out += """        LoadReal();
        if (g_hReal) {
            typedef void* (*VR_GetGenericInterface_t)(const char*, vr::EVRInitError*);
            VR_GetGenericInterface_t f = (VR_GetGenericInterface_t)GetProcAddress(g_hReal, "VR_GetGenericInterface");
            if (f) {
                vr::EVRInitError err;
                vr::IVRSystem* sys = (vr::IVRSystem*)f(vr::IVRSystem_Version, &err);
                if (sys) {
                    if (pRenderPoseArray && unRenderPoseArrayCount > 0) {
                        sys->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, pRenderPoseArray, unRenderPoseArrayCount);
                        for(uint32_t i=0; i<3 && i<unRenderPoseArrayCount; ++i) {
                            pRenderPoseArray[i].bPoseIsValid = true;
                            pRenderPoseArray[i].bDeviceIsConnected = true;
                            pRenderPoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                            if (pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] == 0.0f) {
                                pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                                pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                                pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                            }
                        }
                    }
                    if (pGamePoseArray && unGamePoseArrayCount > 0) {
                        sys->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0.0f, pGamePoseArray, unGamePoseArrayCount);
                        for(uint32_t i=0; i<3 && i<unGamePoseArrayCount; ++i) {
                            pGamePoseArray[i].bPoseIsValid = true;
                            pGamePoseArray[i].bDeviceIsConnected = true;
                            pGamePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                            if (pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] == 0.0f) {
                                pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                                pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                                pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                            }
                        }
                    }
                }
            }
        }
        Sleep(11); // Block for ~90fps
        return vr::VRCompositorError_None;
"""
        elif m['name'] == 'Submit':
            out += """        InitSharedMemory();
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
                    
                    static uint32_t lastWidth = 0;
                    static uint32_t lastHeight = 0;
                    if (pStagingTexture && (lastWidth != desc.Width || lastHeight != desc.Height)) {
                        pStagingTexture->Release();
                        pStagingTexture = nullptr;
                    }
                    
                    if (!pStagingTexture) {
                        desc.Usage = D3D11_USAGE_STAGING;
                        desc.BindFlags = 0;
                        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                        desc.MiscFlags = 0;
                        HRESULT hr = pDevice->CreateTexture2D(&desc, nullptr, &pStagingTexture);
                        if (FAILED(hr)) {
                            static int errCount = 0;
                            if (errCount++ < 10) {
                                FILE* f = fopen("vr_emulator_log.txt", "a");
                                if (f) { fprintf(f, "CreateTexture2D failed! HR: %08x, W: %u, H: %u, Fmt: %u, Samples: %u\\n", hr, desc.Width, desc.Height, desc.Format, desc.SampleDesc.Count); fclose(f); }
                            }
                        }
                        lastWidth = desc.Width;
                        lastHeight = desc.Height;
                    }
                    
                    if (pStagingTexture) {
                        pContext->CopyResource(pStagingTexture, pTex);
                        D3D11_MAPPED_SUBRESOURCE mapped;
                        HRESULT mapHr = pContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mapped);
                        if (SUCCEEDED(mapHr)) {
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
                        } else {
                            static int mapErrCount = 0;
                            if (mapErrCount++ < 10) {
                                FILE* f = fopen("vr_emulator_log.txt", "a");
                                if (f) { fprintf(f, "Map failed! HR: %08x\\n", mapHr); fclose(f); }
                            }
                        }
                    }
                    pContext->Release();
                }
                pDevice->Release();
            }
        }
        return vr::VRCompositorError_None;
"""
        else:
            if m['name'] == 'GetSceneApplicationState':
                out += '        return vr::EVRSceneApplicationState_Running;\n'
            elif m['ret_type'] == 'const char *' or m['ret_type'] == 'char *':
                out += '        return "Unknown";\n'
            elif '*' in m['ret_type'] or m['ret_type'] == 'void*':
                out += "        return nullptr;\n"
            elif m['ret_type'] == 'void':
                out += "        return;\n"
            elif m['ret_type'] == 'bool':
                out += "        return false;\n"
            elif m['ret_type'].endswith('_t') and '*' not in m['ret_type']:
                out += f"        {m['ret_type']} temp; memset(&temp, 0, sizeof(temp)); return temp;\n"
            else:
                out += f"        return ({m['ret_type']})0;\n"
        
        out += "    }\n"
        
    for i in range(100):
        out += f"    virtual void* DummyPadding{i}() {{ return nullptr; }}\n"
        
    out += f"}} g_mock{iface[3:]};\n\n"


out += """
// ----------------------------------------------------------------------------
// Hooks
// ----------------------------------------------------------------------------

typedef uint32_t (__thiscall *GetStringTrackedDeviceProperty_t)(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError *pError);
static GetStringTrackedDeviceProperty_t g_origGetStringTrackedDeviceProperty = nullptr;

uint32_t __fastcall Hooked_GetStringTrackedDeviceProperty(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex, vr::ETrackedDeviceProperty prop, char *pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError *pError) {
    if (unDeviceIndex == 1 || unDeviceIndex == 2) {
        const char* s = nullptr;
        if (prop == vr::Prop_RenderModelName_String) {
            if (unDeviceIndex == 1) s = "{indexcontroller}valve_controller_knu_1_0_left";
            else if (unDeviceIndex == 2) s = "{indexcontroller}valve_controller_knu_1_0_right";
        }
        else if (prop == vr::Prop_ControllerType_String) s = "knuckles";
        else if (prop == vr::Prop_TrackingSystemName_String) s = "lighthouse";
        else if (prop == vr::Prop_ManufacturerName_String) s = "Valve";
        else if (prop == vr::Prop_ModelNumber_String) s = "Valve Index";
        
        if (s) {
            if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\\0'; }
            if(pError) *pError = vr::TrackedProp_Success;
            return (uint32_t)strlen(s) + 1;
        }
    }
    return g_origGetStringTrackedDeviceProperty(_this, unDeviceIndex, prop, pchValue, unBufferSize, pError);
}

typedef vr::ETrackedDeviceClass (__thiscall *GetTrackedDeviceClass_t)(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex);
static GetTrackedDeviceClass_t g_origGetTrackedDeviceClass = nullptr;

vr::ETrackedDeviceClass __fastcall Hooked_GetTrackedDeviceClass(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex) {
    if (unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller;
    return g_origGetTrackedDeviceClass(_this, unDeviceIndex);
}

typedef bool (__thiscall *IsTrackedDeviceConnected_t)(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex);
static IsTrackedDeviceConnected_t g_origIsTrackedDeviceConnected = nullptr;

bool __fastcall Hooked_IsTrackedDeviceConnected(void* _this, vr::TrackedDeviceIndex_t unDeviceIndex) {
    if (unDeviceIndex == 1 || unDeviceIndex == 2) return true;
    return g_origIsTrackedDeviceConnected(_this, unDeviceIndex);
}

typedef vr::EVRCompositorError (__thiscall *WaitGetPoses_t)(void* _this, vr::TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, vr::TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount);
static WaitGetPoses_t g_origWaitGetPoses = nullptr;

vr::EVRCompositorError __fastcall Hooked_WaitGetPoses(void* _this, vr::TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, vr::TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) {
    vr::EVRCompositorError err = g_origWaitGetPoses(_this, pRenderPoseArray, unRenderPoseArrayCount, pGamePoseArray, unGamePoseArrayCount);
    for(uint32_t i=0; i<3 && i<unRenderPoseArrayCount; ++i) {
        pRenderPoseArray[i].bPoseIsValid = true;
        pRenderPoseArray[i].bDeviceIsConnected = true;
        pRenderPoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
        if (pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] == 0.0f) {
            pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
            pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
            pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
        }
    }
    for(uint32_t i=0; i<3 && i<unGamePoseArrayCount; ++i) {
        pGamePoseArray[i].bPoseIsValid = true;
        pGamePoseArray[i].bDeviceIsConnected = true;
        pGamePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
        if (pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] == 0.0f) {
            pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
            pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
            pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
        }
    }
    Sleep(11);
    return vr::VRCompositorError_None;
}

typedef vr::EVRCompositorError (__thiscall *Submit_t)(void* _this, vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags);
static Submit_t g_origSubmit = nullptr;

vr::EVRCompositorError __fastcall Hooked_Submit(void* _this, vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags) {
    FILE* log_f = fopen("vr_emulator_log.txt", "a");
    if(log_f) {
        fprintf(log_f, "Called: IVRCompositor::Submit(Eye: %d)\n", (int)eEye);
        fclose(log_f);
    }
    
    if (eEye == vr::Eye_Left) {
        static uint32_t seq = 0;
        seq++;
        // Write to temp file then rename to avoid file lock collisions with Python viewer
        FILE* f = fopen("/Users/motonishikoudai/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame_tmp.dat", "wb");
        if(f) {
            uint32_t header[4] = {seq, 956, 1076, 27};
            fwrite(header, sizeof(uint32_t), 4, f);
            fclose(f);
            rename("/Users/motonishikoudai/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame_tmp.dat", "/Users/motonishikoudai/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat");
        } else {
            if(log_f = fopen("vr_emulator_log.txt", "a")) {
                fprintf(log_f, "Submit Error: Failed to open vr_shared_frame_tmp.dat for writing!\n");
                fclose(log_f);
            }
        }
    }
    return g_origSubmit(_this, eEye, pTexture, pBounds, nSubmitFlags);
}

static bool g_bIVRSystemHooked = false;
static bool g_bIVRCompositorHooked = false;

extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    if (peError) *peError = vr::VRInitError_None;
    
    // Attempt to get the REAL interface from SteamVR
    LoadReal();
    void* pReal = nullptr;
    if (g_hReal) {
        typedef void* (*VR_GetGenericInterface_t)(const char*, vr::EVRInitError*);
        VR_GetGenericInterface_t realFunc = (VR_GetGenericInterface_t)GetProcAddress(g_hReal, "VR_GetGenericInterface");
        if (realFunc) {
            vr::EVRInitError realErr;
            pReal = realFunc(pchInterfaceVersion, &realErr);
        }
    }

    if (pReal) {
        FILE* f = fopen("vr_emulator_log.txt", "a");
        if(f) { fprintf(f, "Requested interface: %s (Returned REAL)\\n", pchInterfaceVersion); fclose(f); }
        
        // Hook IVRSystem_021
        std::string iface(pchInterfaceVersion);
        if (iface == "IVRSystem_021" && !g_bIVRSystemHooked) {
            g_bIVRSystemHooked = true;
            void** vtable = *(void***)pReal;
            DWORD oldProtect;
            VirtualProtect(vtable, sizeof(void*) * 100, PAGE_EXECUTE_READWRITE, &oldProtect);
            g_origGetStringTrackedDeviceProperty = (GetStringTrackedDeviceProperty_t)vtable[28];
            vtable[28] = (void*)Hooked_GetStringTrackedDeviceProperty;
            g_origGetTrackedDeviceClass = (GetTrackedDeviceClass_t)vtable[20];
            vtable[20] = (void*)Hooked_GetTrackedDeviceClass;
            g_origIsTrackedDeviceConnected = (IsTrackedDeviceConnected_t)vtable[21];
            vtable[21] = (void*)Hooked_IsTrackedDeviceConnected;
            VirtualProtect(vtable, sizeof(void*) * 100, oldProtect, &oldProtect);
        }

        // Hook IVRCompositor
        if (iface.find("IVRCompositor") != std::string::npos && !g_bIVRCompositorHooked) {
            g_bIVRCompositorHooked = true;
            void** vtable = *(void***)pReal;
            DWORD oldProtect;
            VirtualProtect(vtable, sizeof(void*) * 100, PAGE_EXECUTE_READWRITE, &oldProtect);
            // WaitGetPoses is index 2
            g_origWaitGetPoses = (WaitGetPoses_t)vtable[2];
            vtable[2] = (void*)Hooked_WaitGetPoses;
            // Submit is index 6
            g_origSubmit = (Submit_t)vtable[6];
            vtable[6] = (void*)Hooked_Submit;
            VirtualProtect(vtable, sizeof(void*) * 100, oldProtect, &oldProtect);
        }
        
        return pReal;
    }

    FILE* f = fopen("vr_emulator_log.txt", "a");
    if(f) { fprintf(f, "Requested interface: %s (Returned MOCK)\\n", pchInterfaceVersion); fclose(f); }
"""

for iface in found_interfaces:
    out += f'    if (strstr(pchInterfaceVersion, "{iface[3:]}")) return &g_mock{iface[3:]};\n'

out += """
    return &g_universalMock;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal(vr::EVRInitError *peError, vr::EVRApplicationType eType) {
    LoadReal();
    if (g_hReal) {
        typedef uint32_t (*VR_InitInternal_t)(vr::EVRInitError*, vr::EVRApplicationType);
        VR_InitInternal_t f = (VR_InitInternal_t)GetProcAddress(g_hReal, "VR_InitInternal");
        if (f) return f(peError, vr::VRApplication_Other);
    }
    return 0;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal2(vr::EVRInitError *peError, vr::EVRApplicationType eType, const char *pStartupInfo) {
    LoadReal();
    if (g_hReal) {
        typedef uint32_t (*VR_InitInternal2_t)(vr::EVRInitError*, vr::EVRApplicationType, const char*);
        VR_InitInternal2_t f = (VR_InitInternal2_t)GetProcAddress(g_hReal, "VR_InitInternal2");
        if (f) {
            uint32_t token = f(peError, vr::VRApplication_Other, pStartupInfo);
            FILE* logF = fopen("vr_emulator_log.txt", "a");
            if(logF) { fprintf(logF, "VR_InitInternal2(Other) -> err: %d, token: %u\\n", peError ? *peError : 0, token); fclose(logF); }
            return token;
        }
    }
    return 0;
}

extern "C" __declspec(dllexport) void VR_ShutdownInternal() {
    LoadReal();
    if (g_hReal) {
        typedef void (*VR_ShutdownInternal_t)();
        VR_ShutdownInternal_t f = (VR_ShutdownInternal_t)GetProcAddress(g_hReal, "VR_ShutdownInternal");
        if (f) f();
    }
}

extern "C" __declspec(dllexport) bool VR_IsHmdPresent() {
    LoadReal();
    if (g_hReal) {
        typedef bool (*Func_t)();
        Func_t f = (Func_t)GetProcAddress(g_hReal, "VR_IsHmdPresent");
        if (f) return f();
    }
    return true;
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
    if (pchPathBuffer && unBufferSize > 0) pchPathBuffer[0] = '\\0';
    return true;
}

extern "C" __declspec(dllexport) const char *VR_GetStringForHmdError(vr::EVRInitError error) {
    return VR_GetVRInitErrorAsEnglishDescription(error);
}

extern "C" __declspec(dllexport) void* VRControlPanel() { return nullptr; }
"""


with open("hybrid_proxy.cpp", "w") as f:
    f.write(out)
print("Generated hybrid_proxy.cpp!")
