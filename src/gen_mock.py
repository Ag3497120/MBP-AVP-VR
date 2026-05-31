import re
import os

header_path = "../include/openvr.h"

with open(header_path, "r") as f:
    content = f.read()

# Remove C++ comments
content = re.sub(r'//.*', '', content)
content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

interfaces = {}

# Find all classes
class_matches = re.finditer(r'class\s+(IVR[A-Za-z0-9_]+)\s*\{([^}]*)\};', content)

for cm in class_matches:
    iface_name = cm.group(1)
    body = cm.group(2)
    interfaces[iface_name] = []
    
    # Extract virtual methods
    # We look for: virtual [ret_type] [name]([args]) [const] = 0;
    # Body can have newlines, so we use re.DOTALL or just normal match over the body
    method_matches = re.finditer(r'virtual\s+(.*?[\s\*&])(\w+)\s*\((.*?)\)\s*(const)?\s*=\s*0\s*;', body, re.DOTALL)
    for mm in method_matches:
        ret_type = mm.group(1).strip()
        name = mm.group(2).strip()
        args = mm.group(3).strip()
        # Clean up newlines in args
        args = re.sub(r'\s+', ' ', args)
        is_const = bool(mm.group(4))
        interfaces[iface_name].append({
            'ret_type': ret_type,
            'name': name,
            'args': args,
            'is_const': is_const
        })

out = """#include "openvr.h"
#include <windows.h>
#include <iostream>
#include <string.h>

using namespace vr;

class UniversalMock {
public:
"""
for i in range(100):
    out += f"    virtual void* Dummy{i}() {{ return nullptr; }}\n"
out += """};
UniversalMock g_universalMock;

"""

found_interfaces = []
for iface, methods in interfaces.items():
    found_interfaces.append(iface)
    out += f"class Mock_{iface} : public vr::{iface} {{\npublic:\n"
    for m in methods:
        const_str = " const " if m['is_const'] else " "
        
        out += f"    virtual {m['ret_type']} {m['name']}({m['args']}){const_str}override {{\n"
        
        # Audio Played State (State B) Overrides + Error Fallbacks
        
        if m['name'] == 'CanRenderScene' or m['name'] == 'IsInputAvailable':
            out += "        return true;\n"
        elif m['name'] == 'GetCurrentSceneFocusProcess':
            out += "        return GetCurrentProcessId();\n"
        elif m['name'] == 'PollNextEvent':
            out += "        static int count = 0; if (count == 0) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 0; } return true; } else if (count == 1) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)403; pEvent->trackedDeviceIndex = 0; } return true; } return false;\n"
        elif m['name'] == 'WaitGetPoses':
            out += "        if(pRenderPoseArray && unRenderPoseArrayCount > 0) { memset(pRenderPoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unRenderPoseArrayCount); pRenderPoseArray[0].bPoseIsValid = true; pRenderPoseArray[0].bDeviceIsConnected = true; pRenderPoseArray[0].eTrackingResult = vr::TrackingResult_Running_OK; pRenderPoseArray[0].mDeviceToAbsoluteTracking.m[0][0] = 1; pRenderPoseArray[0].mDeviceToAbsoluteTracking.m[1][1] = 1; pRenderPoseArray[0].mDeviceToAbsoluteTracking.m[2][2] = 1; }\n"
            out += "        if(pGamePoseArray && unGamePoseArrayCount > 0) { memset(pGamePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unGamePoseArrayCount); pGamePoseArray[0].bPoseIsValid = true; pGamePoseArray[0].bDeviceIsConnected = true; pGamePoseArray[0].eTrackingResult = vr::TrackingResult_Running_OK; pGamePoseArray[0].mDeviceToAbsoluteTracking.m[0][0] = 1; pGamePoseArray[0].mDeviceToAbsoluteTracking.m[1][1] = 1; pGamePoseArray[0].mDeviceToAbsoluteTracking.m[2][2] = 1; }\n"
            out += "        Sleep(11);\n"
            out += "        return vr::VRCompositorError_None;\n"
        elif m['name'] == 'Submit':
            out += "        return vr::VRCompositorError_None;\n"
        elif m['name'] == 'GetDeviceToAbsoluteTrackingPose':
            out += "        if(pTrackedDevicePoseArray && unTrackedDevicePoseArrayCount > 0) { memset(pTrackedDevicePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unTrackedDevicePoseArrayCount); if(unTrackedDevicePoseArrayCount > 0) { pTrackedDevicePoseArray[0].bPoseIsValid = true; pTrackedDevicePoseArray[0].bDeviceIsConnected = true; pTrackedDevicePoseArray[0].eTrackingResult = vr::TrackingResult_Running_OK; pTrackedDevicePoseArray[0].mDeviceToAbsoluteTracking.m[0][0] = 1; pTrackedDevicePoseArray[0].mDeviceToAbsoluteTracking.m[1][1] = 1; pTrackedDevicePoseArray[0].mDeviceToAbsoluteTracking.m[2][2] = 1; } }\n"
        elif m['name'] == 'IsTrackedDeviceConnected':
            out += "        if(unDeviceIndex == 0) return true;\n"
            out += "        return false;\n"
        elif m['name'] == 'GetTrackedDeviceClass':
            out += "        if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD;\n"
            out += "        return vr::TrackedDeviceClass_Invalid;\n"
        elif m['name'] == 'GetEyeToHeadTransform':
            out += "        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; }\n"
        elif m['name'] == 'GetMatrix34TrackedDeviceProperty':
            out += "        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; }\n"
            out += "        if(pError) *pError = vr::TrackedProp_Success;\n"
        elif m['name'] == 'GetFloatTrackedDeviceProperty':
            out += "        if(pError) *pError = vr::TrackedProp_Success;\n"
            out += "        if(prop == vr::Prop_DisplayFrequency_Float) return 90.0f;\n"
            out += "        return 0.0f;\n"
        elif m['name'] == 'GetInt32TrackedDeviceProperty':
            out += "        if(pError) *pError = vr::TrackedProp_Success;\n"
            out += "        if(prop == vr::Prop_DeviceClass_Int32) return vr::TrackedDeviceClass_HMD;\n"
            out += "        return 0;\n"
        elif m['name'] == 'GetUint64TrackedDeviceProperty':
            out += "        if(pError) *pError = vr::TrackedProp_Success;\n"
            out += "        if(prop == vr::Prop_CurrentUniverseId_Uint64) return 1;\n"
            out += "        return 0;\n"
        elif m['name'] == 'GetEyeOutputViewport':
            out += "        if(pnX) *pnX = 0; if(pnY) *pnY = 0; if(pnWidth) *pnWidth = 1920; if(pnHeight) *pnHeight = 1080;\n"
        elif m['name'] == 'GetProjectionRaw':
            out += "        if(pfLeft) *pfLeft = -1.0f; if(pfRight) *pfRight = 1.0f; if(pfTop) *pfTop = -1.0f; if(pfBottom) *pfBottom = 1.0f;\n"
        elif m['name'] == 'GetTimeSinceLastVsync':
            out += "        if(pfSecondsSinceLastVsync) *pfSecondsSinceLastVsync = 0.0f; if(pulFrameCounter) *pulFrameCounter = 0;\n"
            out += "        return true;\n"
        elif m['name'] == 'GetRecommendedRenderTargetSize':
            out += "        if(pnWidth) *pnWidth = 1920;\n"
            out += "        if(pnHeight) *pnHeight = 1080;\n"
        elif m['name'] == 'GetProjectionMatrix':
            out += "        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; pRet->m[3][3] = 1; }\n"
        elif m['name'] == 'GetHiddenAreaMesh':
            out += "        if(pRet) { pRet->pVertexData = nullptr; pRet->unTriangleCount = 0; }\n"
        elif m['name'] == 'ComputeDistortion':
            out += "        if(pDistortionCoordinates) { memset(pDistortionCoordinates, 0, sizeof(*pDistortionCoordinates)); }\n"
            out += "        return true;\n"
        elif m['name'] == 'GetString':
            out += "        if(peError) *peError = vr::VRSettingsError_None;\n"
            out += "        if(pchValue && unValueLen > 0) pchValue[0] = '\\0';\n"
        elif m['name'] == 'GetVulkanInstanceExtensionsRequired' or m['name'] == 'GetVulkanDeviceExtensionsRequired':
            out += "        if(pchValue && unBufferSize > 0) pchValue[0] = '\\0';\n"
            out += "        return 0;\n"
        elif m['name'] == 'LoadRenderModel_Async':
            out += "        return vr::VRRenderModelError_NotSupported;\n"
        elif m['name'] == 'LoadTexture_Async':
            out += "        return vr::VRRenderModelError_NotSupported;\n"
        elif m['name'] == 'GetDXGIOutputInfo':
            out += "        if(pnAdapterIndex) *pnAdapterIndex = 0;\n"
        elif m['name'] == 'GetStringTrackedDeviceProperty':
            out += "        const char* s = \"Generic\"; if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\\0'; }\n"
            out += "        if(pError) *pError = vr::TrackedProp_Success;\n"
            out += "        return (uint32_t)strlen(s) + 1;\n"
        else:
            # Fallbacks: ONLY modify things that MUST return errors. Do NOT touch pointers!
            if 'EVRRenderModelError' in m['ret_type']:
                out += "        return vr::VRRenderModelError_NotSupported;\n"
            elif 'EVROverlayError' in m['ret_type']:
                out += "        return vr::VROverlayError_UnknownOverlay;\n"
            elif 'EVRSettingsError' in m['ret_type']:
                out += "        return vr::VRSettingsError_IPCFailed;\n"
            elif 'EVRInputError' in m['ret_type']:
                out += "        return vr::VRInputError_InvalidHandle;\n"
            elif 'EVRTrackedCameraError' in m['ret_type']:
                out += "        return vr::VRTrackedCameraError_OperationFailed;\n"
            elif 'pRet' in m['args']:
                out += "        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }\n"
                if 'HmdMatrix34_t' in m['args']:
                    out += "        if(pRet) { ((vr::HmdMatrix34_t*)pRet)->m[0][0] = 1; ((vr::HmdMatrix34_t*)pRet)->m[1][1] = 1; ((vr::HmdMatrix34_t*)pRet)->m[2][2] = 1; }\n"
                elif 'HmdMatrix44_t' in m['args']:
                    out += "        if(pRet) { ((vr::HmdMatrix44_t*)pRet)->m[0][0] = 1; ((vr::HmdMatrix44_t*)pRet)->m[1][1] = 1; ((vr::HmdMatrix44_t*)pRet)->m[2][2] = 1; ((vr::HmdMatrix44_t*)pRet)->m[3][3] = 1; }\n"
                if 'void' not in m['ret_type']:
                    out += f"        return ({m['ret_type']})0;\n"
            elif 'void' == m['ret_type']:
                pass
            elif 'bool' == m['ret_type']:
                out += "        return false;\n"
            elif m['ret_type'] in ['bool']:
                out += "        return false;\n"
            elif 'Error' in m['ret_type']:
                if 'RenderModel' in m['ret_type']:
                    out += f"        return ({m['ret_type']})2;\n"
                else:
                    out += f"        return ({m['ret_type']})1;\n"
            elif '*' in m['ret_type']:
                out += "        return nullptr;\n"
            else:
                out += f"        return ({m['ret_type']})0;\n"
        
        out += "    }\n"
    out += f"}} g_mock{iface[3:]};\n\n"

out += """
extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    if (peError) *peError = vr::VRInitError_None;
    FILE* f = fopen("vr_emulator_log.txt", "a");
    if(f) { fprintf(f, "Requested interface: %s\\n", pchInterfaceVersion); fclose(f); }

    if (strstr(pchInterfaceVersion, "IVRMailbox")) return &g_universalMock;

"""
for iface in found_interfaces:
    out += f'    if (strstr(pchInterfaceVersion, "{iface}")) return &g_mock{iface[3:]};\n'

out += """
    return nullptr;
}

extern "C" __declspec(dllexport) uint32_t VR_InitInternal2(vr::EVRInitError *peError, vr::EVRApplicationType eType, const char *pStartupInfo) {
    if (peError) *peError = vr::VRInitError_None;
    return 1;
}

extern "C" __declspec(dllexport) void VR_ShutdownInternal() {
}

extern "C" __declspec(dllexport) bool VR_IsHmdPresent() {
    return true;
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsSymbol(vr::EVRInitError error) {
    return "";
}

extern "C" __declspec(dllexport) const char* VR_GetVRInitErrorAsEnglishDescription(vr::EVRInitError error) {
    return "";
}

extern "C" __declspec(dllexport) uint32_t VR_GetInitToken() {
    return 1;
}

extern "C" __declspec(dllexport) void* VRControlPanel() { return nullptr; }
extern "C" __declspec(dllexport) const char* VR_GetRuntimePath() { return ""; }
extern "C" __declspec(dllexport) const char* VR_GetStringForHmdError(vr::EVRInitError error) { return ""; }
extern "C" __declspec(dllexport) uint32_t VR_InitInternal(vr::EVRInitError *peError, vr::EVRApplicationType eType) { if(peError) *peError = vr::VRInitError_None; return 1; }
extern "C" __declspec(dllexport) bool VR_IsInterfaceVersionValid(const char *pchInterfaceVersion) { return true; }
extern "C" __declspec(dllexport) bool VR_IsRuntimeInstalled() { return true; }

"""

with open("openvr_emulator.cpp", "w") as f:
    f.write(out)
