#pragma once
#include "official_openvr.h"
#include <string.h>
using namespace vr;


static HmdMatrix34_t Identity34_Gen() {
    HmdMatrix34_t m;
    memset(&m, 0, sizeof(m));
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f;
    return m;
}
static HmdMatrix44_t Identity44_Gen() {
    HmdMatrix44_t m;
    memset(&m, 0, sizeof(m));
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f; m.m[3][3] = 1.0f;
    return m;
}
class FakeSystem : public vr::IVRSystem {
public:
    virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetRecommendedRenderTargetSize\n"); fclose(_f); }
        return;
    }
    virtual void GetProjectionMatrix(HmdMatrix44_t* ret, EVREye eEye, float fNearZ, float fFarZ) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetProjectionMatrix\n"); fclose(_f); }
        if(ret) *ret = Identity44_Gen();
        return;
    }
    virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetProjectionRaw\n"); fclose(_f); }
        return;
    }
    virtual bool ComputeDistortion(EVREye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::ComputeDistortion\n"); fclose(_f); }
        return {};
    }
    virtual void GetEyeToHeadTransform(HmdMatrix34_t* ret, EVREye eEye) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetEyeToHeadTransform\n"); fclose(_f); }
        if(ret) *ret = Identity34_Gen();
        return;
    }
    virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetTimeSinceLastVsync\n"); fclose(_f); }
        return {};
    }
    virtual int32_t GetD3D9AdapterIndex() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetD3D9AdapterIndex\n"); fclose(_f); }
        return {};
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetDXGIOutputInfo\n"); fclose(_f); }
        return;
    }
    virtual void GetOutputDevice(uint64_t *pnDevice, ETextureType textureType, VkInstance_T *pInstance) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetOutputDevice\n"); fclose(_f); }
        return;
    }
    virtual bool IsDisplayOnDesktop() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::IsDisplayOnDesktop\n"); fclose(_f); }
        return {};
    }
    virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::SetDisplayVisibility\n"); fclose(_f); }
        return {};
    }
    virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetDeviceToAbsoluteTrackingPose\n"); fclose(_f); }
        return;
    }
    virtual void ResetSeatedZeroPose() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::ResetSeatedZeroPose\n"); fclose(_f); }
        return;
    }
    virtual void GetSeatedZeroPoseToStandingAbsoluteTrackingPose(HmdMatrix34_t* ret) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetSeatedZeroPoseToStandingAbsoluteTrackingPose\n"); fclose(_f); }
        if(ret) *ret = Identity34_Gen();
        return;
    }
    virtual void GetRawZeroPoseToStandingAbsoluteTrackingPose(HmdMatrix34_t* ret) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetRawZeroPoseToStandingAbsoluteTrackingPose\n"); fclose(_f); }
        if(ret) *ret = Identity34_Gen();
        return;
    }
    virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetSortedTrackedDeviceIndicesOfClass\n"); fclose(_f); }
        return {};
    }
    virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetTrackedDeviceActivityLevel\n"); fclose(_f); }
        return {};
    }
    virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::ApplyTransform\n"); fclose(_f); }
        return;
    }
    virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetTrackedDeviceIndexForControllerRole\n"); fclose(_f); }
        return {};
    }
    virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetControllerRoleForTrackedDeviceIndex\n"); fclose(_f); }
        return {};
    }
    virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetTrackedDeviceClass\n"); fclose(_f); }
        return {};
    }
    virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::IsTrackedDeviceConnected\n"); fclose(_f); }
        return {};
    }
    virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetBoolTrackedDeviceProperty\n"); fclose(_f); }
        return {};
    }
    virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetFloatTrackedDeviceProperty\n"); fclose(_f); }
        return {};
    }
    virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetInt32TrackedDeviceProperty\n"); fclose(_f); }
        return {};
    }
    virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetUint64TrackedDeviceProperty\n"); fclose(_f); }
        return {};
    }
    virtual void GetMatrix34TrackedDeviceProperty(HmdMatrix34_t* ret, vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetMatrix34TrackedDeviceProperty\n"); fclose(_f); }
        if(ret) *ret = Identity34_Gen();
        return;
    }
    virtual uint32_t GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetArrayTrackedDeviceProperty\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetStringTrackedDeviceProperty\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetPropErrorNameFromEnum(ETrackedPropertyError error) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetPropErrorNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::PollNextEvent\n"); fclose(_f); }
        return {};
    }
    virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::PollNextEventWithPose\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetEventTypeNameFromEnum(EVREventType eType) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetEventTypeNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual void GetHiddenAreaMesh(HiddenAreaMesh_t* ret, EVREye eEye, EHiddenAreaMeshType type) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetHiddenAreaMesh\n"); fclose(_f); }
        if(ret) { ret->pVertexData = nullptr; ret->unTriangleCount = 0; }
        return;
    }
    virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetControllerState\n"); fclose(_f); }
        return {};
    }
    virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetControllerStateWithPose\n"); fclose(_f); }
        return {};
    }
    virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::TriggerHapticPulse\n"); fclose(_f); }
        return;
    }
    virtual const char * GetButtonIdNameFromEnum(EVRButtonId eButtonId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetButtonIdNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual const char * GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetControllerAxisTypeNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual bool IsInputAvailable() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::IsInputAvailable\n"); fclose(_f); }
        return {};
    }
    virtual bool IsSteamVRDrawingControllers() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::IsSteamVRDrawingControllers\n"); fclose(_f); }
        return {};
    }
    virtual bool ShouldApplicationPause() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::ShouldApplicationPause\n"); fclose(_f); }
        return {};
    }
    virtual bool ShouldApplicationReduceRenderingWork() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::ShouldApplicationReduceRenderingWork\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::PerformFirmwareUpdate\n"); fclose(_f); }
        return {};
    }
    virtual void AcknowledgeQuit_Exiting() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::AcknowledgeQuit_Exiting\n"); fclose(_f); }
        return;
    }
    virtual uint32_t GetAppContainerFilePaths(VR_OUT_STRING() char *pchBuffer, uint32_t unBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetAppContainerFilePaths\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetRuntimeVersion() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSystem::GetRuntimeVersion\n"); fclose(_f); }
        return "";
    }
};

class FakeApplications : public vr::IVRApplications {
public:
    virtual EVRApplicationError AddApplicationManifest(const char *pchApplicationManifestFullPath, bool bTemporary) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::AddApplicationManifest\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError RemoveApplicationManifest(const char *pchApplicationManifestFullPath) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::RemoveApplicationManifest\n"); fclose(_f); }
        return {};
    }
    virtual bool IsApplicationInstalled(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::IsApplicationInstalled\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetApplicationCount() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationCount\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError GetApplicationKeyByIndex(uint32_t unApplicationIndex, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationKeyByIndex\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError GetApplicationKeyByProcessId(uint32_t unProcessId, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationKeyByProcessId\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError LaunchApplication(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::LaunchApplication\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError LaunchTemplateApplication(const char *pchTemplateAppKey, const char *pchNewAppKey, VR_ARRAY_COUNT( unKeys ) const AppOverrideKeys_t *pKeys, uint32_t unKeys) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::LaunchTemplateApplication\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRApplicationError LaunchApplicationFromMimeType(const char *pchMimeType, const char *pchArgs) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::LaunchApplicationFromMimeType\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError LaunchDashboardOverlay(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::LaunchDashboardOverlay\n"); fclose(_f); }
        return {};
    }
    virtual bool CancelApplicationLaunch(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::CancelApplicationLaunch\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError IdentifyApplication(uint32_t unProcessId, const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::IdentifyApplication\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetApplicationProcessId(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationProcessId\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetApplicationsErrorNameFromEnum(EVRApplicationError error) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationsErrorNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual uint32_t GetApplicationPropertyString(const char *pchAppKey, EVRApplicationProperty eProperty, VR_OUT_STRING() char *pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, EVRApplicationError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationPropertyString\n"); fclose(_f); }
        return {};
    }
    virtual bool GetApplicationPropertyBool(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationPropertyBool\n"); fclose(_f); }
        return {};
    }
    virtual uint64_t GetApplicationPropertyUint64(const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationPropertyUint64\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError SetApplicationAutoLaunch(const char *pchAppKey, bool bAutoLaunch) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::SetApplicationAutoLaunch\n"); fclose(_f); }
        return {};
    }
    virtual bool GetApplicationAutoLaunch(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationAutoLaunch\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError SetDefaultApplicationForMimeType(const char *pchAppKey, const char *pchMimeType) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::SetDefaultApplicationForMimeType\n"); fclose(_f); }
        return {};
    }
    virtual bool GetDefaultApplicationForMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetDefaultApplicationForMimeType\n"); fclose(_f); }
        return {};
    }
    virtual bool GetApplicationSupportedMimeTypes(const char *pchAppKey, VR_OUT_STRING() char *pchMimeTypesBuffer, uint32_t unMimeTypesBuffer) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationSupportedMimeTypes\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetApplicationsThatSupportMimeType(const char *pchMimeType, VR_OUT_STRING() char *pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationsThatSupportMimeType\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetApplicationLaunchArguments(uint32_t unHandle, VR_OUT_STRING() char *pchArgs, uint32_t unArgs) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetApplicationLaunchArguments\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError GetStartingApplication(VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetStartingApplication\n"); fclose(_f); }
        return {};
    }
    virtual EVRSceneApplicationState GetSceneApplicationState() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetSceneApplicationState\n"); fclose(_f); }
        return {};
    }
    virtual EVRApplicationError PerformApplicationPrelaunchCheck(const char *pchAppKey) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::PerformApplicationPrelaunchCheck\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetSceneApplicationStateNameFromEnum(EVRSceneApplicationState state) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetSceneApplicationStateNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual EVRApplicationError LaunchInternalProcess(const char *pchBinaryPath, const char *pchArguments, const char *pchWorkingDirectory) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::LaunchInternalProcess\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetCurrentSceneProcessId() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRApplications::GetCurrentSceneProcessId\n"); fclose(_f); }
        return {};
    }
};

class FakeSettings : public vr::IVRSettings {
public:
    virtual const char * GetSettingsErrorNameFromEnum(EVRSettingsError eError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::GetSettingsErrorNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual void SetBool(const char *pchSection, const char *pchSettingsKey, bool bValue, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::SetBool\n"); fclose(_f); }
        return;
    }
    virtual void SetInt32(const char *pchSection, const char *pchSettingsKey, int32_t nValue, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::SetInt32\n"); fclose(_f); }
        return;
    }
    virtual void SetFloat(const char *pchSection, const char *pchSettingsKey, float flValue, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::SetFloat\n"); fclose(_f); }
        return;
    }
    virtual void SetString(const char *pchSection, const char *pchSettingsKey, const char *pchValue, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::SetString\n"); fclose(_f); }
        return;
    }
    virtual bool GetBool(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::GetBool\n"); fclose(_f); }
        return {};
    }
    virtual int32_t GetInt32(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::GetInt32\n"); fclose(_f); }
        return {};
    }
    virtual float GetFloat(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::GetFloat\n"); fclose(_f); }
        return {};
    }
    virtual void GetString(const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::GetString\n"); fclose(_f); }
        return;
    }
    virtual void RemoveSection(const char *pchSection, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::RemoveSection\n"); fclose(_f); }
        return;
    }
    virtual void RemoveKeyInSection(const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSettings::RemoveKeyInSection\n"); fclose(_f); }
        return;
    }
};

class FakeChaperone : public vr::IVRChaperone {
public:
    virtual ChaperoneCalibrationState GetCalibrationState() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::GetCalibrationState\n"); fclose(_f); }
        return {};
    }
    virtual bool GetPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::GetPlayAreaSize\n"); fclose(_f); }
        return {};
    }
    virtual bool GetPlayAreaRect(HmdQuad_t *rect) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::GetPlayAreaRect\n"); fclose(_f); }
        return {};
    }
    virtual void ReloadInfo(void) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::ReloadInfo\n"); fclose(_f); }
        return;
    }
    virtual void SetSceneColor(HmdColor_t color) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::SetSceneColor\n"); fclose(_f); }
        return;
    }
    virtual void GetBoundsColor(HmdColor_t *pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, HmdColor_t *pOutputCameraColor) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::GetBoundsColor\n"); fclose(_f); }
        return;
    }
    virtual bool AreBoundsVisible() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::AreBoundsVisible\n"); fclose(_f); }
        return {};
    }
    virtual void ForceBoundsVisible(bool bForce) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperone::ForceBoundsVisible\n"); fclose(_f); }
        return;
    }
};

class FakeChaperoneSetup : public vr::IVRChaperoneSetup {
public:
    virtual bool CommitWorkingCopy(EChaperoneConfigFile configFile) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::CommitWorkingCopy\n"); fclose(_f); }
        return {};
    }
    virtual void RevertWorkingCopy() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::RevertWorkingCopy\n"); fclose(_f); }
        return;
    }
    virtual bool GetWorkingPlayAreaSize(float *pSizeX, float *pSizeZ) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetWorkingPlayAreaSize\n"); fclose(_f); }
        return {};
    }
    virtual bool GetWorkingPlayAreaRect(HmdQuad_t *rect) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetWorkingPlayAreaRect\n"); fclose(_f); }
        return {};
    }
    virtual bool GetWorkingCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetWorkingCollisionBoundsInfo\n"); fclose(_f); }
        return {};
    }
    virtual bool GetLiveCollisionBoundsInfo(VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetLiveCollisionBoundsInfo\n"); fclose(_f); }
        return {};
    }
    virtual bool GetWorkingSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetWorkingSeatedZeroPoseToRawTrackingPose\n"); fclose(_f); }
        return {};
    }
    virtual bool GetWorkingStandingZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatStandingZeroPoseToRawTrackingPose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetWorkingStandingZeroPoseToRawTrackingPose\n"); fclose(_f); }
        return {};
    }
    virtual void SetWorkingPlayAreaSize(float sizeX, float sizeZ) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::SetWorkingPlayAreaSize\n"); fclose(_f); }
        return;
    }
    virtual void SetWorkingCollisionBoundsInfo(VR_ARRAY_COUNT(unQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t unQuadsCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::SetWorkingCollisionBoundsInfo\n"); fclose(_f); }
        return;
    }
    virtual void SetWorkingPerimeter(VR_ARRAY_COUNT( unPointCount ) HmdVector2_t *pPointBuffer, uint32_t unPointCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::SetWorkingPerimeter\n"); fclose(_f); }
        return;
    }
    virtual void SetWorkingSeatedZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatSeatedZeroPoseToRawTrackingPose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::SetWorkingSeatedZeroPoseToRawTrackingPose\n"); fclose(_f); }
        return;
    }
    virtual void SetWorkingStandingZeroPoseToRawTrackingPose(const HmdMatrix34_t *pMatStandingZeroPoseToRawTrackingPose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::SetWorkingStandingZeroPoseToRawTrackingPose\n"); fclose(_f); }
        return;
    }
    virtual void ReloadFromDisk(EChaperoneConfigFile configFile) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::ReloadFromDisk\n"); fclose(_f); }
        return;
    }
    virtual bool GetLiveSeatedZeroPoseToRawTrackingPose(HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::GetLiveSeatedZeroPoseToRawTrackingPose\n"); fclose(_f); }
        return {};
    }
    virtual bool ExportLiveToBuffer(VR_OUT_STRING() char *pBuffer, uint32_t *pnBufferLength) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::ExportLiveToBuffer\n"); fclose(_f); }
        return {};
    }
    virtual bool ImportFromBufferToWorking(const char *pBuffer, uint32_t nImportFlags) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::ImportFromBufferToWorking\n"); fclose(_f); }
        return {};
    }
    virtual void ShowWorkingSetPreview() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::ShowWorkingSetPreview\n"); fclose(_f); }
        return;
    }
    virtual void HideWorkingSetPreview() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::HideWorkingSetPreview\n"); fclose(_f); }
        return;
    }
    virtual void RoomSetupStarting() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRChaperoneSetup::RoomSetupStarting\n"); fclose(_f); }
        return;
    }
};

class FakeCompositor : public vr::IVRCompositor {
public:
    virtual void SetTrackingSpace(ETrackingUniverseOrigin eOrigin) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::SetTrackingSpace\n"); fclose(_f); }
        return;
    }
    virtual ETrackingUniverseOrigin GetTrackingSpace() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetTrackingSpace\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError WaitGetPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::WaitGetPoses\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError GetLastPoses(VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetLastPoses\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError GetLastPoseForTrackedDeviceIndex(TrackedDeviceIndex_t unDeviceIndex, TrackedDevicePose_t *pOutputPose, TrackedDevicePose_t *pOutputGamePose) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetLastPoseForTrackedDeviceIndex\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError Submit(EVREye eEye, const Texture_t *pTexture, const VRTextureBounds_t* pBounds, EVRSubmitFlags nSubmitFlags) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::Submit\n"); fclose(_f); }
        return {};
    }
    virtual void ClearLastSubmittedFrame() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ClearLastSubmittedFrame\n"); fclose(_f); }
        return;
    }
    virtual void PostPresentHandoff() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::PostPresentHandoff\n"); fclose(_f); }
        return;
    }
    virtual bool GetFrameTiming(Compositor_FrameTiming *pTiming, uint32_t unFramesAgo) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetFrameTiming\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetFrameTimings(VR_ARRAY_COUNT( nFrames ) Compositor_FrameTiming *pTiming, uint32_t nFrames) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetFrameTimings\n"); fclose(_f); }
        return {};
    }
    virtual float GetFrameTimeRemaining() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetFrameTimeRemaining\n"); fclose(_f); }
        return {};
    }
    virtual void GetCumulativeStats(Compositor_CumulativeStats *pStats, uint32_t nStatsSizeInBytes) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetCumulativeStats\n"); fclose(_f); }
        return;
    }
    virtual void FadeToColor(float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::FadeToColor\n"); fclose(_f); }
        return;
    }
    virtual void GetCurrentFadeColor(HmdColor_t* ret, bool bBackground) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetCurrentFadeColor\n"); fclose(_f); }
        if(ret) { ret->r = 0; ret->g = 0; ret->b = 0; ret->a = 0; }
        return;
    }
    virtual void FadeGrid(float fSeconds, bool bFadeIn) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::FadeGrid\n"); fclose(_f); }
        return;
    }
    virtual float GetCurrentGridAlpha() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetCurrentGridAlpha\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError SetSkyboxOverride(VR_ARRAY_COUNT( unTextureCount ) const Texture_t *pTextures, uint32_t unTextureCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::SetSkyboxOverride\n"); fclose(_f); }
        return {};
    }
    virtual void ClearSkyboxOverride() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ClearSkyboxOverride\n"); fclose(_f); }
        return;
    }
    virtual void CompositorBringToFront() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::CompositorBringToFront\n"); fclose(_f); }
        return;
    }
    virtual void CompositorGoToBack() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::CompositorGoToBack\n"); fclose(_f); }
        return;
    }
    virtual void CompositorQuit() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::CompositorQuit\n"); fclose(_f); }
        return;
    }
    virtual bool IsFullscreen() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::IsFullscreen\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetCurrentSceneFocusProcess() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetCurrentSceneFocusProcess\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetLastFrameRenderer() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetLastFrameRenderer\n"); fclose(_f); }
        return {};
    }
    virtual bool CanRenderScene() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::CanRenderScene\n"); fclose(_f); }
        return {};
    }
    virtual void ShowMirrorWindow() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ShowMirrorWindow\n"); fclose(_f); }
        return;
    }
    virtual void HideMirrorWindow() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::HideMirrorWindow\n"); fclose(_f); }
        return;
    }
    virtual bool IsMirrorWindowVisible() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::IsMirrorWindowVisible\n"); fclose(_f); }
        return {};
    }
    virtual void CompositorDumpImages() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::CompositorDumpImages\n"); fclose(_f); }
        return;
    }
    virtual bool ShouldAppRenderWithLowResources() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ShouldAppRenderWithLowResources\n"); fclose(_f); }
        return {};
    }
    virtual void ForceInterleavedReprojectionOn(bool bOverride) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ForceInterleavedReprojectionOn\n"); fclose(_f); }
        return;
    }
    virtual void ForceReconnectProcess() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ForceReconnectProcess\n"); fclose(_f); }
        return;
    }
    virtual void SuspendRendering(bool bSuspend) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::SuspendRendering\n"); fclose(_f); }
        return;
    }
    virtual vr::EVRCompositorError GetMirrorTextureD3D11(vr::EVREye eEye, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetMirrorTextureD3D11\n"); fclose(_f); }
        return {};
    }
    virtual void ReleaseMirrorTextureD3D11(void *pD3D11ShaderResourceView) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ReleaseMirrorTextureD3D11\n"); fclose(_f); }
        return;
    }
    virtual vr::EVRCompositorError GetMirrorTextureGL(vr::EVREye eEye, vr::glUInt_t *pglTextureId, vr::glSharedTextureHandle_t *pglSharedTextureHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetMirrorTextureGL\n"); fclose(_f); }
        return {};
    }
    virtual bool ReleaseSharedGLTexture(vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ReleaseSharedGLTexture\n"); fclose(_f); }
        return {};
    }
    virtual void LockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::LockGLSharedTextureForAccess\n"); fclose(_f); }
        return;
    }
    virtual void UnlockGLSharedTextureForAccess(vr::glSharedTextureHandle_t glSharedTextureHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::UnlockGLSharedTextureForAccess\n"); fclose(_f); }
        return;
    }
    virtual uint32_t GetVulkanInstanceExtensionsRequired(VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetVulkanInstanceExtensionsRequired\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetVulkanDeviceExtensionsRequired(VkPhysicalDevice_T *pPhysicalDevice, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetVulkanDeviceExtensionsRequired\n"); fclose(_f); }
        return {};
    }
    virtual void SetExplicitTimingMode(EVRCompositorTimingMode eTimingMode) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::SetExplicitTimingMode\n"); fclose(_f); }
        return;
    }
    virtual EVRCompositorError SubmitExplicitTimingData() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::SubmitExplicitTimingData\n"); fclose(_f); }
        return {};
    }
    virtual bool IsMotionSmoothingEnabled() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::IsMotionSmoothingEnabled\n"); fclose(_f); }
        return {};
    }
    virtual bool IsMotionSmoothingSupported() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::IsMotionSmoothingSupported\n"); fclose(_f); }
        return {};
    }
    virtual bool IsCurrentSceneFocusAppLoading() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::IsCurrentSceneFocusAppLoading\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError SetStageOverride_Async(const char *pchRenderModelPath, const HmdMatrix34_t *pTransform, const Compositor_StageRenderSettings *pRenderSettings, uint32_t nSizeOfRenderSettings) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::SetStageOverride_Async\n"); fclose(_f); }
        return {};
    }
    virtual void ClearStageOverride() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::ClearStageOverride\n"); fclose(_f); }
        return;
    }
    virtual bool GetCompositorBenchmarkResults(Compositor_BenchmarkResults *pBenchmarkResults, uint32_t nSizeOfBenchmarkResults) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetCompositorBenchmarkResults\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError GetLastPosePredictionIDs(uint32_t *pRenderPosePredictionID, uint32_t *pGamePosePredictionID) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetLastPosePredictionIDs\n"); fclose(_f); }
        return {};
    }
    virtual EVRCompositorError GetPosesForFrame(uint32_t unPosePredictionID, VR_ARRAY_COUNT( unPoseArrayCount ) TrackedDevicePose_t* pPoseArray, uint32_t unPoseArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRCompositor::GetPosesForFrame\n"); fclose(_f); }
        return {};
    }
};

class FakeHeadsetView : public vr::IVRHeadsetView {
public:
    virtual void SetHeadsetViewSize(uint32_t nWidth, uint32_t nHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::SetHeadsetViewSize\n"); fclose(_f); }
        return;
    }
    virtual void GetHeadsetViewSize(uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::GetHeadsetViewSize\n"); fclose(_f); }
        return;
    }
    virtual void SetHeadsetViewMode(HeadsetViewMode_t eHeadsetViewMode) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::SetHeadsetViewMode\n"); fclose(_f); }
        return;
    }
    virtual HeadsetViewMode_t GetHeadsetViewMode() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::GetHeadsetViewMode\n"); fclose(_f); }
        return {};
    }
    virtual void SetHeadsetViewCropped(bool bCropped) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::SetHeadsetViewCropped\n"); fclose(_f); }
        return;
    }
    virtual bool GetHeadsetViewCropped() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::GetHeadsetViewCropped\n"); fclose(_f); }
        return {};
    }
    virtual float GetHeadsetViewAspectRatio() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::GetHeadsetViewAspectRatio\n"); fclose(_f); }
        return {};
    }
    virtual void SetHeadsetViewBlendRange(float flStartPct, float flEndPct) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::SetHeadsetViewBlendRange\n"); fclose(_f); }
        return;
    }
    virtual void GetHeadsetViewBlendRange(float *pStartPct, float *pEndPct) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRHeadsetView::GetHeadsetViewBlendRange\n"); fclose(_f); }
        return;
    }
};

class FakeNotifications : public vr::IVRNotifications {
public:
    virtual EVRNotificationError CreateNotification(VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, EVRNotificationType type, const char *pchText, EVRNotificationStyle style, const NotificationBitmap_t *pImage, VRNotificationId *pNotificationId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRNotifications::CreateNotification\n"); fclose(_f); }
        return {};
    }
    virtual EVRNotificationError RemoveNotification(VRNotificationId notificationId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRNotifications::RemoveNotification\n"); fclose(_f); }
        return {};
    }
};

class FakeOverlay : public vr::IVROverlay {
public:
    virtual EVROverlayError FindOverlay(const char *pchOverlayKey, VROverlayHandle_t * pOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::FindOverlay\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError CreateOverlay(const char *pchOverlayKey, const char *pchOverlayName, VROverlayHandle_t * pOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::CreateOverlay\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError DestroyOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::DestroyOverlay\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetOverlayKey(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayKey\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetOverlayName(VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayName\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayName(VROverlayHandle_t ulOverlayHandle, const char *pchName) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayName\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayImageData(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unBufferSize, uint32_t *punWidth, uint32_t *punHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayImageData\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetOverlayErrorNameFromEnum(EVROverlayError error) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayErrorNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual EVROverlayError SetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle, uint32_t unPID) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayRenderingPid\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetOverlayRenderingPid(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayRenderingPid\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayFlag\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayFlag(VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool *pbEnabled) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayFlag\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayFlags(VROverlayHandle_t ulOverlayHandle, uint32_t *pFlags) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayFlags\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayColor(VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayColor\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayColor(VROverlayHandle_t ulOverlayHandle, float *pfRed, float *pfGreen, float *pfBlue) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayColor\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float fAlpha) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayAlpha\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayAlpha(VROverlayHandle_t ulOverlayHandle, float *pfAlpha) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayAlpha\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float fTexelAspect) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTexelAspect\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTexelAspect(VROverlayHandle_t ulOverlayHandle, float *pfTexelAspect) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTexelAspect\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlaySortOrder\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlaySortOrder(VROverlayHandle_t ulOverlayHandle, uint32_t *punSortOrder) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlaySortOrder\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float fWidthInMeters) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayWidthInMeters\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayWidthInMeters(VROverlayHandle_t ulOverlayHandle, float *pfWidthInMeters) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayWidthInMeters\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float fCurvature) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayCurvature\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayCurvature(VROverlayHandle_t ulOverlayHandle, float *pfCurvature) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayCurvature\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace eTextureColorSpace) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTextureColorSpace\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTextureColorSpace(VROverlayHandle_t ulOverlayHandle, EColorSpace *peTextureColorSpace) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTextureColorSpace\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, const VRTextureBounds_t *pOverlayTextureBounds) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTextureBounds\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTextureBounds(VROverlayHandle_t ulOverlayHandle, VRTextureBounds_t *pOverlayTextureBounds) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTextureBounds\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTransformType(VROverlayHandle_t ulOverlayHandle, VROverlayTransformType *peTransformType) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTransformType\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTransformAbsolute\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTransformAbsolute(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin *peTrackingOrigin, HmdMatrix34_t *pmatTrackingOriginToOverlayTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTransformAbsolute\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTransformTrackedDeviceRelative\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceRelative(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punTrackedDevice, HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTransformTrackedDeviceRelative\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unDeviceIndex, const char *pchComponentName) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTransformTrackedDeviceComponent\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceComponent(VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punDeviceIndex, VR_OUT_STRING() char *pchComponentName, uint32_t unComponentNameSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTransformTrackedDeviceComponent\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVROverlayError GetOverlayTransformOverlayRelative(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t *ulOverlayHandleParent, HmdMatrix34_t *pmatParentOverlayToOverlayTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTransformOverlayRelative\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVROverlayError SetOverlayTransformOverlayRelative(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulOverlayHandleParent, const HmdMatrix34_t *pmatParentOverlayToOverlayTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTransformOverlayRelative\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTransformCursor(VROverlayHandle_t ulCursorOverlayHandle, const HmdVector2_t *pvHotspot) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTransformCursor\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVROverlayError GetOverlayTransformCursor(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvHotspot) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTransformCursor\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ShowOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ShowOverlay\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError HideOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::HideOverlay\n"); fclose(_f); }
        return {};
    }
    virtual bool IsOverlayVisible(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::IsOverlayVisible\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetTransformForOverlayCoordinates(VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, HmdVector2_t coordinatesInOverlay, HmdMatrix34_t *pmatTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetTransformForOverlayCoordinates\n"); fclose(_f); }
        return {};
    }
    virtual bool PollNextOverlayEvent(VROverlayHandle_t ulOverlayHandle, VREvent_t *pEvent, uint32_t uncbVREvent) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::PollNextOverlayEvent\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod *peInputMethod) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayInputMethod\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayInputMethod(VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayInputMethod\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvecMouseScale) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayMouseScale\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayMouseScale(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvecMouseScale) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayMouseScale\n"); fclose(_f); }
        return {};
    }
    virtual bool ComputeOverlayIntersection(VROverlayHandle_t ulOverlayHandle, const VROverlayIntersectionParams_t *pParams, VROverlayIntersectionResults_t *pResults) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ComputeOverlayIntersection\n"); fclose(_f); }
        return {};
    }
    virtual bool IsHoverTargetOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::IsHoverTargetOverlay\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayIntersectionMask(VROverlayHandle_t ulOverlayHandle, VROverlayIntersectionMaskPrimitive_t *pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayIntersectionMask\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError TriggerLaserMouseHapticVibration(VROverlayHandle_t ulOverlayHandle, float fDurationSeconds, float fFrequency, float fAmplitude) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::TriggerLaserMouseHapticVibration\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayCursor(VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulCursorHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayCursor\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvCursor) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayCursorPositionOverride\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ClearOverlayCursorPositionOverride(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ClearOverlayCursorPositionOverride\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayTexture(VROverlayHandle_t ulOverlayHandle, const Texture_t *pTexture) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayTexture\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ClearOverlayTexture(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ClearOverlayTexture\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayRaw(VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unBytesPerPixel) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayRaw\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetOverlayFromFile(VROverlayHandle_t ulOverlayHandle, const char *pchFilePath) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetOverlayFromFile\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTexture(VROverlayHandle_t ulOverlayHandle, void **pNativeTextureHandle, void *pNativeTextureRef, uint32_t *pWidth, uint32_t *pHeight, uint32_t *pNativeFormat, ETextureType *pAPIType, EColorSpace *pColorSpace, VRTextureBounds_t *pTextureBounds) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTexture\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ReleaseNativeOverlayHandle(VROverlayHandle_t ulOverlayHandle, void *pNativeTextureHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ReleaseNativeOverlayHandle\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetOverlayTextureSize(VROverlayHandle_t ulOverlayHandle, uint32_t *pWidth, uint32_t *pHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetOverlayTextureSize\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError CreateDashboardOverlay(const char *pchOverlayKey, const char *pchOverlayFriendlyName, VROverlayHandle_t * pMainHandle, VROverlayHandle_t *pThumbnailHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::CreateDashboardOverlay\n"); fclose(_f); }
        return {};
    }
    virtual bool IsDashboardVisible() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::IsDashboardVisible\n"); fclose(_f); }
        return {};
    }
    virtual bool IsActiveDashboardOverlay(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::IsActiveDashboardOverlay\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError SetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetDashboardOverlaySceneProcess\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError GetDashboardOverlaySceneProcess(VROverlayHandle_t ulOverlayHandle, uint32_t *punProcessId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetDashboardOverlaySceneProcess\n"); fclose(_f); }
        return {};
    }
    virtual void ShowDashboard(const char *pchOverlayToShow) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ShowDashboard\n"); fclose(_f); }
        return;
    }
    virtual vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetPrimaryDashboardDevice\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ShowKeyboard(EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ShowKeyboard\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ShowKeyboardForOverlay(VROverlayHandle_t ulOverlayHandle, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ShowKeyboardForOverlay\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetKeyboardText(VR_OUT_STRING() char *pchText, uint32_t cchText) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::GetKeyboardText\n"); fclose(_f); }
        return {};
    }
    virtual void HideKeyboard() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::HideKeyboard\n"); fclose(_f); }
        return;
    }
    virtual void SetKeyboardTransformAbsolute(ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToKeyboardTransform) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetKeyboardTransformAbsolute\n"); fclose(_f); }
        return;
    }
    virtual void SetKeyboardPositionForOverlay(VROverlayHandle_t ulOverlayHandle, HmdRect2_t avoidRect) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::SetKeyboardPositionForOverlay\n"); fclose(_f); }
        return;
    }
    virtual VRMessageOverlayResponse ShowMessageOverlay(const char* pchText, const char* pchCaption, const char* pchButton0Text, const char* pchButton1Text, const char* pchButton2Text, const char* pchButton3Text) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::ShowMessageOverlay\n"); fclose(_f); }
        return {};
    }
    virtual void CloseMessageOverlay() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlay::CloseMessageOverlay\n"); fclose(_f); }
        return;
    }
};

class FakeOverlayView : public vr::IVROverlayView {
public:
    virtual EVROverlayError AcquireOverlayView(VROverlayHandle_t ulOverlayHandle, VRNativeDevice_t *pNativeDevice, VROverlayView_t *pOverlayView, uint32_t unOverlayViewSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlayView::AcquireOverlayView\n"); fclose(_f); }
        return {};
    }
    virtual EVROverlayError ReleaseOverlayView(VROverlayView_t *pOverlayView) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlayView::ReleaseOverlayView\n"); fclose(_f); }
        return {};
    }
    virtual void PostOverlayEvent(VROverlayHandle_t ulOverlayHandle, const VREvent_t *pvrEvent) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlayView::PostOverlayEvent\n"); fclose(_f); }
        return;
    }
    virtual bool IsViewingPermitted(VROverlayHandle_t ulOverlayHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVROverlayView::IsViewingPermitted\n"); fclose(_f); }
        return {};
    }
};

class FakeRenderModels : public vr::IVRRenderModels {
public:
    virtual EVRRenderModelError LoadRenderModel_Async(const char *pchRenderModelName, RenderModel_t **ppRenderModel) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::LoadRenderModel_Async\n"); fclose(_f); }
        return {};
    }
    virtual void FreeRenderModel(RenderModel_t *pRenderModel) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::FreeRenderModel\n"); fclose(_f); }
        return;
    }
    virtual EVRRenderModelError LoadTexture_Async(TextureID_t textureId, RenderModel_TextureMap_t **ppTexture) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::LoadTexture_Async\n"); fclose(_f); }
        return {};
    }
    virtual void FreeTexture(RenderModel_TextureMap_t *pTexture) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::FreeTexture\n"); fclose(_f); }
        return;
    }
    virtual EVRRenderModelError LoadTextureD3D11_Async(TextureID_t textureId, void *pD3D11Device, void **ppD3D11Texture2D) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::LoadTextureD3D11_Async\n"); fclose(_f); }
        return {};
    }
    virtual EVRRenderModelError LoadIntoTextureD3D11_Async(TextureID_t textureId, void *pDstTexture) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::LoadIntoTextureD3D11_Async\n"); fclose(_f); }
        return {};
    }
    virtual void FreeTextureD3D11(void *pD3D11Texture2D) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::FreeTextureD3D11\n"); fclose(_f); }
        return;
    }
    virtual uint32_t GetRenderModelName(uint32_t unRenderModelIndex, VR_OUT_STRING() char *pchRenderModelName, uint32_t unRenderModelNameLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetRenderModelName\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetRenderModelCount() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetRenderModelCount\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetComponentCount(const char *pchRenderModelName) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetComponentCount\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetComponentName(const char *pchRenderModelName, uint32_t unComponentIndex, VR_OUT_STRING( ) char *pchComponentName, uint32_t unComponentNameLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetComponentName\n"); fclose(_f); }
        return {};
    }
    virtual uint64_t GetComponentButtonMask(const char *pchRenderModelName, const char *pchComponentName) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetComponentButtonMask\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetComponentRenderModelName(const char *pchRenderModelName, const char *pchComponentName, VR_OUT_STRING( ) char *pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetComponentRenderModelName\n"); fclose(_f); }
        return {};
    }
    virtual bool GetComponentStateForDevicePath(const char *pchRenderModelName, const char *pchComponentName, vr::VRInputValueHandle_t devicePath, const vr::RenderModel_ControllerMode_State_t *pState, vr::RenderModel_ComponentState_t *pComponentState) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetComponentStateForDevicePath\n"); fclose(_f); }
        return {};
    }
    virtual bool GetComponentState(const char *pchRenderModelName, const char *pchComponentName, const vr::VRControllerState_t *pControllerState, const RenderModel_ControllerMode_State_t *pState, RenderModel_ComponentState_t *pComponentState) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetComponentState\n"); fclose(_f); }
        return {};
    }
    virtual bool RenderModelHasComponent(const char *pchRenderModelName, const char *pchComponentName) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::RenderModelHasComponent\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetRenderModelThumbnailURL(const char *pchRenderModelName, VR_OUT_STRING() char *pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetRenderModelThumbnailURL\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetRenderModelOriginalPath(const char *pchRenderModelName, VR_OUT_STRING() char *pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError *peError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetRenderModelOriginalPath\n"); fclose(_f); }
        return {};
    }
    virtual const char * GetRenderModelErrorNameFromEnum(vr::EVRRenderModelError error) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRRenderModels::GetRenderModelErrorNameFromEnum\n"); fclose(_f); }
        return "";
    }
};

class FakeExtendedDisplay : public vr::IVRExtendedDisplay {
public:
    virtual void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRExtendedDisplay::GetWindowBounds\n"); fclose(_f); }
        return;
    }
    virtual void GetEyeOutputViewport(EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRExtendedDisplay::GetEyeOutputViewport\n"); fclose(_f); }
        return;
    }
    virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex, int32_t *pnAdapterOutputIndex) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRExtendedDisplay::GetDXGIOutputInfo\n"); fclose(_f); }
        return;
    }
};

class FakeTrackedCamera : public vr::IVRTrackedCamera {
public:
    virtual const char * GetCameraErrorNameFromEnum(vr::EVRTrackedCameraError eCameraError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetCameraErrorNameFromEnum\n"); fclose(_f); }
        return "";
    }
    virtual vr::EVRTrackedCameraError HasCamera(vr::TrackedDeviceIndex_t nDeviceIndex, bool *pHasCamera) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::HasCamera\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetCameraFrameSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t *pnWidth, uint32_t *pnHeight, uint32_t *pnFrameBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetCameraFrameSize\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetCameraIntrinsics(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t *pFocalLength, vr::HmdVector2_t *pCenter) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetCameraIntrinsics\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetCameraProjection(vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t *pProjection) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetCameraProjection\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError AcquireVideoStreamingService(vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t *pHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::AcquireVideoStreamingService\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamingService(vr::TrackedCameraHandle_t hTrackedCamera) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::ReleaseVideoStreamingService\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamFrameBuffer(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetVideoStreamFrameBuffer\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureSize(vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t *pTextureBounds, uint32_t *pnWidth, uint32_t *pnHeight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetVideoStreamTextureSize\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureD3D11(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetVideoStreamTextureD3D11\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t *pglTextureId, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetVideoStreamTextureGL\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL(vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::ReleaseVideoStreamTextureGL\n"); fclose(_f); }
        return {};
    }
    virtual void SetCameraTrackingSpace(vr::ETrackingUniverseOrigin eUniverse) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::SetCameraTrackingSpace\n"); fclose(_f); }
        return;
    }
    virtual vr::ETrackingUniverseOrigin GetCameraTrackingSpace() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRTrackedCamera::GetCameraTrackingSpace\n"); fclose(_f); }
        return {};
    }
};

class FakeScreenshots : public vr::IVRScreenshots {
public:
    virtual vr::EVRScreenshotError RequestScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, vr::EVRScreenshotType type, const char *pchPreviewFilename, const char *pchVRFilename) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::RequestScreenshot\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRScreenshotError HookScreenshot(VR_ARRAY_COUNT( numTypes ) const vr::EVRScreenshotType *pSupportedTypes, int numTypes) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::HookScreenshot\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRScreenshotType GetScreenshotPropertyType(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::GetScreenshotPropertyType\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetScreenshotPropertyFilename(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, VR_OUT_STRING() char *pchFilename, uint32_t cchFilename, vr::EVRScreenshotError *pError) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::GetScreenshotPropertyFilename\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRScreenshotError UpdateScreenshotProgress(vr::ScreenshotHandle_t screenshotHandle, float flProgress) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::UpdateScreenshotProgress\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRScreenshotError TakeStereoScreenshot(vr::ScreenshotHandle_t *pOutScreenshotHandle, const char *pchPreviewFilename, const char *pchVRFilename) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::TakeStereoScreenshot\n"); fclose(_f); }
        return {};
    }
    virtual vr::EVRScreenshotError SubmitScreenshot(vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char *pchSourcePreviewFilename, const char *pchSourceVRFilename) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRScreenshots::SubmitScreenshot\n"); fclose(_f); }
        return {};
    }
};

class FakeResources : public vr::IVRResources {
public:
    virtual uint32_t LoadSharedResource(const char *pchResourceName, char *pchBuffer, uint32_t unBufferLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRResources::LoadSharedResource\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetResourceFullPath(const char *pchResourceName, const char *pchResourceTypeDirectory, VR_OUT_STRING() char *pchPathBuffer, uint32_t unBufferLen) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRResources::GetResourceFullPath\n"); fclose(_f); }
        return {};
    }
};

class FakeDriverManager : public vr::IVRDriverManager {
public:
    virtual uint32_t GetDriverCount() const override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDriverManager::GetDriverCount\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t GetDriverName(vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDriverManager::GetDriverName\n"); fclose(_f); }
        return {};
    }
    virtual DriverHandle_t GetDriverHandle(const char *pchDriverName) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDriverManager::GetDriverHandle\n"); fclose(_f); }
        return {};
    }
    virtual bool IsEnabled(vr::DriverId_t nDriver) const override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDriverManager::IsEnabled\n"); fclose(_f); }
        return {};
    }
};

class FakeInput : public vr::IVRInput {
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::SetActionManifestPath\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetActionSetHandle\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetActionHandle\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetInputSourceHandle\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError UpdateActionState(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::UpdateActionState\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetDigitalActionData\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetAnalogActionData\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetPoseActionDataRelativeToNow(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetPoseActionDataRelativeToNow\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetPoseActionDataForNextFrame\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetSkeletalActionData(VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetSkeletalActionData\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetDominantHand(ETrackedControllerRole *peDominantHand) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetDominantHand\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError SetDominantHand(ETrackedControllerRole eDominantHand) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::SetDominantHand\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetBoneCount(VRActionHandle_t action, uint32_t* pBoneCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetBoneCount\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetBoneHierarchy(VRActionHandle_t action, VR_ARRAY_COUNT( unIndexArayCount ) BoneIndex_t* pParentIndices, uint32_t unIndexArayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetBoneHierarchy\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetBoneName(VRActionHandle_t action, BoneIndex_t nBoneIndex, VR_OUT_STRING() char* pchBoneName, uint32_t unNameBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetBoneName\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetSkeletalReferenceTransforms(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalReferencePose eReferencePose, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetSkeletalReferenceTransforms\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetSkeletalTrackingLevel(VRActionHandle_t action, EVRSkeletalTrackingLevel* pSkeletalTrackingLevel) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetSkeletalTrackingLevel\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetSkeletalBoneData(VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalMotionRange eMotionRange, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetSkeletalBoneData\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetSkeletalSummaryData(VRActionHandle_t action, EVRSummaryType eSummaryType, VRSkeletalSummaryData_t * pSkeletalSummaryData) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetSkeletalSummaryData\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetSkeletalBoneDataCompressed(VRActionHandle_t action, EVRSkeletalMotionRange eMotionRange, VR_OUT_BUFFER_COUNT( unCompressedSize ) void *pvCompressedData, uint32_t unCompressedSize, uint32_t *punRequiredCompressedSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetSkeletalBoneDataCompressed\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError DecompressSkeletalBoneData(const void *pvCompressedBuffer, uint32_t unCompressedBufferSize, EVRSkeletalTransformSpace eTransformSpace, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::DecompressSkeletalBoneData\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError TriggerHapticVibrationAction(VRActionHandle_t action, float fStartSecondsFromNow, float fDurationSeconds, float fFrequency, float fAmplitude, VRInputValueHandle_t ulRestrictToDevice) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::TriggerHapticVibrationAction\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t digitalActionHandle, VR_ARRAY_COUNT( originOutCount ) VRInputValueHandle_t *originsOut, uint32_t originOutCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetActionOrigins\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetOriginLocalizedName(VRInputValueHandle_t origin, VR_OUT_STRING() char *pchNameArray, uint32_t unNameArraySize, int32_t unStringSectionsToInclude) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetOriginLocalizedName\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetOriginTrackedDeviceInfo(VRInputValueHandle_t origin, InputOriginInfo_t *pOriginInfo, uint32_t unOriginInfoSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetOriginTrackedDeviceInfo\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetActionBindingInfo(VRActionHandle_t action, InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, uint32_t *punReturnedBindingInfoCount) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetActionBindingInfo\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError ShowActionOrigins(VRActionSetHandle_t actionSetHandle, VRActionHandle_t ulActionHandle) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::ShowActionOrigins\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError ShowBindingsForActionSet(VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount, VRInputValueHandle_t originToHighlight) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::ShowBindingsForActionSet\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetComponentStateForBinding(const char *pchRenderModelName, const char *pchComponentName, const InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, vr::RenderModel_ComponentState_t *pComponentState) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetComponentStateForBinding\n"); fclose(_f); }
        return {};
    }
    virtual bool IsUsingLegacyInput() override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::IsUsingLegacyInput\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError OpenBindingUI(const char* pchAppKey, VRActionSetHandle_t ulActionSetHandle, VRInputValueHandle_t ulDeviceHandle, bool bShowOnDesktop) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::OpenBindingUI\n"); fclose(_f); }
        return {};
    }
    virtual EVRInputError GetBindingVariant(vr::VRInputValueHandle_t ulDevicePath, VR_OUT_STRING() char *pchVariantArray, uint32_t unVariantArraySize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRInput::GetBindingVariant\n"); fclose(_f); }
        return {};
    }
};

class FakeIOBuffer : public vr::IVRIOBuffer {
public:
    virtual vr::EIOBufferError Open(const char *pchPath, vr::EIOBufferMode mode, uint32_t unElementSize, uint32_t unElements, vr::IOBufferHandle_t *pulBuffer) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRIOBuffer::Open\n"); fclose(_f); }
        return {};
    }
    virtual vr::EIOBufferError Close(vr::IOBufferHandle_t ulBuffer) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRIOBuffer::Close\n"); fclose(_f); }
        return {};
    }
    virtual vr::EIOBufferError Read(vr::IOBufferHandle_t ulBuffer, void *pDst, uint32_t unBytes, uint32_t *punRead) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRIOBuffer::Read\n"); fclose(_f); }
        return {};
    }
    virtual vr::EIOBufferError Write(vr::IOBufferHandle_t ulBuffer, void *pSrc, uint32_t unBytes) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRIOBuffer::Write\n"); fclose(_f); }
        return {};
    }
    virtual vr::PropertyContainerHandle_t PropertyContainer(vr::IOBufferHandle_t ulBuffer) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRIOBuffer::PropertyContainer\n"); fclose(_f); }
        return {};
    }
    virtual bool HasReaders(vr::IOBufferHandle_t ulBuffer) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRIOBuffer::HasReaders\n"); fclose(_f); }
        return {};
    }
};

class FakeSpatialAnchors : public vr::IVRSpatialAnchors {
public:
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromDescriptor(const char *pchDescriptor, SpatialAnchorHandle_t *pHandleOut) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSpatialAnchors::CreateSpatialAnchorFromDescriptor\n"); fclose(_f); }
        return {};
    }
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromPose(TrackedDeviceIndex_t unDeviceIndex, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPose, SpatialAnchorHandle_t *pHandleOut) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSpatialAnchors::CreateSpatialAnchorFromPose\n"); fclose(_f); }
        return {};
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorPose(SpatialAnchorHandle_t unHandle, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPoseOut) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSpatialAnchors::GetSpatialAnchorPose\n"); fclose(_f); }
        return {};
    }
    virtual EVRSpatialAnchorError GetSpatialAnchorDescriptor(SpatialAnchorHandle_t unHandle, VR_OUT_STRING() char *pchDescriptorOut, uint32_t *punDescriptorBufferLenInOut) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRSpatialAnchors::GetSpatialAnchorDescriptor\n"); fclose(_f); }
        return {};
    }
};

class FakeDebug : public vr::IVRDebug {
public:
    virtual EVRDebugError EmitVrProfilerEvent(const char *pchMessage) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDebug::EmitVrProfilerEvent\n"); fclose(_f); }
        return {};
    }
    virtual EVRDebugError BeginVrProfilerEvent(VrProfilerEventHandle_t *pHandleOut) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDebug::BeginVrProfilerEvent\n"); fclose(_f); }
        return {};
    }
    virtual EVRDebugError FinishVrProfilerEvent(VrProfilerEventHandle_t hHandle, const char *pchMessage) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDebug::FinishVrProfilerEvent\n"); fclose(_f); }
        return {};
    }
    virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, VR_OUT_STRING() char *pchResponseBuffer, uint32_t unResponseBufferSize) override {
        FILE* _f = fopen("C:\\\\vr_emulator.log", "a"); if (_f) { fprintf(_f, "IVRDebug::DriverDebugRequest\n"); fclose(_f); }
        return {};
    }
};

