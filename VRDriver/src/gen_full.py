import re
import os

header_path = "../include/openvr.h"
with open(header_path, "r") as f:
    content = f.read()

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
    out += f"class Mock_{iface} : public vr::{iface} {{\npublic:\n"
    for m in methods:
        const_str = " const " if m['is_const'] else " "
        
        out += f"    virtual {m['ret_type']} {m['name']}({m['args']}){const_str}override {{\n"
        out += f'        FILE* f_{m["name"]} = fopen("vr_emulator_log.txt", "a"); if(f_{m["name"]}) {{ fprintf(f_{m["name"]}, "Called: {iface}::{m["name"]}\\n"); fclose(f_{m["name"]}); }}\n'
        
        if m['name'] == 'CanRenderScene' or m['name'] == 'IsInputAvailable':
            out += "        return true;\n"
        elif m['name'] == 'GetControllerState' or m['name'] == 'GetControllerStateWithPose':
            out += "        if(pControllerState) memset(pControllerState, 0, unControllerStateSize);\n"
            out += "        return true;\n"
        elif m['name'] == 'GetCurrentSceneFocusProcess':
            out += "        return GetCurrentProcessId();\n"
        elif m['name'] == 'PollNextEvent':
            out += "        static int count = 0; if (count == 0) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 0; } return true; } else if (count == 1) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)403; pEvent->trackedDeviceIndex = 0; } return true; } return false;\n"
        elif m['name'] == 'WaitGetPoses':
            out += """        InitSharedMemory();
        if(pRenderPoseArray && unRenderPoseArrayCount > 0) {
            memset(pRenderPoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unRenderPoseArrayCount);
            for(uint32_t i=0; i<3 && i<unRenderPoseArrayCount; ++i) {
                pRenderPoseArray[i].bPoseIsValid = true;
                pRenderPoseArray[i].bDeviceIsConnected = true;
                pRenderPoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                if (i == 1 && pSharedHands && pSharedHands->leftTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->leftTransform[c*4 + r];
                } else if (i == 2 && pSharedHands && pSharedHands->rightTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->rightTransform[c*4 + r];
                } else {
                    pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1; pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1; pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1;
                }
            }
        }
        if(pGamePoseArray && unGamePoseArrayCount > 0) {
            memset(pGamePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unGamePoseArrayCount);
            for(uint32_t i=0; i<3 && i<unGamePoseArrayCount; ++i) {
                pGamePoseArray[i].bPoseIsValid = true;
                pGamePoseArray[i].bDeviceIsConnected = true;
                pGamePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                if (i == 1 && pSharedHands && pSharedHands->leftTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pGamePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->leftTransform[c*4 + r];
                } else if (i == 2 && pSharedHands && pSharedHands->rightTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pGamePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->rightTransform[c*4 + r];
                } else {
                    pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1; pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1; pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1;
                }
            }
        }
        Sleep(11);
        return vr::VRCompositorError_None;
"""
        elif m['name'] == 'Submit':
            out += """        InitSharedMemory();
        if (eEye == vr::Eye_Left && pTexture && pTexture->handle && pHeader && pPixelData && pTexture->eType == vr::TextureType_DirectX) {
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
                        desc.SampleDesc.Count = 1;
                        desc.SampleDesc.Quality = 0;
                        HRESULT hr = pDevice->CreateTexture2D(&desc, nullptr, &pStagingTexture);
                        if (FAILED(hr)) {
                            static int errCount = 0;
                            if (errCount++ < 10) {
                                FILE* f = fopen("vr_emulator_log.txt", "a");
                                if (f) { fprintf(f, "CreateTexture2D failed! HR: %08x, W: %u, H: %u, Fmt: %u\\n", hr, desc.Width, desc.Height, desc.Format); fclose(f); }
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
        elif m['name'] == 'GetTimeSinceLastVsync':
            out += "        if(pfSecondsSinceLastVsync) *pfSecondsSinceLastVsync = 0.0f; if(pulFrameCounter) *pulFrameCounter = 0;\n        return true;\n"
        elif m['name'] == 'GetTrackedDeviceActivityLevel':
            out += "        return vr::k_EDeviceActivityLevel_UserInteraction;\n"
        elif m['name'] == 'GetDeviceToAbsoluteTrackingPose':
            out += "        if(pTrackedDevicePoseArray && unTrackedDevicePoseArrayCount > 0) { memset(pTrackedDevicePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unTrackedDevicePoseArrayCount); for(uint32_t i=0; i<3 && i<unTrackedDevicePoseArrayCount; ++i) { pTrackedDevicePoseArray[i].bPoseIsValid = true; pTrackedDevicePoseArray[i].bDeviceIsConnected = true; pTrackedDevicePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK; pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1; pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1; pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1; } }\n"
        elif m['name'] == 'IsTrackedDeviceConnected':
            out += "        if(unDeviceIndex == 0 || unDeviceIndex == 1 || unDeviceIndex == 2) return true;\n        return false;\n"
        elif m['name'] == 'GetTrackedDeviceClass':
            out += "        if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD;\n        if(unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller;\n        return vr::TrackedDeviceClass_Invalid;\n"
        elif m['name'] == 'GetProjectionMatrix':
            out += "        vr::HmdMatrix44_t temp; memset(&temp, 0, sizeof(temp)); temp.m[0][0] = 1; temp.m[1][1] = 1; temp.m[2][2] = 1; temp.m[3][3] = 1; return temp;\n"
        elif m['name'] == 'GetEyeToHeadTransform':
            out += "        vr::HmdMatrix34_t temp; memset(&temp, 0, sizeof(temp)); temp.m[0][0] = 1; temp.m[1][1] = 1; temp.m[2][2] = 1; if(eEye == vr::Eye_Left) temp.m[0][3] = -0.0315f; else temp.m[0][3] = 0.0315f; return temp;\n"
        elif m['name'] == 'GetMatrix34TrackedDeviceProperty':
            out += "        vr::HmdMatrix34_t temp; memset(&temp, 0, sizeof(temp)); temp.m[0][0] = 1; temp.m[1][1] = 1; temp.m[2][2] = 1; if(pError) *pError = vr::TrackedProp_Success; return temp;\n"
        elif m['name'] == 'GetFloatTrackedDeviceProperty':
            out += """        if(prop == vr::Prop_DisplayFrequency_Float) { if(pError) *pError = vr::TrackedProp_Success; return 90.0f; }
        if(prop == vr::Prop_UserIpdMeters_Float) { if(pError) *pError = vr::TrackedProp_Success; return 0.063f; }
        if(prop == vr::Prop_SecondsFromVsyncToPhotons_Float) { if(pError) *pError = vr::TrackedProp_Success; return 0.011f; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return 0.0f;
"""
        elif m['name'] == 'GetInt32TrackedDeviceProperty':
            out += """        if(prop == vr::Prop_DeviceClass_Int32) { if(pError) *pError = vr::TrackedProp_Success; if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD; if(unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller; }
        if(prop == vr::Prop_ControllerRoleHint_Int32) { if(pError) *pError = vr::TrackedProp_Success; if(unDeviceIndex == 1) return vr::TrackedControllerRole_LeftHand; if(unDeviceIndex == 2) return vr::TrackedControllerRole_RightHand; }
        if(prop == vr::Prop_Axis0Type_Int32 || prop == vr::Prop_Axis1Type_Int32) { if(pError) *pError = vr::TrackedProp_Success; return vr::k_eControllerAxis_TrackPad; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return 0;
"""
        elif m['name'] == 'GetUint64TrackedDeviceProperty':
            out += """        if(prop == vr::Prop_CurrentUniverseId_Uint64) { if(pError) *pError = vr::TrackedProp_Success; return 1; }
        if(prop == vr::Prop_SupportedButtons_Uint64) { if(pError) *pError = vr::TrackedProp_Success; return 0xFFFFFFFFFFFFFFFFULL; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return 0;
"""
        elif m['name'] == 'GetBoolTrackedDeviceProperty':
            out += """        if(prop == vr::Prop_ContainsProximitySensor_Bool) { if(pError) *pError = vr::TrackedProp_Success; return true; }
        if(prop == vr::Prop_DeviceIsWireless_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(prop == vr::Prop_DeviceIsCharging_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(prop == vr::Prop_WillDriftInYaw_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(prop == vr::Prop_DeviceProvidesBatteryStatus_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return false;
"""
        elif m['name'] == 'GetStringTrackedDeviceProperty':
            out += """        const char* s = nullptr;
        if (prop == vr::Prop_RenderModelName_String) {
            if (unDeviceIndex == 0) s = "generic_hmd";
            else if (unDeviceIndex == 1) s = "{indexcontroller}valve_controller_knu_1_0_left";
            else if (unDeviceIndex == 2) s = "{indexcontroller}valve_controller_knu_1_0_right";
        }
        else if (prop == vr::Prop_ControllerType_String) s = "knuckles";
        else if (prop == vr::Prop_TrackingSystemName_String) s = "lighthouse";
        else if (prop == vr::Prop_ManufacturerName_String) s = "Valve";
        else if (prop == vr::Prop_ModelNumber_String) s = "Valve Index";
        else if (prop == vr::Prop_SerialNumber_String) s = "IDX123456";
        
        if (s) {
            if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\\0'; }
            if(pError) *pError = vr::TrackedProp_Success;
            return (uint32_t)strlen(s) + 1;
        } else {
            if(pError) *pError = vr::TrackedProp_UnknownProperty;
            return 0;
        }
"""
        elif m['name'] == 'GetEyeOutputViewport':
            out += "        if(pnX) *pnX = 0; if(pnY) *pnY = 0; if(pnWidth) *pnWidth = 1920; if(pnHeight) *pnHeight = 1080;\n"
        elif m['name'] == 'GetProjectionRaw':
            out += "        if(pfLeft) *pfLeft = -1.0f; if(pfRight) *pfRight = 1.0f; if(pfTop) *pfTop = -1.0f; if(pfBottom) *pfBottom = 1.0f;\n"
        elif m['name'] == 'GetRecommendedRenderTargetSize':
            out += "        if(pnWidth) *pnWidth = 1920;\n        if(pnHeight) *pnHeight = 1080;\n"
        elif m['name'] == 'GetActionSetHandle':
            out += "        static uint64_t nextSetHandle = 1000;\n        if(pHandle) *pHandle = nextSetHandle++;\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetInputSourceHandle':
            out += "        static uint64_t nextSourceHandle = 10000;\n        if(pHandle) { *pHandle = nextSourceHandle++; if(pchInputSourcePath) { if(strstr(pchInputSourcePath, \"left\")) *pHandle = 1; else if(strstr(pchInputSourcePath, \"right\")) *pHandle = 2; } }\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetActionHandle':
            out += "        static uint64_t nextHandle = 10;\n        if(pHandle) { *pHandle = nextHandle++; }\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetDigitalActionData':
            out += """        if(pActionData && unActionDataSize > 0) {
            vr::InputDigitalActionData_t temp = {0};
            temp.bActive = true;
            if (pSharedHands) {
                bool pressed = false;
                if (ulRestrictToDevice == 1) pressed = pSharedHands->leftPinch;
                else if (ulRestrictToDevice == 2) pressed = pSharedHands->rightPinch;
                else pressed = pSharedHands->leftPinch || pSharedHands->rightPinch;
                temp.bState = pressed;
            }
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
"""
        elif m['name'] == 'GetPoseActionDataRelativeToNow' or m['name'] == 'GetPoseActionDataForNextFrame':
            out += """        if(pActionData && unActionDataSize > 0) {
            vr::InputPoseActionData_t temp = {0};
            temp.bActive = true;
            temp.pose.bPoseIsValid = true;
            temp.pose.bDeviceIsConnected = true;
            temp.pose.eTrackingResult = vr::TrackingResult_Running_OK;
            temp.pose.mDeviceToAbsoluteTracking.m[0][0] = 1; temp.pose.mDeviceToAbsoluteTracking.m[1][1] = 1; temp.pose.mDeviceToAbsoluteTracking.m[2][2] = 1;
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
"""
        elif m['name'] == 'GetSkeletalActionData':
            out += "        if(pActionData && unActionDataSize > 0) { vr::InputSkeletalActionData_t temp = {0}; temp.bActive = false; memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize); }\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetAnalogActionData':
            out += "        if(pActionData && unActionDataSize > 0) { vr::InputAnalogActionData_t temp = {0}; temp.bActive = true; memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize); }\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetActionOrigins':
            out += "        if(originsOut && originOutCount > 0) { memset(originsOut, 0, sizeof(vr::VRInputValueHandle_t) * originOutCount); originsOut[0] = actionSetHandle; }\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetOriginTrackedDeviceInfo':
            out += "        if(pOriginInfo && unOriginInfoSize > 0) { vr::InputOriginInfo_t temp = {0}; temp.devicePath = origin; temp.trackedDeviceIndex = (origin == 1) ? 1 : 2; memcpy(pOriginInfo, &temp, unOriginInfoSize > sizeof(temp) ? sizeof(temp) : unOriginInfoSize); }\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'GetActionBindingInfo':
            out += "        if(pOriginInfo && unBindingInfoSize > 0 && unBindingInfoCount > 0) { memset(pOriginInfo, 0, unBindingInfoSize * unBindingInfoCount); }\n        if(punReturnedBindingInfoCount) *punReturnedBindingInfoCount = 0;\n        return vr::VRInputError_None;\n"
        elif m['name'] == 'UpdateActionState':
            out += "        return vr::VRInputError_None;\n"
        elif m['name'] == 'WaitGetPoses':
            out += """        if(pRenderPoseArray && unRenderPoseArrayCount > 0) {
            memset(pRenderPoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unRenderPoseArrayCount);
            for(uint32_t i=0; i<3 && i<unRenderPoseArrayCount; ++i) {
                pRenderPoseArray[i].bPoseIsValid = true;
                pRenderPoseArray[i].bDeviceIsConnected = true;
                pRenderPoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1;
                pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1;
                pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1;
            }
        }
        if(pGamePoseArray && unGamePoseArrayCount > 0) {
            memset(pGamePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unGamePoseArrayCount);
            for(uint32_t i=0; i<3 && i<unGamePoseArrayCount; ++i) {
                pGamePoseArray[i].bPoseIsValid = true;
                pGamePoseArray[i].bDeviceIsConnected = true;
                pGamePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1;
                pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1;
                pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1;
            }
        }
        Sleep(11);
        return vr::VRCompositorError_None;
"""
        elif m['name'] == 'ComputeDistortion':
            out += "        if(pDistortionCoordinates) { memset(pDistortionCoordinates, 0, sizeof(*pDistortionCoordinates)); }\n        return true;\n"
        elif m['name'] == 'GetRuntimeVersion':
            out += """        return "1.23.0";\n"""
        elif m['name'] == 'GetAppContainerFilePaths':
            out += """        if(pchBuffer && unBufferSize > 0) pchBuffer[0] = '\\0';\n        return 0;\n"""
        elif m['name'] == 'GetPropErrorNameFromEnum' or m['name'] == 'GetEventTypeNameFromEnum' or m['name'] == 'GetButtonIdNameFromEnum' or m['name'] == 'GetControllerAxisTypeNameFromEnum' or m['name'] == 'GetApplicationsErrorNameFromEnum' or m['name'] == 'GetSettingsErrorNameFromEnum':
            out += """        return "Unknown";\n"""
        elif m['name'] == 'GetString':
            out += "        if(peError) *peError = vr::VRSettingsError_None;\n        if(pchValue && unValueLen > 0) pchValue[0] = '\\0';\n"
        elif m['name'] == 'GetVulkanInstanceExtensionsRequired' or m['name'] == 'GetVulkanDeviceExtensionsRequired':
            out += "        if(pchValue && unBufferSize > 0) pchValue[0] = '\\0';\n        return 0;\n"
        else:
            if '*' in m['ret_type'] or m['ret_type'] == 'void*':
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
extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    if (peError) *peError = vr::VRInitError_None;
    
    FILE* f = fopen("vr_emulator_log.txt", "a");
    if(f) { fprintf(f, "VR_GetGenericInterface: %s\\n", pchInterfaceVersion); fclose(f); }

    if (strstr(pchInterfaceVersion, "IVRSystem_")) return &g_mockSystem;
    if (strstr(pchInterfaceVersion, "IVRCompositor_")) return &g_mockCompositor;
    if (strstr(pchInterfaceVersion, "IVRInput_")) return &g_mockInput;

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
        FILE* f2 = fopen("vr_emulator_log.txt", "a");
        if(f2) { fprintf(f2, " -> Returned REAL\\n"); fclose(f2); }
        return pReal;
    }
"""
for iface in found_interfaces:
    out += f'    if (strstr(pchInterfaceVersion, "{iface[3:]}")) {{ FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) {{ fprintf(f3, " -> Returned MOCK {iface[3:]}\\n"); fclose(f3); }} return &g_mock{iface[3:]}; }}\n'

out += """
    FILE* f4 = fopen("vr_emulator_log.txt", "a");
    if(f4) { fprintf(f4, " -> Returned NULL/UNIVERSAL MOCK\\n"); fclose(f4); }
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
        if (f) return f(peError, vr::VRApplication_Other, pStartupInfo);
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
    return "Unknown Error";
}

extern "C" __declspec(dllexport) void* VRControlPanel() { return nullptr; }
"""

with open("full_proxy.cpp", "w") as f:
    f.write(out)
print("Generated full_proxy.cpp!")
