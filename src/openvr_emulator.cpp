#include "openvr.h"
#include <windows.h>
#include <iostream>
#include <string.h>

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

class Mock_IVRSystem : public vr::IVRSystem {
public:
    virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        if(pnWidth) *pnWidth = 1920;
        if(pnHeight) *pnHeight = 1080;
    }
    virtual void GetProjectionMatrix(HmdMatrix44_t* pRet, EVREye eEye, float fNearZ, float fFarZ) override {
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; pRet->m[3][3] = 1; }
    }
    virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) override {
        if(pfLeft) *pfLeft = -1.0f; if(pfRight) *pfRight = 1.0f; if(pfTop) *pfTop = -1.0f; if(pfBottom) *pfBottom = 1.0f;
    }
    virtual bool ComputeDistortion(EVREye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates) override {
        if(pDistortionCoordinates) { memset(pDistortionCoordinates, 0, sizeof(*pDistortionCoordinates)); }
        return true;
    }
    virtual bool ComputeDistortionSet(EVREye eEye, EVRDistortionChannel eChannel, bool bAsNormalizedDeviceCoordinates, uint32_t nNumCoordinates, const DistortionCoordinate_t *pInput, DistortionCoordinate_t *pOutput) override {
        return false;
    }
    virtual void GetEyeToHeadTransform(HmdMatrix34_t* pRet, EVREye eEye) override {
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; }
    }
    virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override {
        if(pfSecondsSinceLastVsync) *pfSecondsSinceLastVsync = 0.0f; if(pulFrameCounter) *pulFrameCounter = 0;
        return true;
    }
    virtual int32_t GetD3D9AdapterIndex() override {
        return (int32_t)0;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex) override {
        if(pnAdapterIndex) *pnAdapterIndex = 0;
    }
    virtual void GetOutputDevice(uint64_t *pnDevice, ETextureType textureType, VkInstance_T *pInstance = nullptr) override {
    }
    virtual bool IsDisplayOnDesktop() override {
        return false;
    }
    virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) override {
        return false;
    }
    virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override {
        if(pTrackedDevicePoseArray && unTrackedDevicePoseArrayCount > 0) { memset(pTrackedDevicePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unTrackedDevicePoseArrayCount); if(unTrackedDevicePoseArrayCount > 0) { pTrackedDevicePoseArray[0].bPoseIsValid = true; pTrackedDevicePoseArray[0].bDeviceIsConnected = true; pTrackedDevicePoseArray[0].eTrackingResult = vr::TrackingResult_Running_OK; pTrackedDevicePoseArray[0].mDeviceToAbsoluteTracking.m[0][0] = 1; pTrackedDevicePoseArray[0].mDeviceToAbsoluteTracking.m[1][1] = 1; pTrackedDevicePoseArray[0].mDeviceToAbsoluteTracking.m[2][2] = 1; } }
    }
    virtual void GetSeatedZeroPoseToStandingAbsoluteTrackingPose(HmdMatrix34_t* pRet) override {
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { ((vr::HmdMatrix34_t*)pRet)->m[0][0] = 1; ((vr::HmdMatrix34_t*)pRet)->m[1][1] = 1; ((vr::HmdMatrix34_t*)pRet)->m[2][2] = 1; }
    }
    virtual void GetRawZeroPoseToStandingAbsoluteTrackingPose(HmdMatrix34_t* pRet) override {
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
        if(pRet) { ((vr::HmdMatrix34_t*)pRet)->m[0][0] = 1; ((vr::HmdMatrix34_t*)pRet)->m[1][1] = 1; ((vr::HmdMatrix34_t*)pRet)->m[2][2] = 1; }
    }
    virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd) override {
        return (uint32_t)0;
    }
    virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) override {
        return (EDeviceActivityLevel)0;
    }
    virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform) override {
    }
    virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) override {
        return (vr::TrackedDeviceIndex_t)0;
    }
    virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        return (vr::ETrackedControllerRole)0;
    }
    virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        if(unDeviceIndex == 0) return vr::TrackedDeviceClass_HMD;
        return vr::TrackedDeviceClass_Invalid;
    }
    virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        if(unDeviceIndex == 0) return true;
        return false;
    }
    virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        return false;
    }
    virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        if(pError) *pError = vr::TrackedProp_Success;
        if(prop == vr::Prop_DisplayFrequency_Float) return 90.0f;
        return 0.0f;
    }
    virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        if(pError) *pError = vr::TrackedProp_Success;
        if(prop == vr::Prop_DeviceClass_Int32) return vr::TrackedDeviceClass_HMD;
        return 0;
    }
    virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        if(pError) *pError = vr::TrackedProp_Success;
        if(prop == vr::Prop_CurrentUniverseId_Uint64) return 1;
        return 0;
    }
    virtual void GetMatrix34TrackedDeviceProperty(HmdMatrix34_t* pRet, vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) override {
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); pRet->m[0][0] = 1; pRet->m[1][1] = 1; pRet->m[2][2] = 1; }
        if(pError) *pError = vr::TrackedProp_Success;
    }
    virtual uint32_t GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) override {
        const char* s = "Generic"; if(pchValue && unBufferSize > 0) { strncpy(pchValue, s, unBufferSize - 1); pchValue[unBufferSize - 1] = '\0'; }
        if(pError) *pError = vr::TrackedProp_Success;
        return (uint32_t)strlen(s) + 1;
    }
    virtual const char * GetPropErrorNameFromEnum(ETrackedPropertyError error) override {
        return nullptr;
    }
    virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) override {
        static int count = 0; if (count == 0) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)100; pEvent->trackedDeviceIndex = 0; } return true; } else if (count == 1) { count++; if(pEvent) { memset(pEvent, 0, uncbVREvent); pEvent->eventType = (vr::EVREventType)403; pEvent->trackedDeviceIndex = 0; } return true; } return false;
    }
    virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose) override {
        return false;
    }
    virtual bool PollNextEventWithPoseAndOverlays(vr::ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, TrackedDevicePose_t *pTrackedDevicePose, VROverlayHandle_t *pulOverlayHandle) override {
        return false;
    }
    virtual const char * GetEventTypeNameFromEnum(EVREventType eType) override {
        return nullptr;
    }
    virtual void GetHiddenAreaMesh(HiddenAreaMesh_t* pRet, EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard) override {
        if(pRet) { pRet->pVertexData = nullptr; pRet->unTriangleCount = 0; }
    }
    virtual bool GetEyeTrackedFoveationCenter(HmdVector2_t *pNdcLeft, HmdVector2_t *pNdcRight) override {
        return false;
    }
    virtual bool GetEyeTrackedFoveationCenterForProjection(const HmdMatrix44_t *pProjMat, HmdVector2_t *pNdc) override {
        return false;
    }
    virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize) override {
        return false;
    }
    virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose) override {
        return false;
    }
    virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) override {
    }
    virtual const char * GetButtonIdNameFromEnum(EVRButtonId eButtonId) override {
        return nullptr;
    }
    virtual const char * GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) override {
        return nullptr;
    }
    virtual bool IsInputAvailable() override {
        return true;
    }
    virtual bool IsSteamVRDrawingControllers() override {
        return false;
    }
    virtual bool ShouldApplicationPause() override {
        return false;
    }
    virtual bool ShouldApplicationReduceRenderingWork() override {
        return false;
    }
    virtual vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        return (vr::EVRFirmwareError)1;
    }
    virtual void AcknowledgeQuit_Exiting() override {
    }
    virtual uint32_t GetAppContainerFilePaths(VR_OUT_STRING() char *pchBuffer, uint32_t unBufferSize) override {
        return (uint32_t)0;
    }
    virtual const char * GetRuntimeVersion() override {
        return nullptr;
    }
    virtual vr::EVRInitError SetSDKVersion(uint32_t nVersionMajor, uint32_t nVersionMinor, uint32_t nVersionBuild) override {
        return (vr::EVRInitError)1;
    }
} g_mockSystem;

class Mock_IVRApplications : public vr::IVRApplications {
public:
    virtual EVRApplicationError AddApplicationManifest(const char *pchApplicationManifestFullPath, bool bTemporary = false) override {
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError RemoveApplicationManifest(const char *pchApplicationManifestFullPath) override {
        return (EVRApplicationError)1;
    }
    virtual bool IsApplicationInstalled(const char *pchAppKey) override {
        return false;
    }
    virtual uint32_t GetApplicationCount() override {
        return (uint32_t)0;
    }
    virtual EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError LaunchApplication(const char *pchAppKey) override {
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError LaunchTemplateApplication(const char *pchTemplateAppKey, const char *pchNewAppKey, VR_ARRAY_COUNT( unKeys ) const AppOverrideKeys_t *pKeys, uint32_t unKeys) override {
        return (EVRApplicationError)1;
    }
    virtual vr::EVRApplicationError LaunchApplicationFromMimeType(const char *pchMimeType, const char *pchArgs) override {
        return (vr::EVRApplicationError)1;
    }
    virtual EVRApplicationError LaunchDashboardOverlay(const char *pchAppKey) override {
        return (EVRApplicationError)1;
    }
    virtual bool CancelApplicationLaunch(const char *pchAppKey) override {
        return false;
    }
    virtual EVRApplicationError IdentifyApplication(uint32_t unProcessId, const char *pchAppKey) override {
        return (EVRApplicationError)1;
    }
    virtual uint32_t GetApplicationProcessId(const char *pchAppKey) override {
        return (uint32_t)0;
    }
    virtual const char * GetApplicationsErrorNameFromEnum(EVRApplicationError error) override {
        return nullptr;
    }
    virtual uint32_t GetApplicationPropertyString(const char *pchAppKey, EVRApplicationProperty eProperty, VR_OUT_STRING() char *pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, EVRApplicationError *peError = nullptr) override {
        return (uint32_t)0;
    }
    virtual bool GetApplicationPropertyBool(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr) override {
        return false;
    }
    virtual uint64_t GetApplicationPropertyUint64(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr) override {
        return (uint64_t)0;
    }
    virtual EVRApplicationError SetApplicationAutoLaunch(const char *pchAppKey, bool bAutoLaunch) override {
        return (EVRApplicationError)1;
    }
    virtual bool GetApplicationAutoLaunch(const char *pchAppKey) override {
        return false;
    }
    virtual EVRApplicationError SetDefaultApplicationForMimeType(const char *pchAppKey, const char *pchMimeType) override {
        return (EVRApplicationError)1;
    }
    virtual bool GetDefaultApplicationForMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        return false;
    }
    virtual bool GetApplicationSupportedMimeTypes(const char *pchAppKey, VR_OUT_STRING() char *pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) override {
        return false;
    }
    virtual uint32_t GetApplicationsThatSupportMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetApplicationLaunchArguments(uint32_t unHandle, VR_OUT_STRING() char *pchArgs, uint32_t unArgs) override {
        return (uint32_t)0;
    }
    virtual EVRApplicationError GetStartingApplication(VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        return (EVRApplicationError)1;
    }
    virtual EVRSceneApplicationState GetSceneApplicationState() override {
        return (EVRSceneApplicationState)0;
    }
    virtual EVRApplicationError PerformApplicationPrelaunchCheck(const char *pchAppKey) override {
        return (EVRApplicationError)1;
    }
    virtual const char * GetSceneApplicationStateNameFromEnum(EVRSceneApplicationState state) override {
        return nullptr;
    }
    virtual EVRApplicationError LaunchInternalProcess(const char *pchBinaryPath, const char *pchArguments, const char *pchWorkingDirectory) override {
        return (EVRApplicationError)1;
    }
    virtual EVRApplicationError RegisterSubprocess(uint32_t nPid) override {
        return (EVRApplicationError)1;
    }
    virtual uint32_t GetCurrentSceneProcessId() override {
        return (uint32_t)0;
    }
} g_mockApplications;

class Mock_IVRSettings : public vr::IVRSettings {
public:
    virtual const char * GetSettingsErrorNameFromEnum(EVRSettingsError eError) override {
        return nullptr;
    }
    virtual void SetBool(const char *pchSection, const char *pchSettingsKey, bool bValue, EVRSettingsError *peError = nullptr) override {
    }
    virtual void SetInt32(const char *pchSection, const char *pchSettingsKey, int32_t nValue, EVRSettingsError *peError = nullptr) override {
    }
    virtual void SetFloat(const char *pchSection, const char *pchSettingsKey, float flValue, EVRSettingsError *peError = nullptr) override {
    }
    virtual void SetString(const char *pchSection, const char *pchSettingsKey, const char *pchValue, EVRSettingsError *peError = nullptr) override {
    }
    virtual bool GetBool(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        return false;
    }
    virtual int32_t GetInt32(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        return (int32_t)0;
    }
    virtual float GetFloat(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
        return (float)0;
    }
    virtual void GetString(const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError = nullptr) override {
        if(peError) *peError = vr::VRSettingsError_None;
        if(pchValue && unValueLen > 0) pchValue[0] = '\0';
    }
    virtual void RemoveSection(const char *pchSection, EVRSettingsError *peError = nullptr) override {
    }
    virtual void RemoveKeyInSection(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr) override {
    }
} g_mockSettings;

class Mock_IVRChaperone : public vr::IVRChaperone {
public:
    virtual ChaperoneCalibrationState GetCalibrationState() override {
        return (ChaperoneCalibrationState)0;
    }
    virtual bool GetPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        return false;
    }
    virtual bool GetPlayAreaRect(HmdQuad_t *rect) override {
        return false;
    }
    virtual void ReloadInfo(void) override {
    }
    virtual void SetSceneColor(HmdColor_t color) override {
    }
    virtual void GetBoundsColor(HmdColor_t *pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, HmdColor_t *pOutputCameraColor) override {
    }
    virtual bool AreBoundsVisible() override {
        return false;
    }
    virtual void ForceBoundsVisible(bool bForce) override {
    }
    virtual void ResetZeroPose(ETrackingUniverseOrigin eTrackingUniverseOrigin) override {
    }
} g_mockChaperone;

class Mock_IVRChaperoneSetup : public vr::IVRChaperoneSetup {
public:
    virtual bool CommitWorkingCopy(EChaperoneConfigFile configFile) override {
        return false;
    }
    virtual void RevertWorkingCopy() override {
    }
    virtual bool GetWorkingPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        return false;
    }
    virtual bool GetWorkingPlayAreaRect(HmdQuad_t *rect) override {
        return false;
    }
    virtual bool GetWorkingCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        return false;
    }
    virtual bool GetLiveCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        return false;
    }
    virtual bool GetWorkingSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        return false;
    }
    virtual bool GetWorkingStandingZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatStandingZeroPoseToRawTrackingPose) override {
        return false;
    }
    virtual void SetWorkingPlayAreaSize(float sizeX, float sizeZ) override {
    }
    virtual void SetWorkingCollisionBoundsInfo(VR_ARRAY_COUNT(unQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t unQuadsCount) override {
    }
    virtual void SetWorkingPerimeter(VR_ARRAY_COUNT( unPointCount ) const HmdVector2_t *pPointBuffer, uint32_t unPointCount) override {
    }
    virtual void SetWorkingSeatedZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatSeatedZeroPoseToRawTrackingPose) override {
    }
    virtual void SetWorkingStandingZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatStandingZeroPoseToRawTrackingPose) override {
    }
    virtual void ReloadFromDisk(EChaperoneConfigFile configFile) override {
    }
    virtual bool GetLiveSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        return false;
    }
    virtual bool ExportLiveToBuffer(VR_OUT_STRING() char *pBuffer, uint32_t *pnBufferLength) override {
        return false;
    }
    virtual bool ImportFromBufferToWorking(const char *pBuffer, uint32_t nImportFlags) override {
        return false;
    }
    virtual void ShowWorkingSetPreview() override {
    }
    virtual void HideWorkingSetPreview() override {
    }
    virtual void RoomSetupStarting() override {
    }
} g_mockChaperoneSetup;

class Mock_IVRCompositor : public vr::IVRCompositor {
public:
    virtual void SetTrackingSpace(ETrackingUniverseOrigin eOrigin) override {
    }
    virtual ETrackingUniverseOrigin GetTrackingSpace() override {
        return (ETrackingUniverseOrigin)0;
    }
    virtual EVRCompositorError WaitGetPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        if(pRenderPoseArray && unRenderPoseArrayCount > 0) { memset(pRenderPoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unRenderPoseArrayCount); pRenderPoseArray[0].bPoseIsValid = true; pRenderPoseArray[0].bDeviceIsConnected = true; pRenderPoseArray[0].eTrackingResult = vr::TrackingResult_Running_OK; pRenderPoseArray[0].mDeviceToAbsoluteTracking.m[0][0] = 1; pRenderPoseArray[0].mDeviceToAbsoluteTracking.m[1][1] = 1; pRenderPoseArray[0].mDeviceToAbsoluteTracking.m[2][2] = 1; }
        if(pGamePoseArray && unGamePoseArrayCount > 0) { memset(pGamePoseArray, 0, sizeof(vr::TrackedDevicePose_t) * unGamePoseArrayCount); pGamePoseArray[0].bPoseIsValid = true; pGamePoseArray[0].bDeviceIsConnected = true; pGamePoseArray[0].eTrackingResult = vr::TrackingResult_Running_OK; pGamePoseArray[0].mDeviceToAbsoluteTracking.m[0][0] = 1; pGamePoseArray[0].mDeviceToAbsoluteTracking.m[1][1] = 1; pGamePoseArray[0].mDeviceToAbsoluteTracking.m[2][2] = 1; }
        Sleep(11);
        return vr::VRCompositorError_None;
    }
    virtual EVRCompositorError GetLastPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError GetLastPoseForTrackedDeviceIndex(TrackedDeviceIndex_t unDeviceIndex, TrackedDevicePose_t *pOutputPose, TrackedDevicePose_t *pOutputGamePose) override {
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError GetSubmitTexture(Texture_t *pOutTexture, bool *pNeedsFlush, EVRCompositorTextureUsage eUsage, const Texture_t *pTexture, const VRTextureBounds_t *pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default) override {
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError Submit(EVREye eEye, const Texture_t *pTexture, const VRTextureBounds_t* pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default) override {
        return vr::VRCompositorError_None;
    }
    virtual EVRCompositorError SubmitWithArrayIndex(EVREye eEye, const Texture_t *pTexture, uint32_t unTextureArrayIndex, const VRTextureBounds_t *pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default) override {
        return (EVRCompositorError)1;
    }
    virtual void ClearLastSubmittedFrame() override {
    }
    virtual void PostPresentHandoff() override {
    }
    virtual bool GetFrameTiming(Compositor_FrameTiming *pTiming, uint32_t unFramesAgo = 0) override {
        return false;
    }
    virtual uint32_t GetFrameTimings(VR_ARRAY_COUNT( nFrames ) Compositor_FrameTiming *pTiming, uint32_t nFrames) override {
        return (uint32_t)0;
    }
    virtual float GetFrameTimeRemaining() override {
        return (float)0;
    }
    virtual void GetCumulativeStats(Compositor_CumulativeStats *pStats, uint32_t nStatsSizeInBytes) override {
    }
    virtual void FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground = false) override {
    }
    virtual void GetCurrentFadeColor(HmdColor_t* pRet, bool bBackground = false) override {
        if(pRet) { memset(pRet, 0, sizeof(*pRet)); }
    }
    virtual void FadeGrid(float fSeconds, bool bFadeGridIn) override {
    }
    virtual float GetCurrentGridAlpha() override {
        return (float)0;
    }
    virtual EVRCompositorError SetSkyboxOverride(VR_ARRAY_COUNT( unTextureCount ) const Texture_t *pTextures, uint32_t unTextureCount) override {
        return (EVRCompositorError)1;
    }
    virtual void ClearSkyboxOverride() override {
    }
    virtual void CompositorBringToFront() override {
    }
    virtual void CompositorGoToBack() override {
    }
    virtual void CompositorQuit() override {
    }
    virtual bool IsFullscreen() override {
        return false;
    }
    virtual uint32_t GetCurrentSceneFocusProcess() override {
        return GetCurrentProcessId();
    }
    virtual uint32_t GetLastFrameRenderer() override {
        return (uint32_t)0;
    }
    virtual bool CanRenderScene() override {
        return true;
    }
    virtual void ShowMirrorWindow() override {
    }
    virtual void HideMirrorWindow() override {
    }
    virtual bool IsMirrorWindowVisible() override {
        return false;
    }
    virtual void CompositorDumpImages() override {
    }
    virtual bool ShouldAppRenderWithLowResources() override {
        return false;
    }
    virtual void ForceInterleavedReprojectionOn(bool bOverride) override {
    }
    virtual void ForceReconnectProcess() override {
    }
    virtual void SuspendRendering(bool bSuspend) override {
    }
    virtual vr::EVRCompositorError GetMirrorTextureD3D11(vr::EVREye eEye, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView) override {
        return (vr::EVRCompositorError)1;
    }
    virtual void ReleaseMirrorTextureD3D11(void *pD3D11ShaderResourceView) override {
    }
    virtual vr::EVRCompositorError GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t *pglTextureId, vr::glSharedTextureHandle_t *pglSharedTextureHandle) override {
        return (vr::EVRCompositorError)1;
    }
    virtual bool ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        return false;
    }
    virtual void LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
    }
    virtual void UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
    }
    virtual uint32_t GetVulkanInstanceExtensionsRequired(VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        if(pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return 0;
    }
    virtual uint32_t GetVulkanDeviceExtensionsRequired(VkPhysicalDevice_T *pPhysicalDevice, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        if(pchValue && unBufferSize > 0) pchValue[0] = '\0';
        return 0;
    }
    virtual void SetExplicitTimingMode(EVRCompositorTimingMode eTimingMode) override {
    }
    virtual EVRCompositorError SubmitExplicitTimingData() override {
        return (EVRCompositorError)1;
    }
    virtual bool IsMotionSmoothingEnabled() override {
        return false;
    }
    virtual bool IsMotionSmoothingSupported() override {
        return false;
    }
    virtual bool IsCurrentSceneFocusAppLoading() override {
        return false;
    }
    virtual EVRCompositorError SetStageOverride_Async(const char *pchRenderModelPath, const HmdMatrix34_t *pTransform = 0, const Compositor_StageRenderSettings *pRenderSettings = 0, uint32_t nSizeOfRenderSettings = 0) override {
        return (EVRCompositorError)1;
    }
    virtual void ClearStageOverride() override {
    }
    virtual bool GetCompositorBenchmarkResults(Compositor_BenchmarkResults *pBenchmarkResults, uint32_t nSizeOfBenchmarkResults) override {
        return false;
    }
    virtual EVRCompositorError GetLastPosePredictionIDs(uint32_t *pRenderPosePredictionID, uint32_t *pGamePosePredictionID) override {
        return (EVRCompositorError)1;
    }
    virtual EVRCompositorError GetPosesForFrame(uint32_t unPosePredictionID, VR_ARRAY_COUNT( unPoseArrayCount ) TrackedDevicePose_t* pPoseArray, uint32_t unPoseArrayCount) override {
        return (EVRCompositorError)1;
    }
} g_mockCompositor;

class Mock_IVRHeadsetView : public vr::IVRHeadsetView {
public:
    virtual void SetHeadsetViewSize(uint32_t nWidth, uint32_t nHeight) override {
    }
    virtual void GetHeadsetViewSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
    }
    virtual void SetHeadsetViewMode(HeadsetViewMode_t eHeadsetViewMode) override {
    }
    virtual HeadsetViewMode_t GetHeadsetViewMode() override {
        return (HeadsetViewMode_t)0;
    }
    virtual void SetHeadsetViewCropped(bool bCropped) override {
    }
    virtual bool GetHeadsetViewCropped() override {
        return false;
    }
    virtual float GetHeadsetViewAspectRatio() override {
        return (float)0;
    }
    virtual void SetHeadsetViewBlendRange(float flStartPct, float flEndPct) override {
    }
    virtual void GetHeadsetViewBlendRange(float *pStartPct, float *pEndPct) override {
    }
} g_mockHeadsetView;

class Mock_IVRNotifications : public vr::IVRNotifications {
public:
    virtual EVRNotificationError CreateNotification(VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, EVRNotificationType type, const char *pchText, EVRNotificationStyle style, const NotificationBitmap_t *pImage, VRNotificationId *pNotificationId) override {
        return (EVRNotificationError)1;
    }
    virtual EVRNotificationError RemoveNotification(VRNotificationId notificationId) override {
        return (EVRNotificationError)1;
    }
} g_mockNotifications;

class Mock_IVROverlay : public vr::IVROverlay {
public:
    virtual EVROverlayError FindOverlay(const char *pchOverlayKey, VROverlayHandle_t * pOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError CreateOverlay(const char *pchOverlayKey, const char *pchOverlayName, VROverlayHandle_t * pOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError CreateSubviewOverlay(VROverlayHandle_t parentOverlayHandle, const char *pchSubviewOverlayKey, const char *pchSubviewOverlayName, VROverlayHandle_t *pSubviewOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError DestroyOverlay(VROverlayHandle_t ulOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual uint32_t GetOverlayKey(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetOverlayName(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L) override {
        return (uint32_t)0;
    }
    virtual EVROverlayError SetOverlayName(VROverlayHandle_t ulOverlayHandle, const char *pchName) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayImageData(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unBufferSize, uint32_t *punWidth, uint32_t *punHeight) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual const char * GetOverlayErrorNameFromEnum(EVROverlayError error) override {
        return nullptr;
    }
    virtual EVROverlayError SetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle, uint32_t unPID) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual uint32_t GetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle) override {
        return (uint32_t)0;
    }
    virtual EVROverlayError SetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool *pbEnabled) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayFlags(VROverlayHandle_t ulOverlayHandle, uint32_t *pFlags) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayColor(VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayColor(VROverlayHandle_t ulOverlayHandle, float *pfRed, float *pfGreen, float *pfBlue) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float fAlpha) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float *pfAlpha) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float fTexelAspect) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float *pfTexelAspect) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t *punSortOrder) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float fWidthInMeters) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float *pfWidthInMeters) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float fCurvature) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float *pfCurvature) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayPreCurvePitch(VROverlayHandle_t ulOverlayHandle, float fRadians) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayPreCurvePitch(VROverlayHandle_t ulOverlayHandle, float *pfRadians) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace eTextureColorSpace) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace *peTextureColorSpace) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, const VRTextureBounds_t *pOverlayTextureBounds) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, VRTextureBounds_t *pOverlayTextureBounds) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTransformType(VROverlayHandle_t ulOverlayHandle, VROverlayTransformType *peTransformType) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin *peTrackingOrigin, HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punTrackedDevice, HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unDeviceIndex, const char *pchComponentName) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punDeviceIndex, VR_OUT_STRING() char *pchComponentName, uint32_t unComponentNameSize) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTransformCursor(VROverlayHandle_t ulCursorOverlayHandle, const HmdVector2_t *pvHotspot) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual vr::EVROverlayError GetOverlayTransformCursor(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvHotspot) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual vr::EVROverlayError SetOverlayTransformProjection(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t* pmatTrackingOriginToOverlayTransform, const VROverlayProjection_t *pProjection, vr::EVREye eEye) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetSubviewPosition(VROverlayHandle_t ulOverlayHandle, float fX, float fY) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError ShowOverlay(VROverlayHandle_t ulOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError HideOverlay(VROverlayHandle_t ulOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual bool IsOverlayVisible(VROverlayHandle_t ulOverlayHandle) override {
        return false;
    }
    virtual EVROverlayError GetTransformForOverlayCoordinates(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, HmdVector2_t coordinatesInOverlay, HmdMatrix34_t *pmatTransform) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError WaitFrameSync(uint32_t nTimeoutMs) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual bool PollNextOverlayEvent(VROverlayHandle_t ulOverlayHandle, VREvent_t *pEvent, uint32_t uncbVREvent) override {
        return false;
    }
    virtual EVROverlayError GetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod *peInputMethod) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvecMouseScale) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvecMouseScale) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual bool ComputeOverlayIntersection(VROverlayHandle_t ulOverlayHandle, const VROverlayIntersectionParams_t *pParams, VROverlayIntersectionResults_t *pResults) override {
        return false;
    }
    virtual bool IsHoverTargetOverlay(VROverlayHandle_t ulOverlayHandle) override {
        return false;
    }
    virtual EVROverlayError SetOverlayIntersectionMask(VROverlayHandle_t ulOverlayHandle, VROverlayIntersectionMaskPrimitive_t *pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize = sizeof( VROverlayIntersectionMaskPrimitive_t )) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError TriggerLaserMouseHapticVibration(VROverlayHandle_t ulOverlayHandle, float fDurationSeconds, float fFrequency, float fAmplitude) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayCursor(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulCursorHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvCursor) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError ClearOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayTexture(VROverlayHandle_t ulOverlayHandle, const Texture_t *pTexture) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError ClearOverlayTexture(VROverlayHandle_t ulOverlayHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayRaw(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unBytesPerPixel) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError SetOverlayFromFile(VROverlayHandle_t ulOverlayHandle, const char *pchFilePath) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTexture(VROverlayHandle_t ulOverlayHandle, void **pNativeTextureHandle, void *pNativeTextureRef, uint32_t *pWidth, uint32_t *pHeight, uint32_t *pNativeFormat, ETextureType *pAPIType, EColorSpace *pColorSpace, VRTextureBounds_t *pTextureBounds) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError ReleaseNativeOverlayHandle(VROverlayHandle_t ulOverlayHandle, void *pNativeTextureHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetOverlayTextureSize(VROverlayHandle_t ulOverlayHandle, uint32_t *pWidth, uint32_t *pHeight) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError CreateDashboardOverlay(const char *pchOverlayKey, const char *pchOverlayFriendlyName, VROverlayHandle_t * pMainHandle, VROverlayHandle_t *pThumbnailHandle) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual bool IsDashboardVisible() override {
        return false;
    }
    virtual bool IsActiveDashboardOverlay(VROverlayHandle_t ulOverlayHandle) override {
        return false;
    }
    virtual EVROverlayError SetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError GetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t *punProcessId) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual void ShowDashboard(const char *pchOverlayToShow) override {
    }
    virtual vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override {
        return (vr::TrackedDeviceIndex_t)0;
    }
    virtual EVROverlayError ShowKeyboard(EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError ShowKeyboardForOverlay(VROverlayHandle_t ulOverlayHandle, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual uint32_t GetKeyboardText(VR_OUT_STRING() char *pchText, uint32_t cchText) override {
        return (uint32_t)0;
    }
    virtual void HideKeyboard() override {
    }
    virtual void SetKeyboardTransformAbsolute(ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToKeyboardTransform) override {
    }
    virtual void SetKeyboardPositionForOverlay(VROverlayHandle_t ulOverlayHandle, HmdRect2_t avoidRect) override {
    }
    virtual VRMessageOverlayResponse ShowMessageOverlay(const char* pchText, const char* pchCaption, const char* pchButton0Text, const char* pchButton1Text = nullptr, const char* pchButton2Text = nullptr, const char* pchButton3Text = nullptr) override {
        return (VRMessageOverlayResponse)0;
    }
    virtual void CloseMessageOverlay() override {
    }
} g_mockOverlay;

class Mock_IVROverlayView : public vr::IVROverlayView {
public:
    virtual EVROverlayError AcquireOverlayView(VROverlayHandle_t ulOverlayHandle, VRNativeDevice_t *pNativeDevice, VROverlayView_t *pOverlayView, uint32_t unOverlayViewSize) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual EVROverlayError ReleaseOverlayView(VROverlayView_t *pOverlayView) override {
        return vr::VROverlayError_UnknownOverlay;
    }
    virtual void PostOverlayEvent(VROverlayHandle_t ulOverlayHandle, const VREvent_t *pvrEvent) override {
    }
    virtual bool IsViewingPermitted(VROverlayHandle_t ulOverlayHandle) override {
        return false;
    }
} g_mockOverlayView;

class Mock_IVRRenderModels : public vr::IVRRenderModels {
public:
    virtual EVRRenderModelError LoadRenderModel_Async(const char *pchRenderModelName, RenderModel_t **ppRenderModel) override {
        return vr::VRRenderModelError_NotSupported;
    }
    virtual void FreeRenderModel(RenderModel_t *pRenderModel) override {
    }
    virtual EVRRenderModelError LoadTexture_Async(TextureID_t textureId, RenderModel_TextureMap_t **ppTexture) override {
        return vr::VRRenderModelError_NotSupported;
    }
    virtual void FreeTexture(RenderModel_TextureMap_t *pTexture) override {
    }
    virtual EVRRenderModelError LoadTextureD3D11_Async(TextureID_t textureId, void *pD3D11Device, void **ppD3D11Texture2D) override {
        return vr::VRRenderModelError_NotSupported;
    }
    virtual EVRRenderModelError LoadIntoTextureD3D11_Async(TextureID_t textureId, void *pDstTexture) override {
        return vr::VRRenderModelError_NotSupported;
    }
    virtual void FreeTextureD3D11(void *pD3D11Texture2D) override {
    }
    virtual uint32_t GetRenderModelName(uint32_t unRenderModelIndex, VR_OUT_STRING() char *pchRenderModelName, uint32_t unRenderModelNameLen) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetRenderModelCount() override {
        return (uint32_t)0;
    }
    virtual uint32_t GetComponentCount(const char *pchRenderModelName) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetComponentName(const char *pchRenderModelName, uint32_t unComponentIndex, VR_OUT_STRING( ) char *pchComponentName, uint32_t unComponentNameLen) override {
        return (uint32_t)0;
    }
    virtual uint64_t GetComponentButtonMask(const char *pchRenderModelName, const char *pchComponentName) override {
        return (uint64_t)0;
    }
    virtual uint32_t GetComponentRenderModelName(const char *pchRenderModelName, const char *pchComponentName, VR_OUT_STRING( ) char *pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) override {
        return (uint32_t)0;
    }
    virtual bool GetComponentStateForDevicePath(const char *pchRenderModelName, const char *pchComponentName, vr::VRInputValueHandle_t devicePath, const vr::RenderModel_ControllerMode_State_t *pState, vr::RenderModel_ComponentState_t *pComponentState) override {
        return false;
    }
    virtual bool GetComponentState(const char *pchRenderModelName, const char *pchComponentName, const vr::VRControllerState_t *pControllerState, const RenderModel_ControllerMode_State_t *pState, RenderModel_ComponentState_t *pComponentState) override {
        return false;
    }
    virtual bool RenderModelHasComponent(const char *pchRenderModelName, const char *pchComponentName) override {
        return false;
    }
    virtual uint32_t GetRenderModelThumbnailURL(const char *pchRenderModelName, VR_OUT_STRING() char *pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError *peError) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetRenderModelOriginalPath(const char *pchRenderModelName, VR_OUT_STRING() char *pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError *peError) override {
        return (uint32_t)0;
    }
    virtual const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) override {
        return nullptr;
    }
} g_mockRenderModels;

class Mock_IVRExtendedDisplay : public vr::IVRExtendedDisplay {
public:
    virtual void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
    }
    virtual void GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
        if(pnX) *pnX = 0; if(pnY) *pnY = 0; if(pnWidth) *pnWidth = 1920; if(pnHeight) *pnHeight = 1080;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex, int32_t *pnAdapterOutputIndex) override {
        if(pnAdapterIndex) *pnAdapterIndex = 0;
    }
} g_mockExtendedDisplay;

class Mock_IVRTrackedCamera : public vr::IVRTrackedCamera {
public:
    virtual const char * GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError) override {
        return nullptr;
    }
    virtual vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool *pHasCamera) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t *pnWidth, uint32_t *pnHeight, uint32_t *pnFrameBufferSize) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t *pFocalLength, vr::HmdVector2_t *pCenter) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t *pProjection) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t *pHandle) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t *pTextureBounds, uint32_t *pnWidth, uint32_t *pnHeight) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t *pglTextureId, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId) override {
        return vr::VRTrackedCameraError_OperationFailed;
    }
    virtual void SetCameraTrackingSpace(vr::ETrackingUniverseOrigin eUniverse) override {
    }
    virtual vr::ETrackingUniverseOrigin GetCameraTrackingSpace() override {
        return (vr::ETrackingUniverseOrigin)0;
    }
} g_mockTrackedCamera;

class Mock_IVRScreenshots : public vr::IVRScreenshots {
public:
    virtual vr::EVRScreenshotError RequestScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, vr::EVRScreenshotType type, const char *pchPreviewFilename, const char *pchVRFilename) override {
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotError HookScreenshot(VR_ARRAY_COUNT( numTypes ) const vr::EVRScreenshotType *pSupportedTypes, int numTypes) override {
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotType GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError *pError) override {
        return (vr::EVRScreenshotType)0;
    }
    virtual uint32_t GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, VR_OUT_STRING() char *pchFilename, uint32_t cchFilename, vr::EVRScreenshotError *pError) override {
        return (uint32_t)0;
    }
    virtual vr::EVRScreenshotError UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress) override {
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotError TakeStereoScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, const char *pchPreviewFilename, const char *pchVRFilename) override {
        return (vr::EVRScreenshotError)1;
    }
    virtual vr::EVRScreenshotError SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char *pchSourcePreviewFilename, const char *pchSourceVRFilename) override {
        return (vr::EVRScreenshotError)1;
    }
} g_mockScreenshots;

class Mock_IVRResources : public vr::IVRResources {
public:
    virtual uint32_t LoadSharedResource(const char *pchResourceName, char *pchBuffer, uint32_t unBufferLen) override {
        return (uint32_t)0;
    }
    virtual uint32_t GetResourceFullPath(const char *pchResourceName, const char *pchResourceTypeDirectory, VR_OUT_STRING() char *pchPathBuffer, uint32_t unBufferLen) override {
        return (uint32_t)0;
    }
} g_mockResources;

class Mock_IVRDriverManager : public vr::IVRDriverManager {
public:
    virtual uint32_t GetDriverCount() const override {
        return (uint32_t)0;
    }
    virtual uint32_t GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        return (uint32_t)0;
    }
    virtual DriverHandle_t GetDriverHandle(const char *pchDriverName) override {
        return (DriverHandle_t)0;
    }
    virtual bool IsEnabled(vr::DriverId_t nDriver) const override {
        return false;
    }
} g_mockDriverManager;

class Mock_IVRInput : public vr::IVRInput {
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError UpdateActionState(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetPoseActionDataRelativeToNow(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetSkeletalActionData(VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetDominantHand(ETrackedControllerRole *peDominantHand) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError SetDominantHand(ETrackedControllerRole eDominantHand) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetEyeTrackingDataRelativeToNow(VRActionHandle_t action, vr::ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, vr::VREyeTrackingData_t *pEyeTrackingData, uint32_t ulEyeTrackingDataSize) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetEyeTrackingDataForNextFrame(VRActionHandle_t action, vr::ETrackingUniverseOrigin eOrigin, vr::VREyeTrackingData_t *pEyeTrackingData, uint32_t ulEyeTrackingDataSize) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetBoneCount(VRActionHandle_t action, uint32_t* pBoneCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetBoneHierarchy(VRActionHandle_t action, VR_ARRAY_COUNT( unIndexArayCount ) BoneIndex_t* pParentIndices, uint32_t unIndexArayCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetBoneName(VRActionHandle_t action, BoneIndex_t nBoneIndex, VR_OUT_STRING() char* pchBoneName, uint32_t unNameBufferSize) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetSkeletalReferenceTransforms(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalReferencePose eReferencePose, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetSkeletalTrackingLevel(VRActionHandle_t action, EVRSkeletalTrackingLevel* pSkeletalTrackingLevel) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetSkeletalBoneData(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalMotionRange eMotionRange, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetSkeletalSummaryData(VRActionHandle_t action, EVRSummaryType eSummaryType, VRSkeletalSummaryData_t * pSkeletalSummaryData) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetSkeletalBoneDataCompressed(VRActionHandle_t action, EVRSkeletalMotionRange eMotionRange, VR_OUT_BUFFER_COUNT( unCompressedSize ) void *pvCompressedData, uint32_t unCompressedSize, uint32_t *punRequiredCompressedSize) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError DecompressSkeletalBoneData(const void *pvCompressedBuffer, uint32_t unCompressedBufferSize, EVRSkeletalTransformSpace eTransformSpace, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError TriggerHapticVibrationAction(VRActionHandle_t action, float fStartSecondsFromNow, float fDurationSeconds, float fFrequency, float fAmplitude, VRInputValueHandle_t ulRestrictToDevice) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t digitalActionHandle, VR_ARRAY_COUNT( originOutCount ) VRInputValueHandle_t *originsOut, uint32_t originOutCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetOriginLocalizedName(VRInputValueHandle_t origin, VR_OUT_STRING() char *pchNameArray, uint32_t unNameArraySize, int32_t unStringSectionsToInclude) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetOriginTrackedDeviceInfo(VRInputValueHandle_t origin, InputOriginInfo_t *pOriginInfo, uint32_t unOriginInfoSize) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetActionBindingInfo(VRActionHandle_t action, VR_ARRAY_COUNT( unBindingInfoCount ) InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, uint32_t *punReturnedBindingInfoCount) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError ShowActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t ulActionHandle) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError ShowBindingsForActionSet(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount, VRInputValueHandle_t originToHighlight) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetComponentStateForBinding(const char *pchRenderModelName, const char *pchComponentName, const InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, vr::RenderModel_ComponentState_t *pComponentState) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual bool IsUsingLegacyInput() override {
        return false;
    }
    virtual EVRInputError OpenBindingUI(const char* pchAppKey, VRActionSetHandle_t ulActionSetHandle, VRInputValueHandle_t ulDeviceHandle, bool bShowOnDesktop) override {
        return vr::VRInputError_InvalidHandle;
    }
    virtual EVRInputError GetBindingVariant(vr::VRInputValueHandle_t ulDevicePath, VR_OUT_STRING() char *pchVariantArray, uint32_t unVariantArraySize) override {
        return vr::VRInputError_InvalidHandle;
    }
} g_mockInput;

class Mock_IVRIOBuffer : public vr::IVRIOBuffer {
public:
    virtual vr::EIOBufferError Open(const char *pchPath, vr::EIOBufferMode mode, uint32_t unElementSize, uint32_t unElements, vr::IOBufferHandle_t *pulBuffer) override {
        return (vr::EIOBufferError)1;
    }
    virtual vr::EIOBufferError Close(vr::IOBufferHandle_t ulBuffer) override {
        return (vr::EIOBufferError)1;
    }
    virtual vr::EIOBufferError Read(vr::IOBufferHandle_t ulBuffer, void *pDst, uint32_t unBytes, uint32_t *punRead) override {
        return (vr::EIOBufferError)1;
    }
    virtual vr::EIOBufferError Write(vr::IOBufferHandle_t ulBuffer, void *pSrc, uint32_t unBytes) override {
        return (vr::EIOBufferError)1;
    }
    virtual vr::PropertyContainerHandle_t PropertyContainer(vr::IOBufferHandle_t ulBuffer) override {
        return (vr::PropertyContainerHandle_t)0;
    }
    virtual bool HasReaders(vr::IOBufferHandle_t ulBuffer) override {
        return false;
    }
} g_mockIOBuffer;

class Mock_IVRSpatialAnchors : public vr::IVRSpatialAnchors {
public:
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromDescriptor(const char *pchDescriptor, SpatialAnchorHandle_t *pHandleOut) override {
        return (EVRSpatialAnchorError)1;
    }
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromPose(TrackedDeviceIndex_t unDeviceIndex, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPose, SpatialAnchorHandle_t *pHandleOut) override {
        return (EVRSpatialAnchorError)1;
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorPose(SpatialAnchorHandle_t unHandle, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPoseOut) override {
        return (EVRSpatialAnchorError)1;
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorDescriptor(SpatialAnchorHandle_t unHandle, VR_OUT_STRING() char *pchDescriptorOut, uint32_t *punDescriptorBufferLenInOut) override {
        return (EVRSpatialAnchorError)1;
    }
} g_mockSpatialAnchors;

class Mock_IVRDebug : public vr::IVRDebug {
public:
    virtual EVRDebugError EmitVrProfilerEvent(const char *pchMessage) override {
        return (EVRDebugError)1;
    }
    virtual EVRDebugError BeginVrProfilerEvent(VrProfilerEventHandle_t *pHandleOut) override {
        return (EVRDebugError)1;
    }
    virtual EVRDebugError FinishVrProfilerEvent(VrProfilerEventHandle_t hHandle, const char *pchMessage) override {
        return (EVRDebugError)1;
    }
    virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, VR_OUT_STRING() char *pchResponseBuffer, uint32_t unResponseBufferSize) override {
        return (uint32_t)0;
    }
} g_mockDebug;

class Mock_IVRIPCResourceManagerClient : public vr::IVRIPCResourceManagerClient {
public:
    virtual bool NewSharedVulkanImage(uint32_t nImageFormat, uint32_t nWidth, uint32_t nHeight, bool bRenderable, bool bMappable, bool bComputeAccess, uint32_t unMipLevels, uint32_t unArrayLayerCount, uint32_t unAdditionalVkCreateFlags, uint32_t unAdditionalVkUsageFlags, vr::SharedTextureHandle_t *pSharedHandle) override {
        return false;
    }
    virtual bool NewSharedVulkanBuffer(uint32_t nSize, uint32_t nUsageFlags, vr::SharedTextureHandle_t *pSharedHandle) override {
        return false;
    }
    virtual bool NewSharedVulkanSemaphore(bool bCounting, vr::SharedTextureHandle_t *pSharedHandle) override {
        return false;
    }
    virtual bool RefResource(vr::SharedTextureHandle_t hSharedHandle, uint64_t *pNewIpcHandle) override {
        return false;
    }
    virtual bool UnrefResource(vr::SharedTextureHandle_t hSharedHandle) override {
        return false;
    }
    virtual bool GetDmabufFormats(uint32_t *pOutFormatCount, uint32_t *pOutFormats) override {
        return false;
    }
    virtual bool GetDmabufModifiers(vr::EVRApplicationType eApplicationType, uint32_t unDRMFormat, uint32_t *pOutModifierCount, uint64_t *pOutModifiers) override {
        return false;
    }
    virtual bool ImportDmabuf(vr::EVRApplicationType eApplicationType, vr::DmabufAttributes_t *pDmabufAttributes, vr::SharedTextureHandle_t *pSharedHandle) override {
        return false;
    }
    virtual bool ReceiveSharedFd(uint64_t ulIpcHandle, int *pOutFd) override {
        return false;
    }
} g_mockIPCResourceManagerClient;


extern "C" __declspec(dllexport) void* VR_GetGenericInterface(const char *pchInterfaceVersion, vr::EVRInitError *peError) {
    if (peError) *peError = vr::VRInitError_None;
    FILE* f = fopen("vr_emulator_log.txt", "a");
    if(f) { fprintf(f, "Requested interface: %s\n", pchInterfaceVersion); fclose(f); }

    if (strstr(pchInterfaceVersion, "IVRMailbox")) return &g_universalMock;

    if (strstr(pchInterfaceVersion, "IVRSystem")) return &g_mockSystem;
    if (strstr(pchInterfaceVersion, "IVRApplications")) return &g_mockApplications;
    if (strstr(pchInterfaceVersion, "IVRSettings")) return &g_mockSettings;
    if (strstr(pchInterfaceVersion, "IVRChaperone")) return &g_mockChaperone;
    if (strstr(pchInterfaceVersion, "IVRChaperoneSetup")) return &g_mockChaperoneSetup;
    if (strstr(pchInterfaceVersion, "IVRCompositor")) return &g_mockCompositor;
    if (strstr(pchInterfaceVersion, "IVRHeadsetView")) return &g_mockHeadsetView;
    if (strstr(pchInterfaceVersion, "IVRNotifications")) return &g_mockNotifications;
    if (strstr(pchInterfaceVersion, "IVROverlay")) return &g_mockOverlay;
    if (strstr(pchInterfaceVersion, "IVROverlayView")) return &g_mockOverlayView;
    if (strstr(pchInterfaceVersion, "IVRRenderModels")) return &g_mockRenderModels;
    if (strstr(pchInterfaceVersion, "IVRExtendedDisplay")) return &g_mockExtendedDisplay;
    if (strstr(pchInterfaceVersion, "IVRTrackedCamera")) return &g_mockTrackedCamera;
    if (strstr(pchInterfaceVersion, "IVRScreenshots")) return &g_mockScreenshots;
    if (strstr(pchInterfaceVersion, "IVRResources")) return &g_mockResources;
    if (strstr(pchInterfaceVersion, "IVRDriverManager")) return &g_mockDriverManager;
    if (strstr(pchInterfaceVersion, "IVRInput")) return &g_mockInput;
    if (strstr(pchInterfaceVersion, "IVRIOBuffer")) return &g_mockIOBuffer;
    if (strstr(pchInterfaceVersion, "IVRSpatialAnchors")) return &g_mockSpatialAnchors;
    if (strstr(pchInterfaceVersion, "IVRDebug")) return &g_mockDebug;
    if (strstr(pchInterfaceVersion, "IVRIPCResourceManagerClient")) return &g_mockIPCResourceManagerClient;

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

