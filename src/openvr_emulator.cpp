#include <chrono>

#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void LogMsg(const char* msg) {
    // Disabled to prevent 58GB log file
}
#include "openvr.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <windows.h>
#include <d3d11.h>
#include <d3d11_4.h>

struct SharedFrame {
    uint32_t sequenceNumber;
    uint32_t width;
    uint32_t height;
    uint32_t format;
    double renderedTimestamp;
};

#pragma pack(push, 1)
struct SharedHands {
    double poseTimestamp;
    double renderedTimestamp;
    float headTransform[16];
    float leftTransform[16];
    float rightTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
    uint32_t rightButtons;
    uint32_t leftButtons;
    float rightStickX;
    float rightStickY;
    float leftStickX;
    float leftStickY;
    float rightVelocity[3];
    float leftVelocity[3];
};
#pragma pack(pop)

static HANDLE hMapFile = NULL;
static void* pBuf = NULL;
static SharedFrame* pHeader = NULL;
static uint8_t* pPixelData = NULL;
static SharedHands* pSharedHands = NULL;
static uint32_t frameSeq = 1;
static ID3D11Texture2D* pStagingTexture = NULL;

float g_handVelocityL[3] = {0,0,0};
float g_handVelocityR[3] = {0,0,0};
static void InitSharedMemory() {
    if (hMapFile) return;
    
    const int mapSize = 16 + 4096 * 4096 * 4 + 1024;
    
    HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER fsize;
        GetFileSizeEx(hFile, &fsize);
        if (fsize.QuadPart < mapSize) {
            SetFilePointer(hFile, mapSize - 1, NULL, FILE_BEGIN);
            DWORD written;
            WriteFile(hFile, "", 1, &written, NULL);
        }
        HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 0, "Local\\VRSharedFrame");
        if (hMap) {
            void* pBuf = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, mapSize);
            if (pBuf) {
                pHeader = (SharedFrame*)pBuf;
                pSharedHands = (SharedHands*)((uint8_t*)pBuf + 16 + 4096 * 4096 * 4);
                
                // Initialize with Identity matrices but place head 1.5m above ground to prevent black screen from floor clipping
                if (pSharedHands->headTransform[0] == 0.0f && pSharedHands->headTransform[5] == 0.0f) {
                    float identity[16] = {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 1.5f, 0, 1  // Y translation = 1.5f
                    };
                    memcpy(pSharedHands->headTransform, identity, sizeof(identity));
                    memcpy(pSharedHands->leftTransform, identity, sizeof(identity));
                    memcpy(pSharedHands->rightTransform, identity, sizeof(identity));
                }
            }
        }
    }
}

using namespace vr;

class UniversalMock {
public:
    virtual void* Dummy0() { return nullptr; }
    virtual void* Dummy1() { return nullptr; }
    virtual void* Dummy2() { return nullptr; }
    virtual void* Dummy3() { return nullptr; }
    virtual void* Dummy4() { return nullptr; }
    virtual void* Dummy5() { return nullptr; }
    virtual void* Dummy6() { return nullptr; }
    virtual void* Dummy7() { return nullptr; }
    virtual void* Dummy8() { return nullptr; }
    virtual void* Dummy9() { return nullptr; }
    virtual void* Dummy10() { return nullptr; }
    virtual void* Dummy11() { return nullptr; }
    virtual void* Dummy12() { return nullptr; }
    virtual void* Dummy13() { return nullptr; }
    virtual void* Dummy14() { return nullptr; }
    virtual void* Dummy15() { return nullptr; }
    virtual void* Dummy16() { return nullptr; }
    virtual void* Dummy17() { return nullptr; }
    virtual void* Dummy18() { return nullptr; }
    virtual void* Dummy19() { return nullptr; }
    virtual void* Dummy20() { return nullptr; }
    virtual void* Dummy21() { return nullptr; }
    virtual void* Dummy22() { return nullptr; }
    virtual void* Dummy23() { return nullptr; }
    virtual void* Dummy24() { return nullptr; }
    virtual void* Dummy25() { return nullptr; }
    virtual void* Dummy26() { return nullptr; }
    virtual void* Dummy27() { return nullptr; }
    virtual void* Dummy28() { return nullptr; }
    virtual void* Dummy29() { return nullptr; }
    virtual void* Dummy30() { return nullptr; }
    virtual void* Dummy31() { return nullptr; }
    virtual void* Dummy32() { return nullptr; }
    virtual void* Dummy33() { return nullptr; }
    virtual void* Dummy34() { return nullptr; }
    virtual void* Dummy35() { return nullptr; }
    virtual void* Dummy36() { return nullptr; }
    virtual void* Dummy37() { return nullptr; }
    virtual void* Dummy38() { return nullptr; }
    virtual void* Dummy39() { return nullptr; }
    virtual void* Dummy40() { return nullptr; }
    virtual void* Dummy41() { return nullptr; }
    virtual void* Dummy42() { return nullptr; }
    virtual void* Dummy43() { return nullptr; }
    virtual void* Dummy44() { return nullptr; }
    virtual void* Dummy45() { return nullptr; }
    virtual void* Dummy46() { return nullptr; }
    virtual void* Dummy47() { return nullptr; }
    virtual void* Dummy48() { return nullptr; }
    virtual void* Dummy49() { return nullptr; }
    virtual void* Dummy50() { return nullptr; }
    virtual void* Dummy51() { return nullptr; }
    virtual void* Dummy52() { return nullptr; }
    virtual void* Dummy53() { return nullptr; }
    virtual void* Dummy54() { return nullptr; }
    virtual void* Dummy55() { return nullptr; }
    virtual void* Dummy56() { return nullptr; }
    virtual void* Dummy57() { return nullptr; }
    virtual void* Dummy58() { return nullptr; }
    virtual void* Dummy59() { return nullptr; }
    virtual void* Dummy60() { return nullptr; }
    virtual void* Dummy61() { return nullptr; }
    virtual void* Dummy62() { return nullptr; }
    virtual void* Dummy63() { return nullptr; }
    virtual void* Dummy64() { return nullptr; }
    virtual void* Dummy65() { return nullptr; }
    virtual void* Dummy66() { return nullptr; }
    virtual void* Dummy67() { return nullptr; }
    virtual void* Dummy68() { return nullptr; }
    virtual void* Dummy69() { return nullptr; }
    virtual void* Dummy70() { return nullptr; }
    virtual void* Dummy71() { return nullptr; }
    virtual void* Dummy72() { return nullptr; }
    virtual void* Dummy73() { return nullptr; }
    virtual void* Dummy74() { return nullptr; }
    virtual void* Dummy75() { return nullptr; }
    virtual void* Dummy76() { return nullptr; }
    virtual void* Dummy77() { return nullptr; }
    virtual void* Dummy78() { return nullptr; }
    virtual void* Dummy79() { return nullptr; }
    virtual void* Dummy80() { return nullptr; }
    virtual void* Dummy81() { return nullptr; }
    virtual void* Dummy82() { return nullptr; }
    virtual void* Dummy83() { return nullptr; }
    virtual void* Dummy84() { return nullptr; }
    virtual void* Dummy85() { return nullptr; }
    virtual void* Dummy86() { return nullptr; }
    virtual void* Dummy87() { return nullptr; }
    virtual void* Dummy88() { return nullptr; }
    virtual void* Dummy89() { return nullptr; }
    virtual void* Dummy90() { return nullptr; }
    virtual void* Dummy91() { return nullptr; }
    virtual void* Dummy92() { return nullptr; }
    virtual void* Dummy93() { return nullptr; }
    virtual void* Dummy94() { return nullptr; }
    virtual void* Dummy95() { return nullptr; }
    virtual void* Dummy96() { return nullptr; }
    virtual void* Dummy97() { return nullptr; }
    virtual void* Dummy98() { return nullptr; }
    virtual void* Dummy99() { return nullptr; }
};
UniversalMock g_universalMock;

#include <cmath>

class Mock_IVRSystem : public vr::IVRSystem {
public:
    virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        LogMsg("Called: IVRSystem::GetRecommendedRenderTargetSize\n");
        if(pnWidth) *pnWidth = 1920;
        if(pnHeight) *pnHeight = 1080;
    }
    virtual void GetProjectionMatrix(vr::HmdMatrix44_t *pRet, vr::EVREye eEye, float fNearZ, float fFarZ) override {
        LogMsg("Called: IVRSystem::GetProjectionMatrix\n");
        if (pRet) {
            memset(pRet, 0, sizeof(*pRet));
            // Provide a valid 90-degree FOV perspective projection matrix to prevent zero-division math collapse
            float fov = 90.0f * (3.1415926535f / 180.0f);
            float y_scale = 1.0f / std::tan(fov / 2.0f);
            float x_scale = y_scale; // 1:1 aspect ratio
            pRet->m[0][0] = x_scale;
            pRet->m[1][1] = y_scale;
            pRet->m[2][2] = -fFarZ / (fFarZ - fNearZ);
            pRet->m[2][3] = -(fFarZ * fNearZ) / (fFarZ - fNearZ);
            pRet->m[3][2] = -1.0f;
            pRet->m[3][3] = 0.0f;
        }
        return;
    }
    virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) override {
        // Fallback to 1.0 (90 deg FOV) to prevent Alyx rendering black frames due to FOV limits
        if(pfLeft) *pfLeft = -1.0f; if(pfRight) *pfRight = 1.0f; if(pfTop) *pfTop = -1.0f; if(pfBottom) *pfBottom = 1.0f;
    }
    virtual bool ComputeDistortion(EVREye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates) override {
        if(pDistortionCoordinates) {
            pDistortionCoordinates->rfRed[0] = fU;
            pDistortionCoordinates->rfRed[1] = fV;
            pDistortionCoordinates->rfGreen[0] = fU;
            pDistortionCoordinates->rfGreen[1] = fV;
            pDistortionCoordinates->rfBlue[0] = fU;
            pDistortionCoordinates->rfBlue[1] = fV;
        }
        return true;
    }
    virtual void GetEyeToHeadTransform(HmdMatrix34_t *pRet, EVREye eEye) override {
        LogMsg("Called: IVRSystem::GetEyeToHeadTransform\n");
        if(pRet) { 
            memset(pRet, 0, sizeof(*pRet)); 
            pRet->m[0][0] = 1; 
            pRet->m[1][1] = 1; 
            pRet->m[2][2] = 1; 
            // 6DoF Stereoscopic IPD offset (approx 64mm)
            pRet->m[0][3] = (eEye == vr::Eye_Left) ? -0.032f : 0.032f;
        }
        return;
    }
    virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override {
        static uint64_t frame = 0; frame++;
        if(pfSecondsSinceLastVsync) *pfSecondsSinceLastVsync = 0.011f;
        if(pulFrameCounter) *pulFrameCounter = frame;
        return true;
    }
    virtual int32_t GetD3D9AdapterIndex() override {
        LogMsg("Called: IVRSystem::GetD3D9AdapterIndex\n");
        return (int32_t)0;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex) override {
        LogMsg("Called: IVRSystem::GetDXGIOutputInfo\n");
        if(pnAdapterIndex) *pnAdapterIndex = 0;
    }
    virtual void GetOutputDevice(uint64_t *pnDevice, ETextureType textureType, VkInstance_T *pInstance = nullptr) override {
        LogMsg("Called: IVRSystem::GetOutputDevice\n");
        if(pnDevice) *pnDevice = 0;
    }
    virtual bool IsDisplayOnDesktop() override {
        LogMsg("Called: IVRSystem::IsDisplayOnDesktop\n");
        return false;
    }
    virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) override {
        LogMsg("Called: IVRSystem::SetDisplayVisibility\n");
        return false;
    }
    virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override {
        LogMsg("Called: IVRSystem::GetDeviceToAbsoluteTrackingPose\n");
        if(pTrackedDevicePoseArray && unTrackedDevicePoseArrayCount > 0) { 
            memset(pTrackedDevicePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unTrackedDevicePoseArrayCount); 
            for(uint32_t i=0; i<3 && i<unTrackedDevicePoseArrayCount; ++i) { 
                pTrackedDevicePoseArray[i].bPoseIsValid = true; 
                pTrackedDevicePoseArray[i].bDeviceIsConnected = true; 
                pTrackedDevicePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK; 
                
                if (i == 0 && pSharedHands && pSharedHands->headTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->headTransform[c*4 + r];
                } else if (i == 1 && pSharedHands && pSharedHands->leftTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->leftTransform[c*4 + r];
                } else if (i == 2 && pSharedHands && pSharedHands->rightTransform[0] != 0.0f) {
                    for(int r=0;r<3;r++) for(int c=0;c<4;c++) pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->rightTransform[c*4 + r];
                } else {
                    pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1; 
                    pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1; 
                    pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1;
                    if (i == 0) { // HMD
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // Y (Height)
                    } else if (i == 1) { // Left Controller
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[0][3] = -0.2f; // X
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.2f;  // Y
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[2][3] = -0.4f; // Z
                    } else if (i == 2) { // Right Controller
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[0][3] = 0.2f;  // X
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.2f;  // Y
                        pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[2][3] = -0.4f; // Z
                    }
                }
            } 
        }
    }
    virtual void ResetSeatedZeroPose() override {
        LogMsg("Called: IVRSystem::ResetSeatedZeroPose\n");
    }
    virtual void GetSeatedZeroPoseToStandingAbsoluteTrackingPose(HmdMatrix34_t *pRet) override {
        LogMsg("Called: IVRSystem::GetSeatedZeroPoseToStandingAbsoluteTrackingPose\n");
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { ((vr::HmdMatrix34_t*)pRet)->m[0][0] = 1; ((vr::HmdMatrix34_t*)pRet)->m[1][1] = 1; ((vr::HmdMatrix34_t*)pRet)->m[2][2] = 1; }
        return;
    }
    virtual void GetRawZeroPoseToStandingAbsoluteTrackingPose(HmdMatrix34_t *pRet) override {
        LogMsg("Called: IVRSystem::GetRawZeroPoseToStandingAbsoluteTrackingPose\n");
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { ((vr::HmdMatrix34_t*)pRet)->m[0][0] = 1; ((vr::HmdMatrix34_t*)pRet)->m[1][1] = 1; ((vr::HmdMatrix34_t*)pRet)->m[2][2] = 1; }
        return;
    }
    virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd) override {
        LogMsg("Called: IVRSystem::GetSortedTrackedDeviceIndicesOfClass\n");
        return (uint32_t)0;
    }
    virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) override {
        return vr::k_EDeviceActivityLevel_UserInteraction;
    }
    virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform) override {
        LogMsg("Called: IVRSystem::ApplyTransform\n");
    }
    virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) override {
        LogMsg("Called: IVRSystem::GetTrackedDeviceIndexForControllerRole\n");
        if (unDeviceType == vr::TrackedControllerRole_LeftHand) return 1;
        if (unDeviceType == vr::TrackedControllerRole_RightHand) return 2;
        return vr::k_unTrackedDeviceIndexInvalid;
    }
    virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        LogMsg("Called: IVRSystem::GetControllerRoleForTrackedDeviceIndex\n");
        if (unDeviceIndex == 1) return vr::TrackedControllerRole_LeftHand;
        if (unDeviceIndex == 2) return vr::TrackedControllerRole_RightHand;
        return vr::TrackedControllerRole_Invalid;
    }
    virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        if (unDeviceIndex == vr::k_unTrackedDeviceIndex_Hmd) return vr::TrackedDeviceClass_HMD;
        if (unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller;
        return vr::TrackedDeviceClass_Invalid;
    }
    virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        LogMsg("Called: IVRSystem::IsTrackedDeviceConnected\n");
        if(unDeviceIndex == 0 || unDeviceIndex == 1 || unDeviceIndex == 2) return true;
        return false;
    }
    virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        LogMsg("Called: IVRSystem::GetBoolTrackedDeviceProperty\n");
        return false;
    }
    virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        if(pError) *pError = vr::TrackedProp_Success;
        if(prop == vr::Prop_DisplayFrequency_Float) return 90.0f;
        if(prop == vr::Prop_UserIpdMeters_Float) return 0.065f;
        if(prop == vr::Prop_UserHeadToEyeDepthMeters_Float) return 0.0f;
        return 0.0f;
    }
    virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        LogMsg("Called: IVRSystem::GetInt32TrackedDeviceProperty\n");
        if(pError) *pError = vr::TrackedProp_Success;
        if(prop == vr::Prop_DeviceClass_Int32) { if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD; if(unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller; }
        if(prop == vr::Prop_ControllerRoleHint_Int32) { if(unDeviceIndex == 1) return vr::TrackedControllerRole_LeftHand; if(unDeviceIndex == 2) return vr::TrackedControllerRole_RightHand; }
        if(prop == vr::Prop_Axis0Type_Int32) return vr::k_eControllerAxis_TrackPad;
        if(prop == vr::Prop_Axis1Type_Int32) return vr::k_eControllerAxis_Trigger;
        if(prop == vr::Prop_Axis2Type_Int32) return vr::k_eControllerAxis_Joystick;
        return 0;
    }
    virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        LogMsg("Called: IVRSystem::GetUint64TrackedDeviceProperty\n");
        if(pError) *pError = vr::TrackedProp_Success;
        if(prop == vr::Prop_CurrentUniverseId_Uint64) return 1;
        if(prop == vr::Prop_SupportedButtons_Uint64) return 0xFFFFFFFFFFFFFFFFULL;
        if(prop == vr::Prop_HardwareRevision_Uint64) return 1;
        if(prop == vr::Prop_FirmwareVersion_Uint64) return 1;
        if(prop == vr::Prop_FPGAVersion_Uint64) return 1;
        if(prop == vr::Prop_VRCVersion_Uint64) return 1;
        if(prop == vr::Prop_RadioVersion_Uint64) return 1;
        if(prop == vr::Prop_DongleVersion_Uint64) return 1;
        return 0;
    }
    virtual void GetMatrix34TrackedDeviceProperty(HmdMatrix34_t *pRet, vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        LogMsg("Called: IVRSystem::GetMatrix34TrackedDeviceProperty\n");
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; }
        if(pError) *pError = vr::TrackedProp_Success;
        return;
    }
    virtual uint32_t GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) override {
        LogMsg("Called: IVRSystem::GetArrayTrackedDeviceProperty\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) override {
        const char* s = "Generic";
        if (prop == vr::Prop_RenderModelName_String) {
            if (unDeviceIndex == 0) s = "generic_hmd";
            else if (unDeviceIndex == 1) s = "{indexcontroller}valve_controller_knu_1_0_left";
            else if (unDeviceIndex == 2) s = "{indexcontroller}valve_controller_knu_1_0_right";
        }
        if (prop == vr::Prop_ControllerType_String) s = "knuckles";
        if (prop == vr::Prop_TrackingSystemName_String) s = "lighthouse";
        if (prop == vr::Prop_ManufacturerName_String) s = "Valve";
        if (prop == vr::Prop_ResourceRoot_String) s = "indexcontroller";
        if (prop == vr::Prop_InputProfilePath_String) s = "{indexcontroller}/input/index_controller_profile.json";
        if (prop == vr::Prop_ModelNumber_String) {
            if (unDeviceIndex == 1) s = "Knuckles Left";
            else if (unDeviceIndex == 2) s = "Knuckles Right";
        }
        if (prop == vr::Prop_RegisteredDeviceType_String) {
            if (unDeviceIndex == 1) s = "valve/index_controllerLHR-FFFFFFFF";
            else if (unDeviceIndex == 2) s = "valve/index_controllerLHR-EEEEEEEE";
        }
        if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\0'; }
        if(pError) *pError = vr::TrackedProp_Success;
        return (uint32_t)strlen(s) + 1;
    }
    virtual const char * GetPropErrorNameFromEnum(ETrackedPropertyError error) override {
        LogMsg("Called: IVRSystem::GetPropErrorNameFromEnum\n");
        return "1.10.30";
    }
    virtual bool PollNextEvent(vr::VREvent_t *pEvent, uint32_t uncbVREvent) override {
        static int count = 0;
        if (count == 0) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 0; } return true; }
        else if (count == 1) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 1; } return true; }
        else if (count == 2) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 2; } return true; }
        else if (count == 3) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)403; pEvent->trackedDeviceIndex = 0; } return true; }

        static uint64_t lastLeftVRButtons = 0;
        static uint64_t lastRightVRButtons = 0;
        
        if (pSharedHands && pEvent) {
            uint32_t lb = pSharedHands->leftButtons;
            uint32_t rb = pSharedHands->rightButtons;
            uint64_t currentLeftVRButtons = 0;
            uint64_t currentRightVRButtons = 0;
            
            if (pSharedHands->leftPinch) currentLeftVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_Grip);
            if (lb & (1<<4)) currentLeftVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger); // ZL
            if (lb & (1<<5)) currentLeftVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad); // L3
            if (lb & (1<<6)) currentLeftVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_System); // Minus
            if (lb & (1<<0)) currentLeftVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_A); // A
            if (lb & (1<<1)) currentLeftVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu); // B

            if (pSharedHands->rightPinch) currentRightVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_Grip);
            if (rb & (1<<2)) currentRightVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger); // ZR
            if (rb & (1<<0)) currentRightVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_A); // A
            if (rb & (1<<1)) currentRightVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu); // B -> App Menu
            if (rb & (1<<3)) currentRightVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad); // R3
            if (rb & (1<<4)) currentRightVRButtons |= vr::ButtonMaskFromId(vr::k_EButton_System); // Plus

            uint64_t leftDiff = currentLeftVRButtons ^ lastLeftVRButtons;
            if (leftDiff != 0) {
                for (int i = 0; i < 64; i++) {
                    if (leftDiff & (1ULL << i)) {
                        memset(pEvent, 0, uncbVREvent);
                        pEvent->trackedDeviceIndex = 1;
                        pEvent->data.controller.button = i;
                        pEvent->eventType = (currentLeftVRButtons & (1ULL << i)) ? (vr::EVREventType)200 : (vr::EVREventType)201;
                        lastLeftVRButtons = (lastLeftVRButtons & ~(1ULL << i)) | (currentLeftVRButtons & (1ULL << i));
                        return true;
                    }
                }
            }

            uint64_t rightDiff = currentRightVRButtons ^ lastRightVRButtons;
            if (rightDiff != 0) {
                for (int i = 0; i < 64; i++) {
                    if (rightDiff & (1ULL << i)) {
                        memset(pEvent, 0, uncbVREvent);
                        pEvent->trackedDeviceIndex = 2;
                        pEvent->data.controller.button = i;
                        pEvent->eventType = (currentRightVRButtons & (1ULL << i)) ? (vr::EVREventType)200 : (vr::EVREventType)201;
                        lastRightVRButtons = (lastRightVRButtons & ~(1ULL << i)) | (currentRightVRButtons & (1ULL << i));
                        return true;
                    }
                }
            }
        }
        return false;
    }
    virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, vr::VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose) override {
        return PollNextEvent(pEvent, uncbVREvent);
    }
    virtual const char * GetEventTypeNameFromEnum(EVREventType eType) override {
        LogMsg("Called: IVRSystem::GetEventTypeNameFromEnum\n");
        return "1.10.30";
    }
    virtual void GetHiddenAreaMesh(HiddenAreaMesh_t *pRet, EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard) override {
        LogMsg("Called: IVRSystem::GetHiddenAreaMesh\n");
        static vr::HmdVector2_t dummy_verts[3] = { { 2.0f, 2.0f }, { 2.0f, 2.1f }, { 2.1f, 2.0f } };
        if(pRet) { 
            pRet->pVertexData = dummy_verts; 
            pRet->unTriangleCount = 1; 
        }
        return;
    }
    virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize) override {
        if(pControllerState) {
            memset(pControllerState, 0, unControllerStateSize);
            pControllerState->unPacketNum = frameSeq;
            if (pSharedHands) {
                uint32_t lb = pSharedHands->leftButtons;
                uint32_t rb = pSharedHands->rightButtons;
                if (unControllerDeviceIndex == 1) {
                    if ((lb & (1<<7)) != 0 || (lb & (1<<8)) != 0) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Grip); // L
                    if (lb & (1<<4)) { pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger); pControllerState->rAxis[1].x = 1.0f; } // ZL
                    if (lb & (1<<5)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad); // L3
                    if (lb & (1<<6)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_System); // Minus
                    if (lb & (1<<0)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_A); // Left D-Pad Right -> A
                    if (lb & (1<<1)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu); // Left D-Pad Down -> B
                    pControllerState->rAxis[0].x = pSharedHands->leftStickX;
                    pControllerState->rAxis[0].y = -pSharedHands->leftStickY;
                } else if (unControllerDeviceIndex == 2) {
                    if ((rb & (1<<5)) != 0 || (rb & (1<<6)) != 0) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Grip); // R
                    if ((rb & (1<<2)) != 0 || (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0) { pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger); pControllerState->rAxis[1].x = 1.0f; } // ZR or Space
                    if ((rb & (1<<0)) != 0 || (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_A); // A or Space
                    if (rb & (1<<1)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu); // B
                    if (rb & (1<<3)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad); // R3
                    if (rb & (1<<4)) pControllerState->ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_System); // Plus
                    pControllerState->rAxis[0].x = pSharedHands->rightStickX;
                    pControllerState->rAxis[0].y = -pSharedHands->rightStickY;
                }
            }
        }
        return true;
    }
    virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose) override {
        GetControllerState(unControllerDeviceIndex, pControllerState, unControllerStateSize);
        if(pTrackedDevicePose) {
            memset(pTrackedDevicePose, 0, sizeof(vr::TrackedDevicePose_t));
            pTrackedDevicePose->bPoseIsValid = true;
            pTrackedDevicePose->bDeviceIsConnected = true;
            pTrackedDevicePose->eTrackingResult = vr::TrackingResult_Running_OK;
            if (unControllerDeviceIndex == 1 && pSharedHands && pSharedHands->leftTransform[0] != 0.0f) {
                for(int r=0;r<3;r++) for(int c=0;c<4;c++) pTrackedDevicePose->mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->leftTransform[c*4 + r];
            } else if (unControllerDeviceIndex == 2 && pSharedHands && pSharedHands->rightTransform[0] != 0.0f) {
                for(int r=0;r<3;r++) for(int c=0;c<4;c++) pTrackedDevicePose->mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->rightTransform[c*4 + r];
            } else {
                pTrackedDevicePose->mDeviceToAbsoluteTracking.m[0][0] = 1; pTrackedDevicePose->mDeviceToAbsoluteTracking.m[1][1] = 1; pTrackedDevicePose->mDeviceToAbsoluteTracking.m[2][2] = 1;
                if (unControllerDeviceIndex == 1) { // Left
                    pTrackedDevicePose->mDeviceToAbsoluteTracking.m[0][3] = -0.2f;
                    pTrackedDevicePose->mDeviceToAbsoluteTracking.m[1][3] = 1.2f;
                    pTrackedDevicePose->mDeviceToAbsoluteTracking.m[2][3] = -0.4f;
                } else if (unControllerDeviceIndex == 2) { // Right
                    pTrackedDevicePose->mDeviceToAbsoluteTracking.m[0][3] = 0.2f;
                    pTrackedDevicePose->mDeviceToAbsoluteTracking.m[1][3] = 1.2f;
                    pTrackedDevicePose->mDeviceToAbsoluteTracking.m[2][3] = -0.4f;
                }
            }
        }
        return true;
    }
    virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) override {
        LogMsg("Called: IVRSystem::TriggerHapticPulse\n");
    }
    virtual const char * GetButtonIdNameFromEnum(EVRButtonId eButtonId) override {
        LogMsg("Called: IVRSystem::GetButtonIdNameFromEnum\n");
        return "1.10.30";
    }
    virtual const char * GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) override {
        LogMsg("Called: IVRSystem::GetControllerAxisTypeNameFromEnum\n");
        return "1.10.30";
    }
    virtual bool IsInputAvailable() override {
        LogMsg("Called: IVRSystem::IsInputAvailable\n");
        return true;
    }
    virtual bool IsSteamVRDrawingControllers() override {
        LogMsg("Called: IVRSystem::IsSteamVRDrawingControllers\n");
        return false;
    }
    virtual bool ShouldApplicationPause() override {
        LogMsg("Called: IVRSystem::ShouldApplicationPause\n");
        return false;
    }
    virtual bool ShouldApplicationReduceRenderingWork() override {
        LogMsg("Called: IVRSystem::ShouldApplicationReduceRenderingWork\n");
        return false;
    }
    virtual vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        LogMsg("Called: IVRSystem::PerformFirmwareUpdate\n");
        return (vr::EVRFirmwareError)1;
    }
    virtual void AcknowledgeQuit_Exiting() override {
        LogMsg("Called: IVRSystem::AcknowledgeQuit_Exiting\n");
    }
    virtual uint32_t GetAppContainerFilePaths(VR_OUT_STRING() char *pchBuffer, uint32_t unBufferSize) override {
        LogMsg("Called: IVRSystem::GetAppContainerFilePaths\n");
        return (uint32_t)0;
    }
    virtual const char * GetRuntimeVersion() override {
        LogMsg("Called: IVRSystem::GetRuntimeVersion\n");
        return "1.10.30";
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* Dummy61() { return nullptr; }
    virtual void* Dummy62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockSystem;

class Mock_IVRApplications : public vr::IVRApplications {
public:
    virtual EVRApplicationError AddApplicationManifest(const char *pchApplicationManifestFullPath, bool bTemporary = false) override {
        LogMsg("Called: IVRApplications::AddApplicationManifest\n");
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError RemoveApplicationManifest(const char *pchApplicationManifestFullPath) override {
        LogMsg("Called: IVRApplications::RemoveApplicationManifest\n");
        return (EVRApplicationError)1;
    }
    virtual bool IsApplicationInstalled(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::IsApplicationInstalled\n");
        return false;
    }
    virtual uint32_t GetApplicationCount() override {
        LogMsg("Called: IVRApplications::GetApplicationCount\n");
        return (uint32_t)0;
    }
    virtual EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        LogMsg("Called: IVRApplications::GetApplicationKeyByIndex\n");
        if (pchAppKeyBuffer && unAppKeyBufferLen > 0) pchAppKeyBuffer[0] = '\0';
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        LogMsg("Called: IVRApplications::GetApplicationKeyByProcessId\n");
        if (pchAppKeyBuffer && unAppKeyBufferLen > 0) pchAppKeyBuffer[0] = '\0';
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError LaunchApplication(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::LaunchApplication\n");
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError LaunchTemplateApplication(const char *pchTemplateAppKey, const char *pchNewAppKey, VR_ARRAY_COUNT( unKeys ) const AppOverrideKeys_t *pKeys, uint32_t unKeys) override {
        LogMsg("Called: IVRApplications::LaunchTemplateApplication\n");
        return (EVRApplicationError)1;
    }
    virtual vr::EVRApplicationError LaunchApplicationFromMimeType(const char *pchMimeType, const char *pchArgs) override {
        LogMsg("Called: IVRApplications::LaunchApplicationFromMimeType\n");
        return (vr::EVRApplicationError)1;
    }
    virtual EVRApplicationError LaunchDashboardOverlay(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::LaunchDashboardOverlay\n");
        return (EVRApplicationError)1;
    }
    virtual bool CancelApplicationLaunch(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::CancelApplicationLaunch\n");
        return false;
    }
    virtual EVRApplicationError IdentifyApplication(uint32_t unProcessId, const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::IdentifyApplication\n");
        return (EVRApplicationError)1;
    }
    virtual uint32_t GetApplicationProcessId(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::GetApplicationProcessId\n");
        return (uint32_t)0;
    }
    virtual const char * GetApplicationsErrorNameFromEnum(EVRApplicationError error) override {
        LogMsg("Called: IVRApplications::GetApplicationsErrorNameFromEnum\n");
        return "1.10.30";
    }
    virtual uint32_t GetApplicationPropertyString(const char *pchAppKey, EVRApplicationProperty eProperty, VR_OUT_STRING() char *pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, EVRApplicationError *peError = nullptr) override {
        LogMsg("Called: IVRApplications::GetApplicationPropertyString\n");
        if (pchPropertyValueBuffer && unPropertyValueBufferLen > 0) pchPropertyValueBuffer[0] = '\0';
        return (uint32_t)0;
    }
    virtual bool GetApplicationPropertyBool(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr) override {
        LogMsg("Called: IVRApplications::GetApplicationPropertyBool\n");
        return false;
    }
    virtual uint64_t GetApplicationPropertyUint64(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr) override {
        LogMsg("Called: IVRApplications::GetApplicationPropertyUint64\n");
        return (uint64_t)0;
    }
    virtual EVRApplicationError SetApplicationAutoLaunch(const char *pchAppKey, bool bAutoLaunch) override {
        LogMsg("Called: IVRApplications::SetApplicationAutoLaunch\n");
        return (EVRApplicationError)1;
    }
    virtual bool GetApplicationAutoLaunch(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::GetApplicationAutoLaunch\n");
        return false;
    }
    virtual EVRApplicationError SetDefaultApplicationForMimeType(const char *pchAppKey, const char *pchMimeType) override {
        LogMsg("Called: IVRApplications::SetDefaultApplicationForMimeType\n");
        return (EVRApplicationError)1;
    }
    virtual bool GetDefaultApplicationForMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        LogMsg("Called: IVRApplications::GetDefaultApplicationForMimeType\n");
        return false;
    }
    virtual bool GetApplicationSupportedMimeTypes(const char *pchAppKey, VR_OUT_STRING() char *pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) override {
        LogMsg("Called: IVRApplications::GetApplicationSupportedMimeTypes\n");
        return false;
    }
    virtual uint32_t GetApplicationsThatSupportMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) override {
        LogMsg("Called: IVRApplications::GetApplicationsThatSupportMimeType\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetApplicationLaunchArguments(uint32_t unHandle, VR_OUT_STRING() char *pchArgs, uint32_t unArgs) override {
        LogMsg("Called: IVRApplications::GetApplicationLaunchArguments\n");
        return (uint32_t)0;
    }
    virtual EVRApplicationError GetStartingApplication(VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        LogMsg("Called: IVRApplications::GetStartingApplication\n");
        return (EVRApplicationError)1;
    }
    virtual EVRSceneApplicationState GetSceneApplicationState() override {
        LogMsg("Called: IVRApplications::GetSceneApplicationState\n");
        return (EVRSceneApplicationState)0;
    }
    virtual EVRApplicationError PerformApplicationPrelaunchCheck(const char *pchAppKey) override {
        LogMsg("Called: IVRApplications::PerformApplicationPrelaunchCheck\n");
        return (EVRApplicationError)1;
    }
    virtual const char * GetSceneApplicationStateNameFromEnum(EVRSceneApplicationState state) override {
        LogMsg("Called: IVRApplications::GetSceneApplicationStateNameFromEnum\n");
        return "1.10.30";
    }
    virtual EVRApplicationError LaunchInternalProcess(const char *pchBinaryPath, const char *pchArguments, const char *pchWorkingDirectory) override {
        LogMsg("Called: IVRApplications::LaunchInternalProcess\n");
        return (EVRApplicationError)1;
    }
    virtual uint32_t GetCurrentSceneProcessId() override {
        LogMsg("Called: IVRApplications::GetCurrentSceneProcessId\n");
        return (uint32_t)0;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockApplications;

class Mock_IVRSettings : public vr::IVRSettings {
public:
    virtual const char * GetSettingsErrorNameFromEnum(EVRSettingsError eError) override {
        LogMsg("Called: IVRSettings::GetSettingsErrorNameFromEnum\n");
        return "1.10.30";
    }
    virtual void SetBool(const char *pchSection, const char *pchSettingsKey, bool bValue, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::SetBool\n");
    }
    virtual void SetInt32(const char *pchSection, const char *pchSettingsKey, int32_t nValue, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::SetInt32\n");
    }
    virtual void SetFloat(const char *pchSection, const char *pchSettingsKey, float flValue, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::SetFloat\n");
    }
    virtual void SetString(const char *pchSection, const char *pchSettingsKey, const char *pchValue, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::SetString\n");
    }
    virtual bool GetBool(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::GetBool\n");
        return false;
    }
    virtual int32_t GetInt32(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::GetInt32\n");
        return (int32_t)0;
    }
    virtual float GetFloat(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::GetFloat\n");
        return (float)0;
    }
    virtual void GetString(const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::GetString\n");
        if(peError) *peError = vr::VRSettingsError_None;
        if(pchValue && unValueLen > 0) pchValue[0] = '\0';
    }
    virtual void RemoveSection(const char *pchSection, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::RemoveSection\n");
    }
    virtual void RemoveKeyInSection(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        LogMsg("Called: IVRSettings::RemoveKeyInSection\n");
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* Dummy70() { return nullptr; }
    virtual void* Dummy71() { return nullptr; }
    virtual void* Dummy72() { return nullptr; }
    virtual void* Dummy73() { return nullptr; }
    virtual void* Dummy74() { return nullptr; }
    virtual void* Dummy75() { return nullptr; }
    virtual void* Dummy76() { return nullptr; }
    virtual void* Dummy77() { return nullptr; }
    virtual void* Dummy78() { return nullptr; }
    virtual void* Dummy79() { return nullptr; }
    virtual void* Dummy80() { return nullptr; }
    virtual void* Dummy81() { return nullptr; }
    virtual void* Dummy82() { return nullptr; }
    virtual void* Dummy83() { return nullptr; }
    virtual void* Dummy84() { return nullptr; }
    virtual void* Dummy85() { return nullptr; }
    virtual void* Dummy86() { return nullptr; }
    virtual void* Dummy87() { return nullptr; }
    virtual void* Dummy88() { return nullptr; }
    virtual void* Dummy89() { return nullptr; }
    virtual void* Dummy90() { return nullptr; }
    virtual void* Dummy91() { return nullptr; }
    virtual void* Dummy92() { return nullptr; }
    virtual void* Dummy93() { return nullptr; }
    virtual void* Dummy94() { return nullptr; }
    virtual void* Dummy95() { return nullptr; }
    virtual void* Dummy96() { return nullptr; }
    virtual void* Dummy97() { return nullptr; }
    virtual void* Dummy98() { return nullptr; }
    virtual void* Dummy99() { return nullptr; }
} g_mockSettings;

class Mock_IVRChaperone : public vr::IVRChaperone {
public:
    virtual vr::ChaperoneCalibrationState GetCalibrationState() override {
        return vr::ChaperoneCalibrationState_OK;
    }
    virtual bool GetPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        if(pSizeX) *pSizeX = 1000.0f;
        if(pSizeZ) *pSizeZ = 1000.0f;
        return true;
    }
    virtual bool GetPlayAreaRect(vr::HmdQuad_t *rect) override {
        if(rect) {
            rect->vCorners[0].v[0] = -500.0f; rect->vCorners[0].v[1] = 0.0f; rect->vCorners[0].v[2] = -500.0f;
            rect->vCorners[1].v[0] = -500.0f; rect->vCorners[1].v[1] = 0.0f; rect->vCorners[1].v[2] =  500.0f;
            rect->vCorners[2].v[0] =  500.0f; rect->vCorners[2].v[1] = 0.0f; rect->vCorners[2].v[2] =  500.0f;
            rect->vCorners[3].v[0] =  500.0f; rect->vCorners[3].v[1] = 0.0f; rect->vCorners[3].v[2] = -500.0f;
        }
        return true;
    }
    virtual void ReloadInfo(void) override {
        LogMsg("Called: IVRChaperone::ReloadInfo\n");
    }
    virtual void SetSceneColor(HmdColor_t color) override {
        LogMsg("Called: IVRChaperone::SetSceneColor\n");
    }
    virtual void GetBoundsColor(vr::HmdColor_t *pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, vr::HmdColor_t *pOutputCameraColor) override {
        if(pOutputColorArray) {
            for(int i = 0; i < nNumOutputColors; ++i) {
                pOutputColorArray[i].r = 0.0f;
                pOutputColorArray[i].g = 0.0f;
                pOutputColorArray[i].b = 0.0f;
                pOutputColorArray[i].a = 0.0f;
            }
        }
        if(pOutputCameraColor) {
            pOutputCameraColor->r = 0.0f;
            pOutputCameraColor->g = 0.0f;
            pOutputCameraColor->b = 0.0f;
            pOutputCameraColor->a = 0.0f;
        }
    }
    virtual bool AreBoundsVisible() override {
        LogMsg("Called: IVRChaperone::AreBoundsVisible\n");
        return false;
    }
    virtual void ForceBoundsVisible(bool bForce) override {
        LogMsg("Called: IVRChaperone::ForceBoundsVisible\n");
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockChaperone;

class Mock_IVRChaperoneSetup : public vr::IVRChaperoneSetup {
public:
    virtual bool CommitWorkingCopy(EChaperoneConfigFile configFile) override {
        LogMsg("Called: IVRChaperoneSetup::CommitWorkingCopy\n");
        return false;
    }
    virtual void RevertWorkingCopy() override {
        LogMsg("Called: IVRChaperoneSetup::RevertWorkingCopy\n");
    }
    virtual bool GetWorkingPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        LogMsg("Called: IVRChaperoneSetup::GetWorkingPlayAreaSize\n");
        return false;
    }
    virtual bool GetWorkingPlayAreaRect(HmdQuad_t *rect) override {
        LogMsg("Called: IVRChaperoneSetup::GetWorkingPlayAreaRect\n");
        return false;
    }
    virtual bool GetWorkingCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        LogMsg("Called: IVRChaperoneSetup::GetWorkingCollisionBoundsInfo\n");
        return false;
    }
    virtual bool GetLiveCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        LogMsg("Called: IVRChaperoneSetup::GetLiveCollisionBoundsInfo\n");
        return false;
    }
    virtual bool GetWorkingSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        if(pmatSeatedZeroPoseToRawTrackingPose) { memset(pmatSeatedZeroPoseToRawTrackingPose, 0, sizeof(*pmatSeatedZeroPoseToRawTrackingPose)); pmatSeatedZeroPoseToRawTrackingPose->m[0][0] = 1; pmatSeatedZeroPoseToRawTrackingPose->m[1][1] = 1; pmatSeatedZeroPoseToRawTrackingPose->m[2][2] = 1; }
        return true;
    }
    virtual bool GetWorkingStandingZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatStandingZeroPoseToRawTrackingPose) override {
        if(pmatStandingZeroPoseToRawTrackingPose) { memset(pmatStandingZeroPoseToRawTrackingPose, 0, sizeof(*pmatStandingZeroPoseToRawTrackingPose)); pmatStandingZeroPoseToRawTrackingPose->m[0][0] = 1; pmatStandingZeroPoseToRawTrackingPose->m[1][1] = 1; pmatStandingZeroPoseToRawTrackingPose->m[2][2] = 1; }
        return true;
    }
    virtual void SetWorkingPlayAreaSize(float sizeX, float sizeZ) override {
        LogMsg("Called: IVRChaperoneSetup::SetWorkingPlayAreaSize\n");
    }
    virtual void SetWorkingCollisionBoundsInfo(VR_ARRAY_COUNT(unQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t unQuadsCount) override {
        LogMsg("Called: IVRChaperoneSetup::SetWorkingCollisionBoundsInfo\n");
    }
    virtual void SetWorkingPerimeter(VR_ARRAY_COUNT( unPointCount ) HmdVector2_t *pPointBuffer, uint32_t unPointCount) override {
        LogMsg("Called: IVRChaperoneSetup::SetWorkingPerimeter\n");
    }
    virtual void SetWorkingSeatedZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatSeatedZeroPoseToRawTrackingPose) override {
        LogMsg("Called: IVRChaperoneSetup::SetWorkingSeatedZeroPoseToRawTrackingPose\n");
    }
    virtual void SetWorkingStandingZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatStandingZeroPoseToRawTrackingPose) override {
        LogMsg("Called: IVRChaperoneSetup::SetWorkingStandingZeroPoseToRawTrackingPose\n");
    }
    virtual void ReloadFromDisk(EChaperoneConfigFile configFile) override {
        LogMsg("Called: IVRChaperoneSetup::ReloadFromDisk\n");
    }
    virtual bool GetLiveSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        if(pmatSeatedZeroPoseToRawTrackingPose) { memset(pmatSeatedZeroPoseToRawTrackingPose, 0, sizeof(*pmatSeatedZeroPoseToRawTrackingPose)); pmatSeatedZeroPoseToRawTrackingPose->m[0][0] = 1; pmatSeatedZeroPoseToRawTrackingPose->m[1][1] = 1; pmatSeatedZeroPoseToRawTrackingPose->m[2][2] = 1; }
        return true;
    }
    virtual bool ExportLiveToBuffer(VR_OUT_STRING() char *pBuffer, uint32_t *pnBufferLength) override {
        LogMsg("Called: IVRChaperoneSetup::ExportLiveToBuffer\n");
        return false;
    }
    virtual bool ImportFromBufferToWorking(const char *pBuffer, uint32_t nImportFlags) override {
        LogMsg("Called: IVRChaperoneSetup::ImportFromBufferToWorking\n");
        return false;
    }
    virtual void ShowWorkingSetPreview() override {
        LogMsg("Called: IVRChaperoneSetup::ShowWorkingSetPreview\n");
    }
    virtual void HideWorkingSetPreview() override {
        LogMsg("Called: IVRChaperoneSetup::HideWorkingSetPreview\n");
    }
    virtual void RoomSetupStarting() override {
        LogMsg("Called: IVRChaperoneSetup::RoomSetupStarting\n");
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* Dummy62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* Dummy70() { return nullptr; }
    virtual void* Dummy71() { return nullptr; }
    virtual void* Dummy72() { return nullptr; }
    virtual void* Dummy73() { return nullptr; }
    virtual void* Dummy74() { return nullptr; }
    virtual void* Dummy75() { return nullptr; }
    virtual void* Dummy76() { return nullptr; }
    virtual void* Dummy77() { return nullptr; }
    virtual void* Dummy78() { return nullptr; }
    virtual void* Dummy79() { return nullptr; }
    virtual void* Dummy80() { return nullptr; }
    virtual void* Dummy81() { return nullptr; }
    virtual void* Dummy82() { return nullptr; }
    virtual void* Dummy83() { return nullptr; }
    virtual void* Dummy84() { return nullptr; }
    virtual void* Dummy85() { return nullptr; }
    virtual void* Dummy86() { return nullptr; }
    virtual void* Dummy87() { return nullptr; }
    virtual void* Dummy88() { return nullptr; }
    virtual void* Dummy89() { return nullptr; }
    virtual void* Dummy90() { return nullptr; }
    virtual void* Dummy91() { return nullptr; }
    virtual void* Dummy92() { return nullptr; }
    virtual void* Dummy93() { return nullptr; }
    virtual void* Dummy94() { return nullptr; }
    virtual void* Dummy95() { return nullptr; }
    virtual void* Dummy96() { return nullptr; }
    virtual void* Dummy97() { return nullptr; }
    virtual void* Dummy98() { return nullptr; }
    virtual void* Dummy99() { return nullptr; }
} g_mockChaperoneSetup;

inline uint8_t Cvt11F(uint32_t val) {
    uint32_t mantissa = val & 0x3F;
    uint32_t exponent = (val >> 6) & 0x1F;
    if (exponent == 0) return 0;
    if (exponent == 31) return 255;
    int exp_adj = (int)exponent - 15;
    if (exp_adj > 0) return 255;
    if (exp_adj < -8) return 0;
    uint32_t v = (64 + mantissa) * 255;
    if (exp_adj < 0) v >>= (-exp_adj);
    return (v / 64) > 255 ? 255 : (uint8_t)(v / 64);
}

inline uint8_t Cvt10F(uint32_t val) {
    uint32_t mantissa = val & 0x1F;
    uint32_t exponent = (val >> 5) & 0x1F;
    if (exponent == 0) return 0;
    if (exponent == 31) return 255;
    int exp_adj = (int)exponent - 15;
    if (exp_adj > 0) return 255;
    if (exp_adj < -8) return 0;
    uint32_t v = (32 + mantissa) * 255;
    if (exp_adj < 0) v >>= (-exp_adj);
    return (v / 32) > 255 ? 255 : (uint8_t)(v / 32);
}

void CopyAndConvertFormat(uint8_t* dstRow, const uint8_t* srcRow, uint32_t width, DXGI_FORMAT format) {
    if (format == DXGI_FORMAT_R11G11B10_FLOAT) {
        uint32_t* dst32 = (uint32_t*)dstRow;
        const uint32_t* src32 = (const uint32_t*)srcRow;
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t p = src32[x];
            uint32_t r = p & 0x7FF;
            uint32_t g = (p >> 11) & 0x7FF;
            uint32_t b = (p >> 22) & 0x3FF;
            dst32[x] = (255 << 24) | (Cvt11F(r) << 16) | (Cvt11F(g) << 8) | Cvt10F(b);
        }
    } else if (format == DXGI_FORMAT_R16G16B16A16_FLOAT) {
        uint32_t* dst32 = (uint32_t*)dstRow;
        const uint16_t* src16 = (const uint16_t*)srcRow;
        for (uint32_t x = 0; x < width; ++x) {
            uint16_t r = src16[x*4];
            uint16_t g = src16[x*4+1];
            uint16_t b = src16[x*4+2];
            
            auto Cvt16F = [](uint16_t val) -> uint8_t {
                uint32_t mantissa = val & 0x3FF;
                uint32_t exponent = (val >> 10) & 0x1F;
                if (exponent == 0) return 0;
                if (exponent == 31) return 255;
                int exp_adj = (int)exponent - 15;
                if (exp_adj > 0) return 255;
                if (exp_adj < -8) return 0;
                uint32_t v = (1024 + mantissa) * 255;
                if (exp_adj < 0) v >>= (-exp_adj);
                return (v / 1024) > 255 ? 255 : (uint8_t)(v / 1024);
            };
            
            dst32[x] = (255 << 24) | (Cvt16F(r) << 16) | (Cvt16F(g) << 8) | Cvt16F(b);
        }
    } else if (format == DXGI_FORMAT_R8G8B8A8_UNORM || format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) {
        uint32_t* dst32 = (uint32_t*)dstRow;
        const uint8_t* src8 = srcRow;
        for (uint32_t x = 0; x < width; ++x) {
            uint8_t r = src8[x*4];
            uint8_t g = src8[x*4+1];
            uint8_t b = src8[x*4+2];
            uint8_t a = src8[x*4+3];
            dst32[x] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    } else {
        memcpy(dstRow, srcRow, width * 4);
    }
}

class Mock_IVRCompositor : public vr::IVRCompositor {
public:
    virtual void SetTrackingSpace(ETrackingUniverseOrigin eOrigin) override {
        LogMsg("Called: IVRCompositor::SetTrackingSpace\n");
    }
    virtual ETrackingUniverseOrigin GetTrackingSpace() override {
        LogMsg("Called: IVRCompositor::GetTrackingSpace\n");
        return (ETrackingUniverseOrigin)0;
    }
    double g_LastWaitTimestamp = 0.0;
    
    virtual EVRCompositorError WaitGetPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        LogMsg("Called: IVRCompositor::WaitGetPoses\n");
        if (pSharedHands) {
            g_LastWaitTimestamp = pSharedHands->poseTimestamp;
        }
        if(pRenderPoseArray && unRenderPoseArrayCount > 0) {
            memset(pRenderPoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unRenderPoseArrayCount);
            for(uint32_t i=0; i<3 && i<unRenderPoseArrayCount; ++i) {
                pRenderPoseArray[i].bPoseIsValid = true;
                pRenderPoseArray[i].bDeviceIsConnected = true;
                pRenderPoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                if (pSharedHands) {
                    float* srcTransform = nullptr;
                    if (i == 0) srcTransform = pSharedHands->headTransform;
                    else if (i == 1) srcTransform = pSharedHands->leftTransform;
                    else if (i == 2) srcTransform = pSharedHands->rightTransform;
                    
                    // Fallback to head tracking if controller tracking is empty (e.g. JoyCon only)
                    if (srcTransform && srcTransform[0] == 0.0f && i > 0) {
                        srcTransform = pSharedHands->headTransform;
                    }
                    
                    if (srcTransform && srcTransform[0] != 0.0f) {
                        for(int r=0;r<3;r++) for(int c=0;c<4;c++) pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = srcTransform[c*4 + r];
                        if (i > 0 && srcTransform == pSharedHands->headTransform) {
                            // Offset controller slightly from head so laser isn't in eyes
                            pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][3] += (i == 1) ? -0.2f : 0.2f;
                            pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] -= 0.2f;
                            pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][3] -= 0.3f;
                        }
                    } else {
                        // Identity fallback
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // Place head 1.5m above ground
                    }
                }
            }
        }
        if(pGamePoseArray && unGamePoseArrayCount > 0) {
            memset(pGamePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unGamePoseArrayCount);
            for(uint32_t i=0; i<3 && i<unGamePoseArrayCount; ++i) {
                pGamePoseArray[i].bPoseIsValid = true;
                pGamePoseArray[i].bDeviceIsConnected = true;
                pGamePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK;
                if (pSharedHands) {
                    float* srcTransform = nullptr;
                    if (i == 0) srcTransform = pSharedHands->headTransform;
                    else if (i == 1) srcTransform = pSharedHands->leftTransform;
                    else if (i == 2) srcTransform = pSharedHands->rightTransform;
                    
                    if (srcTransform && srcTransform[0] == 0.0f && i > 0) {
                        srcTransform = pSharedHands->headTransform;
                    }
                    
                    if (srcTransform && srcTransform[0] != 0.0f) {
                        for(int r=0;r<3;r++) for(int c=0;c<4;c++) pGamePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = srcTransform[c*4 + r];
                        if (i > 0 && srcTransform == pSharedHands->headTransform) {
                            pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][3] += (i == 1) ? -0.2f : 0.2f;
                            pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] -= 0.2f;
                            pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][3] -= 0.3f;
                        }
                    } else {
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // Place head 1.5m above ground
                    }
                }
            }
        }
        
        static float lastLeft[3] = {0,0,0};
        static float lastRight[3] = {0,0,0};
        static DWORD lastTime = GetTickCount();
        DWORD now = GetTickCount();
        float dt = (now - lastTime) / 1000.0f;
        if (dt < 0.001f) dt = 0.001f;
        if (dt > 0.1f) dt = 0.1f;
        
        if (pSharedHands) {
            float cL[3] = { pSharedHands->leftTransform[12], pSharedHands->leftTransform[13], pSharedHands->leftTransform[14] };
            float cR[3] = { pSharedHands->rightTransform[12], pSharedHands->rightTransform[13], pSharedHands->rightTransform[14] };
            
            g_handVelocityL[0] = (cL[0]-lastLeft[0])/dt; g_handVelocityL[1] = (cL[1]-lastLeft[1])/dt; g_handVelocityL[2] = (cL[2]-lastLeft[2])/dt;
            g_handVelocityR[0] = (cR[0]-lastRight[0])/dt; g_handVelocityR[1] = (cR[1]-lastRight[1])/dt; g_handVelocityR[2] = (cR[2]-lastRight[2])/dt;
            
            if (pRenderPoseArray && unRenderPoseArrayCount > 1) {
                pRenderPoseArray[1].vVelocity.v[0] = g_handVelocityL[0]; pRenderPoseArray[1].vVelocity.v[1] = g_handVelocityL[1]; pRenderPoseArray[1].vVelocity.v[2] = g_handVelocityL[2];
            }
            if (pRenderPoseArray && unRenderPoseArrayCount > 2) {
                pRenderPoseArray[2].vVelocity.v[0] = g_handVelocityR[0]; pRenderPoseArray[2].vVelocity.v[1] = g_handVelocityR[1]; pRenderPoseArray[2].vVelocity.v[2] = g_handVelocityR[2];
            }
            if (pGamePoseArray && unGamePoseArrayCount > 1) {
                pGamePoseArray[1].vVelocity.v[0] = g_handVelocityL[0]; pGamePoseArray[1].vVelocity.v[1] = g_handVelocityL[1]; pGamePoseArray[1].vVelocity.v[2] = g_handVelocityL[2];
            }
            if (pGamePoseArray && unGamePoseArrayCount > 2) {
                pGamePoseArray[2].vVelocity.v[0] = g_handVelocityR[0]; pGamePoseArray[2].vVelocity.v[1] = g_handVelocityR[1]; pGamePoseArray[2].vVelocity.v[2] = g_handVelocityR[2];
            }
            
            lastLeft[0] = cL[0]; lastLeft[1] = cL[1]; lastLeft[2] = cL[2];
            lastRight[0] = cR[0]; lastRight[1] = cR[1]; lastRight[2] = cR[2];
        }
        lastTime = now;
        
        static auto lastFrameStart = std::chrono::high_resolution_clock::now();
        auto currentFrameEnd = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameEnd - lastFrameStart).count();
        long targetFrameTime = 11; // ~90fps
        if (elapsed < targetFrameTime) {
            Sleep(targetFrameTime - elapsed);
        }
        lastFrameStart = std::chrono::high_resolution_clock::now();
        
        return vr::VRCompositorError_None;
    }
    virtual EVRCompositorError GetLastPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        LogMsg("Called: IVRCompositor::GetLastPoses\n");
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError GetLastPoseForTrackedDeviceIndex(TrackedDeviceIndex_t unDeviceIndex, TrackedDevicePose_t *pOutputPose, TrackedDevicePose_t *pOutputGamePose) override {
        LogMsg("Called: IVRCompositor::GetLastPoseForTrackedDeviceIndex\n");
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError Submit(vr::EVREye eEye, const vr::Texture_t *pTexture, const vr::VRTextureBounds_t* pBounds, vr::EVRSubmitFlags nSubmitFlags) override {
        if(eEye == vr::Eye_Right) {
            if (pSharedHands) {
                //pSharedHands->sequenceNumber++;
            }
            if (pSharedHands) {
                pSharedHands->renderedTimestamp = g_LastWaitTimestamp;
            }
        }
        if(pTexture && pTexture->handle && pTexture->eType == vr::TextureType_DirectX) {
            ID3D11Texture2D* pGameTex = (ID3D11Texture2D*)pTexture->handle;
            D3D11_TEXTURE2D_DESC desc;
            pGameTex->GetDesc(&desc);
            
            if ((eEye == vr::Eye_Left || eEye == vr::Eye_Right) && hMapFile) {
                ID3D11Device* pDevice = nullptr;
                pGameTex->GetDevice(&pDevice);
                if (pDevice) {
                    ID3D11DeviceContext* pContext = nullptr;
                    pDevice->GetImmediateContext(&pContext);
                    
                    if (pContext) {
                        ID3D11Multithread* pMt = nullptr;
                        pContext->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt);
                        if (pMt) pMt->Enter();

                        bool needsRecreate = false;
                        if (pStagingTexture) {
                            D3D11_TEXTURE2D_DESC existingDesc;
                            pStagingTexture->GetDesc(&existingDesc);
                            if (existingDesc.Width != desc.Width || existingDesc.Height != desc.Height || existingDesc.Format != desc.Format) {
                                pStagingTexture->Release();
                                pStagingTexture = nullptr;
                            }
                        }

                        if (!pStagingTexture) {
                            D3D11_TEXTURE2D_DESC stDesc = desc;
                            stDesc.Usage = D3D11_USAGE_STAGING;
                            stDesc.BindFlags = 0;
                            stDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
                            stDesc.MiscFlags = 0;
                            pDevice->CreateTexture2D(&stDesc, nullptr, &pStagingTexture);
                        }
                        
                        if (pStagingTexture) {
                            pContext->CopyResource(pStagingTexture, pGameTex);
                            
                            D3D11_MAPPED_SUBRESOURCE mapped;
                            if (SUCCEEDED(pContext->Map(pStagingTexture, 0, D3D11_MAP_READ, 0, &mapped))) {
                                    float uMin = pBounds ? pBounds->uMin : 0.0f;
                                    float vMin = pBounds ? pBounds->vMin : 0.0f;
                                    float uMax = pBounds ? pBounds->uMax : 1.0f;
                                    float vMax = pBounds ? pBounds->vMax : 1.0f;
                                    
                                    // Sometimes SteamVR passes inverted v bounds
                                    if (vMin > vMax) {
                                        float tmp = vMin; vMin = vMax; vMax = tmp;
                                    }
                                    if (uMin > uMax) {
                                        float tmp = uMin; uMin = uMax; uMax = tmp;
                                    }
                                    
                                    uint32_t srcWidth = (uint32_t)(desc.Width * (uMax - uMin));
                                    uint32_t srcHeight = (uint32_t)(desc.Height * (vMax - vMin));
                                    uint32_t srcX = (uint32_t)(desc.Width * uMin);
                                    uint32_t srcY = (uint32_t)(desc.Height * vMin);
                                    
                                    if (pHeader && pPixelData) {
                                        pHeader->width = srcWidth * 2;
                                        pHeader->height = srcHeight;
                                        pHeader->format = desc.Format;
                                        pHeader->renderedTimestamp = pSharedHands ? pSharedHands->poseTimestamp : 0.0;
                                    }
                                    
                                    uint32_t copyWidth = (srcWidth > 4096) ? 4096 : srcWidth;
                                    uint32_t copyHeight = (srcHeight > 4096) ? 4096 : srcHeight;
                                    
                                    uint32_t srcBpp = 4;
                                    if (desc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT) srcBpp = 8;
                                    
                                    uint8_t* dst = pPixelData;
                                    uint8_t* src = ((uint8_t*)mapped.pData) + srcY * mapped.RowPitch + srcX * srcBpp;
                                    
                                    if (pHeader) {
                                        pHeader->format = DXGI_FORMAT_B8G8R8A8_UNORM; // Force to 32BGRA
                                    }
                                    
                                    if (eEye == vr::Eye_Left) {
                                        for(uint32_t y=0; y<copyHeight; ++y) {
                                            CopyAndConvertFormat(dst + y * (copyWidth * 2) * 4, src + y * mapped.RowPitch, copyWidth, desc.Format);
                                        }
                                    } else {
                                        // Right eye mapped next to it
                                        for(uint32_t y=0; y<copyHeight; ++y) {
                                            CopyAndConvertFormat(dst + y * (copyWidth * 2) * 4 + copyWidth * 4, src + y * mapped.RowPitch, copyWidth, desc.Format);
                                        }
                                    }
                                    
                                    if (eEye == vr::Eye_Right) {
                                        pHeader->sequenceNumber = ++frameSeq;
                                    }
                                    pContext->Unmap(pStagingTexture, 0);
                                }
                            }
                        if (pMt) { pMt->Leave(); pMt->Release(); }
                        pContext->Release();
                    }
                    pDevice->Release();
                }
            }
        }
        return vr::VRCompositorError_None;
    }
    virtual void ClearLastSubmittedFrame() override {
        LogMsg("Called: IVRCompositor::ClearLastSubmittedFrame\n");
    }
    virtual void PostPresentHandoff() override {
        LogMsg("Called: IVRCompositor::PostPresentHandoff\n");
    }
    virtual bool GetFrameTiming(Compositor_FrameTiming *pTiming, uint32_t unFramesAgo = 0) override {
        LogMsg("Called: IVRCompositor::GetFrameTiming\n");
        return false;
    }
    virtual uint32_t GetFrameTimings(VR_ARRAY_COUNT( nFrames ) Compositor_FrameTiming *pTiming, uint32_t nFrames) override {
        LogMsg("Called: IVRCompositor::GetFrameTimings\n");
        return (uint32_t)0;
    }
    virtual float GetFrameTimeRemaining() override {
        LogMsg("Called: IVRCompositor::GetFrameTimeRemaining\n");
        return (float)0;
    }
    virtual void GetCumulativeStats(Compositor_CumulativeStats *pStats, uint32_t nStatsSizeInBytes) override {
        LogMsg("Called: IVRCompositor::GetCumulativeStats\n");
    }
    virtual void FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground = false) override {
        LogMsg("Called: IVRCompositor::FadeToColor\n");
    }
    virtual void GetCurrentFadeColor(HmdColor_t *pRet, bool bBackground = false) override {
        LogMsg("Called: IVRCompositor::GetCurrentFadeColor\n");
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        return;
    }
    virtual void FadeGrid(float fSeconds, bool bFadeIn) override {
        LogMsg("Called: IVRCompositor::FadeGrid\n");
    }
    virtual float GetCurrentGridAlpha() override {
        LogMsg("Called: IVRCompositor::GetCurrentGridAlpha\n");
        return (float)0;
    }
    virtual EVRCompositorError SetSkyboxOverride(VR_ARRAY_COUNT( unTextureCount ) const Texture_t *pTextures, uint32_t unTextureCount) override {
        LogMsg("Called: IVRCompositor::SetSkyboxOverride\n");
        return vr::VRCompositorError_None;
    }
    virtual void ClearSkyboxOverride() override {
        LogMsg("Called: IVRCompositor::ClearSkyboxOverride\n");
    }
    virtual void CompositorBringToFront() override {
        LogMsg("Called: IVRCompositor::CompositorBringToFront\n");
    }
    virtual void CompositorGoToBack() override {
        LogMsg("Called: IVRCompositor::CompositorGoToBack\n");
    }
    virtual void CompositorQuit() override {
        LogMsg("Called: IVRCompositor::CompositorQuit\n");
    }
    virtual bool IsFullscreen() override {
        LogMsg("Called: IVRCompositor::IsFullscreen\n");
        return false;
    }
    virtual uint32_t GetCurrentSceneFocusProcess() override {
        LogMsg("Called: IVRCompositor::GetCurrentSceneFocusProcess\n");
        return GetCurrentProcessId();
    }
    virtual uint32_t GetLastFrameRenderer() override {
        LogMsg("Called: IVRCompositor::GetLastFrameRenderer\n");
        return (uint32_t)0;
    }
    virtual bool CanRenderScene() override {
        LogMsg("Called: IVRCompositor::CanRenderScene\n");
        return true;
    }
    virtual void ShowMirrorWindow() override {
        LogMsg("Called: IVRCompositor::ShowMirrorWindow\n");
    }
    virtual void HideMirrorWindow() override {
        LogMsg("Called: IVRCompositor::HideMirrorWindow\n");
    }
    virtual bool IsMirrorWindowVisible() override {
        LogMsg("Called: IVRCompositor::IsMirrorWindowVisible\n");
        return false;
    }
    virtual void CompositorDumpImages() override {
        LogMsg("Called: IVRCompositor::CompositorDumpImages\n");
    }
    virtual bool ShouldAppRenderWithLowResources() override {
        LogMsg("Called: IVRCompositor::ShouldAppRenderWithLowResources\n");
        return false;
    }
    virtual void ForceInterleavedReprojectionOn(bool bOverride) override {
        LogMsg("Called: IVRCompositor::ForceInterleavedReprojectionOn\n");
    }
    virtual void ForceReconnectProcess() override {
        LogMsg("Called: IVRCompositor::ForceReconnectProcess\n");
    }
    virtual void SuspendRendering(bool bSuspend) override {
        LogMsg("Called: IVRCompositor::SuspendRendering\n");
    }
    virtual vr::EVRCompositorError GetMirrorTextureD3D11(vr::EVREye eEye, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView) override {
        LogMsg("Called: IVRCompositor::GetMirrorTextureD3D11\n");
        return (vr::EVRCompositorError)1;
    }
    virtual void ReleaseMirrorTextureD3D11(void *pD3D11ShaderResourceView) override {
        LogMsg("Called: IVRCompositor::ReleaseMirrorTextureD3D11\n");
    }
    virtual vr::EVRCompositorError GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t *pglTextureId, vr::glSharedTextureHandle_t *pglSharedTextureHandle) override {
        LogMsg("Called: IVRCompositor::GetMirrorTextureGL\n");
        return (vr::EVRCompositorError)1;
    }
    virtual bool ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        LogMsg("Called: IVRCompositor::ReleaseSharedGLTexture\n");
        return false;
    }
    virtual void LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        LogMsg("Called: IVRCompositor::LockGLSharedTextureForAccess\n");
    }
    virtual void UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        LogMsg("Called: IVRCompositor::UnlockGLSharedTextureForAccess\n");
    }
    virtual uint32_t GetVulkanInstanceExtensionsRequired(VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        LogMsg("Called: IVRCompositor::GetVulkanInstanceExtensionsRequired\n");
        if(pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return 0;
    }
    virtual uint32_t GetVulkanDeviceExtensionsRequired(VkPhysicalDevice_T *pPhysicalDevice, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        LogMsg("Called: IVRCompositor::GetVulkanDeviceExtensionsRequired\n");
        if(pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return 0;
    }
    virtual void SetExplicitTimingMode(EVRCompositorTimingMode eTimingMode) override {
        LogMsg("Called: IVRCompositor::SetExplicitTimingMode\n");
    }
    virtual EVRCompositorError SubmitExplicitTimingData() override {
        LogMsg("Called: IVRCompositor::SubmitExplicitTimingData\n");
        return (EVRCompositorError)1;
    }
    virtual bool IsMotionSmoothingEnabled() override {
        LogMsg("Called: IVRCompositor::IsMotionSmoothingEnabled\n");
        return false;
    }
    virtual bool IsMotionSmoothingSupported() override {
        LogMsg("Called: IVRCompositor::IsMotionSmoothingSupported\n");
        return false;
    }
    virtual bool IsCurrentSceneFocusAppLoading() override {
        LogMsg("Called: IVRCompositor::IsCurrentSceneFocusAppLoading\n");
        return false;
    }
    virtual EVRCompositorError SetStageOverride_Async(const char *pchRenderModelPath, const HmdMatrix34_t *pTransform = 0, const Compositor_StageRenderSettings *pRenderSettings = 0, uint32_t nSizeOfRenderSettings = 0) override {
        LogMsg("Called: IVRCompositor::SetStageOverride_Async\n");
        return vr::VRCompositorError_None;
    }
    virtual void ClearStageOverride() override {
        LogMsg("Called: IVRCompositor::ClearStageOverride\n");
    }
    virtual bool GetCompositorBenchmarkResults(Compositor_BenchmarkResults *pBenchmarkResults, uint32_t nSizeOfBenchmarkResults) override {
        LogMsg("Called: IVRCompositor::GetCompositorBenchmarkResults\n");
        return false;
    }
    virtual EVRCompositorError GetLastPosePredictionIDs(uint32_t *pRenderPosePredictionID, uint32_t *pGamePosePredictionID) override {
        LogMsg("Called: IVRCompositor::GetLastPosePredictionIDs\n");
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError GetPosesForFrame(uint32_t unPosePredictionID, VR_ARRAY_COUNT( unPoseArrayCount ) TrackedDevicePose_t* pPoseArray, uint32_t unPoseArrayCount) override {
        LogMsg("Called: IVRCompositor::GetPosesForFrame\n");
        return (EVRCompositorError)1;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockCompositor;

class Mock_IVRHeadsetView : public vr::IVRHeadsetView {
public:
    virtual void SetHeadsetViewSize(uint32_t nWidth, uint32_t nHeight) override {
        LogMsg("Called: IVRHeadsetView::SetHeadsetViewSize\n");
    }
    virtual void GetHeadsetViewSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        LogMsg("Called: IVRHeadsetView::GetHeadsetViewSize\n");
    }
    virtual void SetHeadsetViewMode(HeadsetViewMode_t eHeadsetViewMode) override {
        LogMsg("Called: IVRHeadsetView::SetHeadsetViewMode\n");
    }
    virtual HeadsetViewMode_t GetHeadsetViewMode() override {
        LogMsg("Called: IVRHeadsetView::GetHeadsetViewMode\n");
        return (HeadsetViewMode_t)0;
    }
    virtual void SetHeadsetViewCropped(bool bCropped) override {
        LogMsg("Called: IVRHeadsetView::SetHeadsetViewCropped\n");
    }
    virtual bool GetHeadsetViewCropped() override {
        LogMsg("Called: IVRHeadsetView::GetHeadsetViewCropped\n");
        return false;
    }
    virtual float GetHeadsetViewAspectRatio() override {
        LogMsg("Called: IVRHeadsetView::GetHeadsetViewAspectRatio\n");
        return (float)0;
    }
    virtual void SetHeadsetViewBlendRange(float flStartPct, float flEndPct) override {
        LogMsg("Called: IVRHeadsetView::SetHeadsetViewBlendRange\n");
    }
    virtual void GetHeadsetViewBlendRange(float *pStartPct, float *pEndPct) override {
        LogMsg("Called: IVRHeadsetView::GetHeadsetViewBlendRange\n");
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockHeadsetView;

class Mock_IVRNotifications : public vr::IVRNotifications {
public:
    virtual EVRNotificationError CreateNotification(VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, EVRNotificationType type, const char *pchText, EVRNotificationStyle style, const NotificationBitmap_t *pImage, VRNotificationId *pNotificationId) override {
        LogMsg("Called: IVRNotifications::CreateNotification\n");
        return (EVRNotificationError)1;
    }
    virtual EVRNotificationError RemoveNotification(VRNotificationId notificationId) override {
        LogMsg("Called: IVRNotifications::RemoveNotification\n");
        return (EVRNotificationError)1;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockNotifications;

class Mock_IVROverlay : public vr::IVROverlay {
public:
    virtual EVROverlayError FindOverlay(const char *pchOverlayKey, VROverlayHandle_t * pOverlayHandle) override {
        LogMsg("Called: IVROverlay::FindOverlay\n");
        if (pOverlayHandle) *pOverlayHandle = 100;
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError CreateOverlay(const char *pchOverlayKey, const char *pchOverlayName, VROverlayHandle_t * pOverlayHandle) override {
        LogMsg("Called: IVROverlay::CreateOverlay\n");
        if (pOverlayHandle) *pOverlayHandle = 100;
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError DestroyOverlay(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::DestroyOverlay\n");
        return vr::VROverlayError_None;
    }
    virtual uint32_t GetOverlayKey(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L) override {
        LogMsg("Called: IVROverlay::GetOverlayKey\n");
        if (pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return (uint32_t)0;
    }
    virtual uint32_t GetOverlayName(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L) override {
        LogMsg("Called: IVROverlay::GetOverlayName\n");
        if (pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return (uint32_t)0;
    }
    virtual EVROverlayError SetOverlayName(VROverlayHandle_t ulOverlayHandle, const char *pchName) override {
        LogMsg("Called: IVROverlay::SetOverlayName\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayImageData(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unBufferSize, uint32_t *punWidth, uint32_t *punHeight) override {
        LogMsg("Called: IVROverlay::GetOverlayImageData\n");
        return vr::VROverlayError_None;
    }
    virtual const char * GetOverlayErrorNameFromEnum(EVROverlayError error) override {
        LogMsg("Called: IVROverlay::GetOverlayErrorNameFromEnum\n");
        return "1.10.30";
    }
    virtual EVROverlayError SetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle, uint32_t unPID) override {
        LogMsg("Called: IVROverlay::SetOverlayRenderingPid\n");
        return vr::VROverlayError_None;
    }
    virtual uint32_t GetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::GetOverlayRenderingPid\n");
        return (uint32_t)0;
    }
    virtual EVROverlayError SetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled) override {
        LogMsg("Called: IVROverlay::SetOverlayFlag\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool *pbEnabled) override {
        LogMsg("Called: IVROverlay::GetOverlayFlag\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayFlags(VROverlayHandle_t ulOverlayHandle, uint32_t *pFlags) override {
        LogMsg("Called: IVROverlay::GetOverlayFlags\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayColor(VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue) override {
        LogMsg("Called: IVROverlay::SetOverlayColor\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayColor(VROverlayHandle_t ulOverlayHandle, float *pfRed, float *pfGreen, float *pfBlue) override {
        LogMsg("Called: IVROverlay::GetOverlayColor\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float fAlpha) override {
        LogMsg("Called: IVROverlay::SetOverlayAlpha\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float *pfAlpha) override {
        LogMsg("Called: IVROverlay::GetOverlayAlpha\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float fTexelAspect) override {
        LogMsg("Called: IVROverlay::SetOverlayTexelAspect\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float *pfTexelAspect) override {
        LogMsg("Called: IVROverlay::GetOverlayTexelAspect\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder) override {
        LogMsg("Called: IVROverlay::SetOverlaySortOrder\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t *punSortOrder) override {
        LogMsg("Called: IVROverlay::GetOverlaySortOrder\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float fWidthInMeters) override {
        LogMsg("Called: IVROverlay::SetOverlayWidthInMeters\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float *pfWidthInMeters) override {
        LogMsg("Called: IVROverlay::GetOverlayWidthInMeters\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float fCurvature) override {
        LogMsg("Called: IVROverlay::SetOverlayCurvature\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float *pfCurvature) override {
        LogMsg("Called: IVROverlay::GetOverlayCurvature\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace eTextureColorSpace) override {
        LogMsg("Called: IVROverlay::SetOverlayTextureColorSpace\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace *peTextureColorSpace) override {
        LogMsg("Called: IVROverlay::GetOverlayTextureColorSpace\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, const VRTextureBounds_t *pOverlayTextureBounds) override {
        LogMsg("Called: IVROverlay::SetOverlayTextureBounds\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, VRTextureBounds_t *pOverlayTextureBounds) override {
        LogMsg("Called: IVROverlay::GetOverlayTextureBounds\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTransformType(VROverlayHandle_t ulOverlayHandle, VROverlayTransformType *peTransformType) override {
        LogMsg("Called: IVROverlay::GetOverlayTransformType\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        LogMsg("Called: IVROverlay::SetOverlayTransformAbsolute\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin *peTrackingOrigin, HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        LogMsg("Called: IVROverlay::GetOverlayTransformAbsolute\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        LogMsg("Called: IVROverlay::SetOverlayTransformTrackedDeviceRelative\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punTrackedDevice, HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        LogMsg("Called: IVROverlay::GetOverlayTransformTrackedDeviceRelative\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unDeviceIndex, const char *pchComponentName) override {
        LogMsg("Called: IVROverlay::SetOverlayTransformTrackedDeviceComponent\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punDeviceIndex, VR_OUT_STRING() char *pchComponentName, uint32_t unComponentNameSize) override {
        LogMsg("Called: IVROverlay::GetOverlayTransformTrackedDeviceComponent\n");
        if (pchComponentName && unComponentNameSize > 0) pchComponentName[0] = '\0';
        return vr::VROverlayError_None;
    }
    virtual vr::EVROverlayError GetOverlayTransformOverlayRelative(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t *ulOverlayHandleParent, HmdMatrix34_t *pmatParentOverlayToOverlayTransform) override {
        LogMsg("Called: IVROverlay::GetOverlayTransformOverlayRelative\n");
        return vr::VROverlayError_None;
    }
    virtual vr::EVROverlayError SetOverlayTransformOverlayRelative(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulOverlayHandleParent, const HmdMatrix34_t *pmatParentOverlayToOverlayTransform) override {
        LogMsg("Called: IVROverlay::SetOverlayTransformOverlayRelative\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTransformCursor(VROverlayHandle_t ulCursorOverlayHandle, const HmdVector2_t *pvHotspot) override {
        LogMsg("Called: IVROverlay::SetOverlayTransformCursor\n");
        return vr::VROverlayError_None;
    }
    virtual vr::EVROverlayError GetOverlayTransformCursor(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvHotspot) override {
        LogMsg("Called: IVROverlay::GetOverlayTransformCursor\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError ShowOverlay(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::ShowOverlay\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError HideOverlay(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::HideOverlay\n");
        return vr::VROverlayError_None;
    }
    virtual bool IsOverlayVisible(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::IsOverlayVisible\n");
        return false;
    }
    virtual EVROverlayError GetTransformForOverlayCoordinates(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, HmdVector2_t coordinatesInOverlay, HmdMatrix34_t *pmatTransform) override {
        LogMsg("Called: IVROverlay::GetTransformForOverlayCoordinates\n");
        return vr::VROverlayError_None;
    }
    virtual bool PollNextOverlayEvent(VROverlayHandle_t ulOverlayHandle, VREvent_t *pEvent, uint32_t uncbVREvent) override {
        LogMsg("Called: IVROverlay::PollNextOverlayEvent\n");
        return false;
    }
    virtual EVROverlayError GetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod *peInputMethod) override {
        LogMsg("Called: IVROverlay::GetOverlayInputMethod\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod) override {
        LogMsg("Called: IVROverlay::SetOverlayInputMethod\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvecMouseScale) override {
        LogMsg("Called: IVROverlay::GetOverlayMouseScale\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvecMouseScale) override {
        LogMsg("Called: IVROverlay::SetOverlayMouseScale\n");
        return vr::VROverlayError_None;
    }
    virtual bool ComputeOverlayIntersection(VROverlayHandle_t ulOverlayHandle, const VROverlayIntersectionParams_t *pParams, VROverlayIntersectionResults_t *pResults) override {
        LogMsg("Called: IVROverlay::ComputeOverlayIntersection\n");
        return false;
    }
    virtual bool IsHoverTargetOverlay(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::IsHoverTargetOverlay\n");
        return false;
    }
    virtual EVROverlayError SetOverlayIntersectionMask(VROverlayHandle_t ulOverlayHandle, VROverlayIntersectionMaskPrimitive_t *pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize = sizeof( VROverlayIntersectionMaskPrimitive_t )) override {
        LogMsg("Called: IVROverlay::SetOverlayIntersectionMask\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError TriggerLaserMouseHapticVibration(VROverlayHandle_t ulOverlayHandle, float fDurationSeconds, float fFrequency, float fAmplitude) override {
        LogMsg("Called: IVROverlay::TriggerLaserMouseHapticVibration\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayCursor(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulCursorHandle) override {
        LogMsg("Called: IVROverlay::SetOverlayCursor\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvCursor) override {
        LogMsg("Called: IVROverlay::SetOverlayCursorPositionOverride\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError ClearOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::ClearOverlayCursorPositionOverride\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayTexture(VROverlayHandle_t ulOverlayHandle, const Texture_t *pTexture) override {
        LogMsg("Called: IVROverlay::SetOverlayTexture\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError ClearOverlayTexture(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::ClearOverlayTexture\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayRaw(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unBytesPerPixel) override {
        LogMsg("Called: IVROverlay::SetOverlayRaw\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError SetOverlayFromFile(VROverlayHandle_t ulOverlayHandle, const char *pchFilePath) override {
        LogMsg("Called: IVROverlay::SetOverlayFromFile\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTexture(VROverlayHandle_t ulOverlayHandle, void **pNativeTextureHandle, void *pNativeTextureRef, uint32_t *pWidth, uint32_t *pHeight, uint32_t *pNativeFormat, ETextureType *pAPIType, EColorSpace *pColorSpace, VRTextureBounds_t *pTextureBounds) override {
        LogMsg("Called: IVROverlay::GetOverlayTexture\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError ReleaseNativeOverlayHandle(VROverlayHandle_t ulOverlayHandle, void *pNativeTextureHandle) override {
        LogMsg("Called: IVROverlay::ReleaseNativeOverlayHandle\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetOverlayTextureSize(VROverlayHandle_t ulOverlayHandle, uint32_t *pWidth, uint32_t *pHeight) override {
        LogMsg("Called: IVROverlay::GetOverlayTextureSize\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError CreateDashboardOverlay(const char *pchOverlayKey, const char *pchOverlayFriendlyName, VROverlayHandle_t * pMainHandle, VROverlayHandle_t *pThumbnailHandle) override {
        LogMsg("Called: IVROverlay::CreateDashboardOverlay\n");
        if (pMainHandle) *pMainHandle = 101;
        if (pThumbnailHandle) *pThumbnailHandle = 102;
        return vr::VROverlayError_None;
    }
    virtual bool IsDashboardVisible() override {
        LogMsg("Called: IVROverlay::IsDashboardVisible\n");
        return false;
    }
    virtual bool IsActiveDashboardOverlay(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlay::IsActiveDashboardOverlay\n");
        return false;
    }
    virtual EVROverlayError SetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId) override {
        LogMsg("Called: IVROverlay::SetDashboardOverlaySceneProcess\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError GetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t *punProcessId) override {
        LogMsg("Called: IVROverlay::GetDashboardOverlaySceneProcess\n");
        return vr::VROverlayError_None;
    }
    virtual void ShowDashboard(const char *pchOverlayToShow) override {
        LogMsg("Called: IVROverlay::ShowDashboard\n");
    }
    virtual vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override {
        LogMsg("Called: IVROverlay::GetPrimaryDashboardDevice\n");
        return (vr::TrackedDeviceIndex_t)0;
    }
    virtual EVROverlayError ShowKeyboard(EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        LogMsg("Called: IVROverlay::ShowKeyboard\n");
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError ShowKeyboardForOverlay(VROverlayHandle_t ulOverlayHandle, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        LogMsg("Called: IVROverlay::ShowKeyboardForOverlay\n");
        return vr::VROverlayError_None;
    }
    virtual uint32_t GetKeyboardText(VR_OUT_STRING() char *pchText, uint32_t cchText) override {
        LogMsg("Called: IVROverlay::GetKeyboardText\n");
        if (pchText && cchText > 0) pchText[0] = '\0';
        return (uint32_t)0;
    }
    virtual void HideKeyboard() override {
        LogMsg("Called: IVROverlay::HideKeyboard\n");
    }
    virtual void SetKeyboardTransformAbsolute(ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToKeyboardTransform) override {
        LogMsg("Called: IVROverlay::SetKeyboardTransformAbsolute\n");
    }
    virtual void SetKeyboardPositionForOverlay(VROverlayHandle_t ulOverlayHandle, HmdRect2_t avoidRect) override {
        LogMsg("Called: IVROverlay::SetKeyboardPositionForOverlay\n");
    }
    virtual VRMessageOverlayResponse ShowMessageOverlay(const char* pchText, const char* pchCaption, const char* pchButton0Text, const char* pchButton1Text = nullptr, const char* pchButton2Text = nullptr, const char* pchButton3Text = nullptr) override {
        LogMsg("Called: IVROverlay::ShowMessageOverlay\n");
        return (VRMessageOverlayResponse)0;
    }
    virtual void CloseMessageOverlay() override {
        LogMsg("Called: IVROverlay::CloseMessageOverlay\n");
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockOverlay;

class Mock_IVROverlayView : public vr::IVROverlayView {
public:
    virtual EVROverlayError AcquireOverlayView(VROverlayHandle_t ulOverlayHandle, VRNativeDevice_t *pNativeDevice, VROverlayView_t *pOverlayView, uint32_t unOverlayViewSize) override {
        LogMsg("Called: IVROverlayView::AcquireOverlayView\n");
        if (pOverlayView && unOverlayViewSize >= sizeof(VROverlayView_t)) {
            memset(pOverlayView, 0, unOverlayViewSize);
        }
        return vr::VROverlayError_None;
    }
    virtual EVROverlayError ReleaseOverlayView(VROverlayView_t *pOverlayView) override {
        LogMsg("Called: IVROverlayView::ReleaseOverlayView\n");
        return vr::VROverlayError_None;
    }
    virtual void PostOverlayEvent(VROverlayHandle_t ulOverlayHandle, const VREvent_t *pvrEvent) override {
        LogMsg("Called: IVROverlayView::PostOverlayEvent\n");
    }
    virtual bool IsViewingPermitted(VROverlayHandle_t ulOverlayHandle) override {
        LogMsg("Called: IVROverlayView::IsViewingPermitted\n");
        return false;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockOverlayView;

class Mock_IVRRenderModels : public vr::IVRRenderModels {
public:
    virtual EVRRenderModelError LoadRenderModel_Async(const char *pchRenderModelName, RenderModel_t **ppRenderModel) override {
        LogMsg("Called: IVRRenderModels::LoadRenderModel_Async\n");
        if (ppRenderModel) *ppRenderModel = nullptr;
        return vr::VRRenderModelError_NotSupported;
    }
    virtual void FreeRenderModel(RenderModel_t *pRenderModel) override {
        LogMsg("Called: IVRRenderModels::FreeRenderModel\n");
    }
    virtual EVRRenderModelError LoadTexture_Async(TextureID_t textureId, RenderModel_TextureMap_t **ppTexture) override {
        LogMsg("Called: IVRRenderModels::LoadTexture_Async\n");
        return vr::VRRenderModelError_NotSupported;
    }
    virtual void FreeTexture(RenderModel_TextureMap_t *pTexture) override {
        LogMsg("Called: IVRRenderModels::FreeTexture\n");
    }
    virtual EVRRenderModelError LoadTextureD3D11_Async(TextureID_t textureId, void *pD3D11Device, void **ppD3D11Texture2D) override {
        LogMsg("Called: IVRRenderModels::LoadTextureD3D11_Async\n");
        return vr::VRRenderModelError_NotSupported;
    }
    virtual EVRRenderModelError LoadIntoTextureD3D11_Async(TextureID_t textureId, void *pDstTexture) override {
        LogMsg("Called: IVRRenderModels::LoadIntoTextureD3D11_Async\n");
        return vr::VRRenderModelError_NotSupported;
    }
    virtual void FreeTextureD3D11(void *pD3D11Texture2D) override {
        LogMsg("Called: IVRRenderModels::FreeTextureD3D11\n");
    }
    virtual uint32_t GetRenderModelName(uint32_t unRenderModelIndex, VR_OUT_STRING() char *pchRenderModelName, uint32_t unRenderModelNameLen) override {
        LogMsg("Called: IVRRenderModels::GetRenderModelName\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetRenderModelCount() override {
        LogMsg("Called: IVRRenderModels::GetRenderModelCount\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetComponentCount(const char *pchRenderModelName) override {
        LogMsg("Called: IVRRenderModels::GetComponentCount\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetComponentName(const char *pchRenderModelName, uint32_t unComponentIndex, VR_OUT_STRING( ) char *pchComponentName, uint32_t unComponentNameLen) override {
        LogMsg("Called: IVRRenderModels::GetComponentName\n");
        return (uint32_t)0;
    }
    virtual uint64_t GetComponentButtonMask(const char *pchRenderModelName, const char *pchComponentName) override {
        LogMsg("Called: IVRRenderModels::GetComponentButtonMask\n");
        return (uint64_t)0;
    }
    virtual uint32_t GetComponentRenderModelName(const char *pchRenderModelName, const char *pchComponentName, VR_OUT_STRING( ) char *pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) override {
        LogMsg("Called: IVRRenderModels::GetComponentRenderModelName\n");
        return (uint32_t)0;
    }
    virtual bool GetComponentStateForDevicePath(const char *pchRenderModelName, const char *pchComponentName, vr::VRInputValueHandle_t devicePath, const vr::RenderModel_ControllerMode_State_t *pState, vr::RenderModel_ComponentState_t *pComponentState) override {
        LogMsg("Called: IVRRenderModels::GetComponentStateForDevicePath\n");
        return false;
    }
    virtual bool GetComponentState(const char *pchRenderModelName, const char *pchComponentName, const vr::VRControllerState_t *pControllerState, const RenderModel_ControllerMode_State_t *pState, RenderModel_ComponentState_t *pComponentState) override {
        LogMsg("Called: IVRRenderModels::GetComponentState\n");
        return false;
    }
    virtual bool RenderModelHasComponent(const char *pchRenderModelName, const char *pchComponentName) override {
        LogMsg("Called: IVRRenderModels::RenderModelHasComponent\n");
        return false;
    }
    virtual uint32_t GetRenderModelThumbnailURL(const char *pchRenderModelName, VR_OUT_STRING() char *pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError *peError) override {
        LogMsg("Called: IVRRenderModels::GetRenderModelThumbnailURL\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetRenderModelOriginalPath(const char *pchRenderModelName, VR_OUT_STRING() char *pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError *peError) override {
        LogMsg("Called: IVRRenderModels::GetRenderModelOriginalPath\n");
        return (uint32_t)0;
    }
    virtual const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) override {
        LogMsg("Called: IVRRenderModels::GetRenderModelErrorNameFromEnum\n");
        return "1.10.30";
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockRenderModels;

class Mock_IVRExtendedDisplay : public vr::IVRExtendedDisplay {
public:
    virtual void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
        LogMsg("Called: IVRExtendedDisplay::GetWindowBounds\n");
    }
    virtual void GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
        LogMsg("Called: IVRExtendedDisplay::GetEyeOutputViewport\n");
        if(pnX) *pnX = 0; if(pnY) *pnY = 0; if(pnWidth) *pnWidth = 1920; if(pnHeight) *pnHeight = 1080;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex, int32_t *pnAdapterOutputIndex) override {
        LogMsg("Called: IVRExtendedDisplay::GetDXGIOutputInfo\n");
        if(pnAdapterIndex) *pnAdapterIndex = 0;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockExtendedDisplay;

class Mock_IVRTrackedCamera : public vr::IVRTrackedCamera {
public:
    virtual const char * GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError) override {
        LogMsg("Called: IVRTrackedCamera::GetCameraErrorNameFromEnum\n");
        return "1.10.30";
    }
    virtual vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool *pHasCamera) override {
        LogMsg("Called: IVRTrackedCamera::HasCamera\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t *pnWidth, uint32_t *pnHeight, uint32_t *pnFrameBufferSize) override {
        LogMsg("Called: IVRTrackedCamera::GetCameraFrameSize\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t *pFocalLength, vr::HmdVector2_t *pCenter) override {
        LogMsg("Called: IVRTrackedCamera::GetCameraIntrinsics\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t *pProjection) override {
        LogMsg("Called: IVRTrackedCamera::GetCameraProjection\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t *pHandle) override {
        LogMsg("Called: IVRTrackedCamera::AcquireVideoStreamingService\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera) override {
        LogMsg("Called: IVRTrackedCamera::ReleaseVideoStreamingService\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        LogMsg("Called: IVRTrackedCamera::GetVideoStreamFrameBuffer\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t *pTextureBounds, uint32_t *pnWidth, uint32_t *pnHeight) override {
        LogMsg("Called: IVRTrackedCamera::GetVideoStreamTextureSize\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        LogMsg("Called: IVRTrackedCamera::GetVideoStreamTextureD3D11\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t *pglTextureId, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        LogMsg("Called: IVRTrackedCamera::GetVideoStreamTextureGL\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId) override {
        LogMsg("Called: IVRTrackedCamera::ReleaseVideoStreamTextureGL\n");
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual void SetCameraTrackingSpace(vr::ETrackingUniverseOrigin eUniverse) override {
        LogMsg("Called: IVRTrackedCamera::SetCameraTrackingSpace\n");
    }
    virtual vr::ETrackingUniverseOrigin GetCameraTrackingSpace() override {
        LogMsg("Called: IVRTrackedCamera::GetCameraTrackingSpace\n");
        return (vr::ETrackingUniverseOrigin)0;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockTrackedCamera;

class Mock_IVRScreenshots : public vr::IVRScreenshots {
public:
    virtual vr::EVRScreenshotError RequestScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, vr::EVRScreenshotType type, const char *pchPreviewFilename, const char *pchVRFilename) override {
        LogMsg("Called: IVRScreenshots::RequestScreenshot\n");
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotError HookScreenshot(VR_ARRAY_COUNT( numTypes ) const vr::EVRScreenshotType *pSupportedTypes, int numTypes) override {
        LogMsg("Called: IVRScreenshots::HookScreenshot\n");
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotType GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError *pError) override {
        LogMsg("Called: IVRScreenshots::GetScreenshotPropertyType\n");
        return (vr::EVRScreenshotType)0;
    }
    virtual uint32_t GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, VR_OUT_STRING() char *pchFilename, uint32_t cchFilename, vr::EVRScreenshotError *pError) override {
        LogMsg("Called: IVRScreenshots::GetScreenshotPropertyFilename\n");
        return (uint32_t)0;
    }
    virtual vr::EVRScreenshotError UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress) override {
        LogMsg("Called: IVRScreenshots::UpdateScreenshotProgress\n");
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotError TakeStereoScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, const char *pchPreviewFilename, const char *pchVRFilename) override {
        LogMsg("Called: IVRScreenshots::TakeStereoScreenshot\n");
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotError SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char *pchSourcePreviewFilename, const char *pchSourceVRFilename) override {
        LogMsg("Called: IVRScreenshots::SubmitScreenshot\n");
        return (vr::EVRScreenshotError)1;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockScreenshots;

class Mock_IVRResources : public vr::IVRResources {
public:
    virtual uint32_t LoadSharedResource(const char *pchResourceName, char *pchBuffer, uint32_t unBufferLen) override {
        LogMsg("Called: IVRResources::LoadSharedResource\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetResourceFullPath(const char *pchResourceName, const char *pchResourceTypeDirectory, VR_OUT_STRING() char *pchPathBuffer, uint32_t unBufferLen) override {
        char msg[512];
        if (pchResourceName) {
            snprintf(msg, sizeof(msg), "GetResourceFullPath requested: %s\n", pchResourceName);
            LogMsg(msg);
        }
        if (pchResourceName && strstr(pchResourceName, "index_controller_profile.json")) {
            const char* path = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\SteamVR\\drivers\\indexcontroller\\resources\\input\\index_controller_profile.json";
            if (pchPathBuffer && unBufferLen > 0) {
                strncpy(pchPathBuffer, path, unBufferLen - 1);
                pchPathBuffer[unBufferLen - 1] = '\0';
                return strlen(path) + 1;
            }
        }
        return (uint32_t)0;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockResources;

class Mock_IVRDriverManager : public vr::IVRDriverManager {
public:
    virtual uint32_t GetDriverCount() const override {
        LogMsg("Called: IVRDriverManager::GetDriverCount\n");
        return (uint32_t)0;
    }
    virtual uint32_t GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        LogMsg("Called: IVRDriverManager::GetDriverName\n");
        return (uint32_t)0;
    }
    virtual DriverHandle_t GetDriverHandle(const char *pchDriverName) override {
        LogMsg("Called: IVRDriverManager::GetDriverHandle\n");
        return (DriverHandle_t)0;
    }
    virtual bool IsEnabled(vr::DriverId_t nDriver) const override {
        LogMsg("Called: IVRDriverManager::IsEnabled\n");
        return false;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockDriverManager;

static std::map<vr::VRActionHandle_t, std::string> g_actionNames;

class Mock_IVRInput : public vr::IVRInput {
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        LogMsg("Called: IVRInput::SetActionManifestPath\n");
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        LogMsg("Called: IVRInput::GetActionSetHandle\n");
        static uint64_t nextSetHandle = 100;
        if(pHandle) *pHandle = nextSetHandle++;
        return vr::VRInputError_None;
    }
    
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        static uint64_t nextHandle = 10;
        if(pHandle) { 
            *pHandle = nextHandle++; 
            if (pchActionName) g_actionNames[*pHandle] = pchActionName;
            char buf[256];
            snprintf(buf, sizeof(buf), "GetActionHandle: %s -> %llu\n", pchActionName, *pHandle);
            LogMsg(buf);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        LogMsg("Called: IVRInput::GetInputSourceHandle\n");
        static uint64_t nextSourceHandle = 10000;
        if(pHandle) { *pHandle = nextSourceHandle++; if(pchInputSourcePath) { if(strstr(pchInputSourcePath, "left")) *pHandle = 1; else if(strstr(pchInputSourcePath, "right")) *pHandle = 2; } }
        return vr::VRInputError_None;
    }
    virtual EVRInputError UpdateActionState(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if(pActionData && unActionDataSize > 0) {
            vr::InputDigitalActionData_t temp = {0};
            temp.bActive = true;
            if (pSharedHands) {
                bool pressed = false;
                uint64_t origin = ulRestrictToDevice;
                if (origin == 0) origin = 2;
                
                std::string name = g_actionNames[action];
                
                static bool leftGripToggle = false;
                static bool rightGripToggle = false;
                static uint32_t lastLeftBtns = 0;
                static uint32_t lastRightBtns = 0;
                
                uint32_t lb = pSharedHands->leftButtons;
                uint32_t rb = pSharedHands->rightButtons;
                
                if ((lb & (1<<7)) && !(lastLeftBtns & (1<<7))) leftGripToggle = !leftGripToggle;
                if ((lb & (1<<8)) && !(lastLeftBtns & (1<<8))) leftGripToggle = !leftGripToggle;
                if ((rb & (1<<5)) && !(lastRightBtns & (1<<5))) rightGripToggle = !rightGripToggle;
                if ((rb & (1<<6)) && !(lastRightBtns & (1<<6))) rightGripToggle = !rightGripToggle;
                
                lastLeftBtns = lb;
                lastRightBtns = rb;
                
                if (name.find("GrabGrip") != std::string::npos || 
                    name.find("GrabPinch") != std::string::npos ||
                    name.find("Pinch") != std::string::npos ||
                    name.find("Grip") != std::string::npos ||
                    name.find("Grab") != std::string::npos) {
                    if (origin == 1) pressed = ((lb & (1<<7)) != 0) || ((lb & (1<<8)) != 0) || pSharedHands->leftPinch; // L or ZL/SL or Pinch
                    else pressed = ((rb & (1<<5)) != 0) || ((rb & (1<<6)) != 0) || pSharedHands->rightPinch; // R or ZR/SR or Pinch
                }
                else if (name.find("Trigger") != std::string::npos || name.find("Fire") != std::string::npos || name.find("Interact") != std::string::npos || name.find("Use") != std::string::npos) {
                    if (origin == 1) pressed = ((lb & (1<<4)) != 0) || pSharedHands->leftPinch; // ZL or Pinch
                    else pressed = ((rb & (1<<2)) != 0) || ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0) || pSharedHands->rightPinch; // ZR or Spacebar or Pinch
                }
                else if (name.find("Teleport") != std::string::npos) {
                    if (origin == 2) pressed = (rb & (1<<3)) != 0; // R3
                    else pressed = (lb & (1<<5)) != 0; // L3
                }
                else if (name.find("a_button") != std::string::npos || name.find("AButton") != std::string::npos) {
                    if (origin == 2) pressed = ((rb & (1<<0)) != 0) || ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0); // Right A or Spacebar
                    else pressed = (lb & (1<<0)) != 0; // Left D-Pad Right
                }
                else if (name.find("b_button") != std::string::npos || name.find("BButton") != std::string::npos) {
                    if (origin == 2) pressed = (rb & (1<<1)) != 0; // Right B
                    else pressed = (lb & (1<<1)) != 0; // Left D-Pad Down
                }
                else if (name.find("Pause") != std::string::npos || name.find("System") != std::string::npos || name.find("Menu") != std::string::npos) {
                    if (origin == 2) pressed = (rb & (1<<4)) != 0; // Plus
                    else pressed = (lb & (1<<6)) != 0; // Minus
                }
                
                static std::map<std::pair<VRActionHandle_t, uint64_t>, bool> s_lastState;
                std::pair<VRActionHandle_t, uint64_t> key = {action, origin};
                temp.bState = pressed;
                temp.bChanged = (s_lastState[key] != pressed);
                temp.activeOrigin = origin;
                temp.fUpdateTime = (float)GetTickCount() / 1000.0f;
                s_lastState[key] = pressed;
                
                if (temp.bChanged || temp.bState) {
                    char msg[256];
                    snprintf(msg, 256, "Action '%s' (origin=%llu): bState=%d, bChanged=%d\n", name.c_str(), origin, temp.bState, temp.bChanged);
                    LogMsg(msg);
                }
            }
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if(pActionData && unActionDataSize > 0) {
            vr::InputAnalogActionData_t temp = {0};
            temp.bActive = true;
            if (pSharedHands) {
                float x = 0.0f;
                float y = 0.0f;
                uint64_t origin = ulRestrictToDevice;
                if (origin == 0) origin = 2;

                std::string name = g_actionNames[action];
                uint32_t lb = pSharedHands->leftButtons;
                uint32_t rb = pSharedHands->rightButtons;

                if (name.find("Move") != std::string::npos || name.find("Trackpad") != std::string::npos || name.find("Thumbstick") != std::string::npos || name.find("Turn") != std::string::npos || name.find("Scroll") != std::string::npos) {
                    if (origin == 1) {
                        x = pSharedHands->leftStickX;
                        y = -pSharedHands->leftStickY;
                    } else {
                        x = pSharedHands->rightStickX;
                        y = -pSharedHands->rightStickY;
                    }
                }
                else if (name.find("Trigger") != std::string::npos || name.find("Pull") != std::string::npos || name.find("Pinch") != std::string::npos || name.find("Use") != std::string::npos) {
                    if (origin == 1) x = ((lb & (1<<4)) || pSharedHands->leftPinch) ? 1.0f : 0.0f;
                    else x = ((rb & (1<<2)) || pSharedHands->rightPinch) ? 1.0f : 0.0f;
                }
                
                static std::map<std::pair<VRActionHandle_t, uint64_t>, std::pair<float, float>> s_lastAnalog;
                std::pair<VRActionHandle_t, uint64_t> key = {action, origin};
                float lastX = s_lastAnalog[key].first;
                float lastY = s_lastAnalog[key].second;
                
                temp.x = x;
                temp.y = y;
                temp.deltaX = x - lastX;
                temp.deltaY = y - lastY;
                temp.activeOrigin = origin;
                temp.fUpdateTime = (float)GetTickCount() / 1000.0f;
                
                s_lastAnalog[key] = {x, y};
            }
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataRelativeToNow(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if(pActionData && unActionDataSize > 0) {
            vr::InputPoseActionData_t temp = {0};
            temp.bActive = true;
            temp.pose.bPoseIsValid = true;
            temp.pose.bDeviceIsConnected = true;
            temp.pose.eTrackingResult = vr::TrackingResult_Running_OK;
            uint64_t origin = ulRestrictToDevice;
            if (origin == 0) origin = 2;
            temp.activeOrigin = origin;
            if (origin == 1 && pSharedHands && pSharedHands->leftTransform[0] != 0.0f) {
                for(int r=0;r<3;r++) for(int c=0;c<4;c++) temp.pose.mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->leftTransform[c*4 + r];
                temp.pose.vVelocity.v[0] = g_handVelocityL[0]; temp.pose.vVelocity.v[1] = g_handVelocityL[1]; temp.pose.vVelocity.v[2] = g_handVelocityL[2];
            } else if (origin == 2 && pSharedHands && pSharedHands->rightTransform[0] != 0.0f) {
                for(int r=0;r<3;r++) for(int c=0;c<4;c++) temp.pose.mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->rightTransform[c*4 + r];
                temp.pose.vVelocity.v[0] = g_handVelocityR[0]; temp.pose.vVelocity.v[1] = g_handVelocityR[1]; temp.pose.vVelocity.v[2] = g_handVelocityR[2];
            } else {
                temp.pose.mDeviceToAbsoluteTracking.m[0][0] = 1; temp.pose.mDeviceToAbsoluteTracking.m[1][1] = 1; temp.pose.mDeviceToAbsoluteTracking.m[2][2] = 1;
            }
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if(pActionData && unActionDataSize > 0) {
            vr::InputPoseActionData_t temp = {0};
            temp.bActive = true;
            temp.pose.bPoseIsValid = true;
            temp.pose.bDeviceIsConnected = true;
            temp.pose.eTrackingResult = vr::TrackingResult_Running_OK;
            uint64_t origin = ulRestrictToDevice;
            if (origin == 0) origin = 2;
            temp.activeOrigin = origin;
            if (origin == 1 && pSharedHands && pSharedHands->leftTransform[0] != 0.0f) {
                for(int r=0;r<3;r++) for(int c=0;c<4;c++) temp.pose.mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->leftTransform[c*4 + r];
                temp.pose.vVelocity.v[0] = g_handVelocityL[0]; temp.pose.vVelocity.v[1] = g_handVelocityL[1]; temp.pose.vVelocity.v[2] = g_handVelocityL[2];
            } else if (origin == 2 && pSharedHands && pSharedHands->rightTransform[0] != 0.0f) {
                for(int r=0;r<3;r++) for(int c=0;c<4;c++) temp.pose.mDeviceToAbsoluteTracking.m[r][c] = pSharedHands->rightTransform[c*4 + r];
                temp.pose.vVelocity.v[0] = g_handVelocityR[0]; temp.pose.vVelocity.v[1] = g_handVelocityR[1]; temp.pose.vVelocity.v[2] = g_handVelocityR[2];
            } else {
                temp.pose.mDeviceToAbsoluteTracking.m[0][0] = 1; temp.pose.mDeviceToAbsoluteTracking.m[1][1] = 1; temp.pose.mDeviceToAbsoluteTracking.m[2][2] = 1;
            }
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalActionData(VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize) override {
        if (pActionData && unActionDataSize >= sizeof(InputSkeletalActionData_t)) {
            pActionData->bActive = true;
            pActionData->activeOrigin = 2;
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetDominantHand(ETrackedControllerRole *peDominantHand) override {
        if(peDominantHand) *peDominantHand = vr::TrackedControllerRole_RightHand;
        return vr::VRInputError_None;
    }
    virtual EVRInputError SetDominantHand(ETrackedControllerRole eDominantHand) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetBoneCount(VRActionHandle_t action, uint32_t* pBoneCount) override {
        if(pBoneCount) *pBoneCount = 31;
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetBoneHierarchy(VRActionHandle_t action, BoneIndex_t* pParentIndices, uint32_t unIndexArayCount) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetBoneName(VRActionHandle_t action, BoneIndex_t nBoneIndex, char* pchBoneName, uint32_t unNameBufferSize) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalReferenceTransforms(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalReferencePose eReferencePose, VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalTrackingLevel(VRActionHandle_t action, EVRSkeletalTrackingLevel* pSkeletalTrackingLevel) override {
        if(pSkeletalTrackingLevel) *pSkeletalTrackingLevel = vr::VRSkeletalTracking_Partial;
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalBoneData(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalMotionRange eMotionRange, VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalSummaryData(VRActionHandle_t action, EVRSummaryType eSummaryType, VRSkeletalSummaryData_t * pSkeletalSummaryData) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalBoneDataCompressed(VRActionHandle_t action, EVRSkeletalMotionRange eMotionRange, void *pvCompressedData, uint32_t unCompressedSize, uint32_t *punRequiredCompressedSize) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError DecompressSkeletalBoneData(const void *pvCompressedBuffer, uint32_t unCompressedBufferSize, EVRSkeletalTransformSpace eTransformSpace, VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError TriggerHapticVibrationAction(VRActionHandle_t action, float fStartSecondsFromNow, float fDurationSeconds, float fFrequency, float fAmplitude, VRInputValueHandle_t ulRestrictToDevice) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t digitalActionHandle, VRInputValueHandle_t *originsOut, uint32_t originOutCount) override {
        if (originsOut && originOutCount > 0) {
            memset(originsOut, 0, sizeof(vr::VRInputValueHandle_t) * originOutCount);
            if (originOutCount >= 2) {
                originsOut[0] = 1; // Left Hand
                originsOut[1] = 2; // Right Hand
            } else {
                originsOut[0] = 1;
            }
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetOriginLocalizedName(VRInputValueHandle_t origin, char *pchNameArray, uint32_t unNameArraySize, int32_t unStringSectionsToInclude) override {
        if(pchNameArray && unNameArraySize > 0) {
            strncpy(pchNameArray, "Trigger", unNameArraySize - 1);
            pchNameArray[unNameArraySize - 1] = '\0';
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetOriginTrackedDeviceInfo(VRInputValueHandle_t origin, InputOriginInfo_t *pOriginInfo, uint32_t unOriginInfoSize) override {
        if(pOriginInfo && unOriginInfoSize > 0) {
            memset(pOriginInfo, 0, unOriginInfoSize);
            if (unOriginInfoSize >= sizeof(vr::VRInputValueHandle_t)) {
                pOriginInfo->devicePath = origin;
            }
            if (unOriginInfoSize >= offsetof(InputOriginInfo_t, trackedDeviceIndex) + sizeof(vr::TrackedDeviceIndex_t)) {
                pOriginInfo->trackedDeviceIndex = origin;
            }
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetActionBindingInfo(VRActionHandle_t action, InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, uint32_t *punReturnedBindingInfoCount) override {
        if (pOriginInfo && unBindingInfoCount > 0 && unBindingInfoSize > 0) {
            memset(pOriginInfo, 0, unBindingInfoSize * unBindingInfoCount);
            
            // Write only within the allowed bounds
            auto safe_strcpy = [unBindingInfoSize](char* dst, const char* src, size_t offset, size_t max_len) {
                if (offset < unBindingInfoSize) {
                    size_t available = unBindingInfoSize - offset;
                    size_t copy_len = (max_len < available) ? max_len : available;
                    strncpy(dst, src, copy_len - 1);
                    dst[copy_len - 1] = '\0';
                }
            };
            
            // Fallback to simple copying if we know the struct matches our definition
            if (unBindingInfoSize >= sizeof(InputBindingInfo_t)) {
                strncpy(pOriginInfo[0].rchDevicePathName, "/user/hand/right", 127);
                strncpy(pOriginInfo[0].rchInputPathName, "/user/hand/right/input/trigger", 127);
                strncpy(pOriginInfo[0].rchModeName, "trigger", 127);
                strncpy(pOriginInfo[0].rchSlotName, "click", 127);
                strncpy(pOriginInfo[0].rchInputSourceType, "trackpad", 31);
            }
        }
        if (punReturnedBindingInfoCount) *punReturnedBindingInfoCount = 1;
        return vr::VRInputError_None;
    }
    virtual EVRInputError ShowActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t ulActionHandle) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError ShowBindingsForActionSet(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount, VRInputValueHandle_t originToHighlight) override {
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetComponentStateForBinding(const char *pchRenderModelName, const char *pchComponentName, const InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, vr::RenderModel_ComponentState_t *pComponentState) override {
        return vr::VRInputError_None;
    }
    virtual bool IsUsingLegacyInput() override {
        return false;
    }
    virtual EVRInputError OpenBindingUI(const char* pchAppKey, VRActionSetHandle_t ulActionSetHandle, VRInputValueHandle_t ulDeviceHandle, bool bShowOnDesktop) override {
        LogMsg("Called: IVRInput::OpenBindingUI\n");
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetBindingVariant(vr::VRInputValueHandle_t ulDevicePath, VR_OUT_STRING() char *pchVariantArray, uint32_t unVariantArraySize) override {
        LogMsg("Called: IVRInput::GetBindingVariant\n");
        return vr::VRInputError_InvalidHandle;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockInput;

class Mock_IVRIOBuffer : public vr::IVRIOBuffer {
public:
    virtual vr::EIOBufferError Open(const char *pchPath, vr::EIOBufferMode mode, uint32_t unElementSize, uint32_t unElements, vr::IOBufferHandle_t *pulBuffer) override {
        LogMsg("Called: IVRIOBuffer::Open\n");
        return (vr::EIOBufferError)1;
    }
    virtual vr::EIOBufferError Close(vr::IOBufferHandle_t ulBuffer) override {
        LogMsg("Called: IVRIOBuffer::Close\n");
        return (vr::EIOBufferError)1;
    }
    virtual vr::EIOBufferError Read(vr::IOBufferHandle_t ulBuffer, void *pDst, uint32_t unBytes, uint32_t *punRead) override {
        LogMsg("Called: IVRIOBuffer::Read\n");
        return (vr::EIOBufferError)1;
    }
    virtual vr::EIOBufferError Write(vr::IOBufferHandle_t ulBuffer, void *pSrc, uint32_t unBytes) override {
        LogMsg("Called: IVRIOBuffer::Write\n");
        return (vr::EIOBufferError)1;
    }
    virtual vr::PropertyContainerHandle_t PropertyContainer(vr::IOBufferHandle_t ulBuffer) override {
        LogMsg("Called: IVRIOBuffer::PropertyContainer\n");
        return (vr::PropertyContainerHandle_t)0;
    }
    virtual bool HasReaders(vr::IOBufferHandle_t ulBuffer) override {
        LogMsg("Called: IVRIOBuffer::HasReaders\n");
        return false;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockIOBuffer;

class Mock_IVRSpatialAnchors : public vr::IVRSpatialAnchors {
public:
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromDescriptor(const char *pchDescriptor, SpatialAnchorHandle_t *pHandleOut) override {
        LogMsg("Called: IVRSpatialAnchors::CreateSpatialAnchorFromDescriptor\n");
        return (EVRSpatialAnchorError)1;
    }
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromPose(TrackedDeviceIndex_t unDeviceIndex, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPose, SpatialAnchorHandle_t *pHandleOut) override {
        LogMsg("Called: IVRSpatialAnchors::CreateSpatialAnchorFromPose\n");
        return (EVRSpatialAnchorError)1;
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorPose(SpatialAnchorHandle_t unHandle, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPoseOut) override {
        LogMsg("Called: IVRSpatialAnchors::GetSpatialAnchorPose\n");
        return (EVRSpatialAnchorError)1;
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorDescriptor(SpatialAnchorHandle_t unHandle, VR_OUT_STRING() char *pchDescriptorOut, uint32_t *punDescriptorBufferLenInOut) override {
        LogMsg("Called: IVRSpatialAnchors::GetSpatialAnchorDescriptor\n");
        return (EVRSpatialAnchorError)1;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockSpatialAnchors;

class Mock_IVRDebug : public vr::IVRDebug {
public:
    virtual EVRDebugError EmitVrProfilerEvent(const char *pchMessage) override {
        LogMsg("Called: IVRDebug::EmitVrProfilerEvent\n");
        return (EVRDebugError)1;
    }
    virtual EVRDebugError BeginVrProfilerEvent(VrProfilerEventHandle_t *pHandleOut) override {
        LogMsg("Called: IVRDebug::BeginVrProfilerEvent\n");
        return (EVRDebugError)1;
    }
    virtual EVRDebugError FinishVrProfilerEvent(VrProfilerEventHandle_t hHandle, const char *pchMessage) override {
        LogMsg("Called: IVRDebug::FinishVrProfilerEvent\n");
        return (EVRDebugError)1;
    }
    virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, VR_OUT_STRING() char *pchResponseBuffer, uint32_t unResponseBufferSize) override {
        LogMsg("Called: IVRDebug::DriverDebugRequest\n");
        return (uint32_t)0;
    }
    virtual void* DummyPadding0() { return nullptr; }
    virtual void* DummyPadding1() { return nullptr; }
    virtual void* DummyPadding2() { return nullptr; }
    virtual void* DummyPadding3() { return nullptr; }
    virtual void* DummyPadding4() { return nullptr; }
    virtual void* DummyPadding5() { return nullptr; }
    virtual void* DummyPadding6() { return nullptr; }
    virtual void* DummyPadding7() { return nullptr; }
    virtual void* DummyPadding8() { return nullptr; }
    virtual void* DummyPadding9() { return nullptr; }
    virtual void* DummyPadding10() { return nullptr; }
    virtual void* DummyPadding11() { return nullptr; }
    virtual void* DummyPadding12() { return nullptr; }
    virtual void* DummyPadding13() { return nullptr; }
    virtual void* DummyPadding14() { return nullptr; }
    virtual void* DummyPadding15() { return nullptr; }
    virtual void* DummyPadding16() { return nullptr; }
    virtual void* DummyPadding17() { return nullptr; }
    virtual void* DummyPadding18() { return nullptr; }
    virtual void* DummyPadding19() { return nullptr; }
    virtual void* DummyPadding20() { return nullptr; }
    virtual void* DummyPadding21() { return nullptr; }
    virtual void* DummyPadding22() { return nullptr; }
    virtual void* DummyPadding23() { return nullptr; }
    virtual void* DummyPadding24() { return nullptr; }
    virtual void* DummyPadding25() { return nullptr; }
    virtual void* DummyPadding26() { return nullptr; }
    virtual void* DummyPadding27() { return nullptr; }
    virtual void* DummyPadding28() { return nullptr; }
    virtual void* DummyPadding29() { return nullptr; }
    virtual void* DummyPadding30() { return nullptr; }
    virtual void* DummyPadding31() { return nullptr; }
    virtual void* DummyPadding32() { return nullptr; }
    virtual void* DummyPadding33() { return nullptr; }
    virtual void* DummyPadding34() { return nullptr; }
    virtual void* DummyPadding35() { return nullptr; }
    virtual void* DummyPadding36() { return nullptr; }
    virtual void* DummyPadding37() { return nullptr; }
    virtual void* DummyPadding38() { return nullptr; }
    virtual void* DummyPadding39() { return nullptr; }
    virtual void* DummyPadding40() { return nullptr; }
    virtual void* DummyPadding41() { return nullptr; }
    virtual void* DummyPadding42() { return nullptr; }
    virtual void* DummyPadding43() { return nullptr; }
    virtual void* DummyPadding44() { return nullptr; }
    virtual void* DummyPadding45() { return nullptr; }
    virtual void* DummyPadding46() { return nullptr; }
    virtual void* DummyPadding47() { return nullptr; }
    virtual void* DummyPadding48() { return nullptr; }
    virtual void* DummyPadding49() { return nullptr; }
    virtual void* DummyPadding50() { return nullptr; }
    virtual void* DummyPadding51() { return nullptr; }
    virtual void* DummyPadding52() { return nullptr; }
    virtual void* DummyPadding53() { return nullptr; }
    virtual void* DummyPadding54() { return nullptr; }
    virtual void* DummyPadding55() { return nullptr; }
    virtual void* DummyPadding56() { return nullptr; }
    virtual void* DummyPadding57() { return nullptr; }
    virtual void* DummyPadding58() { return nullptr; }
    virtual void* DummyPadding59() { return nullptr; }
    virtual void* DummyPadding60() { return nullptr; }
    virtual void* DummyPadding61() { return nullptr; }
    virtual void* DummyPadding62() { return nullptr; }
    virtual void* DummyPadding63() { return nullptr; }
    virtual void* DummyPadding64() { return nullptr; }
    virtual void* DummyPadding65() { return nullptr; }
    virtual void* DummyPadding66() { return nullptr; }
    virtual void* DummyPadding67() { return nullptr; }
    virtual void* DummyPadding68() { return nullptr; }
    virtual void* DummyPadding69() { return nullptr; }
    virtual void* DummyPadding70() { return nullptr; }
    virtual void* DummyPadding71() { return nullptr; }
    virtual void* DummyPadding72() { return nullptr; }
    virtual void* DummyPadding73() { return nullptr; }
    virtual void* DummyPadding74() { return nullptr; }
    virtual void* DummyPadding75() { return nullptr; }
    virtual void* DummyPadding76() { return nullptr; }
    virtual void* DummyPadding77() { return nullptr; }
    virtual void* DummyPadding78() { return nullptr; }
    virtual void* DummyPadding79() { return nullptr; }
    virtual void* DummyPadding80() { return nullptr; }
    virtual void* DummyPadding81() { return nullptr; }
    virtual void* DummyPadding82() { return nullptr; }
    virtual void* DummyPadding83() { return nullptr; }
    virtual void* DummyPadding84() { return nullptr; }
    virtual void* DummyPadding85() { return nullptr; }
    virtual void* DummyPadding86() { return nullptr; }
    virtual void* DummyPadding87() { return nullptr; }
    virtual void* DummyPadding88() { return nullptr; }
    virtual void* DummyPadding89() { return nullptr; }
    virtual void* DummyPadding90() { return nullptr; }
    virtual void* DummyPadding91() { return nullptr; }
    virtual void* DummyPadding92() { return nullptr; }
    virtual void* DummyPadding93() { return nullptr; }
    virtual void* DummyPadding94() { return nullptr; }
    virtual void* DummyPadding95() { return nullptr; }
    virtual void* DummyPadding96() { return nullptr; }
    virtual void* DummyPadding97() { return nullptr; }
    virtual void* DummyPadding98() { return nullptr; }
    virtual void* DummyPadding99() { return nullptr; }
} g_mockDebug;


extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    if (peError) *peError = vr::VRInitError_None;
    {
    char _buf[256];
    snprintf(_buf, sizeof(_buf), "Requested interface: %s\n", pchInterfaceVersion);
    LogMsg(_buf);
}

    if (strstr(pchInterfaceVersion, "IVRMailbox")) return &g_universalMock;

    if (strstr(pchInterfaceVersion, "System")) return &g_mockSystem;
    if (strstr(pchInterfaceVersion, "Applications")) return &g_mockApplications;
    if (strstr(pchInterfaceVersion, "Settings")) return &g_mockSettings;
    if (strstr(pchInterfaceVersion, "Chaperone")) return &g_mockChaperone;
    if (strstr(pchInterfaceVersion, "ChaperoneSetup")) return &g_mockChaperoneSetup;
    if (strstr(pchInterfaceVersion, "Compositor")) return &g_mockCompositor;
    if (strstr(pchInterfaceVersion, "HeadsetView")) return &g_mockHeadsetView;
    if (strstr(pchInterfaceVersion, "Notifications")) return &g_mockNotifications;
    if (strstr(pchInterfaceVersion, "Overlay")) return &g_mockOverlay;
    if (strstr(pchInterfaceVersion, "OverlayView")) return &g_mockOverlayView;
    if (strstr(pchInterfaceVersion, "RenderModels")) return &g_mockRenderModels;
    if (strstr(pchInterfaceVersion, "ExtendedDisplay")) return &g_mockExtendedDisplay;
    if (strstr(pchInterfaceVersion, "TrackedCamera")) return &g_mockTrackedCamera;
    if (strstr(pchInterfaceVersion, "Screenshots")) return &g_mockScreenshots;
    if (strstr(pchInterfaceVersion, "Resources")) return &g_mockResources;
    if (strstr(pchInterfaceVersion, "DriverManager")) return &g_mockDriverManager;
    if (strstr(pchInterfaceVersion, "Input")) return &g_mockInput;
    if (strstr(pchInterfaceVersion, "IOBuffer")) return &g_mockIOBuffer;
    if (strstr(pchInterfaceVersion, "SpatialAnchors")) return &g_mockSpatialAnchors;
    if (strstr(pchInterfaceVersion, "Debug")) return &g_mockDebug;

    return nullptr;
}

void ApplyCOMHooks();

extern "C" __declspec(dllexport) uint32_t VR_InitInternal2(vr::EVRInitError *peError, vr::EVRApplicationType eType, const char *pStartupInfo) {
    if (peError) *peError = vr::VRInitError_None;
    LogMsg("Called: VR_InitInternal2\n");

    // Inject COM VTable hooks to capture rendering context safely
    //ApplyCOMHooks();

    InitSharedMemory();
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
extern "C" __declspec(dllexport) bool VR_GetRuntimePath(char *pchPathBuffer, uint32_t unBufferSize, uint32_t *punRequiredBufferSize) { 
    if (punRequiredBufferSize) *punRequiredBufferSize = 1; 
    if (pchPathBuffer && unBufferSize > 0) pchPathBuffer[0] = ' '; 
    return true; 
}
extern "C" __declspec(dllexport) const char* VR_GetStringForHmdError(vr::EVRInitError error) { return ""; }
extern "C" __declspec(dllexport) uint32_t VR_InitInternal(vr::EVRInitError *peError, vr::EVRApplicationType eType) {
    if (peError) *peError = vr::VRInitError_None;
    InitSharedMemory();
    return 1;
}
extern "C" __declspec(dllexport) bool VR_IsInterfaceVersionValid(const char *pchInterfaceVersion) { return true; }
extern "C" __declspec(dllexport) bool VR_IsRuntimeInstalled() { return true; }

#include "iat_hook.h"
#include <d3d11_4.h> // Include proper header for ID3D11Multithread
#include "MinHook.h"

/* REMOVED_LOGMSG */ void OldLogMsg(const char* msg) {
    HANDLE hFile = CreateFileA("vr_emulator_log.txt", FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(hFile, msg, strlen(msg), &written, nullptr);
        CloseHandle(hFile);
    }
}


// --- COM VTABLE HOOKING ---
typedef HRESULT (WINAPI *CreateTexture2D_t)(ID3D11Device* This, const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
typedef HRESULT (WINAPI *CreateSwapChain_t)(IDXGIFactory* This, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain);

CreateTexture2D_t g_pOriginalCreateTexture2D = nullptr;
CreateSwapChain_t g_pOriginalCreateSwapChain = nullptr;

__attribute__((force_align_arg_pointer))
HRESULT WINAPI Hooked_CreateTexture2D(ID3D11Device* This, const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D) {
    static bool bMtEnabled = false;
    if (!bMtEnabled) {
        bMtEnabled = true;
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED(This->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt)) && pMt) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
            FILE* _f = fopen("vr_emulator_log.txt", "a");
            if (_f) { fprintf(_f, "[Verantyx] ID3D11Device::CreateTexture2D HOOK: Multithreading Enabled!\n"); fclose(_f); }
        }
    }
    return g_pOriginalCreateTexture2D(This, pDesc, pInitialData, ppTexture2D);
}

__attribute__((force_align_arg_pointer))
HRESULT WINAPI Hooked_CreateSwapChain(IDXGIFactory* This, IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) {
    if (pDevice) {
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED(pDevice->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt)) && pMt) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
            FILE* _f = fopen("vr_emulator_log.txt", "a");
            if (_f) { fprintf(_f, "[Verantyx] IDXGIFactory::CreateSwapChain HOOK: Multithreading Enabled!\n"); fclose(_f); }
        }
    }
    return g_pOriginalCreateSwapChain(This, pDevice, pDesc, ppSwapChain);
}

typedef HRESULT (WINAPI *CreateBuffer_t)(ID3D11Device* This, const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);
typedef HRESULT (WINAPI *CreateSwapChainForHwnd_t)(IUnknown* This, IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain);

CreateBuffer_t g_pOriginalCreateBuffer = nullptr;
CreateSwapChainForHwnd_t g_pOriginalCreateSwapChainForHwnd = nullptr;

__attribute__((force_align_arg_pointer))
HRESULT WINAPI Hooked_CreateBuffer(ID3D11Device* This, const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer) {
    static bool bMtEnabled = false;
    if (!bMtEnabled) {
        bMtEnabled = true;
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED(This->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt)) && pMt) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
            FILE* _f = fopen("vr_emulator_log.txt", "a");
            if (_f) { fprintf(_f, "[Verantyx] ID3D11Device::CreateBuffer HOOK: Multithreading Enabled!\n"); fclose(_f); }
        }
    }
    return g_pOriginalCreateBuffer(This, pDesc, pInitialData, ppBuffer);
}

__attribute__((force_align_arg_pointer))
HRESULT WINAPI Hooked_CreateSwapChainForHwnd(IUnknown* This, IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) {
    if (pDevice) {
        ID3D11Multithread* pMt = nullptr;
        if (SUCCEEDED(pDevice->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt)) && pMt) {
            pMt->SetMultithreadProtected(TRUE);
            pMt->Release();
            FILE* _f = fopen("vr_emulator_log.txt", "a");
            if (_f) { fprintf(_f, "[Verantyx] IDXGIFactory2::CreateSwapChainForHwnd HOOK: Multithreading Enabled!\n"); fclose(_f); }
        }
    }
    return g_pOriginalCreateSwapChainForHwnd(This, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
}

void ApplyCOMHooks() {
    static bool bInjected = false;
    if (bInjected) return;
    bInjected = true;

    MH_Initialize();

    HMODULE hD3D11 = LoadLibraryA("d3d11.dll");
    HMODULE hDXGI = LoadLibraryA("dxgi.dll");

    if (hD3D11 && hDXGI) {
        typedef HRESULT (WINAPI *D3D11CreateDevice_t)(IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT, const D3D_FEATURE_LEVEL*, UINT, UINT, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);
        D3D11CreateDevice_t pD3D11CreateDevice = (D3D11CreateDevice_t)GetProcAddress(hD3D11, "D3D11CreateDevice");

        typedef HRESULT (WINAPI *CreateDXGIFactory1_t)(REFIID riid, void **ppFactory);
        CreateDXGIFactory1_t pCreateDXGIFactory1 = (CreateDXGIFactory1_t)GetProcAddress(hDXGI, "CreateDXGIFactory1");

        if (pD3D11CreateDevice) {
            ID3D11Device* pDummyDevice = nullptr;
            ID3D11DeviceContext* pDummyContext = nullptr;
            D3D_FEATURE_LEVEL featureLevel;
            if (SUCCEEDED(pD3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &pDummyDevice, &featureLevel, &pDummyContext)) && pDummyDevice) {
                void** pVTable = *(void***)pDummyDevice;
                
                void* pCreateBuffer = pVTable[3];
                void* pCreateTexture2D = pVTable[5];
                
                MH_CreateHook(pCreateBuffer, (void*)Hooked_CreateBuffer, (LPVOID*)&g_pOriginalCreateBuffer);
                MH_CreateHook(pCreateTexture2D, (void*)Hooked_CreateTexture2D, (LPVOID*)&g_pOriginalCreateTexture2D);
                
                pDummyDevice->Release();
                if(pDummyContext) pDummyContext->Release();
            }
        }

        if (pCreateDXGIFactory1) {
            IDXGIFactory1* pFactory = nullptr;
            if (SUCCEEDED(pCreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)) && pFactory) {
                void** pFactoryVTable = *(void***)pFactory;
                void* pCreateSwapChain = pFactoryVTable[10];
                MH_CreateHook(pCreateSwapChain, (void*)Hooked_CreateSwapChain, (LPVOID*)&g_pOriginalCreateSwapChain);
                
                // Try to get IDXGIFactory2 for CreateSwapChainForHwnd
                IUnknown* pFactory2 = nullptr;
                // IID_IDXGIFactory2 is 50c83a1c-e072-4c48-87b0-3630fa36a6d0
                static const GUID IID_IDXGIFactory2_Verantyx = { 0x50c83a1c, 0xe072, 0x4c48, { 0x87, 0xb0, 0x36, 0x30, 0xfa, 0x36, 0xa6, 0xd0 } };
                if (SUCCEEDED(pFactory->QueryInterface(IID_IDXGIFactory2_Verantyx, (void**)&pFactory2)) && pFactory2) {
                    void** pFactory2VTable = *(void***)pFactory2;
                    void* pCreateSwapChainForHwnd = pFactory2VTable[15];
                    MH_CreateHook(pCreateSwapChainForHwnd, (void*)Hooked_CreateSwapChainForHwnd, (LPVOID*)&g_pOriginalCreateSwapChainForHwnd);
                    pFactory2->Release();
                }
                
                pFactory->Release();
            }
        }

        MH_EnableHook(MH_ALL_HOOKS);
        FILE* _f = fopen("vr_emulator_log.txt", "a");
        if (_f) { fprintf(_f, "[Verantyx] Successfully injected COM VTable hooks (CreateBuffer, CreateTexture2D, CreateSwapChain, CreateSwapChainForHwnd)!\n"); fclose(_f); }
    }
}
// --- END COM VTABLE HOOKING ---

void InjectMinHook() {
    // Keep empty to avoid hooking LoadLibrary
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        InjectMinHook();
    }
    return TRUE;
}

