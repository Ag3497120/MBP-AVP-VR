#include "openvr.h"
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
    HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
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
} g_universalMock;

using namespace vr;

class Mock_IVRSystem : public vr::IVRSystem {
public:
    virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* f_GetRecommendedRenderTargetSize = fopen("vr_emulator_log.txt", "a"); if(f_GetRecommendedRenderTargetSize) { fprintf(f_GetRecommendedRenderTargetSize, "Called: IVRSystem::GetRecommendedRenderTargetSize\n"); fclose(f_GetRecommendedRenderTargetSize); }
        if(pnWidth) *pnWidth = 1920;
        if(pnHeight) *pnHeight = 1080;
    }
    virtual HmdMatrix44_t GetProjectionMatrix(EVREye eEye, float fNearZ, float fFarZ) override {
        FILE* f_GetProjectionMatrix = fopen("vr_emulator_log.txt", "a"); if(f_GetProjectionMatrix) { fprintf(f_GetProjectionMatrix, "Called: IVRSystem::GetProjectionMatrix\n"); fclose(f_GetProjectionMatrix); }
        vr::HmdMatrix44_t temp; memset(&temp, 0, sizeof(temp)); temp.m[0][0] = 1; temp.m[1][1] = 1; temp.m[2][2] = 1; temp.m[3][3] = 1; return temp;
    }
    virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) override {
        FILE* f_GetProjectionRaw = fopen("vr_emulator_log.txt", "a"); if(f_GetProjectionRaw) { fprintf(f_GetProjectionRaw, "Called: IVRSystem::GetProjectionRaw\n"); fclose(f_GetProjectionRaw); }
        if(pfLeft) *pfLeft = -1.0f; if(pfRight) *pfRight = 1.0f; if(pfTop) *pfTop = -1.0f; if(pfBottom) *pfBottom = 1.0f;
    }
    virtual bool ComputeDistortion(EVREye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates) override {
        FILE* f_ComputeDistortion = fopen("vr_emulator_log.txt", "a"); if(f_ComputeDistortion) { fprintf(f_ComputeDistortion, "Called: IVRSystem::ComputeDistortion\n"); fclose(f_ComputeDistortion); }
        if(pDistortionCoordinates) { memset(pDistortionCoordinates, 0, sizeof(*pDistortionCoordinates)); }
        return true;
    }
    virtual HmdMatrix34_t GetEyeToHeadTransform(EVREye eEye) override {
        FILE* f_GetEyeToHeadTransform = fopen("vr_emulator_log.txt", "a"); if(f_GetEyeToHeadTransform) { fprintf(f_GetEyeToHeadTransform, "Called: IVRSystem::GetEyeToHeadTransform\n"); fclose(f_GetEyeToHeadTransform); }
        vr::HmdMatrix34_t temp; memset(&temp, 0, sizeof(temp)); temp.m[0][0] = 1; temp.m[1][1] = 1; temp.m[2][2] = 1; if(eEye == vr::Eye_Left) temp.m[0][3] = -0.0315f; else temp.m[0][3] = 0.0315f; return temp;
    }
    virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override {
        FILE* f_GetTimeSinceLastVsync = fopen("vr_emulator_log.txt", "a"); if(f_GetTimeSinceLastVsync) { fprintf(f_GetTimeSinceLastVsync, "Called: IVRSystem::GetTimeSinceLastVsync\n"); fclose(f_GetTimeSinceLastVsync); }
        if(pfSecondsSinceLastVsync) *pfSecondsSinceLastVsync = 0.0f; if(pulFrameCounter) *pulFrameCounter = 0;
        return true;
    }
    virtual int32_t GetD3D9AdapterIndex() override {
        FILE* f_GetD3D9AdapterIndex = fopen("vr_emulator_log.txt", "a"); if(f_GetD3D9AdapterIndex) { fprintf(f_GetD3D9AdapterIndex, "Called: IVRSystem::GetD3D9AdapterIndex\n"); fclose(f_GetD3D9AdapterIndex); }
        int32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex) override {
        FILE* f_GetDXGIOutputInfo = fopen("vr_emulator_log.txt", "a"); if(f_GetDXGIOutputInfo) { fprintf(f_GetDXGIOutputInfo, "Called: IVRSystem::GetDXGIOutputInfo\n"); fclose(f_GetDXGIOutputInfo); }
        return;
    }
    virtual void GetOutputDevice(uint64_t *pnDevice, ETextureType textureType, VkInstance_T *pInstance = nullptr) override {
        FILE* f_GetOutputDevice = fopen("vr_emulator_log.txt", "a"); if(f_GetOutputDevice) { fprintf(f_GetOutputDevice, "Called: IVRSystem::GetOutputDevice\n"); fclose(f_GetOutputDevice); }
        return;
    }
    virtual bool IsDisplayOnDesktop() override {
        FILE* f_IsDisplayOnDesktop = fopen("vr_emulator_log.txt", "a"); if(f_IsDisplayOnDesktop) { fprintf(f_IsDisplayOnDesktop, "Called: IVRSystem::IsDisplayOnDesktop\n"); fclose(f_IsDisplayOnDesktop); }
        return false;
    }
    virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) override {
        FILE* f_SetDisplayVisibility = fopen("vr_emulator_log.txt", "a"); if(f_SetDisplayVisibility) { fprintf(f_SetDisplayVisibility, "Called: IVRSystem::SetDisplayVisibility\n"); fclose(f_SetDisplayVisibility); }
        return false;
    }
    virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override {
        FILE* f_GetDeviceToAbsoluteTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_GetDeviceToAbsoluteTrackingPose) { fprintf(f_GetDeviceToAbsoluteTrackingPose, "Called: IVRSystem::GetDeviceToAbsoluteTrackingPose\n"); fclose(f_GetDeviceToAbsoluteTrackingPose); }
        if(pTrackedDevicePoseArray && unTrackedDevicePoseArrayCount > 0) { memset(pTrackedDevicePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unTrackedDevicePoseArrayCount); for(uint32_t i=0; i<3 && i<unTrackedDevicePoseArrayCount; ++i) { pTrackedDevicePoseArray[i].bPoseIsValid = true; pTrackedDevicePoseArray[i].bDeviceIsConnected = true; pTrackedDevicePoseArray[i].eTrackingResult = vr::TrackingResult_Running_OK; pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1; pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1; pTrackedDevicePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1; } }
    }
    virtual void ResetSeatedZeroPose() override {
        FILE* f_ResetSeatedZeroPose = fopen("vr_emulator_log.txt", "a"); if(f_ResetSeatedZeroPose) { fprintf(f_ResetSeatedZeroPose, "Called: IVRSystem::ResetSeatedZeroPose\n"); fclose(f_ResetSeatedZeroPose); }
        return;
    }
    virtual HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() override {
        FILE* f_GetSeatedZeroPoseToStandingAbsoluteTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_GetSeatedZeroPoseToStandingAbsoluteTrackingPose) { fprintf(f_GetSeatedZeroPoseToStandingAbsoluteTrackingPose, "Called: IVRSystem::GetSeatedZeroPoseToStandingAbsoluteTrackingPose\n"); fclose(f_GetSeatedZeroPoseToStandingAbsoluteTrackingPose); }
        HmdMatrix34_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() override {
        FILE* f_GetRawZeroPoseToStandingAbsoluteTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_GetRawZeroPoseToStandingAbsoluteTrackingPose) { fprintf(f_GetRawZeroPoseToStandingAbsoluteTrackingPose, "Called: IVRSystem::GetRawZeroPoseToStandingAbsoluteTrackingPose\n"); fclose(f_GetRawZeroPoseToStandingAbsoluteTrackingPose); }
        HmdMatrix34_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd) override {
        FILE* f_GetSortedTrackedDeviceIndicesOfClass = fopen("vr_emulator_log.txt", "a"); if(f_GetSortedTrackedDeviceIndicesOfClass) { fprintf(f_GetSortedTrackedDeviceIndicesOfClass, "Called: IVRSystem::GetSortedTrackedDeviceIndicesOfClass\n"); fclose(f_GetSortedTrackedDeviceIndicesOfClass); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) override {
        FILE* f_GetTrackedDeviceActivityLevel = fopen("vr_emulator_log.txt", "a"); if(f_GetTrackedDeviceActivityLevel) { fprintf(f_GetTrackedDeviceActivityLevel, "Called: IVRSystem::GetTrackedDeviceActivityLevel\n"); fclose(f_GetTrackedDeviceActivityLevel); }
        return vr::k_EDeviceActivityLevel_UserInteraction;
    }
    virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform) override {
        FILE* f_ApplyTransform = fopen("vr_emulator_log.txt", "a"); if(f_ApplyTransform) { fprintf(f_ApplyTransform, "Called: IVRSystem::ApplyTransform\n"); fclose(f_ApplyTransform); }
        return;
    }
    virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) override {
        FILE* f_GetTrackedDeviceIndexForControllerRole = fopen("vr_emulator_log.txt", "a"); if(f_GetTrackedDeviceIndexForControllerRole) { fprintf(f_GetTrackedDeviceIndexForControllerRole, "Called: IVRSystem::GetTrackedDeviceIndexForControllerRole\n"); fclose(f_GetTrackedDeviceIndexForControllerRole); }
        vr::TrackedDeviceIndex_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* f_GetControllerRoleForTrackedDeviceIndex = fopen("vr_emulator_log.txt", "a"); if(f_GetControllerRoleForTrackedDeviceIndex) { fprintf(f_GetControllerRoleForTrackedDeviceIndex, "Called: IVRSystem::GetControllerRoleForTrackedDeviceIndex\n"); fclose(f_GetControllerRoleForTrackedDeviceIndex); }
        return (vr::ETrackedControllerRole)0;
    }
    virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* f_GetTrackedDeviceClass = fopen("vr_emulator_log.txt", "a"); if(f_GetTrackedDeviceClass) { fprintf(f_GetTrackedDeviceClass, "Called: IVRSystem::GetTrackedDeviceClass\n"); fclose(f_GetTrackedDeviceClass); }
        if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD;
        if(unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller;
        return vr::TrackedDeviceClass_Invalid;
    }
    virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* f_IsTrackedDeviceConnected = fopen("vr_emulator_log.txt", "a"); if(f_IsTrackedDeviceConnected) { fprintf(f_IsTrackedDeviceConnected, "Called: IVRSystem::IsTrackedDeviceConnected\n"); fclose(f_IsTrackedDeviceConnected); }
        if(unDeviceIndex == 0 || unDeviceIndex == 1 || unDeviceIndex == 2) return true;
        return false;
    }
    virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetBoolTrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetBoolTrackedDeviceProperty) { fprintf(f_GetBoolTrackedDeviceProperty, "Called: IVRSystem::GetBoolTrackedDeviceProperty\n"); fclose(f_GetBoolTrackedDeviceProperty); }
        if(prop == vr::Prop_ContainsProximitySensor_Bool) { if(pError) *pError = vr::TrackedProp_Success; return true; }
        if(prop == vr::Prop_DeviceIsWireless_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(prop == vr::Prop_DeviceIsCharging_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(prop == vr::Prop_WillDriftInYaw_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(prop == vr::Prop_DeviceProvidesBatteryStatus_Bool) { if(pError) *pError = vr::TrackedProp_Success; return false; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return false;
    }
    virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetFloatTrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetFloatTrackedDeviceProperty) { fprintf(f_GetFloatTrackedDeviceProperty, "Called: IVRSystem::GetFloatTrackedDeviceProperty\n"); fclose(f_GetFloatTrackedDeviceProperty); }
        if(prop == vr::Prop_DisplayFrequency_Float) { if(pError) *pError = vr::TrackedProp_Success; return 90.0f; }
        if(prop == vr::Prop_UserIpdMeters_Float) { if(pError) *pError = vr::TrackedProp_Success; return 0.063f; }
        if(prop == vr::Prop_SecondsFromVsyncToPhotons_Float) { if(pError) *pError = vr::TrackedProp_Success; return 0.011f; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return 0.0f;
    }
    virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetInt32TrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetInt32TrackedDeviceProperty) { fprintf(f_GetInt32TrackedDeviceProperty, "Called: IVRSystem::GetInt32TrackedDeviceProperty\n"); fclose(f_GetInt32TrackedDeviceProperty); }
        if(prop == vr::Prop_DeviceClass_Int32) { if(pError) *pError = vr::TrackedProp_Success; if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD; if(unDeviceIndex == 1 || unDeviceIndex == 2) return vr::TrackedDeviceClass_Controller; }
        if(prop == vr::Prop_ControllerRoleHint_Int32) { if(pError) *pError = vr::TrackedProp_Success; if(unDeviceIndex == 1) return vr::TrackedControllerRole_LeftHand; if(unDeviceIndex == 2) return vr::TrackedControllerRole_RightHand; }
        if(prop == vr::Prop_Axis0Type_Int32 || prop == vr::Prop_Axis1Type_Int32) { if(pError) *pError = vr::TrackedProp_Success; return vr::k_eControllerAxis_TrackPad; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return 0;
    }
    virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetUint64TrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetUint64TrackedDeviceProperty) { fprintf(f_GetUint64TrackedDeviceProperty, "Called: IVRSystem::GetUint64TrackedDeviceProperty\n"); fclose(f_GetUint64TrackedDeviceProperty); }
        if(prop == vr::Prop_CurrentUniverseId_Uint64) { if(pError) *pError = vr::TrackedProp_Success; return 1; }
        if(prop == vr::Prop_SupportedButtons_Uint64) { if(pError) *pError = vr::TrackedProp_Success; return 0xFFFFFFFFFFFFFFFFULL; }
        if(pError) *pError = vr::TrackedProp_UnknownProperty;
        return 0;
    }
    virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetMatrix34TrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetMatrix34TrackedDeviceProperty) { fprintf(f_GetMatrix34TrackedDeviceProperty, "Called: IVRSystem::GetMatrix34TrackedDeviceProperty\n"); fclose(f_GetMatrix34TrackedDeviceProperty); }
        vr::HmdMatrix34_t temp; memset(&temp, 0, sizeof(temp)); temp.m[0][0] = 1; temp.m[1][1] = 1; temp.m[2][2] = 1; if(pError) *pError = vr::TrackedProp_Success; return temp;
    }
    virtual uint32_t GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetArrayTrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetArrayTrackedDeviceProperty) { fprintf(f_GetArrayTrackedDeviceProperty, "Called: IVRSystem::GetArrayTrackedDeviceProperty\n"); fclose(f_GetArrayTrackedDeviceProperty); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) override {
        FILE* f_GetStringTrackedDeviceProperty = fopen("vr_emulator_log.txt", "a"); if(f_GetStringTrackedDeviceProperty) { fprintf(f_GetStringTrackedDeviceProperty, "Called: IVRSystem::GetStringTrackedDeviceProperty\n"); fclose(f_GetStringTrackedDeviceProperty); }
        const char* s = nullptr;
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
            if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\0'; }
            if(pError) *pError = vr::TrackedProp_Success;
            return (uint32_t)strlen(s) + 1;
        } else {
            if(pError) *pError = vr::TrackedProp_UnknownProperty;
            return 0;
        }
    }
    virtual const char * GetPropErrorNameFromEnum(ETrackedPropertyError error) override {
        FILE* f_GetPropErrorNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetPropErrorNameFromEnum) { fprintf(f_GetPropErrorNameFromEnum, "Called: IVRSystem::GetPropErrorNameFromEnum\n"); fclose(f_GetPropErrorNameFromEnum); }
        return "Unknown";
    }
    virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) override {
        FILE* f_PollNextEvent = fopen("vr_emulator_log.txt", "a"); if(f_PollNextEvent) { fprintf(f_PollNextEvent, "Called: IVRSystem::PollNextEvent\n"); fclose(f_PollNextEvent); }
        static int count = 0; if (count == 0) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 0; } return true; } else if (count == 1) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)403; pEvent->trackedDeviceIndex = 0; } return true; } return false;
    }
    virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose) override {
        FILE* f_PollNextEventWithPose = fopen("vr_emulator_log.txt", "a"); if(f_PollNextEventWithPose) { fprintf(f_PollNextEventWithPose, "Called: IVRSystem::PollNextEventWithPose\n"); fclose(f_PollNextEventWithPose); }
        return false;
    }
    virtual const char * GetEventTypeNameFromEnum(EVREventType eType) override {
        FILE* f_GetEventTypeNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetEventTypeNameFromEnum) { fprintf(f_GetEventTypeNameFromEnum, "Called: IVRSystem::GetEventTypeNameFromEnum\n"); fclose(f_GetEventTypeNameFromEnum); }
        return "Unknown";
    }
    virtual HiddenAreaMesh_t GetHiddenAreaMesh(EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard) override {
        FILE* f_GetHiddenAreaMesh = fopen("vr_emulator_log.txt", "a"); if(f_GetHiddenAreaMesh) { fprintf(f_GetHiddenAreaMesh, "Called: IVRSystem::GetHiddenAreaMesh\n"); fclose(f_GetHiddenAreaMesh); }
        HiddenAreaMesh_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize) override {
        FILE* f_GetControllerState = fopen("vr_emulator_log.txt", "a"); if(f_GetControllerState) { fprintf(f_GetControllerState, "Called: IVRSystem::GetControllerState\n"); fclose(f_GetControllerState); }
        if(pControllerState) memset(pControllerState, 0, unControllerStateSize);
        return true;
    }
    virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose) override {
        FILE* f_GetControllerStateWithPose = fopen("vr_emulator_log.txt", "a"); if(f_GetControllerStateWithPose) { fprintf(f_GetControllerStateWithPose, "Called: IVRSystem::GetControllerStateWithPose\n"); fclose(f_GetControllerStateWithPose); }
        if(pControllerState) memset(pControllerState, 0, unControllerStateSize);
        return true;
    }
    virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) override {
        FILE* f_TriggerHapticPulse = fopen("vr_emulator_log.txt", "a"); if(f_TriggerHapticPulse) { fprintf(f_TriggerHapticPulse, "Called: IVRSystem::TriggerHapticPulse\n"); fclose(f_TriggerHapticPulse); }
        return;
    }
    virtual const char * GetButtonIdNameFromEnum(EVRButtonId eButtonId) override {
        FILE* f_GetButtonIdNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetButtonIdNameFromEnum) { fprintf(f_GetButtonIdNameFromEnum, "Called: IVRSystem::GetButtonIdNameFromEnum\n"); fclose(f_GetButtonIdNameFromEnum); }
        return "Unknown";
    }
    virtual const char * GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) override {
        FILE* f_GetControllerAxisTypeNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetControllerAxisTypeNameFromEnum) { fprintf(f_GetControllerAxisTypeNameFromEnum, "Called: IVRSystem::GetControllerAxisTypeNameFromEnum\n"); fclose(f_GetControllerAxisTypeNameFromEnum); }
        return "Unknown";
    }
    virtual bool IsInputAvailable() override {
        FILE* f_IsInputAvailable = fopen("vr_emulator_log.txt", "a"); if(f_IsInputAvailable) { fprintf(f_IsInputAvailable, "Called: IVRSystem::IsInputAvailable\n"); fclose(f_IsInputAvailable); }
        return true;
    }
    virtual bool IsSteamVRDrawingControllers() override {
        FILE* f_IsSteamVRDrawingControllers = fopen("vr_emulator_log.txt", "a"); if(f_IsSteamVRDrawingControllers) { fprintf(f_IsSteamVRDrawingControllers, "Called: IVRSystem::IsSteamVRDrawingControllers\n"); fclose(f_IsSteamVRDrawingControllers); }
        return false;
    }
    virtual bool ShouldApplicationPause() override {
        FILE* f_ShouldApplicationPause = fopen("vr_emulator_log.txt", "a"); if(f_ShouldApplicationPause) { fprintf(f_ShouldApplicationPause, "Called: IVRSystem::ShouldApplicationPause\n"); fclose(f_ShouldApplicationPause); }
        return false;
    }
    virtual bool ShouldApplicationReduceRenderingWork() override {
        FILE* f_ShouldApplicationReduceRenderingWork = fopen("vr_emulator_log.txt", "a"); if(f_ShouldApplicationReduceRenderingWork) { fprintf(f_ShouldApplicationReduceRenderingWork, "Called: IVRSystem::ShouldApplicationReduceRenderingWork\n"); fclose(f_ShouldApplicationReduceRenderingWork); }
        return false;
    }
    virtual vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* f_PerformFirmwareUpdate = fopen("vr_emulator_log.txt", "a"); if(f_PerformFirmwareUpdate) { fprintf(f_PerformFirmwareUpdate, "Called: IVRSystem::PerformFirmwareUpdate\n"); fclose(f_PerformFirmwareUpdate); }
        return (vr::EVRFirmwareError)0;
    }
    virtual void AcknowledgeQuit_Exiting() override {
        FILE* f_AcknowledgeQuit_Exiting = fopen("vr_emulator_log.txt", "a"); if(f_AcknowledgeQuit_Exiting) { fprintf(f_AcknowledgeQuit_Exiting, "Called: IVRSystem::AcknowledgeQuit_Exiting\n"); fclose(f_AcknowledgeQuit_Exiting); }
        return;
    }
    virtual uint32_t GetAppContainerFilePaths(VR_OUT_STRING() char *pchBuffer, uint32_t unBufferSize) override {
        FILE* f_GetAppContainerFilePaths = fopen("vr_emulator_log.txt", "a"); if(f_GetAppContainerFilePaths) { fprintf(f_GetAppContainerFilePaths, "Called: IVRSystem::GetAppContainerFilePaths\n"); fclose(f_GetAppContainerFilePaths); }
        if(pchBuffer && unBufferSize > 0) pchBuffer[0] = '\0';
        return 0;
    }
    virtual const char * GetRuntimeVersion() override {
        FILE* f_GetRuntimeVersion = fopen("vr_emulator_log.txt", "a"); if(f_GetRuntimeVersion) { fprintf(f_GetRuntimeVersion, "Called: IVRSystem::GetRuntimeVersion\n"); fclose(f_GetRuntimeVersion); }
        return "1.23.0";
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
} g_mockSystem;

class Mock_IVRApplications : public vr::IVRApplications {
public:
    virtual EVRApplicationError AddApplicationManifest(const char *pchApplicationManifestFullPath, bool bTemporary = false) override {
        FILE* f_AddApplicationManifest = fopen("vr_emulator_log.txt", "a"); if(f_AddApplicationManifest) { fprintf(f_AddApplicationManifest, "Called: IVRApplications::AddApplicationManifest\n"); fclose(f_AddApplicationManifest); }
        return (EVRApplicationError)0;
    }
    virtual EVRApplicationError RemoveApplicationManifest(const char *pchApplicationManifestFullPath) override {
        FILE* f_RemoveApplicationManifest = fopen("vr_emulator_log.txt", "a"); if(f_RemoveApplicationManifest) { fprintf(f_RemoveApplicationManifest, "Called: IVRApplications::RemoveApplicationManifest\n"); fclose(f_RemoveApplicationManifest); }
        return (EVRApplicationError)0;
    }
    virtual bool IsApplicationInstalled(const char *pchAppKey) override {
        FILE* f_IsApplicationInstalled = fopen("vr_emulator_log.txt", "a"); if(f_IsApplicationInstalled) { fprintf(f_IsApplicationInstalled, "Called: IVRApplications::IsApplicationInstalled\n"); fclose(f_IsApplicationInstalled); }
        return false;
    }
    virtual uint32_t GetApplicationCount() override {
        FILE* f_GetApplicationCount = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationCount) { fprintf(f_GetApplicationCount, "Called: IVRApplications::GetApplicationCount\n"); fclose(f_GetApplicationCount); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* f_GetApplicationKeyByIndex = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationKeyByIndex) { fprintf(f_GetApplicationKeyByIndex, "Called: IVRApplications::GetApplicationKeyByIndex\n"); fclose(f_GetApplicationKeyByIndex); }
        return (EVRApplicationError)0;
    }
    virtual EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* f_GetApplicationKeyByProcessId = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationKeyByProcessId) { fprintf(f_GetApplicationKeyByProcessId, "Called: IVRApplications::GetApplicationKeyByProcessId\n"); fclose(f_GetApplicationKeyByProcessId); }
        return (EVRApplicationError)0;
    }
    virtual EVRApplicationError LaunchApplication(const char *pchAppKey) override {
        FILE* f_LaunchApplication = fopen("vr_emulator_log.txt", "a"); if(f_LaunchApplication) { fprintf(f_LaunchApplication, "Called: IVRApplications::LaunchApplication\n"); fclose(f_LaunchApplication); }
        return (EVRApplicationError)0;
    }
    virtual EVRApplicationError LaunchTemplateApplication(const char *pchTemplateAppKey, const char *pchNewAppKey, VR_ARRAY_COUNT( unKeys ) const AppOverrideKeys_t *pKeys, uint32_t unKeys) override {
        FILE* f_LaunchTemplateApplication = fopen("vr_emulator_log.txt", "a"); if(f_LaunchTemplateApplication) { fprintf(f_LaunchTemplateApplication, "Called: IVRApplications::LaunchTemplateApplication\n"); fclose(f_LaunchTemplateApplication); }
        return (EVRApplicationError)0;
    }
    virtual vr::EVRApplicationError LaunchApplicationFromMimeType(const char *pchMimeType, const char *pchArgs) override {
        FILE* f_LaunchApplicationFromMimeType = fopen("vr_emulator_log.txt", "a"); if(f_LaunchApplicationFromMimeType) { fprintf(f_LaunchApplicationFromMimeType, "Called: IVRApplications::LaunchApplicationFromMimeType\n"); fclose(f_LaunchApplicationFromMimeType); }
        return (vr::EVRApplicationError)0;
    }
    virtual EVRApplicationError LaunchDashboardOverlay(const char *pchAppKey) override {
        FILE* f_LaunchDashboardOverlay = fopen("vr_emulator_log.txt", "a"); if(f_LaunchDashboardOverlay) { fprintf(f_LaunchDashboardOverlay, "Called: IVRApplications::LaunchDashboardOverlay\n"); fclose(f_LaunchDashboardOverlay); }
        return (EVRApplicationError)0;
    }
    virtual bool CancelApplicationLaunch(const char *pchAppKey) override {
        FILE* f_CancelApplicationLaunch = fopen("vr_emulator_log.txt", "a"); if(f_CancelApplicationLaunch) { fprintf(f_CancelApplicationLaunch, "Called: IVRApplications::CancelApplicationLaunch\n"); fclose(f_CancelApplicationLaunch); }
        return false;
    }
    virtual EVRApplicationError IdentifyApplication(uint32_t unProcessId, const char *pchAppKey) override {
        FILE* f_IdentifyApplication = fopen("vr_emulator_log.txt", "a"); if(f_IdentifyApplication) { fprintf(f_IdentifyApplication, "Called: IVRApplications::IdentifyApplication\n"); fclose(f_IdentifyApplication); }
        return (EVRApplicationError)0;
    }
    virtual uint32_t GetApplicationProcessId(const char *pchAppKey) override {
        FILE* f_GetApplicationProcessId = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationProcessId) { fprintf(f_GetApplicationProcessId, "Called: IVRApplications::GetApplicationProcessId\n"); fclose(f_GetApplicationProcessId); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual const char * GetApplicationsErrorNameFromEnum(EVRApplicationError error) override {
        FILE* f_GetApplicationsErrorNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationsErrorNameFromEnum) { fprintf(f_GetApplicationsErrorNameFromEnum, "Called: IVRApplications::GetApplicationsErrorNameFromEnum\n"); fclose(f_GetApplicationsErrorNameFromEnum); }
        return "Unknown";
    }
    virtual uint32_t GetApplicationPropertyString(const char *pchAppKey, EVRApplicationProperty eProperty, VR_OUT_STRING() char *pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, EVRApplicationError *peError = nullptr) override {
        FILE* f_GetApplicationPropertyString = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationPropertyString) { fprintf(f_GetApplicationPropertyString, "Called: IVRApplications::GetApplicationPropertyString\n"); fclose(f_GetApplicationPropertyString); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual bool GetApplicationPropertyBool(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr) override {
        FILE* f_GetApplicationPropertyBool = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationPropertyBool) { fprintf(f_GetApplicationPropertyBool, "Called: IVRApplications::GetApplicationPropertyBool\n"); fclose(f_GetApplicationPropertyBool); }
        return false;
    }
    virtual uint64_t GetApplicationPropertyUint64(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr) override {
        FILE* f_GetApplicationPropertyUint64 = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationPropertyUint64) { fprintf(f_GetApplicationPropertyUint64, "Called: IVRApplications::GetApplicationPropertyUint64\n"); fclose(f_GetApplicationPropertyUint64); }
        uint64_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EVRApplicationError SetApplicationAutoLaunch(const char *pchAppKey, bool bAutoLaunch) override {
        FILE* f_SetApplicationAutoLaunch = fopen("vr_emulator_log.txt", "a"); if(f_SetApplicationAutoLaunch) { fprintf(f_SetApplicationAutoLaunch, "Called: IVRApplications::SetApplicationAutoLaunch\n"); fclose(f_SetApplicationAutoLaunch); }
        return (EVRApplicationError)0;
    }
    virtual bool GetApplicationAutoLaunch(const char *pchAppKey) override {
        FILE* f_GetApplicationAutoLaunch = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationAutoLaunch) { fprintf(f_GetApplicationAutoLaunch, "Called: IVRApplications::GetApplicationAutoLaunch\n"); fclose(f_GetApplicationAutoLaunch); }
        return false;
    }
    virtual EVRApplicationError SetDefaultApplicationForMimeType(const char *pchAppKey, const char *pchMimeType) override {
        FILE* f_SetDefaultApplicationForMimeType = fopen("vr_emulator_log.txt", "a"); if(f_SetDefaultApplicationForMimeType) { fprintf(f_SetDefaultApplicationForMimeType, "Called: IVRApplications::SetDefaultApplicationForMimeType\n"); fclose(f_SetDefaultApplicationForMimeType); }
        return (EVRApplicationError)0;
    }
    virtual bool GetDefaultApplicationForMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* f_GetDefaultApplicationForMimeType = fopen("vr_emulator_log.txt", "a"); if(f_GetDefaultApplicationForMimeType) { fprintf(f_GetDefaultApplicationForMimeType, "Called: IVRApplications::GetDefaultApplicationForMimeType\n"); fclose(f_GetDefaultApplicationForMimeType); }
        return false;
    }
    virtual bool GetApplicationSupportedMimeTypes(const char *pchAppKey, VR_OUT_STRING() char *pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) override {
        FILE* f_GetApplicationSupportedMimeTypes = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationSupportedMimeTypes) { fprintf(f_GetApplicationSupportedMimeTypes, "Called: IVRApplications::GetApplicationSupportedMimeTypes\n"); fclose(f_GetApplicationSupportedMimeTypes); }
        return false;
    }
    virtual uint32_t GetApplicationsThatSupportMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) override {
        FILE* f_GetApplicationsThatSupportMimeType = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationsThatSupportMimeType) { fprintf(f_GetApplicationsThatSupportMimeType, "Called: IVRApplications::GetApplicationsThatSupportMimeType\n"); fclose(f_GetApplicationsThatSupportMimeType); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetApplicationLaunchArguments(uint32_t unHandle, VR_OUT_STRING() char *pchArgs, uint32_t unArgs) override {
        FILE* f_GetApplicationLaunchArguments = fopen("vr_emulator_log.txt", "a"); if(f_GetApplicationLaunchArguments) { fprintf(f_GetApplicationLaunchArguments, "Called: IVRApplications::GetApplicationLaunchArguments\n"); fclose(f_GetApplicationLaunchArguments); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EVRApplicationError GetStartingApplication(VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* f_GetStartingApplication = fopen("vr_emulator_log.txt", "a"); if(f_GetStartingApplication) { fprintf(f_GetStartingApplication, "Called: IVRApplications::GetStartingApplication\n"); fclose(f_GetStartingApplication); }
        return (EVRApplicationError)0;
    }
    virtual EVRSceneApplicationState GetSceneApplicationState() override {
        FILE* f_GetSceneApplicationState = fopen("vr_emulator_log.txt", "a"); if(f_GetSceneApplicationState) { fprintf(f_GetSceneApplicationState, "Called: IVRApplications::GetSceneApplicationState\n"); fclose(f_GetSceneApplicationState); }
        return (EVRSceneApplicationState)0;
    }
    virtual EVRApplicationError PerformApplicationPrelaunchCheck(const char *pchAppKey) override {
        FILE* f_PerformApplicationPrelaunchCheck = fopen("vr_emulator_log.txt", "a"); if(f_PerformApplicationPrelaunchCheck) { fprintf(f_PerformApplicationPrelaunchCheck, "Called: IVRApplications::PerformApplicationPrelaunchCheck\n"); fclose(f_PerformApplicationPrelaunchCheck); }
        return (EVRApplicationError)0;
    }
    virtual const char * GetSceneApplicationStateNameFromEnum(EVRSceneApplicationState state) override {
        FILE* f_GetSceneApplicationStateNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetSceneApplicationStateNameFromEnum) { fprintf(f_GetSceneApplicationStateNameFromEnum, "Called: IVRApplications::GetSceneApplicationStateNameFromEnum\n"); fclose(f_GetSceneApplicationStateNameFromEnum); }
        return nullptr;
    }
    virtual EVRApplicationError LaunchInternalProcess(const char *pchBinaryPath, const char *pchArguments, const char *pchWorkingDirectory) override {
        FILE* f_LaunchInternalProcess = fopen("vr_emulator_log.txt", "a"); if(f_LaunchInternalProcess) { fprintf(f_LaunchInternalProcess, "Called: IVRApplications::LaunchInternalProcess\n"); fclose(f_LaunchInternalProcess); }
        return (EVRApplicationError)0;
    }
    virtual uint32_t GetCurrentSceneProcessId() override {
        FILE* f_GetCurrentSceneProcessId = fopen("vr_emulator_log.txt", "a"); if(f_GetCurrentSceneProcessId) { fprintf(f_GetCurrentSceneProcessId, "Called: IVRApplications::GetCurrentSceneProcessId\n"); fclose(f_GetCurrentSceneProcessId); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
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
        FILE* f_GetSettingsErrorNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetSettingsErrorNameFromEnum) { fprintf(f_GetSettingsErrorNameFromEnum, "Called: IVRSettings::GetSettingsErrorNameFromEnum\n"); fclose(f_GetSettingsErrorNameFromEnum); }
        return "Unknown";
    }
    virtual void SetBool(const char *pchSection, const char *pchSettingsKey, bool bValue, EVRSettingsError *peError = nullptr) override {
        FILE* f_SetBool = fopen("vr_emulator_log.txt", "a"); if(f_SetBool) { fprintf(f_SetBool, "Called: IVRSettings::SetBool\n"); fclose(f_SetBool); }
        return;
    }
    virtual void SetInt32(const char *pchSection, const char *pchSettingsKey, int32_t nValue, EVRSettingsError *peError = nullptr) override {
        FILE* f_SetInt32 = fopen("vr_emulator_log.txt", "a"); if(f_SetInt32) { fprintf(f_SetInt32, "Called: IVRSettings::SetInt32\n"); fclose(f_SetInt32); }
        return;
    }
    virtual void SetFloat(const char *pchSection, const char *pchSettingsKey, float flValue, EVRSettingsError *peError = nullptr) override {
        FILE* f_SetFloat = fopen("vr_emulator_log.txt", "a"); if(f_SetFloat) { fprintf(f_SetFloat, "Called: IVRSettings::SetFloat\n"); fclose(f_SetFloat); }
        return;
    }
    virtual void SetString(const char *pchSection, const char *pchSettingsKey, const char *pchValue, EVRSettingsError *peError = nullptr) override {
        FILE* f_SetString = fopen("vr_emulator_log.txt", "a"); if(f_SetString) { fprintf(f_SetString, "Called: IVRSettings::SetString\n"); fclose(f_SetString); }
        return;
    }
    virtual bool GetBool(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        FILE* f_GetBool = fopen("vr_emulator_log.txt", "a"); if(f_GetBool) { fprintf(f_GetBool, "Called: IVRSettings::GetBool\n"); fclose(f_GetBool); }
        return false;
    }
    virtual int32_t GetInt32(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        FILE* f_GetInt32 = fopen("vr_emulator_log.txt", "a"); if(f_GetInt32) { fprintf(f_GetInt32, "Called: IVRSettings::GetInt32\n"); fclose(f_GetInt32); }
        int32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual float GetFloat(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        FILE* f_GetFloat = fopen("vr_emulator_log.txt", "a"); if(f_GetFloat) { fprintf(f_GetFloat, "Called: IVRSettings::GetFloat\n"); fclose(f_GetFloat); }
        return (float)0;
    }
    virtual void GetString(const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError = nullptr) override {
        FILE* f_GetString = fopen("vr_emulator_log.txt", "a"); if(f_GetString) { fprintf(f_GetString, "Called: IVRSettings::GetString\n"); fclose(f_GetString); }
        if(peError) *peError = vr::VRSettingsError_None;
        if(pchValue && unValueLen > 0) pchValue[0] = '\0';
    }
    virtual void RemoveSection(const char *pchSection, EVRSettingsError *peError = nullptr) override {
        FILE* f_RemoveSection = fopen("vr_emulator_log.txt", "a"); if(f_RemoveSection) { fprintf(f_RemoveSection, "Called: IVRSettings::RemoveSection\n"); fclose(f_RemoveSection); }
        return;
    }
    virtual void RemoveKeyInSection(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        FILE* f_RemoveKeyInSection = fopen("vr_emulator_log.txt", "a"); if(f_RemoveKeyInSection) { fprintf(f_RemoveKeyInSection, "Called: IVRSettings::RemoveKeyInSection\n"); fclose(f_RemoveKeyInSection); }
        return;
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
} g_mockSettings;

class Mock_IVRChaperone : public vr::IVRChaperone {
public:
    virtual ChaperoneCalibrationState GetCalibrationState() override {
        FILE* f_GetCalibrationState = fopen("vr_emulator_log.txt", "a"); if(f_GetCalibrationState) { fprintf(f_GetCalibrationState, "Called: IVRChaperone::GetCalibrationState\n"); fclose(f_GetCalibrationState); }
        return (ChaperoneCalibrationState)0;
    }
    virtual bool GetPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        FILE* f_GetPlayAreaSize = fopen("vr_emulator_log.txt", "a"); if(f_GetPlayAreaSize) { fprintf(f_GetPlayAreaSize, "Called: IVRChaperone::GetPlayAreaSize\n"); fclose(f_GetPlayAreaSize); }
        return false;
    }
    virtual bool GetPlayAreaRect(HmdQuad_t *rect) override {
        FILE* f_GetPlayAreaRect = fopen("vr_emulator_log.txt", "a"); if(f_GetPlayAreaRect) { fprintf(f_GetPlayAreaRect, "Called: IVRChaperone::GetPlayAreaRect\n"); fclose(f_GetPlayAreaRect); }
        return false;
    }
    virtual void ReloadInfo(void) override {
        FILE* f_ReloadInfo = fopen("vr_emulator_log.txt", "a"); if(f_ReloadInfo) { fprintf(f_ReloadInfo, "Called: IVRChaperone::ReloadInfo\n"); fclose(f_ReloadInfo); }
        return;
    }
    virtual void SetSceneColor(HmdColor_t color) override {
        FILE* f_SetSceneColor = fopen("vr_emulator_log.txt", "a"); if(f_SetSceneColor) { fprintf(f_SetSceneColor, "Called: IVRChaperone::SetSceneColor\n"); fclose(f_SetSceneColor); }
        return;
    }
    virtual void GetBoundsColor(HmdColor_t *pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, HmdColor_t *pOutputCameraColor) override {
        FILE* f_GetBoundsColor = fopen("vr_emulator_log.txt", "a"); if(f_GetBoundsColor) { fprintf(f_GetBoundsColor, "Called: IVRChaperone::GetBoundsColor\n"); fclose(f_GetBoundsColor); }
        return;
    }
    virtual bool AreBoundsVisible() override {
        FILE* f_AreBoundsVisible = fopen("vr_emulator_log.txt", "a"); if(f_AreBoundsVisible) { fprintf(f_AreBoundsVisible, "Called: IVRChaperone::AreBoundsVisible\n"); fclose(f_AreBoundsVisible); }
        return false;
    }
    virtual void ForceBoundsVisible(bool bForce) override {
        FILE* f_ForceBoundsVisible = fopen("vr_emulator_log.txt", "a"); if(f_ForceBoundsVisible) { fprintf(f_ForceBoundsVisible, "Called: IVRChaperone::ForceBoundsVisible\n"); fclose(f_ForceBoundsVisible); }
        return;
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
        FILE* f_CommitWorkingCopy = fopen("vr_emulator_log.txt", "a"); if(f_CommitWorkingCopy) { fprintf(f_CommitWorkingCopy, "Called: IVRChaperoneSetup::CommitWorkingCopy\n"); fclose(f_CommitWorkingCopy); }
        return false;
    }
    virtual void RevertWorkingCopy() override {
        FILE* f_RevertWorkingCopy = fopen("vr_emulator_log.txt", "a"); if(f_RevertWorkingCopy) { fprintf(f_RevertWorkingCopy, "Called: IVRChaperoneSetup::RevertWorkingCopy\n"); fclose(f_RevertWorkingCopy); }
        return;
    }
    virtual bool GetWorkingPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        FILE* f_GetWorkingPlayAreaSize = fopen("vr_emulator_log.txt", "a"); if(f_GetWorkingPlayAreaSize) { fprintf(f_GetWorkingPlayAreaSize, "Called: IVRChaperoneSetup::GetWorkingPlayAreaSize\n"); fclose(f_GetWorkingPlayAreaSize); }
        return false;
    }
    virtual bool GetWorkingPlayAreaRect(HmdQuad_t *rect) override {
        FILE* f_GetWorkingPlayAreaRect = fopen("vr_emulator_log.txt", "a"); if(f_GetWorkingPlayAreaRect) { fprintf(f_GetWorkingPlayAreaRect, "Called: IVRChaperoneSetup::GetWorkingPlayAreaRect\n"); fclose(f_GetWorkingPlayAreaRect); }
        return false;
    }
    virtual bool GetWorkingCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        FILE* f_GetWorkingCollisionBoundsInfo = fopen("vr_emulator_log.txt", "a"); if(f_GetWorkingCollisionBoundsInfo) { fprintf(f_GetWorkingCollisionBoundsInfo, "Called: IVRChaperoneSetup::GetWorkingCollisionBoundsInfo\n"); fclose(f_GetWorkingCollisionBoundsInfo); }
        return false;
    }
    virtual bool GetLiveCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        FILE* f_GetLiveCollisionBoundsInfo = fopen("vr_emulator_log.txt", "a"); if(f_GetLiveCollisionBoundsInfo) { fprintf(f_GetLiveCollisionBoundsInfo, "Called: IVRChaperoneSetup::GetLiveCollisionBoundsInfo\n"); fclose(f_GetLiveCollisionBoundsInfo); }
        return false;
    }
    virtual bool GetWorkingSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        FILE* f_GetWorkingSeatedZeroPoseToRawTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_GetWorkingSeatedZeroPoseToRawTrackingPose) { fprintf(f_GetWorkingSeatedZeroPoseToRawTrackingPose, "Called: IVRChaperoneSetup::GetWorkingSeatedZeroPoseToRawTrackingPose\n"); fclose(f_GetWorkingSeatedZeroPoseToRawTrackingPose); }
        return false;
    }
    virtual bool GetWorkingStandingZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatStandingZeroPoseToRawTrackingPose) override {
        FILE* f_GetWorkingStandingZeroPoseToRawTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_GetWorkingStandingZeroPoseToRawTrackingPose) { fprintf(f_GetWorkingStandingZeroPoseToRawTrackingPose, "Called: IVRChaperoneSetup::GetWorkingStandingZeroPoseToRawTrackingPose\n"); fclose(f_GetWorkingStandingZeroPoseToRawTrackingPose); }
        return false;
    }
    virtual void SetWorkingPlayAreaSize(float sizeX, float sizeZ) override {
        FILE* f_SetWorkingPlayAreaSize = fopen("vr_emulator_log.txt", "a"); if(f_SetWorkingPlayAreaSize) { fprintf(f_SetWorkingPlayAreaSize, "Called: IVRChaperoneSetup::SetWorkingPlayAreaSize\n"); fclose(f_SetWorkingPlayAreaSize); }
        return;
    }
    virtual void SetWorkingCollisionBoundsInfo(VR_ARRAY_COUNT(unQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t unQuadsCount) override {
        FILE* f_SetWorkingCollisionBoundsInfo = fopen("vr_emulator_log.txt", "a"); if(f_SetWorkingCollisionBoundsInfo) { fprintf(f_SetWorkingCollisionBoundsInfo, "Called: IVRChaperoneSetup::SetWorkingCollisionBoundsInfo\n"); fclose(f_SetWorkingCollisionBoundsInfo); }
        return;
    }
    virtual void SetWorkingPerimeter(VR_ARRAY_COUNT( unPointCount ) HmdVector2_t *pPointBuffer, uint32_t unPointCount) override {
        FILE* f_SetWorkingPerimeter = fopen("vr_emulator_log.txt", "a"); if(f_SetWorkingPerimeter) { fprintf(f_SetWorkingPerimeter, "Called: IVRChaperoneSetup::SetWorkingPerimeter\n"); fclose(f_SetWorkingPerimeter); }
        return;
    }
    virtual void SetWorkingSeatedZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatSeatedZeroPoseToRawTrackingPose) override {
        FILE* f_SetWorkingSeatedZeroPoseToRawTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_SetWorkingSeatedZeroPoseToRawTrackingPose) { fprintf(f_SetWorkingSeatedZeroPoseToRawTrackingPose, "Called: IVRChaperoneSetup::SetWorkingSeatedZeroPoseToRawTrackingPose\n"); fclose(f_SetWorkingSeatedZeroPoseToRawTrackingPose); }
        return;
    }
    virtual void SetWorkingStandingZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatStandingZeroPoseToRawTrackingPose) override {
        FILE* f_SetWorkingStandingZeroPoseToRawTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_SetWorkingStandingZeroPoseToRawTrackingPose) { fprintf(f_SetWorkingStandingZeroPoseToRawTrackingPose, "Called: IVRChaperoneSetup::SetWorkingStandingZeroPoseToRawTrackingPose\n"); fclose(f_SetWorkingStandingZeroPoseToRawTrackingPose); }
        return;
    }
    virtual void ReloadFromDisk(EChaperoneConfigFile configFile) override {
        FILE* f_ReloadFromDisk = fopen("vr_emulator_log.txt", "a"); if(f_ReloadFromDisk) { fprintf(f_ReloadFromDisk, "Called: IVRChaperoneSetup::ReloadFromDisk\n"); fclose(f_ReloadFromDisk); }
        return;
    }
    virtual bool GetLiveSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        FILE* f_GetLiveSeatedZeroPoseToRawTrackingPose = fopen("vr_emulator_log.txt", "a"); if(f_GetLiveSeatedZeroPoseToRawTrackingPose) { fprintf(f_GetLiveSeatedZeroPoseToRawTrackingPose, "Called: IVRChaperoneSetup::GetLiveSeatedZeroPoseToRawTrackingPose\n"); fclose(f_GetLiveSeatedZeroPoseToRawTrackingPose); }
        return false;
    }
    virtual bool ExportLiveToBuffer(VR_OUT_STRING() char *pBuffer, uint32_t *pnBufferLength) override {
        FILE* f_ExportLiveToBuffer = fopen("vr_emulator_log.txt", "a"); if(f_ExportLiveToBuffer) { fprintf(f_ExportLiveToBuffer, "Called: IVRChaperoneSetup::ExportLiveToBuffer\n"); fclose(f_ExportLiveToBuffer); }
        return false;
    }
    virtual bool ImportFromBufferToWorking(const char *pBuffer, uint32_t nImportFlags) override {
        FILE* f_ImportFromBufferToWorking = fopen("vr_emulator_log.txt", "a"); if(f_ImportFromBufferToWorking) { fprintf(f_ImportFromBufferToWorking, "Called: IVRChaperoneSetup::ImportFromBufferToWorking\n"); fclose(f_ImportFromBufferToWorking); }
        return false;
    }
    virtual void ShowWorkingSetPreview() override {
        FILE* f_ShowWorkingSetPreview = fopen("vr_emulator_log.txt", "a"); if(f_ShowWorkingSetPreview) { fprintf(f_ShowWorkingSetPreview, "Called: IVRChaperoneSetup::ShowWorkingSetPreview\n"); fclose(f_ShowWorkingSetPreview); }
        return;
    }
    virtual void HideWorkingSetPreview() override {
        FILE* f_HideWorkingSetPreview = fopen("vr_emulator_log.txt", "a"); if(f_HideWorkingSetPreview) { fprintf(f_HideWorkingSetPreview, "Called: IVRChaperoneSetup::HideWorkingSetPreview\n"); fclose(f_HideWorkingSetPreview); }
        return;
    }
    virtual void RoomSetupStarting() override {
        FILE* f_RoomSetupStarting = fopen("vr_emulator_log.txt", "a"); if(f_RoomSetupStarting) { fprintf(f_RoomSetupStarting, "Called: IVRChaperoneSetup::RoomSetupStarting\n"); fclose(f_RoomSetupStarting); }
        return;
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
} g_mockChaperoneSetup;

class Mock_IVRCompositor : public vr::IVRCompositor {
public:
    virtual void SetTrackingSpace(ETrackingUniverseOrigin eOrigin) override {
        FILE* f_SetTrackingSpace = fopen("vr_emulator_log.txt", "a"); if(f_SetTrackingSpace) { fprintf(f_SetTrackingSpace, "Called: IVRCompositor::SetTrackingSpace\n"); fclose(f_SetTrackingSpace); }
        return;
    }
    virtual ETrackingUniverseOrigin GetTrackingSpace() override {
        FILE* f_GetTrackingSpace = fopen("vr_emulator_log.txt", "a"); if(f_GetTrackingSpace) { fprintf(f_GetTrackingSpace, "Called: IVRCompositor::GetTrackingSpace\n"); fclose(f_GetTrackingSpace); }
        return (ETrackingUniverseOrigin)0;
    }
    virtual EVRCompositorError WaitGetPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        FILE* f_WaitGetPoses = fopen("vr_emulator_log.txt", "a"); if(f_WaitGetPoses) { fprintf(f_WaitGetPoses, "Called: IVRCompositor::WaitGetPoses\n"); fclose(f_WaitGetPoses); }
        InitSharedMemory();
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
    }
    virtual EVRCompositorError GetLastPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        FILE* f_GetLastPoses = fopen("vr_emulator_log.txt", "a"); if(f_GetLastPoses) { fprintf(f_GetLastPoses, "Called: IVRCompositor::GetLastPoses\n"); fclose(f_GetLastPoses); }
        return (EVRCompositorError)0;
    }
    virtual EVRCompositorError GetLastPoseForTrackedDeviceIndex(TrackedDeviceIndex_t unDeviceIndex, TrackedDevicePose_t *pOutputPose, TrackedDevicePose_t *pOutputGamePose) override {
        FILE* f_GetLastPoseForTrackedDeviceIndex = fopen("vr_emulator_log.txt", "a"); if(f_GetLastPoseForTrackedDeviceIndex) { fprintf(f_GetLastPoseForTrackedDeviceIndex, "Called: IVRCompositor::GetLastPoseForTrackedDeviceIndex\n"); fclose(f_GetLastPoseForTrackedDeviceIndex); }
        return (EVRCompositorError)0;
    }
    virtual EVRCompositorError Submit(EVREye eEye, const Texture_t *pTexture, const VRTextureBounds_t* pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default) override {
        FILE* f_Submit = fopen("vr_emulator_log.txt", "a"); if(f_Submit) { fprintf(f_Submit, "Called: IVRCompositor::Submit\n"); fclose(f_Submit); }
        InitSharedMemory();
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
                                if (f) { fprintf(f, "CreateTexture2D failed! HR: %08x, W: %u, H: %u, Fmt: %u\n", hr, desc.Width, desc.Height, desc.Format); fclose(f); }
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
                                if (f) { fprintf(f, "Map failed! HR: %08x\n", mapHr); fclose(f); }
                            }
                        }
                    }
                    pContext->Release();
                }
                pDevice->Release();
            }
        }
        return vr::VRCompositorError_None;
    }
    virtual void ClearLastSubmittedFrame() override {
        FILE* f_ClearLastSubmittedFrame = fopen("vr_emulator_log.txt", "a"); if(f_ClearLastSubmittedFrame) { fprintf(f_ClearLastSubmittedFrame, "Called: IVRCompositor::ClearLastSubmittedFrame\n"); fclose(f_ClearLastSubmittedFrame); }
        return;
    }
    virtual void PostPresentHandoff() override {
        FILE* f_PostPresentHandoff = fopen("vr_emulator_log.txt", "a"); if(f_PostPresentHandoff) { fprintf(f_PostPresentHandoff, "Called: IVRCompositor::PostPresentHandoff\n"); fclose(f_PostPresentHandoff); }
        return;
    }
    virtual bool GetFrameTiming(Compositor_FrameTiming *pTiming, uint32_t unFramesAgo = 0) override {
        FILE* f_GetFrameTiming = fopen("vr_emulator_log.txt", "a"); if(f_GetFrameTiming) { fprintf(f_GetFrameTiming, "Called: IVRCompositor::GetFrameTiming\n"); fclose(f_GetFrameTiming); }
        return false;
    }
    virtual uint32_t GetFrameTimings(VR_ARRAY_COUNT( nFrames ) Compositor_FrameTiming *pTiming, uint32_t nFrames) override {
        FILE* f_GetFrameTimings = fopen("vr_emulator_log.txt", "a"); if(f_GetFrameTimings) { fprintf(f_GetFrameTimings, "Called: IVRCompositor::GetFrameTimings\n"); fclose(f_GetFrameTimings); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual float GetFrameTimeRemaining() override {
        FILE* f_GetFrameTimeRemaining = fopen("vr_emulator_log.txt", "a"); if(f_GetFrameTimeRemaining) { fprintf(f_GetFrameTimeRemaining, "Called: IVRCompositor::GetFrameTimeRemaining\n"); fclose(f_GetFrameTimeRemaining); }
        return (float)0;
    }
    virtual void GetCumulativeStats(Compositor_CumulativeStats *pStats, uint32_t nStatsSizeInBytes) override {
        FILE* f_GetCumulativeStats = fopen("vr_emulator_log.txt", "a"); if(f_GetCumulativeStats) { fprintf(f_GetCumulativeStats, "Called: IVRCompositor::GetCumulativeStats\n"); fclose(f_GetCumulativeStats); }
        return;
    }
    virtual void FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground = false) override {
        FILE* f_FadeToColor = fopen("vr_emulator_log.txt", "a"); if(f_FadeToColor) { fprintf(f_FadeToColor, "Called: IVRCompositor::FadeToColor\n"); fclose(f_FadeToColor); }
        return;
    }
    virtual HmdColor_t GetCurrentFadeColor(bool bBackground = false) override {
        FILE* f_GetCurrentFadeColor = fopen("vr_emulator_log.txt", "a"); if(f_GetCurrentFadeColor) { fprintf(f_GetCurrentFadeColor, "Called: IVRCompositor::GetCurrentFadeColor\n"); fclose(f_GetCurrentFadeColor); }
        HmdColor_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual void FadeGrid(float fSeconds, bool bFadeIn) override {
        FILE* f_FadeGrid = fopen("vr_emulator_log.txt", "a"); if(f_FadeGrid) { fprintf(f_FadeGrid, "Called: IVRCompositor::FadeGrid\n"); fclose(f_FadeGrid); }
        return;
    }
    virtual float GetCurrentGridAlpha() override {
        FILE* f_GetCurrentGridAlpha = fopen("vr_emulator_log.txt", "a"); if(f_GetCurrentGridAlpha) { fprintf(f_GetCurrentGridAlpha, "Called: IVRCompositor::GetCurrentGridAlpha\n"); fclose(f_GetCurrentGridAlpha); }
        return (float)0;
    }
    virtual EVRCompositorError SetSkyboxOverride(VR_ARRAY_COUNT( unTextureCount ) const Texture_t *pTextures, uint32_t unTextureCount) override {
        FILE* f_SetSkyboxOverride = fopen("vr_emulator_log.txt", "a"); if(f_SetSkyboxOverride) { fprintf(f_SetSkyboxOverride, "Called: IVRCompositor::SetSkyboxOverride\n"); fclose(f_SetSkyboxOverride); }
        return (EVRCompositorError)0;
    }
    virtual void ClearSkyboxOverride() override {
        FILE* f_ClearSkyboxOverride = fopen("vr_emulator_log.txt", "a"); if(f_ClearSkyboxOverride) { fprintf(f_ClearSkyboxOverride, "Called: IVRCompositor::ClearSkyboxOverride\n"); fclose(f_ClearSkyboxOverride); }
        return;
    }
    virtual void CompositorBringToFront() override {
        FILE* f_CompositorBringToFront = fopen("vr_emulator_log.txt", "a"); if(f_CompositorBringToFront) { fprintf(f_CompositorBringToFront, "Called: IVRCompositor::CompositorBringToFront\n"); fclose(f_CompositorBringToFront); }
        return;
    }
    virtual void CompositorGoToBack() override {
        FILE* f_CompositorGoToBack = fopen("vr_emulator_log.txt", "a"); if(f_CompositorGoToBack) { fprintf(f_CompositorGoToBack, "Called: IVRCompositor::CompositorGoToBack\n"); fclose(f_CompositorGoToBack); }
        return;
    }
    virtual void CompositorQuit() override {
        FILE* f_CompositorQuit = fopen("vr_emulator_log.txt", "a"); if(f_CompositorQuit) { fprintf(f_CompositorQuit, "Called: IVRCompositor::CompositorQuit\n"); fclose(f_CompositorQuit); }
        return;
    }
    virtual bool IsFullscreen() override {
        FILE* f_IsFullscreen = fopen("vr_emulator_log.txt", "a"); if(f_IsFullscreen) { fprintf(f_IsFullscreen, "Called: IVRCompositor::IsFullscreen\n"); fclose(f_IsFullscreen); }
        return false;
    }
    virtual uint32_t GetCurrentSceneFocusProcess() override {
        FILE* f_GetCurrentSceneFocusProcess = fopen("vr_emulator_log.txt", "a"); if(f_GetCurrentSceneFocusProcess) { fprintf(f_GetCurrentSceneFocusProcess, "Called: IVRCompositor::GetCurrentSceneFocusProcess\n"); fclose(f_GetCurrentSceneFocusProcess); }
        return GetCurrentProcessId();
    }
    virtual uint32_t GetLastFrameRenderer() override {
        FILE* f_GetLastFrameRenderer = fopen("vr_emulator_log.txt", "a"); if(f_GetLastFrameRenderer) { fprintf(f_GetLastFrameRenderer, "Called: IVRCompositor::GetLastFrameRenderer\n"); fclose(f_GetLastFrameRenderer); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual bool CanRenderScene() override {
        FILE* f_CanRenderScene = fopen("vr_emulator_log.txt", "a"); if(f_CanRenderScene) { fprintf(f_CanRenderScene, "Called: IVRCompositor::CanRenderScene\n"); fclose(f_CanRenderScene); }
        return true;
    }
    virtual void ShowMirrorWindow() override {
        FILE* f_ShowMirrorWindow = fopen("vr_emulator_log.txt", "a"); if(f_ShowMirrorWindow) { fprintf(f_ShowMirrorWindow, "Called: IVRCompositor::ShowMirrorWindow\n"); fclose(f_ShowMirrorWindow); }
        return;
    }
    virtual void HideMirrorWindow() override {
        FILE* f_HideMirrorWindow = fopen("vr_emulator_log.txt", "a"); if(f_HideMirrorWindow) { fprintf(f_HideMirrorWindow, "Called: IVRCompositor::HideMirrorWindow\n"); fclose(f_HideMirrorWindow); }
        return;
    }
    virtual bool IsMirrorWindowVisible() override {
        FILE* f_IsMirrorWindowVisible = fopen("vr_emulator_log.txt", "a"); if(f_IsMirrorWindowVisible) { fprintf(f_IsMirrorWindowVisible, "Called: IVRCompositor::IsMirrorWindowVisible\n"); fclose(f_IsMirrorWindowVisible); }
        return false;
    }
    virtual void CompositorDumpImages() override {
        FILE* f_CompositorDumpImages = fopen("vr_emulator_log.txt", "a"); if(f_CompositorDumpImages) { fprintf(f_CompositorDumpImages, "Called: IVRCompositor::CompositorDumpImages\n"); fclose(f_CompositorDumpImages); }
        return;
    }
    virtual bool ShouldAppRenderWithLowResources() override {
        FILE* f_ShouldAppRenderWithLowResources = fopen("vr_emulator_log.txt", "a"); if(f_ShouldAppRenderWithLowResources) { fprintf(f_ShouldAppRenderWithLowResources, "Called: IVRCompositor::ShouldAppRenderWithLowResources\n"); fclose(f_ShouldAppRenderWithLowResources); }
        return false;
    }
    virtual void ForceInterleavedReprojectionOn(bool bOverride) override {
        FILE* f_ForceInterleavedReprojectionOn = fopen("vr_emulator_log.txt", "a"); if(f_ForceInterleavedReprojectionOn) { fprintf(f_ForceInterleavedReprojectionOn, "Called: IVRCompositor::ForceInterleavedReprojectionOn\n"); fclose(f_ForceInterleavedReprojectionOn); }
        return;
    }
    virtual void ForceReconnectProcess() override {
        FILE* f_ForceReconnectProcess = fopen("vr_emulator_log.txt", "a"); if(f_ForceReconnectProcess) { fprintf(f_ForceReconnectProcess, "Called: IVRCompositor::ForceReconnectProcess\n"); fclose(f_ForceReconnectProcess); }
        return;
    }
    virtual void SuspendRendering(bool bSuspend) override {
        FILE* f_SuspendRendering = fopen("vr_emulator_log.txt", "a"); if(f_SuspendRendering) { fprintf(f_SuspendRendering, "Called: IVRCompositor::SuspendRendering\n"); fclose(f_SuspendRendering); }
        return;
    }
    virtual vr::EVRCompositorError GetMirrorTextureD3D11(vr::EVREye eEye, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView) override {
        FILE* f_GetMirrorTextureD3D11 = fopen("vr_emulator_log.txt", "a"); if(f_GetMirrorTextureD3D11) { fprintf(f_GetMirrorTextureD3D11, "Called: IVRCompositor::GetMirrorTextureD3D11\n"); fclose(f_GetMirrorTextureD3D11); }
        return (vr::EVRCompositorError)0;
    }
    virtual void ReleaseMirrorTextureD3D11(void *pD3D11ShaderResourceView) override {
        FILE* f_ReleaseMirrorTextureD3D11 = fopen("vr_emulator_log.txt", "a"); if(f_ReleaseMirrorTextureD3D11) { fprintf(f_ReleaseMirrorTextureD3D11, "Called: IVRCompositor::ReleaseMirrorTextureD3D11\n"); fclose(f_ReleaseMirrorTextureD3D11); }
        return;
    }
    virtual vr::EVRCompositorError GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t *pglTextureId, vr::glSharedTextureHandle_t *pglSharedTextureHandle) override {
        FILE* f_GetMirrorTextureGL = fopen("vr_emulator_log.txt", "a"); if(f_GetMirrorTextureGL) { fprintf(f_GetMirrorTextureGL, "Called: IVRCompositor::GetMirrorTextureGL\n"); fclose(f_GetMirrorTextureGL); }
        return (vr::EVRCompositorError)0;
    }
    virtual bool ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        FILE* f_ReleaseSharedGLTexture = fopen("vr_emulator_log.txt", "a"); if(f_ReleaseSharedGLTexture) { fprintf(f_ReleaseSharedGLTexture, "Called: IVRCompositor::ReleaseSharedGLTexture\n"); fclose(f_ReleaseSharedGLTexture); }
        return false;
    }
    virtual void LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        FILE* f_LockGLSharedTextureForAccess = fopen("vr_emulator_log.txt", "a"); if(f_LockGLSharedTextureForAccess) { fprintf(f_LockGLSharedTextureForAccess, "Called: IVRCompositor::LockGLSharedTextureForAccess\n"); fclose(f_LockGLSharedTextureForAccess); }
        return;
    }
    virtual void UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        FILE* f_UnlockGLSharedTextureForAccess = fopen("vr_emulator_log.txt", "a"); if(f_UnlockGLSharedTextureForAccess) { fprintf(f_UnlockGLSharedTextureForAccess, "Called: IVRCompositor::UnlockGLSharedTextureForAccess\n"); fclose(f_UnlockGLSharedTextureForAccess); }
        return;
    }
    virtual uint32_t GetVulkanInstanceExtensionsRequired(VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        FILE* f_GetVulkanInstanceExtensionsRequired = fopen("vr_emulator_log.txt", "a"); if(f_GetVulkanInstanceExtensionsRequired) { fprintf(f_GetVulkanInstanceExtensionsRequired, "Called: IVRCompositor::GetVulkanInstanceExtensionsRequired\n"); fclose(f_GetVulkanInstanceExtensionsRequired); }
        if(pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return 0;
    }
    virtual uint32_t GetVulkanDeviceExtensionsRequired(VkPhysicalDevice_T *pPhysicalDevice, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        FILE* f_GetVulkanDeviceExtensionsRequired = fopen("vr_emulator_log.txt", "a"); if(f_GetVulkanDeviceExtensionsRequired) { fprintf(f_GetVulkanDeviceExtensionsRequired, "Called: IVRCompositor::GetVulkanDeviceExtensionsRequired\n"); fclose(f_GetVulkanDeviceExtensionsRequired); }
        if(pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return 0;
    }
    virtual void SetExplicitTimingMode(EVRCompositorTimingMode eTimingMode) override {
        FILE* f_SetExplicitTimingMode = fopen("vr_emulator_log.txt", "a"); if(f_SetExplicitTimingMode) { fprintf(f_SetExplicitTimingMode, "Called: IVRCompositor::SetExplicitTimingMode\n"); fclose(f_SetExplicitTimingMode); }
        return;
    }
    virtual EVRCompositorError SubmitExplicitTimingData() override {
        FILE* f_SubmitExplicitTimingData = fopen("vr_emulator_log.txt", "a"); if(f_SubmitExplicitTimingData) { fprintf(f_SubmitExplicitTimingData, "Called: IVRCompositor::SubmitExplicitTimingData\n"); fclose(f_SubmitExplicitTimingData); }
        return (EVRCompositorError)0;
    }
    virtual bool IsMotionSmoothingEnabled() override {
        FILE* f_IsMotionSmoothingEnabled = fopen("vr_emulator_log.txt", "a"); if(f_IsMotionSmoothingEnabled) { fprintf(f_IsMotionSmoothingEnabled, "Called: IVRCompositor::IsMotionSmoothingEnabled\n"); fclose(f_IsMotionSmoothingEnabled); }
        return false;
    }
    virtual bool IsMotionSmoothingSupported() override {
        FILE* f_IsMotionSmoothingSupported = fopen("vr_emulator_log.txt", "a"); if(f_IsMotionSmoothingSupported) { fprintf(f_IsMotionSmoothingSupported, "Called: IVRCompositor::IsMotionSmoothingSupported\n"); fclose(f_IsMotionSmoothingSupported); }
        return false;
    }
    virtual bool IsCurrentSceneFocusAppLoading() override {
        FILE* f_IsCurrentSceneFocusAppLoading = fopen("vr_emulator_log.txt", "a"); if(f_IsCurrentSceneFocusAppLoading) { fprintf(f_IsCurrentSceneFocusAppLoading, "Called: IVRCompositor::IsCurrentSceneFocusAppLoading\n"); fclose(f_IsCurrentSceneFocusAppLoading); }
        return false;
    }
    virtual EVRCompositorError SetStageOverride_Async(const char *pchRenderModelPath, const HmdMatrix34_t *pTransform = 0, const Compositor_StageRenderSettings *pRenderSettings = 0, uint32_t nSizeOfRenderSettings = 0) override {
        FILE* f_SetStageOverride_Async = fopen("vr_emulator_log.txt", "a"); if(f_SetStageOverride_Async) { fprintf(f_SetStageOverride_Async, "Called: IVRCompositor::SetStageOverride_Async\n"); fclose(f_SetStageOverride_Async); }
        return (EVRCompositorError)0;
    }
    virtual void ClearStageOverride() override {
        FILE* f_ClearStageOverride = fopen("vr_emulator_log.txt", "a"); if(f_ClearStageOverride) { fprintf(f_ClearStageOverride, "Called: IVRCompositor::ClearStageOverride\n"); fclose(f_ClearStageOverride); }
        return;
    }
    virtual bool GetCompositorBenchmarkResults(Compositor_BenchmarkResults *pBenchmarkResults, uint32_t nSizeOfBenchmarkResults) override {
        FILE* f_GetCompositorBenchmarkResults = fopen("vr_emulator_log.txt", "a"); if(f_GetCompositorBenchmarkResults) { fprintf(f_GetCompositorBenchmarkResults, "Called: IVRCompositor::GetCompositorBenchmarkResults\n"); fclose(f_GetCompositorBenchmarkResults); }
        return false;
    }
    virtual EVRCompositorError GetLastPosePredictionIDs(uint32_t *pRenderPosePredictionID, uint32_t *pGamePosePredictionID) override {
        FILE* f_GetLastPosePredictionIDs = fopen("vr_emulator_log.txt", "a"); if(f_GetLastPosePredictionIDs) { fprintf(f_GetLastPosePredictionIDs, "Called: IVRCompositor::GetLastPosePredictionIDs\n"); fclose(f_GetLastPosePredictionIDs); }
        return (EVRCompositorError)0;
    }
    virtual EVRCompositorError GetPosesForFrame(uint32_t unPosePredictionID, VR_ARRAY_COUNT( unPoseArrayCount ) TrackedDevicePose_t* pPoseArray, uint32_t unPoseArrayCount) override {
        FILE* f_GetPosesForFrame = fopen("vr_emulator_log.txt", "a"); if(f_GetPosesForFrame) { fprintf(f_GetPosesForFrame, "Called: IVRCompositor::GetPosesForFrame\n"); fclose(f_GetPosesForFrame); }
        return (EVRCompositorError)0;
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
        FILE* f_SetHeadsetViewSize = fopen("vr_emulator_log.txt", "a"); if(f_SetHeadsetViewSize) { fprintf(f_SetHeadsetViewSize, "Called: IVRHeadsetView::SetHeadsetViewSize\n"); fclose(f_SetHeadsetViewSize); }
        return;
    }
    virtual void GetHeadsetViewSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* f_GetHeadsetViewSize = fopen("vr_emulator_log.txt", "a"); if(f_GetHeadsetViewSize) { fprintf(f_GetHeadsetViewSize, "Called: IVRHeadsetView::GetHeadsetViewSize\n"); fclose(f_GetHeadsetViewSize); }
        return;
    }
    virtual void SetHeadsetViewMode(HeadsetViewMode_t eHeadsetViewMode) override {
        FILE* f_SetHeadsetViewMode = fopen("vr_emulator_log.txt", "a"); if(f_SetHeadsetViewMode) { fprintf(f_SetHeadsetViewMode, "Called: IVRHeadsetView::SetHeadsetViewMode\n"); fclose(f_SetHeadsetViewMode); }
        return;
    }
    virtual HeadsetViewMode_t GetHeadsetViewMode() override {
        FILE* f_GetHeadsetViewMode = fopen("vr_emulator_log.txt", "a"); if(f_GetHeadsetViewMode) { fprintf(f_GetHeadsetViewMode, "Called: IVRHeadsetView::GetHeadsetViewMode\n"); fclose(f_GetHeadsetViewMode); }
        HeadsetViewMode_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual void SetHeadsetViewCropped(bool bCropped) override {
        FILE* f_SetHeadsetViewCropped = fopen("vr_emulator_log.txt", "a"); if(f_SetHeadsetViewCropped) { fprintf(f_SetHeadsetViewCropped, "Called: IVRHeadsetView::SetHeadsetViewCropped\n"); fclose(f_SetHeadsetViewCropped); }
        return;
    }
    virtual bool GetHeadsetViewCropped() override {
        FILE* f_GetHeadsetViewCropped = fopen("vr_emulator_log.txt", "a"); if(f_GetHeadsetViewCropped) { fprintf(f_GetHeadsetViewCropped, "Called: IVRHeadsetView::GetHeadsetViewCropped\n"); fclose(f_GetHeadsetViewCropped); }
        return false;
    }
    virtual float GetHeadsetViewAspectRatio() override {
        FILE* f_GetHeadsetViewAspectRatio = fopen("vr_emulator_log.txt", "a"); if(f_GetHeadsetViewAspectRatio) { fprintf(f_GetHeadsetViewAspectRatio, "Called: IVRHeadsetView::GetHeadsetViewAspectRatio\n"); fclose(f_GetHeadsetViewAspectRatio); }
        return (float)0;
    }
    virtual void SetHeadsetViewBlendRange(float flStartPct, float flEndPct) override {
        FILE* f_SetHeadsetViewBlendRange = fopen("vr_emulator_log.txt", "a"); if(f_SetHeadsetViewBlendRange) { fprintf(f_SetHeadsetViewBlendRange, "Called: IVRHeadsetView::SetHeadsetViewBlendRange\n"); fclose(f_SetHeadsetViewBlendRange); }
        return;
    }
    virtual void GetHeadsetViewBlendRange(float *pStartPct, float *pEndPct) override {
        FILE* f_GetHeadsetViewBlendRange = fopen("vr_emulator_log.txt", "a"); if(f_GetHeadsetViewBlendRange) { fprintf(f_GetHeadsetViewBlendRange, "Called: IVRHeadsetView::GetHeadsetViewBlendRange\n"); fclose(f_GetHeadsetViewBlendRange); }
        return;
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
        FILE* f_CreateNotification = fopen("vr_emulator_log.txt", "a"); if(f_CreateNotification) { fprintf(f_CreateNotification, "Called: IVRNotifications::CreateNotification\n"); fclose(f_CreateNotification); }
        return (EVRNotificationError)0;
    }
    virtual EVRNotificationError RemoveNotification(VRNotificationId notificationId) override {
        FILE* f_RemoveNotification = fopen("vr_emulator_log.txt", "a"); if(f_RemoveNotification) { fprintf(f_RemoveNotification, "Called: IVRNotifications::RemoveNotification\n"); fclose(f_RemoveNotification); }
        return (EVRNotificationError)0;
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
        FILE* f_FindOverlay = fopen("vr_emulator_log.txt", "a"); if(f_FindOverlay) { fprintf(f_FindOverlay, "Called: IVROverlay::FindOverlay\n"); fclose(f_FindOverlay); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError CreateOverlay(const char *pchOverlayKey, const char *pchOverlayName, VROverlayHandle_t * pOverlayHandle) override {
        FILE* f_CreateOverlay = fopen("vr_emulator_log.txt", "a"); if(f_CreateOverlay) { fprintf(f_CreateOverlay, "Called: IVROverlay::CreateOverlay\n"); fclose(f_CreateOverlay); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError DestroyOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_DestroyOverlay = fopen("vr_emulator_log.txt", "a"); if(f_DestroyOverlay) { fprintf(f_DestroyOverlay, "Called: IVROverlay::DestroyOverlay\n"); fclose(f_DestroyOverlay); }
        return (EVROverlayError)0;
    }
    virtual uint32_t GetOverlayKey(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L) override {
        FILE* f_GetOverlayKey = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayKey) { fprintf(f_GetOverlayKey, "Called: IVROverlay::GetOverlayKey\n"); fclose(f_GetOverlayKey); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetOverlayName(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L) override {
        FILE* f_GetOverlayName = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayName) { fprintf(f_GetOverlayName, "Called: IVROverlay::GetOverlayName\n"); fclose(f_GetOverlayName); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EVROverlayError SetOverlayName(VROverlayHandle_t ulOverlayHandle, const char *pchName) override {
        FILE* f_SetOverlayName = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayName) { fprintf(f_SetOverlayName, "Called: IVROverlay::SetOverlayName\n"); fclose(f_SetOverlayName); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayImageData(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unBufferSize, uint32_t *punWidth, uint32_t *punHeight) override {
        FILE* f_GetOverlayImageData = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayImageData) { fprintf(f_GetOverlayImageData, "Called: IVROverlay::GetOverlayImageData\n"); fclose(f_GetOverlayImageData); }
        return (EVROverlayError)0;
    }
    virtual const char * GetOverlayErrorNameFromEnum(EVROverlayError error) override {
        FILE* f_GetOverlayErrorNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayErrorNameFromEnum) { fprintf(f_GetOverlayErrorNameFromEnum, "Called: IVROverlay::GetOverlayErrorNameFromEnum\n"); fclose(f_GetOverlayErrorNameFromEnum); }
        return nullptr;
    }
    virtual EVROverlayError SetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle, uint32_t unPID) override {
        FILE* f_SetOverlayRenderingPid = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayRenderingPid) { fprintf(f_SetOverlayRenderingPid, "Called: IVROverlay::SetOverlayRenderingPid\n"); fclose(f_SetOverlayRenderingPid); }
        return (EVROverlayError)0;
    }
    virtual uint32_t GetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_GetOverlayRenderingPid = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayRenderingPid) { fprintf(f_GetOverlayRenderingPid, "Called: IVROverlay::GetOverlayRenderingPid\n"); fclose(f_GetOverlayRenderingPid); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EVROverlayError SetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled) override {
        FILE* f_SetOverlayFlag = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayFlag) { fprintf(f_SetOverlayFlag, "Called: IVROverlay::SetOverlayFlag\n"); fclose(f_SetOverlayFlag); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool *pbEnabled) override {
        FILE* f_GetOverlayFlag = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayFlag) { fprintf(f_GetOverlayFlag, "Called: IVROverlay::GetOverlayFlag\n"); fclose(f_GetOverlayFlag); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayFlags(VROverlayHandle_t ulOverlayHandle, uint32_t *pFlags) override {
        FILE* f_GetOverlayFlags = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayFlags) { fprintf(f_GetOverlayFlags, "Called: IVROverlay::GetOverlayFlags\n"); fclose(f_GetOverlayFlags); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayColor(VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue) override {
        FILE* f_SetOverlayColor = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayColor) { fprintf(f_SetOverlayColor, "Called: IVROverlay::SetOverlayColor\n"); fclose(f_SetOverlayColor); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayColor(VROverlayHandle_t ulOverlayHandle, float *pfRed, float *pfGreen, float *pfBlue) override {
        FILE* f_GetOverlayColor = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayColor) { fprintf(f_GetOverlayColor, "Called: IVROverlay::GetOverlayColor\n"); fclose(f_GetOverlayColor); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float fAlpha) override {
        FILE* f_SetOverlayAlpha = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayAlpha) { fprintf(f_SetOverlayAlpha, "Called: IVROverlay::SetOverlayAlpha\n"); fclose(f_SetOverlayAlpha); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float *pfAlpha) override {
        FILE* f_GetOverlayAlpha = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayAlpha) { fprintf(f_GetOverlayAlpha, "Called: IVROverlay::GetOverlayAlpha\n"); fclose(f_GetOverlayAlpha); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float fTexelAspect) override {
        FILE* f_SetOverlayTexelAspect = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTexelAspect) { fprintf(f_SetOverlayTexelAspect, "Called: IVROverlay::SetOverlayTexelAspect\n"); fclose(f_SetOverlayTexelAspect); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float *pfTexelAspect) override {
        FILE* f_GetOverlayTexelAspect = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTexelAspect) { fprintf(f_GetOverlayTexelAspect, "Called: IVROverlay::GetOverlayTexelAspect\n"); fclose(f_GetOverlayTexelAspect); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder) override {
        FILE* f_SetOverlaySortOrder = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlaySortOrder) { fprintf(f_SetOverlaySortOrder, "Called: IVROverlay::SetOverlaySortOrder\n"); fclose(f_SetOverlaySortOrder); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t *punSortOrder) override {
        FILE* f_GetOverlaySortOrder = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlaySortOrder) { fprintf(f_GetOverlaySortOrder, "Called: IVROverlay::GetOverlaySortOrder\n"); fclose(f_GetOverlaySortOrder); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float fWidthInMeters) override {
        FILE* f_SetOverlayWidthInMeters = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayWidthInMeters) { fprintf(f_SetOverlayWidthInMeters, "Called: IVROverlay::SetOverlayWidthInMeters\n"); fclose(f_SetOverlayWidthInMeters); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float *pfWidthInMeters) override {
        FILE* f_GetOverlayWidthInMeters = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayWidthInMeters) { fprintf(f_GetOverlayWidthInMeters, "Called: IVROverlay::GetOverlayWidthInMeters\n"); fclose(f_GetOverlayWidthInMeters); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float fCurvature) override {
        FILE* f_SetOverlayCurvature = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayCurvature) { fprintf(f_SetOverlayCurvature, "Called: IVROverlay::SetOverlayCurvature\n"); fclose(f_SetOverlayCurvature); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float *pfCurvature) override {
        FILE* f_GetOverlayCurvature = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayCurvature) { fprintf(f_GetOverlayCurvature, "Called: IVROverlay::GetOverlayCurvature\n"); fclose(f_GetOverlayCurvature); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace eTextureColorSpace) override {
        FILE* f_SetOverlayTextureColorSpace = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTextureColorSpace) { fprintf(f_SetOverlayTextureColorSpace, "Called: IVROverlay::SetOverlayTextureColorSpace\n"); fclose(f_SetOverlayTextureColorSpace); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace *peTextureColorSpace) override {
        FILE* f_GetOverlayTextureColorSpace = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTextureColorSpace) { fprintf(f_GetOverlayTextureColorSpace, "Called: IVROverlay::GetOverlayTextureColorSpace\n"); fclose(f_GetOverlayTextureColorSpace); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, const VRTextureBounds_t *pOverlayTextureBounds) override {
        FILE* f_SetOverlayTextureBounds = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTextureBounds) { fprintf(f_SetOverlayTextureBounds, "Called: IVROverlay::SetOverlayTextureBounds\n"); fclose(f_SetOverlayTextureBounds); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, VRTextureBounds_t *pOverlayTextureBounds) override {
        FILE* f_GetOverlayTextureBounds = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTextureBounds) { fprintf(f_GetOverlayTextureBounds, "Called: IVROverlay::GetOverlayTextureBounds\n"); fclose(f_GetOverlayTextureBounds); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTransformType(VROverlayHandle_t ulOverlayHandle, VROverlayTransformType *peTransformType) override {
        FILE* f_GetOverlayTransformType = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTransformType) { fprintf(f_GetOverlayTransformType, "Called: IVROverlay::GetOverlayTransformType\n"); fclose(f_GetOverlayTransformType); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        FILE* f_SetOverlayTransformAbsolute = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTransformAbsolute) { fprintf(f_SetOverlayTransformAbsolute, "Called: IVROverlay::SetOverlayTransformAbsolute\n"); fclose(f_SetOverlayTransformAbsolute); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin *peTrackingOrigin, HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        FILE* f_GetOverlayTransformAbsolute = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTransformAbsolute) { fprintf(f_GetOverlayTransformAbsolute, "Called: IVROverlay::GetOverlayTransformAbsolute\n"); fclose(f_GetOverlayTransformAbsolute); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        FILE* f_SetOverlayTransformTrackedDeviceRelative = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTransformTrackedDeviceRelative) { fprintf(f_SetOverlayTransformTrackedDeviceRelative, "Called: IVROverlay::SetOverlayTransformTrackedDeviceRelative\n"); fclose(f_SetOverlayTransformTrackedDeviceRelative); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punTrackedDevice, HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        FILE* f_GetOverlayTransformTrackedDeviceRelative = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTransformTrackedDeviceRelative) { fprintf(f_GetOverlayTransformTrackedDeviceRelative, "Called: IVROverlay::GetOverlayTransformTrackedDeviceRelative\n"); fclose(f_GetOverlayTransformTrackedDeviceRelative); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unDeviceIndex, const char *pchComponentName) override {
        FILE* f_SetOverlayTransformTrackedDeviceComponent = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTransformTrackedDeviceComponent) { fprintf(f_SetOverlayTransformTrackedDeviceComponent, "Called: IVROverlay::SetOverlayTransformTrackedDeviceComponent\n"); fclose(f_SetOverlayTransformTrackedDeviceComponent); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punDeviceIndex, VR_OUT_STRING() char *pchComponentName, uint32_t unComponentNameSize) override {
        FILE* f_GetOverlayTransformTrackedDeviceComponent = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTransformTrackedDeviceComponent) { fprintf(f_GetOverlayTransformTrackedDeviceComponent, "Called: IVROverlay::GetOverlayTransformTrackedDeviceComponent\n"); fclose(f_GetOverlayTransformTrackedDeviceComponent); }
        return (EVROverlayError)0;
    }
    virtual vr::EVROverlayError GetOverlayTransformOverlayRelative(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t *ulOverlayHandleParent, HmdMatrix34_t *pmatParentOverlayToOverlayTransform) override {
        FILE* f_GetOverlayTransformOverlayRelative = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTransformOverlayRelative) { fprintf(f_GetOverlayTransformOverlayRelative, "Called: IVROverlay::GetOverlayTransformOverlayRelative\n"); fclose(f_GetOverlayTransformOverlayRelative); }
        return (vr::EVROverlayError)0;
    }
    virtual vr::EVROverlayError SetOverlayTransformOverlayRelative(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulOverlayHandleParent, const HmdMatrix34_t *pmatParentOverlayToOverlayTransform) override {
        FILE* f_SetOverlayTransformOverlayRelative = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTransformOverlayRelative) { fprintf(f_SetOverlayTransformOverlayRelative, "Called: IVROverlay::SetOverlayTransformOverlayRelative\n"); fclose(f_SetOverlayTransformOverlayRelative); }
        return (vr::EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTransformCursor(VROverlayHandle_t ulCursorOverlayHandle, const HmdVector2_t *pvHotspot) override {
        FILE* f_SetOverlayTransformCursor = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTransformCursor) { fprintf(f_SetOverlayTransformCursor, "Called: IVROverlay::SetOverlayTransformCursor\n"); fclose(f_SetOverlayTransformCursor); }
        return (EVROverlayError)0;
    }
    virtual vr::EVROverlayError GetOverlayTransformCursor(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvHotspot) override {
        FILE* f_GetOverlayTransformCursor = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTransformCursor) { fprintf(f_GetOverlayTransformCursor, "Called: IVROverlay::GetOverlayTransformCursor\n"); fclose(f_GetOverlayTransformCursor); }
        return (vr::EVROverlayError)0;
    }
    virtual EVROverlayError ShowOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_ShowOverlay = fopen("vr_emulator_log.txt", "a"); if(f_ShowOverlay) { fprintf(f_ShowOverlay, "Called: IVROverlay::ShowOverlay\n"); fclose(f_ShowOverlay); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError HideOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_HideOverlay = fopen("vr_emulator_log.txt", "a"); if(f_HideOverlay) { fprintf(f_HideOverlay, "Called: IVROverlay::HideOverlay\n"); fclose(f_HideOverlay); }
        return (EVROverlayError)0;
    }
    virtual bool IsOverlayVisible(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_IsOverlayVisible = fopen("vr_emulator_log.txt", "a"); if(f_IsOverlayVisible) { fprintf(f_IsOverlayVisible, "Called: IVROverlay::IsOverlayVisible\n"); fclose(f_IsOverlayVisible); }
        return false;
    }
    virtual EVROverlayError GetTransformForOverlayCoordinates(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, HmdVector2_t coordinatesInOverlay, HmdMatrix34_t *pmatTransform) override {
        FILE* f_GetTransformForOverlayCoordinates = fopen("vr_emulator_log.txt", "a"); if(f_GetTransformForOverlayCoordinates) { fprintf(f_GetTransformForOverlayCoordinates, "Called: IVROverlay::GetTransformForOverlayCoordinates\n"); fclose(f_GetTransformForOverlayCoordinates); }
        return (EVROverlayError)0;
    }
    virtual bool PollNextOverlayEvent(VROverlayHandle_t ulOverlayHandle, VREvent_t *pEvent, uint32_t uncbVREvent) override {
        FILE* f_PollNextOverlayEvent = fopen("vr_emulator_log.txt", "a"); if(f_PollNextOverlayEvent) { fprintf(f_PollNextOverlayEvent, "Called: IVROverlay::PollNextOverlayEvent\n"); fclose(f_PollNextOverlayEvent); }
        return false;
    }
    virtual EVROverlayError GetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod *peInputMethod) override {
        FILE* f_GetOverlayInputMethod = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayInputMethod) { fprintf(f_GetOverlayInputMethod, "Called: IVROverlay::GetOverlayInputMethod\n"); fclose(f_GetOverlayInputMethod); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod) override {
        FILE* f_SetOverlayInputMethod = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayInputMethod) { fprintf(f_SetOverlayInputMethod, "Called: IVROverlay::SetOverlayInputMethod\n"); fclose(f_SetOverlayInputMethod); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvecMouseScale) override {
        FILE* f_GetOverlayMouseScale = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayMouseScale) { fprintf(f_GetOverlayMouseScale, "Called: IVROverlay::GetOverlayMouseScale\n"); fclose(f_GetOverlayMouseScale); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvecMouseScale) override {
        FILE* f_SetOverlayMouseScale = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayMouseScale) { fprintf(f_SetOverlayMouseScale, "Called: IVROverlay::SetOverlayMouseScale\n"); fclose(f_SetOverlayMouseScale); }
        return (EVROverlayError)0;
    }
    virtual bool ComputeOverlayIntersection(VROverlayHandle_t ulOverlayHandle, const VROverlayIntersectionParams_t *pParams, VROverlayIntersectionResults_t *pResults) override {
        FILE* f_ComputeOverlayIntersection = fopen("vr_emulator_log.txt", "a"); if(f_ComputeOverlayIntersection) { fprintf(f_ComputeOverlayIntersection, "Called: IVROverlay::ComputeOverlayIntersection\n"); fclose(f_ComputeOverlayIntersection); }
        return false;
    }
    virtual bool IsHoverTargetOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_IsHoverTargetOverlay = fopen("vr_emulator_log.txt", "a"); if(f_IsHoverTargetOverlay) { fprintf(f_IsHoverTargetOverlay, "Called: IVROverlay::IsHoverTargetOverlay\n"); fclose(f_IsHoverTargetOverlay); }
        return false;
    }
    virtual EVROverlayError SetOverlayIntersectionMask(VROverlayHandle_t ulOverlayHandle, VROverlayIntersectionMaskPrimitive_t *pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize = sizeof( VROverlayIntersectionMaskPrimitive_t )) override {
        FILE* f_SetOverlayIntersectionMask = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayIntersectionMask) { fprintf(f_SetOverlayIntersectionMask, "Called: IVROverlay::SetOverlayIntersectionMask\n"); fclose(f_SetOverlayIntersectionMask); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError TriggerLaserMouseHapticVibration(VROverlayHandle_t ulOverlayHandle, float fDurationSeconds, float fFrequency, float fAmplitude) override {
        FILE* f_TriggerLaserMouseHapticVibration = fopen("vr_emulator_log.txt", "a"); if(f_TriggerLaserMouseHapticVibration) { fprintf(f_TriggerLaserMouseHapticVibration, "Called: IVROverlay::TriggerLaserMouseHapticVibration\n"); fclose(f_TriggerLaserMouseHapticVibration); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayCursor(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulCursorHandle) override {
        FILE* f_SetOverlayCursor = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayCursor) { fprintf(f_SetOverlayCursor, "Called: IVROverlay::SetOverlayCursor\n"); fclose(f_SetOverlayCursor); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvCursor) override {
        FILE* f_SetOverlayCursorPositionOverride = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayCursorPositionOverride) { fprintf(f_SetOverlayCursorPositionOverride, "Called: IVROverlay::SetOverlayCursorPositionOverride\n"); fclose(f_SetOverlayCursorPositionOverride); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError ClearOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_ClearOverlayCursorPositionOverride = fopen("vr_emulator_log.txt", "a"); if(f_ClearOverlayCursorPositionOverride) { fprintf(f_ClearOverlayCursorPositionOverride, "Called: IVROverlay::ClearOverlayCursorPositionOverride\n"); fclose(f_ClearOverlayCursorPositionOverride); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayTexture(VROverlayHandle_t ulOverlayHandle, const Texture_t *pTexture) override {
        FILE* f_SetOverlayTexture = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayTexture) { fprintf(f_SetOverlayTexture, "Called: IVROverlay::SetOverlayTexture\n"); fclose(f_SetOverlayTexture); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError ClearOverlayTexture(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_ClearOverlayTexture = fopen("vr_emulator_log.txt", "a"); if(f_ClearOverlayTexture) { fprintf(f_ClearOverlayTexture, "Called: IVROverlay::ClearOverlayTexture\n"); fclose(f_ClearOverlayTexture); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayRaw(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unBytesPerPixel) override {
        FILE* f_SetOverlayRaw = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayRaw) { fprintf(f_SetOverlayRaw, "Called: IVROverlay::SetOverlayRaw\n"); fclose(f_SetOverlayRaw); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError SetOverlayFromFile(VROverlayHandle_t ulOverlayHandle, const char *pchFilePath) override {
        FILE* f_SetOverlayFromFile = fopen("vr_emulator_log.txt", "a"); if(f_SetOverlayFromFile) { fprintf(f_SetOverlayFromFile, "Called: IVROverlay::SetOverlayFromFile\n"); fclose(f_SetOverlayFromFile); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTexture(VROverlayHandle_t ulOverlayHandle, void **pNativeTextureHandle, void *pNativeTextureRef, uint32_t *pWidth, uint32_t *pHeight, uint32_t *pNativeFormat, ETextureType *pAPIType, EColorSpace *pColorSpace, VRTextureBounds_t *pTextureBounds) override {
        FILE* f_GetOverlayTexture = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTexture) { fprintf(f_GetOverlayTexture, "Called: IVROverlay::GetOverlayTexture\n"); fclose(f_GetOverlayTexture); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError ReleaseNativeOverlayHandle(VROverlayHandle_t ulOverlayHandle, void *pNativeTextureHandle) override {
        FILE* f_ReleaseNativeOverlayHandle = fopen("vr_emulator_log.txt", "a"); if(f_ReleaseNativeOverlayHandle) { fprintf(f_ReleaseNativeOverlayHandle, "Called: IVROverlay::ReleaseNativeOverlayHandle\n"); fclose(f_ReleaseNativeOverlayHandle); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetOverlayTextureSize(VROverlayHandle_t ulOverlayHandle, uint32_t *pWidth, uint32_t *pHeight) override {
        FILE* f_GetOverlayTextureSize = fopen("vr_emulator_log.txt", "a"); if(f_GetOverlayTextureSize) { fprintf(f_GetOverlayTextureSize, "Called: IVROverlay::GetOverlayTextureSize\n"); fclose(f_GetOverlayTextureSize); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError CreateDashboardOverlay(const char *pchOverlayKey, const char *pchOverlayFriendlyName, VROverlayHandle_t * pMainHandle, VROverlayHandle_t *pThumbnailHandle) override {
        FILE* f_CreateDashboardOverlay = fopen("vr_emulator_log.txt", "a"); if(f_CreateDashboardOverlay) { fprintf(f_CreateDashboardOverlay, "Called: IVROverlay::CreateDashboardOverlay\n"); fclose(f_CreateDashboardOverlay); }
        return (EVROverlayError)0;
    }
    virtual bool IsDashboardVisible() override {
        FILE* f_IsDashboardVisible = fopen("vr_emulator_log.txt", "a"); if(f_IsDashboardVisible) { fprintf(f_IsDashboardVisible, "Called: IVROverlay::IsDashboardVisible\n"); fclose(f_IsDashboardVisible); }
        return false;
    }
    virtual bool IsActiveDashboardOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_IsActiveDashboardOverlay = fopen("vr_emulator_log.txt", "a"); if(f_IsActiveDashboardOverlay) { fprintf(f_IsActiveDashboardOverlay, "Called: IVROverlay::IsActiveDashboardOverlay\n"); fclose(f_IsActiveDashboardOverlay); }
        return false;
    }
    virtual EVROverlayError SetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId) override {
        FILE* f_SetDashboardOverlaySceneProcess = fopen("vr_emulator_log.txt", "a"); if(f_SetDashboardOverlaySceneProcess) { fprintf(f_SetDashboardOverlaySceneProcess, "Called: IVROverlay::SetDashboardOverlaySceneProcess\n"); fclose(f_SetDashboardOverlaySceneProcess); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError GetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t *punProcessId) override {
        FILE* f_GetDashboardOverlaySceneProcess = fopen("vr_emulator_log.txt", "a"); if(f_GetDashboardOverlaySceneProcess) { fprintf(f_GetDashboardOverlaySceneProcess, "Called: IVROverlay::GetDashboardOverlaySceneProcess\n"); fclose(f_GetDashboardOverlaySceneProcess); }
        return (EVROverlayError)0;
    }
    virtual void ShowDashboard(const char *pchOverlayToShow) override {
        FILE* f_ShowDashboard = fopen("vr_emulator_log.txt", "a"); if(f_ShowDashboard) { fprintf(f_ShowDashboard, "Called: IVROverlay::ShowDashboard\n"); fclose(f_ShowDashboard); }
        return;
    }
    virtual vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override {
        FILE* f_GetPrimaryDashboardDevice = fopen("vr_emulator_log.txt", "a"); if(f_GetPrimaryDashboardDevice) { fprintf(f_GetPrimaryDashboardDevice, "Called: IVROverlay::GetPrimaryDashboardDevice\n"); fclose(f_GetPrimaryDashboardDevice); }
        vr::TrackedDeviceIndex_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual EVROverlayError ShowKeyboard(EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        FILE* f_ShowKeyboard = fopen("vr_emulator_log.txt", "a"); if(f_ShowKeyboard) { fprintf(f_ShowKeyboard, "Called: IVROverlay::ShowKeyboard\n"); fclose(f_ShowKeyboard); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError ShowKeyboardForOverlay(VROverlayHandle_t ulOverlayHandle, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        FILE* f_ShowKeyboardForOverlay = fopen("vr_emulator_log.txt", "a"); if(f_ShowKeyboardForOverlay) { fprintf(f_ShowKeyboardForOverlay, "Called: IVROverlay::ShowKeyboardForOverlay\n"); fclose(f_ShowKeyboardForOverlay); }
        return (EVROverlayError)0;
    }
    virtual uint32_t GetKeyboardText(VR_OUT_STRING() char *pchText, uint32_t cchText) override {
        FILE* f_GetKeyboardText = fopen("vr_emulator_log.txt", "a"); if(f_GetKeyboardText) { fprintf(f_GetKeyboardText, "Called: IVROverlay::GetKeyboardText\n"); fclose(f_GetKeyboardText); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual void HideKeyboard() override {
        FILE* f_HideKeyboard = fopen("vr_emulator_log.txt", "a"); if(f_HideKeyboard) { fprintf(f_HideKeyboard, "Called: IVROverlay::HideKeyboard\n"); fclose(f_HideKeyboard); }
        return;
    }
    virtual void SetKeyboardTransformAbsolute(ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToKeyboardTransform) override {
        FILE* f_SetKeyboardTransformAbsolute = fopen("vr_emulator_log.txt", "a"); if(f_SetKeyboardTransformAbsolute) { fprintf(f_SetKeyboardTransformAbsolute, "Called: IVROverlay::SetKeyboardTransformAbsolute\n"); fclose(f_SetKeyboardTransformAbsolute); }
        return;
    }
    virtual void SetKeyboardPositionForOverlay(VROverlayHandle_t ulOverlayHandle, HmdRect2_t avoidRect) override {
        FILE* f_SetKeyboardPositionForOverlay = fopen("vr_emulator_log.txt", "a"); if(f_SetKeyboardPositionForOverlay) { fprintf(f_SetKeyboardPositionForOverlay, "Called: IVROverlay::SetKeyboardPositionForOverlay\n"); fclose(f_SetKeyboardPositionForOverlay); }
        return;
    }
    virtual VRMessageOverlayResponse ShowMessageOverlay(const char* pchText, const char* pchCaption, const char* pchButton0Text, const char* pchButton1Text = nullptr, const char* pchButton2Text = nullptr, const char* pchButton3Text = nullptr) override {
        FILE* f_ShowMessageOverlay = fopen("vr_emulator_log.txt", "a"); if(f_ShowMessageOverlay) { fprintf(f_ShowMessageOverlay, "Called: IVROverlay::ShowMessageOverlay\n"); fclose(f_ShowMessageOverlay); }
        return (VRMessageOverlayResponse)0;
    }
    virtual void CloseMessageOverlay() override {
        FILE* f_CloseMessageOverlay = fopen("vr_emulator_log.txt", "a"); if(f_CloseMessageOverlay) { fprintf(f_CloseMessageOverlay, "Called: IVROverlay::CloseMessageOverlay\n"); fclose(f_CloseMessageOverlay); }
        return;
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
        FILE* f_AcquireOverlayView = fopen("vr_emulator_log.txt", "a"); if(f_AcquireOverlayView) { fprintf(f_AcquireOverlayView, "Called: IVROverlayView::AcquireOverlayView\n"); fclose(f_AcquireOverlayView); }
        return (EVROverlayError)0;
    }
    virtual EVROverlayError ReleaseOverlayView(VROverlayView_t *pOverlayView) override {
        FILE* f_ReleaseOverlayView = fopen("vr_emulator_log.txt", "a"); if(f_ReleaseOverlayView) { fprintf(f_ReleaseOverlayView, "Called: IVROverlayView::ReleaseOverlayView\n"); fclose(f_ReleaseOverlayView); }
        return (EVROverlayError)0;
    }
    virtual void PostOverlayEvent(VROverlayHandle_t ulOverlayHandle, const VREvent_t *pvrEvent) override {
        FILE* f_PostOverlayEvent = fopen("vr_emulator_log.txt", "a"); if(f_PostOverlayEvent) { fprintf(f_PostOverlayEvent, "Called: IVROverlayView::PostOverlayEvent\n"); fclose(f_PostOverlayEvent); }
        return;
    }
    virtual bool IsViewingPermitted(VROverlayHandle_t ulOverlayHandle) override {
        FILE* f_IsViewingPermitted = fopen("vr_emulator_log.txt", "a"); if(f_IsViewingPermitted) { fprintf(f_IsViewingPermitted, "Called: IVROverlayView::IsViewingPermitted\n"); fclose(f_IsViewingPermitted); }
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
        FILE* f_LoadRenderModel_Async = fopen("vr_emulator_log.txt", "a"); if(f_LoadRenderModel_Async) { fprintf(f_LoadRenderModel_Async, "Called: IVRRenderModels::LoadRenderModel_Async\n"); fclose(f_LoadRenderModel_Async); }
        return (EVRRenderModelError)0;
    }
    virtual void FreeRenderModel(RenderModel_t *pRenderModel) override {
        FILE* f_FreeRenderModel = fopen("vr_emulator_log.txt", "a"); if(f_FreeRenderModel) { fprintf(f_FreeRenderModel, "Called: IVRRenderModels::FreeRenderModel\n"); fclose(f_FreeRenderModel); }
        return;
    }
    virtual EVRRenderModelError LoadTexture_Async(TextureID_t textureId, RenderModel_TextureMap_t **ppTexture) override {
        FILE* f_LoadTexture_Async = fopen("vr_emulator_log.txt", "a"); if(f_LoadTexture_Async) { fprintf(f_LoadTexture_Async, "Called: IVRRenderModels::LoadTexture_Async\n"); fclose(f_LoadTexture_Async); }
        return (EVRRenderModelError)0;
    }
    virtual void FreeTexture(RenderModel_TextureMap_t *pTexture) override {
        FILE* f_FreeTexture = fopen("vr_emulator_log.txt", "a"); if(f_FreeTexture) { fprintf(f_FreeTexture, "Called: IVRRenderModels::FreeTexture\n"); fclose(f_FreeTexture); }
        return;
    }
    virtual EVRRenderModelError LoadTextureD3D11_Async(TextureID_t textureId, void *pD3D11Device, void **ppD3D11Texture2D) override {
        FILE* f_LoadTextureD3D11_Async = fopen("vr_emulator_log.txt", "a"); if(f_LoadTextureD3D11_Async) { fprintf(f_LoadTextureD3D11_Async, "Called: IVRRenderModels::LoadTextureD3D11_Async\n"); fclose(f_LoadTextureD3D11_Async); }
        return (EVRRenderModelError)0;
    }
    virtual EVRRenderModelError LoadIntoTextureD3D11_Async(TextureID_t textureId, void *pDstTexture) override {
        FILE* f_LoadIntoTextureD3D11_Async = fopen("vr_emulator_log.txt", "a"); if(f_LoadIntoTextureD3D11_Async) { fprintf(f_LoadIntoTextureD3D11_Async, "Called: IVRRenderModels::LoadIntoTextureD3D11_Async\n"); fclose(f_LoadIntoTextureD3D11_Async); }
        return (EVRRenderModelError)0;
    }
    virtual void FreeTextureD3D11(void *pD3D11Texture2D) override {
        FILE* f_FreeTextureD3D11 = fopen("vr_emulator_log.txt", "a"); if(f_FreeTextureD3D11) { fprintf(f_FreeTextureD3D11, "Called: IVRRenderModels::FreeTextureD3D11\n"); fclose(f_FreeTextureD3D11); }
        return;
    }
    virtual uint32_t GetRenderModelName(uint32_t unRenderModelIndex, VR_OUT_STRING() char *pchRenderModelName, uint32_t unRenderModelNameLen) override {
        FILE* f_GetRenderModelName = fopen("vr_emulator_log.txt", "a"); if(f_GetRenderModelName) { fprintf(f_GetRenderModelName, "Called: IVRRenderModels::GetRenderModelName\n"); fclose(f_GetRenderModelName); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetRenderModelCount() override {
        FILE* f_GetRenderModelCount = fopen("vr_emulator_log.txt", "a"); if(f_GetRenderModelCount) { fprintf(f_GetRenderModelCount, "Called: IVRRenderModels::GetRenderModelCount\n"); fclose(f_GetRenderModelCount); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetComponentCount(const char *pchRenderModelName) override {
        FILE* f_GetComponentCount = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentCount) { fprintf(f_GetComponentCount, "Called: IVRRenderModels::GetComponentCount\n"); fclose(f_GetComponentCount); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetComponentName(const char *pchRenderModelName, uint32_t unComponentIndex, VR_OUT_STRING( ) char *pchComponentName, uint32_t unComponentNameLen) override {
        FILE* f_GetComponentName = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentName) { fprintf(f_GetComponentName, "Called: IVRRenderModels::GetComponentName\n"); fclose(f_GetComponentName); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint64_t GetComponentButtonMask(const char *pchRenderModelName, const char *pchComponentName) override {
        FILE* f_GetComponentButtonMask = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentButtonMask) { fprintf(f_GetComponentButtonMask, "Called: IVRRenderModels::GetComponentButtonMask\n"); fclose(f_GetComponentButtonMask); }
        uint64_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetComponentRenderModelName(const char *pchRenderModelName, const char *pchComponentName, VR_OUT_STRING( ) char *pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) override {
        FILE* f_GetComponentRenderModelName = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentRenderModelName) { fprintf(f_GetComponentRenderModelName, "Called: IVRRenderModels::GetComponentRenderModelName\n"); fclose(f_GetComponentRenderModelName); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual bool GetComponentStateForDevicePath(const char *pchRenderModelName, const char *pchComponentName, vr::VRInputValueHandle_t devicePath, const vr::RenderModel_ControllerMode_State_t *pState, vr::RenderModel_ComponentState_t *pComponentState) override {
        FILE* f_GetComponentStateForDevicePath = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentStateForDevicePath) { fprintf(f_GetComponentStateForDevicePath, "Called: IVRRenderModels::GetComponentStateForDevicePath\n"); fclose(f_GetComponentStateForDevicePath); }
        return false;
    }
    virtual bool GetComponentState(const char *pchRenderModelName, const char *pchComponentName, const vr::VRControllerState_t *pControllerState, const RenderModel_ControllerMode_State_t *pState, RenderModel_ComponentState_t *pComponentState) override {
        FILE* f_GetComponentState = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentState) { fprintf(f_GetComponentState, "Called: IVRRenderModels::GetComponentState\n"); fclose(f_GetComponentState); }
        return false;
    }
    virtual bool RenderModelHasComponent(const char *pchRenderModelName, const char *pchComponentName) override {
        FILE* f_RenderModelHasComponent = fopen("vr_emulator_log.txt", "a"); if(f_RenderModelHasComponent) { fprintf(f_RenderModelHasComponent, "Called: IVRRenderModels::RenderModelHasComponent\n"); fclose(f_RenderModelHasComponent); }
        return false;
    }
    virtual uint32_t GetRenderModelThumbnailURL(const char *pchRenderModelName, VR_OUT_STRING() char *pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError *peError) override {
        FILE* f_GetRenderModelThumbnailURL = fopen("vr_emulator_log.txt", "a"); if(f_GetRenderModelThumbnailURL) { fprintf(f_GetRenderModelThumbnailURL, "Called: IVRRenderModels::GetRenderModelThumbnailURL\n"); fclose(f_GetRenderModelThumbnailURL); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetRenderModelOriginalPath(const char *pchRenderModelName, VR_OUT_STRING() char *pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError *peError) override {
        FILE* f_GetRenderModelOriginalPath = fopen("vr_emulator_log.txt", "a"); if(f_GetRenderModelOriginalPath) { fprintf(f_GetRenderModelOriginalPath, "Called: IVRRenderModels::GetRenderModelOriginalPath\n"); fclose(f_GetRenderModelOriginalPath); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) override {
        FILE* f_GetRenderModelErrorNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetRenderModelErrorNameFromEnum) { fprintf(f_GetRenderModelErrorNameFromEnum, "Called: IVRRenderModels::GetRenderModelErrorNameFromEnum\n"); fclose(f_GetRenderModelErrorNameFromEnum); }
        return nullptr;
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
        FILE* f_GetWindowBounds = fopen("vr_emulator_log.txt", "a"); if(f_GetWindowBounds) { fprintf(f_GetWindowBounds, "Called: IVRExtendedDisplay::GetWindowBounds\n"); fclose(f_GetWindowBounds); }
        return;
    }
    virtual void GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* f_GetEyeOutputViewport = fopen("vr_emulator_log.txt", "a"); if(f_GetEyeOutputViewport) { fprintf(f_GetEyeOutputViewport, "Called: IVRExtendedDisplay::GetEyeOutputViewport\n"); fclose(f_GetEyeOutputViewport); }
        if(pnX) *pnX = 0; if(pnY) *pnY = 0; if(pnWidth) *pnWidth = 1920; if(pnHeight) *pnHeight = 1080;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex, int32_t *pnAdapterOutputIndex) override {
        FILE* f_GetDXGIOutputInfo = fopen("vr_emulator_log.txt", "a"); if(f_GetDXGIOutputInfo) { fprintf(f_GetDXGIOutputInfo, "Called: IVRExtendedDisplay::GetDXGIOutputInfo\n"); fclose(f_GetDXGIOutputInfo); }
        return;
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
        FILE* f_GetCameraErrorNameFromEnum = fopen("vr_emulator_log.txt", "a"); if(f_GetCameraErrorNameFromEnum) { fprintf(f_GetCameraErrorNameFromEnum, "Called: IVRTrackedCamera::GetCameraErrorNameFromEnum\n"); fclose(f_GetCameraErrorNameFromEnum); }
        return nullptr;
    }
    virtual vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool *pHasCamera) override {
        FILE* f_HasCamera = fopen("vr_emulator_log.txt", "a"); if(f_HasCamera) { fprintf(f_HasCamera, "Called: IVRTrackedCamera::HasCamera\n"); fclose(f_HasCamera); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t *pnWidth, uint32_t *pnHeight, uint32_t *pnFrameBufferSize) override {
        FILE* f_GetCameraFrameSize = fopen("vr_emulator_log.txt", "a"); if(f_GetCameraFrameSize) { fprintf(f_GetCameraFrameSize, "Called: IVRTrackedCamera::GetCameraFrameSize\n"); fclose(f_GetCameraFrameSize); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t *pFocalLength, vr::HmdVector2_t *pCenter) override {
        FILE* f_GetCameraIntrinsics = fopen("vr_emulator_log.txt", "a"); if(f_GetCameraIntrinsics) { fprintf(f_GetCameraIntrinsics, "Called: IVRTrackedCamera::GetCameraIntrinsics\n"); fclose(f_GetCameraIntrinsics); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t *pProjection) override {
        FILE* f_GetCameraProjection = fopen("vr_emulator_log.txt", "a"); if(f_GetCameraProjection) { fprintf(f_GetCameraProjection, "Called: IVRTrackedCamera::GetCameraProjection\n"); fclose(f_GetCameraProjection); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t *pHandle) override {
        FILE* f_AcquireVideoStreamingService = fopen("vr_emulator_log.txt", "a"); if(f_AcquireVideoStreamingService) { fprintf(f_AcquireVideoStreamingService, "Called: IVRTrackedCamera::AcquireVideoStreamingService\n"); fclose(f_AcquireVideoStreamingService); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera) override {
        FILE* f_ReleaseVideoStreamingService = fopen("vr_emulator_log.txt", "a"); if(f_ReleaseVideoStreamingService) { fprintf(f_ReleaseVideoStreamingService, "Called: IVRTrackedCamera::ReleaseVideoStreamingService\n"); fclose(f_ReleaseVideoStreamingService); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        FILE* f_GetVideoStreamFrameBuffer = fopen("vr_emulator_log.txt", "a"); if(f_GetVideoStreamFrameBuffer) { fprintf(f_GetVideoStreamFrameBuffer, "Called: IVRTrackedCamera::GetVideoStreamFrameBuffer\n"); fclose(f_GetVideoStreamFrameBuffer); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t *pTextureBounds, uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* f_GetVideoStreamTextureSize = fopen("vr_emulator_log.txt", "a"); if(f_GetVideoStreamTextureSize) { fprintf(f_GetVideoStreamTextureSize, "Called: IVRTrackedCamera::GetVideoStreamTextureSize\n"); fclose(f_GetVideoStreamTextureSize); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        FILE* f_GetVideoStreamTextureD3D11 = fopen("vr_emulator_log.txt", "a"); if(f_GetVideoStreamTextureD3D11) { fprintf(f_GetVideoStreamTextureD3D11, "Called: IVRTrackedCamera::GetVideoStreamTextureD3D11\n"); fclose(f_GetVideoStreamTextureD3D11); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t *pglTextureId, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        FILE* f_GetVideoStreamTextureGL = fopen("vr_emulator_log.txt", "a"); if(f_GetVideoStreamTextureGL) { fprintf(f_GetVideoStreamTextureGL, "Called: IVRTrackedCamera::GetVideoStreamTextureGL\n"); fclose(f_GetVideoStreamTextureGL); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId) override {
        FILE* f_ReleaseVideoStreamTextureGL = fopen("vr_emulator_log.txt", "a"); if(f_ReleaseVideoStreamTextureGL) { fprintf(f_ReleaseVideoStreamTextureGL, "Called: IVRTrackedCamera::ReleaseVideoStreamTextureGL\n"); fclose(f_ReleaseVideoStreamTextureGL); }
        return (vr::EVRTrackedCameraError)0;
    }
    virtual void SetCameraTrackingSpace(vr::ETrackingUniverseOrigin eUniverse) override {
        FILE* f_SetCameraTrackingSpace = fopen("vr_emulator_log.txt", "a"); if(f_SetCameraTrackingSpace) { fprintf(f_SetCameraTrackingSpace, "Called: IVRTrackedCamera::SetCameraTrackingSpace\n"); fclose(f_SetCameraTrackingSpace); }
        return;
    }
    virtual vr::ETrackingUniverseOrigin GetCameraTrackingSpace() override {
        FILE* f_GetCameraTrackingSpace = fopen("vr_emulator_log.txt", "a"); if(f_GetCameraTrackingSpace) { fprintf(f_GetCameraTrackingSpace, "Called: IVRTrackedCamera::GetCameraTrackingSpace\n"); fclose(f_GetCameraTrackingSpace); }
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
        FILE* f_RequestScreenshot = fopen("vr_emulator_log.txt", "a"); if(f_RequestScreenshot) { fprintf(f_RequestScreenshot, "Called: IVRScreenshots::RequestScreenshot\n"); fclose(f_RequestScreenshot); }
        return (vr::EVRScreenshotError)0;
    }
    virtual vr::EVRScreenshotError HookScreenshot(VR_ARRAY_COUNT( numTypes ) const vr::EVRScreenshotType *pSupportedTypes, int numTypes) override {
        FILE* f_HookScreenshot = fopen("vr_emulator_log.txt", "a"); if(f_HookScreenshot) { fprintf(f_HookScreenshot, "Called: IVRScreenshots::HookScreenshot\n"); fclose(f_HookScreenshot); }
        return (vr::EVRScreenshotError)0;
    }
    virtual vr::EVRScreenshotType GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError *pError) override {
        FILE* f_GetScreenshotPropertyType = fopen("vr_emulator_log.txt", "a"); if(f_GetScreenshotPropertyType) { fprintf(f_GetScreenshotPropertyType, "Called: IVRScreenshots::GetScreenshotPropertyType\n"); fclose(f_GetScreenshotPropertyType); }
        return (vr::EVRScreenshotType)0;
    }
    virtual uint32_t GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, VR_OUT_STRING() char *pchFilename, uint32_t cchFilename, vr::EVRScreenshotError *pError) override {
        FILE* f_GetScreenshotPropertyFilename = fopen("vr_emulator_log.txt", "a"); if(f_GetScreenshotPropertyFilename) { fprintf(f_GetScreenshotPropertyFilename, "Called: IVRScreenshots::GetScreenshotPropertyFilename\n"); fclose(f_GetScreenshotPropertyFilename); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual vr::EVRScreenshotError UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress) override {
        FILE* f_UpdateScreenshotProgress = fopen("vr_emulator_log.txt", "a"); if(f_UpdateScreenshotProgress) { fprintf(f_UpdateScreenshotProgress, "Called: IVRScreenshots::UpdateScreenshotProgress\n"); fclose(f_UpdateScreenshotProgress); }
        return (vr::EVRScreenshotError)0;
    }
    virtual vr::EVRScreenshotError TakeStereoScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, const char *pchPreviewFilename, const char *pchVRFilename) override {
        FILE* f_TakeStereoScreenshot = fopen("vr_emulator_log.txt", "a"); if(f_TakeStereoScreenshot) { fprintf(f_TakeStereoScreenshot, "Called: IVRScreenshots::TakeStereoScreenshot\n"); fclose(f_TakeStereoScreenshot); }
        return (vr::EVRScreenshotError)0;
    }
    virtual vr::EVRScreenshotError SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char *pchSourcePreviewFilename, const char *pchSourceVRFilename) override {
        FILE* f_SubmitScreenshot = fopen("vr_emulator_log.txt", "a"); if(f_SubmitScreenshot) { fprintf(f_SubmitScreenshot, "Called: IVRScreenshots::SubmitScreenshot\n"); fclose(f_SubmitScreenshot); }
        return (vr::EVRScreenshotError)0;
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
        FILE* f_LoadSharedResource = fopen("vr_emulator_log.txt", "a"); if(f_LoadSharedResource) { fprintf(f_LoadSharedResource, "Called: IVRResources::LoadSharedResource\n"); fclose(f_LoadSharedResource); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetResourceFullPath(const char *pchResourceName, const char *pchResourceTypeDirectory, VR_OUT_STRING() char *pchPathBuffer, uint32_t unBufferLen) override {
        FILE* f_GetResourceFullPath = fopen("vr_emulator_log.txt", "a"); if(f_GetResourceFullPath) { fprintf(f_GetResourceFullPath, "Called: IVRResources::GetResourceFullPath\n"); fclose(f_GetResourceFullPath); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
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
        FILE* f_GetDriverCount = fopen("vr_emulator_log.txt", "a"); if(f_GetDriverCount) { fprintf(f_GetDriverCount, "Called: IVRDriverManager::GetDriverCount\n"); fclose(f_GetDriverCount); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual uint32_t GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        FILE* f_GetDriverName = fopen("vr_emulator_log.txt", "a"); if(f_GetDriverName) { fprintf(f_GetDriverName, "Called: IVRDriverManager::GetDriverName\n"); fclose(f_GetDriverName); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual DriverHandle_t GetDriverHandle(const char *pchDriverName) override {
        FILE* f_GetDriverHandle = fopen("vr_emulator_log.txt", "a"); if(f_GetDriverHandle) { fprintf(f_GetDriverHandle, "Called: IVRDriverManager::GetDriverHandle\n"); fclose(f_GetDriverHandle); }
        DriverHandle_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual bool IsEnabled(vr::DriverId_t nDriver) const override {
        FILE* f_IsEnabled = fopen("vr_emulator_log.txt", "a"); if(f_IsEnabled) { fprintf(f_IsEnabled, "Called: IVRDriverManager::IsEnabled\n"); fclose(f_IsEnabled); }
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

class Mock_IVRInput : public vr::IVRInput {
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        FILE* f_SetActionManifestPath = fopen("vr_emulator_log.txt", "a"); if(f_SetActionManifestPath) { fprintf(f_SetActionManifestPath, "Called: IVRInput::SetActionManifestPath\n"); fclose(f_SetActionManifestPath); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        FILE* f_GetActionSetHandle = fopen("vr_emulator_log.txt", "a"); if(f_GetActionSetHandle) { fprintf(f_GetActionSetHandle, "Called: IVRInput::GetActionSetHandle\n"); fclose(f_GetActionSetHandle); }
        static uint64_t nextSetHandle = 1000;
        if(pHandle) *pHandle = nextSetHandle++;
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        FILE* f_GetActionHandle = fopen("vr_emulator_log.txt", "a"); if(f_GetActionHandle) { fprintf(f_GetActionHandle, "Called: IVRInput::GetActionHandle\n"); fclose(f_GetActionHandle); }
        static uint64_t nextHandle = 10;
        if(pHandle) { *pHandle = nextHandle++; }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        FILE* f_GetInputSourceHandle = fopen("vr_emulator_log.txt", "a"); if(f_GetInputSourceHandle) { fprintf(f_GetInputSourceHandle, "Called: IVRInput::GetInputSourceHandle\n"); fclose(f_GetInputSourceHandle); }
        static uint64_t nextSourceHandle = 10000;
        if(pHandle) { *pHandle = nextSourceHandle++; if(pchInputSourcePath) { if(strstr(pchInputSourcePath, "left")) *pHandle = 1; else if(strstr(pchInputSourcePath, "right")) *pHandle = 2; } }
        return vr::VRInputError_None;
    }
    virtual EVRInputError UpdateActionState(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount) override {
        FILE* f_UpdateActionState = fopen("vr_emulator_log.txt", "a"); if(f_UpdateActionState) { fprintf(f_UpdateActionState, "Called: IVRInput::UpdateActionState\n"); fclose(f_UpdateActionState); }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* f_GetDigitalActionData = fopen("vr_emulator_log.txt", "a"); if(f_GetDigitalActionData) { fprintf(f_GetDigitalActionData, "Called: IVRInput::GetDigitalActionData\n"); fclose(f_GetDigitalActionData); }
        if(pActionData && unActionDataSize > 0) {
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
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* f_GetAnalogActionData = fopen("vr_emulator_log.txt", "a"); if(f_GetAnalogActionData) { fprintf(f_GetAnalogActionData, "Called: IVRInput::GetAnalogActionData\n"); fclose(f_GetAnalogActionData); }
        if(pActionData && unActionDataSize > 0) { vr::InputAnalogActionData_t temp = {0}; temp.bActive = true; memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize); }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataRelativeToNow(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* f_GetPoseActionDataRelativeToNow = fopen("vr_emulator_log.txt", "a"); if(f_GetPoseActionDataRelativeToNow) { fprintf(f_GetPoseActionDataRelativeToNow, "Called: IVRInput::GetPoseActionDataRelativeToNow\n"); fclose(f_GetPoseActionDataRelativeToNow); }
        if(pActionData && unActionDataSize > 0) {
            vr::InputPoseActionData_t temp = {0};
            temp.bActive = true;
            temp.pose.bPoseIsValid = true;
            temp.pose.bDeviceIsConnected = true;
            temp.pose.eTrackingResult = vr::TrackingResult_Running_OK;
            temp.pose.mDeviceToAbsoluteTracking.m[0][0] = 1; temp.pose.mDeviceToAbsoluteTracking.m[1][1] = 1; temp.pose.mDeviceToAbsoluteTracking.m[2][2] = 1;
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* f_GetPoseActionDataForNextFrame = fopen("vr_emulator_log.txt", "a"); if(f_GetPoseActionDataForNextFrame) { fprintf(f_GetPoseActionDataForNextFrame, "Called: IVRInput::GetPoseActionDataForNextFrame\n"); fclose(f_GetPoseActionDataForNextFrame); }
        if(pActionData && unActionDataSize > 0) {
            vr::InputPoseActionData_t temp = {0};
            temp.bActive = true;
            temp.pose.bPoseIsValid = true;
            temp.pose.bDeviceIsConnected = true;
            temp.pose.eTrackingResult = vr::TrackingResult_Running_OK;
            temp.pose.mDeviceToAbsoluteTracking.m[0][0] = 1; temp.pose.mDeviceToAbsoluteTracking.m[1][1] = 1; temp.pose.mDeviceToAbsoluteTracking.m[2][2] = 1;
            memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize);
        }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetSkeletalActionData(VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize) override {
        FILE* f_GetSkeletalActionData = fopen("vr_emulator_log.txt", "a"); if(f_GetSkeletalActionData) { fprintf(f_GetSkeletalActionData, "Called: IVRInput::GetSkeletalActionData\n"); fclose(f_GetSkeletalActionData); }
        if(pActionData && unActionDataSize > 0) { vr::InputSkeletalActionData_t temp = {0}; temp.bActive = false; memcpy(pActionData, &temp, unActionDataSize > sizeof(temp) ? sizeof(temp) : unActionDataSize); }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetDominantHand(ETrackedControllerRole *peDominantHand) override {
        FILE* f_GetDominantHand = fopen("vr_emulator_log.txt", "a"); if(f_GetDominantHand) { fprintf(f_GetDominantHand, "Called: IVRInput::GetDominantHand\n"); fclose(f_GetDominantHand); }
        return (EVRInputError)0;
    }
    virtual EVRInputError SetDominantHand(ETrackedControllerRole eDominantHand) override {
        FILE* f_SetDominantHand = fopen("vr_emulator_log.txt", "a"); if(f_SetDominantHand) { fprintf(f_SetDominantHand, "Called: IVRInput::SetDominantHand\n"); fclose(f_SetDominantHand); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetBoneCount(VRActionHandle_t action, uint32_t* pBoneCount) override {
        FILE* f_GetBoneCount = fopen("vr_emulator_log.txt", "a"); if(f_GetBoneCount) { fprintf(f_GetBoneCount, "Called: IVRInput::GetBoneCount\n"); fclose(f_GetBoneCount); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetBoneHierarchy(VRActionHandle_t action, VR_ARRAY_COUNT( unIndexArayCount ) BoneIndex_t* pParentIndices, uint32_t unIndexArayCount) override {
        FILE* f_GetBoneHierarchy = fopen("vr_emulator_log.txt", "a"); if(f_GetBoneHierarchy) { fprintf(f_GetBoneHierarchy, "Called: IVRInput::GetBoneHierarchy\n"); fclose(f_GetBoneHierarchy); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetBoneName(VRActionHandle_t action, BoneIndex_t nBoneIndex, VR_OUT_STRING() char* pchBoneName, uint32_t unNameBufferSize) override {
        FILE* f_GetBoneName = fopen("vr_emulator_log.txt", "a"); if(f_GetBoneName) { fprintf(f_GetBoneName, "Called: IVRInput::GetBoneName\n"); fclose(f_GetBoneName); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetSkeletalReferenceTransforms(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalReferencePose eReferencePose, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        FILE* f_GetSkeletalReferenceTransforms = fopen("vr_emulator_log.txt", "a"); if(f_GetSkeletalReferenceTransforms) { fprintf(f_GetSkeletalReferenceTransforms, "Called: IVRInput::GetSkeletalReferenceTransforms\n"); fclose(f_GetSkeletalReferenceTransforms); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetSkeletalTrackingLevel(VRActionHandle_t action, EVRSkeletalTrackingLevel* pSkeletalTrackingLevel) override {
        FILE* f_GetSkeletalTrackingLevel = fopen("vr_emulator_log.txt", "a"); if(f_GetSkeletalTrackingLevel) { fprintf(f_GetSkeletalTrackingLevel, "Called: IVRInput::GetSkeletalTrackingLevel\n"); fclose(f_GetSkeletalTrackingLevel); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetSkeletalBoneData(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalMotionRange eMotionRange, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        FILE* f_GetSkeletalBoneData = fopen("vr_emulator_log.txt", "a"); if(f_GetSkeletalBoneData) { fprintf(f_GetSkeletalBoneData, "Called: IVRInput::GetSkeletalBoneData\n"); fclose(f_GetSkeletalBoneData); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetSkeletalSummaryData(VRActionHandle_t action, EVRSummaryType eSummaryType, VRSkeletalSummaryData_t * pSkeletalSummaryData) override {
        FILE* f_GetSkeletalSummaryData = fopen("vr_emulator_log.txt", "a"); if(f_GetSkeletalSummaryData) { fprintf(f_GetSkeletalSummaryData, "Called: IVRInput::GetSkeletalSummaryData\n"); fclose(f_GetSkeletalSummaryData); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetSkeletalBoneDataCompressed(VRActionHandle_t action, EVRSkeletalMotionRange eMotionRange, VR_OUT_BUFFER_COUNT( unCompressedSize ) void *pvCompressedData, uint32_t unCompressedSize, uint32_t *punRequiredCompressedSize) override {
        FILE* f_GetSkeletalBoneDataCompressed = fopen("vr_emulator_log.txt", "a"); if(f_GetSkeletalBoneDataCompressed) { fprintf(f_GetSkeletalBoneDataCompressed, "Called: IVRInput::GetSkeletalBoneDataCompressed\n"); fclose(f_GetSkeletalBoneDataCompressed); }
        return (EVRInputError)0;
    }
    virtual EVRInputError DecompressSkeletalBoneData(const void *pvCompressedBuffer, uint32_t unCompressedBufferSize, EVRSkeletalTransformSpace eTransformSpace, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        FILE* f_DecompressSkeletalBoneData = fopen("vr_emulator_log.txt", "a"); if(f_DecompressSkeletalBoneData) { fprintf(f_DecompressSkeletalBoneData, "Called: IVRInput::DecompressSkeletalBoneData\n"); fclose(f_DecompressSkeletalBoneData); }
        return (EVRInputError)0;
    }
    virtual EVRInputError TriggerHapticVibrationAction(VRActionHandle_t action, float fStartSecondsFromNow, float fDurationSeconds, float fFrequency, float fAmplitude, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* f_TriggerHapticVibrationAction = fopen("vr_emulator_log.txt", "a"); if(f_TriggerHapticVibrationAction) { fprintf(f_TriggerHapticVibrationAction, "Called: IVRInput::TriggerHapticVibrationAction\n"); fclose(f_TriggerHapticVibrationAction); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t digitalActionHandle, VR_ARRAY_COUNT( originOutCount ) VRInputValueHandle_t *originsOut, uint32_t originOutCount) override {
        FILE* f_GetActionOrigins = fopen("vr_emulator_log.txt", "a"); if(f_GetActionOrigins) { fprintf(f_GetActionOrigins, "Called: IVRInput::GetActionOrigins\n"); fclose(f_GetActionOrigins); }
        if(originsOut && originOutCount > 0) { memset(originsOut, 0, sizeof(vr::VRInputValueHandle_t) * originOutCount); originsOut[0] = actionSetHandle; }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetOriginLocalizedName(VRInputValueHandle_t origin, VR_OUT_STRING() char *pchNameArray, uint32_t unNameArraySize, int32_t unStringSectionsToInclude) override {
        FILE* f_GetOriginLocalizedName = fopen("vr_emulator_log.txt", "a"); if(f_GetOriginLocalizedName) { fprintf(f_GetOriginLocalizedName, "Called: IVRInput::GetOriginLocalizedName\n"); fclose(f_GetOriginLocalizedName); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetOriginTrackedDeviceInfo(VRInputValueHandle_t origin, InputOriginInfo_t *pOriginInfo, uint32_t unOriginInfoSize) override {
        FILE* f_GetOriginTrackedDeviceInfo = fopen("vr_emulator_log.txt", "a"); if(f_GetOriginTrackedDeviceInfo) { fprintf(f_GetOriginTrackedDeviceInfo, "Called: IVRInput::GetOriginTrackedDeviceInfo\n"); fclose(f_GetOriginTrackedDeviceInfo); }
        if(pOriginInfo && unOriginInfoSize > 0) { vr::InputOriginInfo_t temp = {0}; temp.devicePath = origin; temp.trackedDeviceIndex = (origin == 1) ? 1 : 2; memcpy(pOriginInfo, &temp, unOriginInfoSize > sizeof(temp) ? sizeof(temp) : unOriginInfoSize); }
        return vr::VRInputError_None;
    }
    virtual EVRInputError GetActionBindingInfo(VRActionHandle_t action, InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, uint32_t *punReturnedBindingInfoCount) override {
        FILE* f_GetActionBindingInfo = fopen("vr_emulator_log.txt", "a"); if(f_GetActionBindingInfo) { fprintf(f_GetActionBindingInfo, "Called: IVRInput::GetActionBindingInfo\n"); fclose(f_GetActionBindingInfo); }
        if(pOriginInfo && unBindingInfoSize > 0 && unBindingInfoCount > 0) { memset(pOriginInfo, 0, unBindingInfoSize * unBindingInfoCount); }
        if(punReturnedBindingInfoCount) *punReturnedBindingInfoCount = 0;
        return vr::VRInputError_None;
    }
    virtual EVRInputError ShowActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t ulActionHandle) override {
        FILE* f_ShowActionOrigins = fopen("vr_emulator_log.txt", "a"); if(f_ShowActionOrigins) { fprintf(f_ShowActionOrigins, "Called: IVRInput::ShowActionOrigins\n"); fclose(f_ShowActionOrigins); }
        return (EVRInputError)0;
    }
    virtual EVRInputError ShowBindingsForActionSet(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount, VRInputValueHandle_t originToHighlight) override {
        FILE* f_ShowBindingsForActionSet = fopen("vr_emulator_log.txt", "a"); if(f_ShowBindingsForActionSet) { fprintf(f_ShowBindingsForActionSet, "Called: IVRInput::ShowBindingsForActionSet\n"); fclose(f_ShowBindingsForActionSet); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetComponentStateForBinding(const char *pchRenderModelName, const char *pchComponentName, const InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, vr::RenderModel_ComponentState_t *pComponentState) override {
        FILE* f_GetComponentStateForBinding = fopen("vr_emulator_log.txt", "a"); if(f_GetComponentStateForBinding) { fprintf(f_GetComponentStateForBinding, "Called: IVRInput::GetComponentStateForBinding\n"); fclose(f_GetComponentStateForBinding); }
        return (EVRInputError)0;
    }
    virtual bool IsUsingLegacyInput() override {
        FILE* f_IsUsingLegacyInput = fopen("vr_emulator_log.txt", "a"); if(f_IsUsingLegacyInput) { fprintf(f_IsUsingLegacyInput, "Called: IVRInput::IsUsingLegacyInput\n"); fclose(f_IsUsingLegacyInput); }
        return false;
    }
    virtual EVRInputError OpenBindingUI(const char* pchAppKey, VRActionSetHandle_t ulActionSetHandle, VRInputValueHandle_t ulDeviceHandle, bool bShowOnDesktop) override {
        FILE* f_OpenBindingUI = fopen("vr_emulator_log.txt", "a"); if(f_OpenBindingUI) { fprintf(f_OpenBindingUI, "Called: IVRInput::OpenBindingUI\n"); fclose(f_OpenBindingUI); }
        return (EVRInputError)0;
    }
    virtual EVRInputError GetBindingVariant(vr::VRInputValueHandle_t ulDevicePath, VR_OUT_STRING() char *pchVariantArray, uint32_t unVariantArraySize) override {
        FILE* f_GetBindingVariant = fopen("vr_emulator_log.txt", "a"); if(f_GetBindingVariant) { fprintf(f_GetBindingVariant, "Called: IVRInput::GetBindingVariant\n"); fclose(f_GetBindingVariant); }
        return (EVRInputError)0;
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
        FILE* f_Open = fopen("vr_emulator_log.txt", "a"); if(f_Open) { fprintf(f_Open, "Called: IVRIOBuffer::Open\n"); fclose(f_Open); }
        return (vr::EIOBufferError)0;
    }
    virtual vr::EIOBufferError Close(vr::IOBufferHandle_t ulBuffer) override {
        FILE* f_Close = fopen("vr_emulator_log.txt", "a"); if(f_Close) { fprintf(f_Close, "Called: IVRIOBuffer::Close\n"); fclose(f_Close); }
        return (vr::EIOBufferError)0;
    }
    virtual vr::EIOBufferError Read(vr::IOBufferHandle_t ulBuffer, void *pDst, uint32_t unBytes, uint32_t *punRead) override {
        FILE* f_Read = fopen("vr_emulator_log.txt", "a"); if(f_Read) { fprintf(f_Read, "Called: IVRIOBuffer::Read\n"); fclose(f_Read); }
        return (vr::EIOBufferError)0;
    }
    virtual vr::EIOBufferError Write(vr::IOBufferHandle_t ulBuffer, void *pSrc, uint32_t unBytes) override {
        FILE* f_Write = fopen("vr_emulator_log.txt", "a"); if(f_Write) { fprintf(f_Write, "Called: IVRIOBuffer::Write\n"); fclose(f_Write); }
        return (vr::EIOBufferError)0;
    }
    virtual vr::PropertyContainerHandle_t PropertyContainer(vr::IOBufferHandle_t ulBuffer) override {
        FILE* f_PropertyContainer = fopen("vr_emulator_log.txt", "a"); if(f_PropertyContainer) { fprintf(f_PropertyContainer, "Called: IVRIOBuffer::PropertyContainer\n"); fclose(f_PropertyContainer); }
        vr::PropertyContainerHandle_t temp; memset(&temp, 0, sizeof(temp)); return temp;
    }
    virtual bool HasReaders(vr::IOBufferHandle_t ulBuffer) override {
        FILE* f_HasReaders = fopen("vr_emulator_log.txt", "a"); if(f_HasReaders) { fprintf(f_HasReaders, "Called: IVRIOBuffer::HasReaders\n"); fclose(f_HasReaders); }
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
        FILE* f_CreateSpatialAnchorFromDescriptor = fopen("vr_emulator_log.txt", "a"); if(f_CreateSpatialAnchorFromDescriptor) { fprintf(f_CreateSpatialAnchorFromDescriptor, "Called: IVRSpatialAnchors::CreateSpatialAnchorFromDescriptor\n"); fclose(f_CreateSpatialAnchorFromDescriptor); }
        return (EVRSpatialAnchorError)0;
    }
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromPose(TrackedDeviceIndex_t unDeviceIndex, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPose, SpatialAnchorHandle_t *pHandleOut) override {
        FILE* f_CreateSpatialAnchorFromPose = fopen("vr_emulator_log.txt", "a"); if(f_CreateSpatialAnchorFromPose) { fprintf(f_CreateSpatialAnchorFromPose, "Called: IVRSpatialAnchors::CreateSpatialAnchorFromPose\n"); fclose(f_CreateSpatialAnchorFromPose); }
        return (EVRSpatialAnchorError)0;
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorPose(SpatialAnchorHandle_t unHandle, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPoseOut) override {
        FILE* f_GetSpatialAnchorPose = fopen("vr_emulator_log.txt", "a"); if(f_GetSpatialAnchorPose) { fprintf(f_GetSpatialAnchorPose, "Called: IVRSpatialAnchors::GetSpatialAnchorPose\n"); fclose(f_GetSpatialAnchorPose); }
        return (EVRSpatialAnchorError)0;
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorDescriptor(SpatialAnchorHandle_t unHandle, VR_OUT_STRING() char *pchDescriptorOut, uint32_t *punDescriptorBufferLenInOut) override {
        FILE* f_GetSpatialAnchorDescriptor = fopen("vr_emulator_log.txt", "a"); if(f_GetSpatialAnchorDescriptor) { fprintf(f_GetSpatialAnchorDescriptor, "Called: IVRSpatialAnchors::GetSpatialAnchorDescriptor\n"); fclose(f_GetSpatialAnchorDescriptor); }
        return (EVRSpatialAnchorError)0;
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
        FILE* f_EmitVrProfilerEvent = fopen("vr_emulator_log.txt", "a"); if(f_EmitVrProfilerEvent) { fprintf(f_EmitVrProfilerEvent, "Called: IVRDebug::EmitVrProfilerEvent\n"); fclose(f_EmitVrProfilerEvent); }
        return (EVRDebugError)0;
    }
    virtual EVRDebugError BeginVrProfilerEvent(VrProfilerEventHandle_t *pHandleOut) override {
        FILE* f_BeginVrProfilerEvent = fopen("vr_emulator_log.txt", "a"); if(f_BeginVrProfilerEvent) { fprintf(f_BeginVrProfilerEvent, "Called: IVRDebug::BeginVrProfilerEvent\n"); fclose(f_BeginVrProfilerEvent); }
        return (EVRDebugError)0;
    }
    virtual EVRDebugError FinishVrProfilerEvent(VrProfilerEventHandle_t hHandle, const char *pchMessage) override {
        FILE* f_FinishVrProfilerEvent = fopen("vr_emulator_log.txt", "a"); if(f_FinishVrProfilerEvent) { fprintf(f_FinishVrProfilerEvent, "Called: IVRDebug::FinishVrProfilerEvent\n"); fclose(f_FinishVrProfilerEvent); }
        return (EVRDebugError)0;
    }
    virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, VR_OUT_STRING() char *pchResponseBuffer, uint32_t unResponseBufferSize) override {
        FILE* f_DriverDebugRequest = fopen("vr_emulator_log.txt", "a"); if(f_DriverDebugRequest) { fprintf(f_DriverDebugRequest, "Called: IVRDebug::DriverDebugRequest\n"); fclose(f_DriverDebugRequest); }
        uint32_t temp; memset(&temp, 0, sizeof(temp)); return temp;
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
    
    FILE* f = fopen("vr_emulator_log.txt", "a");
    if(f) { fprintf(f, "VR_GetGenericInterface: %s\n", pchInterfaceVersion); fclose(f); }

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
        if(f2) { fprintf(f2, " -> Returned REAL\n"); fclose(f2); }
        return pReal;
    }
    if (strstr(pchInterfaceVersion, "System")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK System\n"); fclose(f3); } return &g_mockSystem; }
    if (strstr(pchInterfaceVersion, "Applications")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Applications\n"); fclose(f3); } return &g_mockApplications; }
    if (strstr(pchInterfaceVersion, "Settings")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Settings\n"); fclose(f3); } return &g_mockSettings; }
    if (strstr(pchInterfaceVersion, "Chaperone")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Chaperone\n"); fclose(f3); } return &g_mockChaperone; }
    if (strstr(pchInterfaceVersion, "ChaperoneSetup")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK ChaperoneSetup\n"); fclose(f3); } return &g_mockChaperoneSetup; }
    if (strstr(pchInterfaceVersion, "Compositor")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Compositor\n"); fclose(f3); } return &g_mockCompositor; }
    if (strstr(pchInterfaceVersion, "HeadsetView")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK HeadsetView\n"); fclose(f3); } return &g_mockHeadsetView; }
    if (strstr(pchInterfaceVersion, "Notifications")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Notifications\n"); fclose(f3); } return &g_mockNotifications; }
    if (strstr(pchInterfaceVersion, "Overlay")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Overlay\n"); fclose(f3); } return &g_mockOverlay; }
    if (strstr(pchInterfaceVersion, "OverlayView")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK OverlayView\n"); fclose(f3); } return &g_mockOverlayView; }
    if (strstr(pchInterfaceVersion, "RenderModels")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK RenderModels\n"); fclose(f3); } return &g_mockRenderModels; }
    if (strstr(pchInterfaceVersion, "ExtendedDisplay")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK ExtendedDisplay\n"); fclose(f3); } return &g_mockExtendedDisplay; }
    if (strstr(pchInterfaceVersion, "TrackedCamera")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK TrackedCamera\n"); fclose(f3); } return &g_mockTrackedCamera; }
    if (strstr(pchInterfaceVersion, "Screenshots")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Screenshots\n"); fclose(f3); } return &g_mockScreenshots; }
    if (strstr(pchInterfaceVersion, "Resources")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Resources\n"); fclose(f3); } return &g_mockResources; }
    if (strstr(pchInterfaceVersion, "DriverManager")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK DriverManager\n"); fclose(f3); } return &g_mockDriverManager; }
    if (strstr(pchInterfaceVersion, "Input")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Input\n"); fclose(f3); } return &g_mockInput; }
    if (strstr(pchInterfaceVersion, "IOBuffer")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK IOBuffer\n"); fclose(f3); } return &g_mockIOBuffer; }
    if (strstr(pchInterfaceVersion, "SpatialAnchors")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK SpatialAnchors\n"); fclose(f3); } return &g_mockSpatialAnchors; }
    if (strstr(pchInterfaceVersion, "Debug")) { FILE* f3 = fopen("vr_emulator_log.txt", "a"); if(f3) { fprintf(f3, " -> Returned MOCK Debug\n"); fclose(f3); } return &g_mockDebug; }

    FILE* f4 = fopen("vr_emulator_log.txt", "a");
    if(f4) { fprintf(f4, " -> Returned NULL/UNIVERSAL MOCK\n"); fclose(f4); }
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
    if (pchPathBuffer && unBufferSize > 0) pchPathBuffer[0] = '\0';
    return true;
}

extern "C" __declspec(dllexport) const char *VR_GetStringForHmdError(vr::EVRInitError error) {
    return "Unknown Error";
}

extern "C" __declspec(dllexport) void* VRControlPanel() { return nullptr; }
