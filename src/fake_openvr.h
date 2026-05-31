#pragma once
#include <openvr.h>
#include <stdio.h>
namespace vr {

class FakeIVRSystem : public IVRSystem {
public:
    virtual void GetRecommendedRenderTargetSize( uint32_t *pnWidth, uint32_t *pnHeight ) override {}
    virtual HmdMatrix44_t GetProjectionMatrix( EVREye eEye, float fNearZ, float fFarZ ) override { return {}; }
    virtual void GetProjectionRaw( EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom ) override {}
    virtual bool ComputeDistortion( EVREye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates ) override { return false; }
    virtual bool ComputeDistortionSet( EVREye eEye, EVRDistortionChannel eChannel, bool bAsNormalizedDeviceCoordinates, uint32_t nNumCoordinates, const DistortionCoordinate_t *pInput, DistortionCoordinate_t *pOutput ) override { return false; }
    virtual HmdMatrix34_t GetEyeToHeadTransform( EVREye eEye ) override { return {}; }
    virtual bool GetTimeSinceLastVsync( float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter ) override { return false; }
    virtual int32_t GetD3D9AdapterIndex() override { return {}; }
    virtual void GetDXGIOutputInfo( int32_t *pnAdapterIndex ) override {}
    virtual void GetOutputDevice( uint64_t *pnDevice, ETextureType textureType, VkInstance_T *pInstance = nullptr ) override {}
    virtual bool IsDisplayOnDesktop() override { return false; }
    virtual bool SetDisplayVisibility( bool bIsVisibleOnDesktop ) override { return false; }
    virtual void GetDeviceToAbsoluteTrackingPose( ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount ) override {}
    virtual HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() override { return {}; }
    virtual HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() override { return {}; }
    virtual uint32_t GetSortedTrackedDeviceIndicesOfClass( ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd ) override { return {}; }
    virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel( vr::TrackedDeviceIndex_t unDeviceId ) override { return {}; }
    virtual void ApplyTransform( TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform ) override {}
    virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole( vr::ETrackedControllerRole unDeviceType ) override { return {}; }
    virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex( vr::TrackedDeviceIndex_t unDeviceIndex ) override { return (ETrackedControllerRole)0; }
    virtual ETrackedDeviceClass GetTrackedDeviceClass( vr::TrackedDeviceIndex_t unDeviceIndex ) override { return (ETrackedDeviceClass)0; }
    virtual bool IsTrackedDeviceConnected( vr::TrackedDeviceIndex_t unDeviceIndex ) override { return false; }
    virtual bool GetBoolTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) override { return false; }
    virtual float GetFloatTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) override { return 0.0f; }
    virtual int32_t GetInt32TrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) override { return {}; }
    virtual uint64_t GetUint64TrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) override { return {}; }
    virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) override { return {}; }
    virtual uint32_t GetArrayTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L ) override { return {}; }
    virtual uint32_t GetStringTrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L ) override { return {}; }
    virtual const char *GetPropErrorNameFromEnum( ETrackedPropertyError error ) override { return ""; }
    virtual bool PollNextEvent( VREvent_t *pEvent, uint32_t uncbVREvent ) override { return false; }
    virtual bool PollNextEventWithPose( ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose ) override { return false; }
    virtual bool PollNextEventWithPoseAndOverlays( vr::ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, TrackedDevicePose_t *pTrackedDevicePose, VROverlayHandle_t *pulOverlayHandle ) override { return false; }
    virtual const char *GetEventTypeNameFromEnum( EVREventType eType ) override { return ""; }
    virtual HiddenAreaMesh_t GetHiddenAreaMesh( EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard ) override { return {}; }
    virtual bool GetEyeTrackedFoveationCenter( HmdVector2_t *pNdcLeft, HmdVector2_t *pNdcRight ) override { return false; }
    virtual bool GetEyeTrackedFoveationCenterForProjection( const HmdMatrix44_t *pProjMat, HmdVector2_t *pNdc ) override { return false; }
    virtual bool GetControllerState( vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize ) override { return false; }
    virtual bool GetControllerStateWithPose( ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose ) override { return false; }
    virtual void TriggerHapticPulse( vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec ) override {}
    virtual const char *GetButtonIdNameFromEnum( EVRButtonId eButtonId ) override { return ""; }
    virtual const char *GetControllerAxisTypeNameFromEnum( EVRControllerAxisType eAxisType ) override { return ""; }
    virtual bool IsInputAvailable() override { return false; }
    virtual bool IsSteamVRDrawingControllers() override { return false; }
    virtual bool ShouldApplicationPause() override { return false; }
    virtual bool ShouldApplicationReduceRenderingWork() override { return false; }
    virtual vr::EVRFirmwareError PerformFirmwareUpdate( vr::TrackedDeviceIndex_t unDeviceIndex ) override { return (EVRFirmwareError)0; }
    virtual void AcknowledgeQuit_Exiting() override {}
    virtual uint32_t GetAppContainerFilePaths( VR_OUT_STRING() char *pchBuffer, uint32_t unBufferSize ) override { return {}; }
    virtual const char *GetRuntimeVersion() override { return ""; }
    virtual vr::EVRInitError SetSDKVersion( uint32_t nVersionMajor, uint32_t nVersionMinor, uint32_t nVersionBuild ) override { return (EVRInitError)0; }
};

class FakeIVRApplications : public IVRApplications {
public:
    virtual EVRApplicationError AddApplicationManifest( const char *pchApplicationManifestFullPath, bool bTemporary = false ) override { return (EVRApplicationError)0; }
    virtual EVRApplicationError RemoveApplicationManifest( const char *pchApplicationManifestFullPath ) override { return (EVRApplicationError)0; }
    virtual bool IsApplicationInstalled( const char *pchAppKey ) override { return false; }
    virtual uint32_t GetApplicationCount() override { return {}; }
    virtual EVRApplicationError GetApplicationKeyByIndex( uint32_t unApplicationIndex, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen ) override { return (EVRApplicationError)0; }
    virtual EVRApplicationError GetApplicationKeyByProcessId( uint32_t unProcessId, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen ) override { return (EVRApplicationError)0; }
    virtual EVRApplicationError LaunchApplication( const char *pchAppKey ) override { return (EVRApplicationError)0; }
    virtual EVRApplicationError LaunchTemplateApplication( const char *pchTemplateAppKey, const char *pchNewAppKey, VR_ARRAY_COUNT( unKeys ) const AppOverrideKeys_t *pKeys, uint32_t unKeys ) override { return (EVRApplicationError)0; }
    virtual vr::EVRApplicationError LaunchApplicationFromMimeType( const char *pchMimeType, const char *pchArgs ) override { return (EVRApplicationError)0; }
    virtual EVRApplicationError LaunchDashboardOverlay( const char *pchAppKey ) override { return (EVRApplicationError)0; }
    virtual bool CancelApplicationLaunch( const char *pchAppKey ) override { return false; }
    virtual EVRApplicationError IdentifyApplication( uint32_t unProcessId, const char *pchAppKey ) override { return (EVRApplicationError)0; }
    virtual uint32_t GetApplicationProcessId( const char *pchAppKey ) override { return {}; }
    virtual const char *GetApplicationsErrorNameFromEnum( EVRApplicationError error ) override { return ""; }
    virtual uint32_t GetApplicationPropertyString( const char *pchAppKey, EVRApplicationProperty eProperty, VR_OUT_STRING() char *pchPropertyValueBuffer, uint32_t unPropertyValueBufferLen, EVRApplicationError *peError = nullptr ) override { return {}; }
    virtual bool GetApplicationPropertyBool( const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr ) override { return false; }
    virtual uint64_t GetApplicationPropertyUint64( const char *pchAppKey, EVRApplicationProperty eProperty, EVRApplicationError *peError = nullptr ) override { return {}; }
    virtual EVRApplicationError SetApplicationAutoLaunch( const char *pchAppKey, bool bAutoLaunch ) override { return (EVRApplicationError)0; }
    virtual bool GetApplicationAutoLaunch( const char *pchAppKey ) override { return false; }
    virtual EVRApplicationError SetDefaultApplicationForMimeType( const char *pchAppKey, const char *pchMimeType ) override { return (EVRApplicationError)0; }
    virtual bool GetDefaultApplicationForMimeType( const char *pchMimeType, VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen ) override { return false; }
    virtual bool GetApplicationSupportedMimeTypes( const char *pchAppKey, VR_OUT_STRING() char *pchMimeTypesBuffer, uint32_t unMimeTypesBuffer ) override { return false; }
    virtual uint32_t GetApplicationsThatSupportMimeType( const char *pchMimeType, VR_OUT_STRING() char *pchAppKeysThatSupportBuffer, uint32_t unAppKeysThatSupportBuffer ) override { return {}; }
    virtual uint32_t GetApplicationLaunchArguments( uint32_t unHandle, VR_OUT_STRING() char *pchArgs, uint32_t unArgs ) override { return {}; }
    virtual EVRApplicationError GetStartingApplication( VR_OUT_STRING() char *pchAppKeyBuffer, uint32_t unAppKeyBufferLen ) override { return (EVRApplicationError)0; }
    virtual EVRSceneApplicationState GetSceneApplicationState() override { return (EVRSceneApplicationState)0; }
    virtual EVRApplicationError PerformApplicationPrelaunchCheck( const char *pchAppKey ) override { return (EVRApplicationError)0; }
    virtual const char *GetSceneApplicationStateNameFromEnum( EVRSceneApplicationState state ) override { return ""; }
    virtual EVRApplicationError LaunchInternalProcess( const char *pchBinaryPath, const char *pchArguments, const char *pchWorkingDirectory ) override { return (EVRApplicationError)0; }
    virtual EVRApplicationError RegisterSubprocess( uint32_t nPid ) override { return (EVRApplicationError)0; }
    virtual uint32_t GetCurrentSceneProcessId() override { return {}; }
};

class FakeIVRSettings : public IVRSettings {
public:
    virtual const char *GetSettingsErrorNameFromEnum( EVRSettingsError eError ) override { return ""; }
    virtual void SetBool( const char *pchSection, const char *pchSettingsKey, bool bValue, EVRSettingsError *peError = nullptr ) override {}
    virtual void SetInt32( const char *pchSection, const char *pchSettingsKey, int32_t nValue, EVRSettingsError *peError = nullptr ) override {}
    virtual void SetFloat( const char *pchSection, const char *pchSettingsKey, float flValue, EVRSettingsError *peError = nullptr ) override {}
    virtual void SetString( const char *pchSection, const char *pchSettingsKey, const char *pchValue, EVRSettingsError *peError = nullptr ) override {}
    virtual bool GetBool( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr ) override { return false; }
    virtual int32_t GetInt32( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr ) override { return {}; }
    virtual float GetFloat( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr ) override { return 0.0f; }
    virtual void GetString( const char *pchSection, const char *pchSettingsKey, VR_OUT_STRING() char *pchValue, uint32_t unValueLen, EVRSettingsError *peError = nullptr ) override {}
    virtual void RemoveSection( const char *pchSection, EVRSettingsError *peError = nullptr ) override {}
    virtual void RemoveKeyInSection( const char *pchSection, const char *pchSettingsKey, EVRSettingsError *peError = nullptr ) override {}
};

class FakeIVRChaperone : public IVRChaperone {
public:
    virtual ChaperoneCalibrationState GetCalibrationState() override { return {}; }
    virtual bool GetPlayAreaSize( float *pSizeX, float *pSizeZ ) override { return false; }
    virtual bool GetPlayAreaRect( HmdQuad_t *rect ) override { return false; }
    virtual void ReloadInfo( void ) override {}
    virtual void SetSceneColor( HmdColor_t color ) override {}
    virtual void GetBoundsColor( HmdColor_t *pOutputColorArray, int nNumOutputColors, float flCollisionBoundsFadeDistance, HmdColor_t *pOutputCameraColor ) override {}
    virtual bool AreBoundsVisible() override { return false; }
    virtual void ForceBoundsVisible( bool bForce ) override {}
    virtual void ResetZeroPose( ETrackingUniverseOrigin eTrackingUniverseOrigin ) override {}
};

class FakeIVRChaperoneSetup : public IVRChaperoneSetup {
public:
    virtual bool CommitWorkingCopy( EChaperoneConfigFile configFile ) override { return false; }
    virtual void RevertWorkingCopy() override {}
    virtual bool GetWorkingPlayAreaSize( float *pSizeX, float *pSizeZ ) override { return false; }
    virtual bool GetWorkingPlayAreaRect( HmdQuad_t *rect ) override { return false; }
    virtual bool GetWorkingCollisionBoundsInfo( VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount ) override { return false; }
    virtual bool GetLiveCollisionBoundsInfo( VR_OUT_ARRAY_COUNT(punQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t* punQuadsCount ) override { return false; }
    virtual bool GetWorkingSeatedZeroPoseToRawTrackingPose( HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose ) override { return false; }
    virtual bool GetWorkingStandingZeroPoseToRawTrackingPose( HmdMatrix34_t *pmatStandingZeroPoseToRawTrackingPose ) override { return false; }
    virtual void SetWorkingPlayAreaSize( float sizeX, float sizeZ ) override {}
    virtual void SetWorkingCollisionBoundsInfo( VR_ARRAY_COUNT(unQuadsCount) HmdQuad_t *pQuadsBuffer, uint32_t unQuadsCount ) override {}
    virtual void SetWorkingPerimeter( VR_ARRAY_COUNT( unPointCount ) const HmdVector2_t *pPointBuffer, uint32_t unPointCount ) override {}
    virtual void SetWorkingSeatedZeroPoseToRawTrackingPose( const HmdMatrix34_t *pMatSeatedZeroPoseToRawTrackingPose ) override {}
    virtual void SetWorkingStandingZeroPoseToRawTrackingPose( const HmdMatrix34_t *pMatStandingZeroPoseToRawTrackingPose ) override {}
    virtual void ReloadFromDisk( EChaperoneConfigFile configFile ) override {}
    virtual bool GetLiveSeatedZeroPoseToRawTrackingPose( HmdMatrix34_t *pmatSeatedZeroPoseToRawTrackingPose ) override { return false; }
    virtual bool ExportLiveToBuffer( VR_OUT_STRING() char *pBuffer, uint32_t *pnBufferLength ) override { return false; }
    virtual bool ImportFromBufferToWorking( const char *pBuffer, uint32_t nImportFlags ) override { return false; }
    virtual void ShowWorkingSetPreview() override {}
    virtual void HideWorkingSetPreview() override {}
    virtual void RoomSetupStarting() override {}
};

class FakeIVRCompositor : public IVRCompositor {
public:
    virtual void SetTrackingSpace( ETrackingUniverseOrigin eOrigin ) override {}
    virtual ETrackingUniverseOrigin GetTrackingSpace() override { return {}; }
    virtual EVRCompositorError WaitGetPoses( VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount ) override { return (EVRCompositorError)0; }
    virtual EVRCompositorError GetLastPoses( VR_ARRAY_COUNT( unRenderPoseArrayCount ) TrackedDevicePose_t* pRenderPoseArray, uint32_t unRenderPoseArrayCount, VR_ARRAY_COUNT( unGamePoseArrayCount ) TrackedDevicePose_t* pGamePoseArray, uint32_t unGamePoseArrayCount ) override { return (EVRCompositorError)0; }
    virtual EVRCompositorError GetLastPoseForTrackedDeviceIndex( TrackedDeviceIndex_t unDeviceIndex, TrackedDevicePose_t *pOutputPose, TrackedDevicePose_t *pOutputGamePose ) override { return (EVRCompositorError)0; }
    virtual EVRCompositorError GetSubmitTexture( Texture_t *pOutTexture, bool *pNeedsFlush, EVRCompositorTextureUsage eUsage, const Texture_t *pTexture, const VRTextureBounds_t *pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default ) override { return (EVRCompositorError)0; }
    virtual EVRCompositorError Submit( EVREye eEye, const Texture_t *pTexture, const VRTextureBounds_t* pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default ) override { return (EVRCompositorError)0; }
    virtual EVRCompositorError SubmitWithArrayIndex( EVREye eEye, const Texture_t *pTexture, uint32_t unTextureArrayIndex, const VRTextureBounds_t *pBounds = 0, EVRSubmitFlags nSubmitFlags = Submit_Default ) override { return (EVRCompositorError)0; }
    virtual void ClearLastSubmittedFrame() override {}
    virtual void PostPresentHandoff() override {}
    virtual bool GetFrameTiming( Compositor_FrameTiming *pTiming, uint32_t unFramesAgo = 0 ) override { return false; }
    virtual uint32_t GetFrameTimings( VR_ARRAY_COUNT( nFrames ) Compositor_FrameTiming *pTiming, uint32_t nFrames ) override { return {}; }
    virtual float GetFrameTimeRemaining() override { return 0.0f; }
    virtual void GetCumulativeStats( Compositor_CumulativeStats *pStats, uint32_t nStatsSizeInBytes ) override {}
    virtual void FadeToColor( float fSeconds, float fRed, float fGreen, float fBlue, float fAlpha, bool bBackground = false ) override {}
    virtual HmdColor_t GetCurrentFadeColor( bool bBackground = false ) override { return {}; }
    virtual void FadeGrid( float fSeconds, bool bFadeGridIn ) override {}
    virtual float GetCurrentGridAlpha() override { return 0.0f; }
    virtual EVRCompositorError SetSkyboxOverride( VR_ARRAY_COUNT( unTextureCount ) const Texture_t *pTextures, uint32_t unTextureCount ) override { return (EVRCompositorError)0; }
    virtual void ClearSkyboxOverride() override {}
    virtual void CompositorBringToFront() override {}
    virtual void CompositorGoToBack() override {}
    virtual void CompositorQuit() override {}
    virtual bool IsFullscreen() override { return false; }
    virtual uint32_t GetCurrentSceneFocusProcess() override { return {}; }
    virtual uint32_t GetLastFrameRenderer() override { return {}; }
    virtual bool CanRenderScene() override { return false; }
    virtual void ShowMirrorWindow() override {}
    virtual void HideMirrorWindow() override {}
    virtual bool IsMirrorWindowVisible() override { return false; }
    virtual void CompositorDumpImages() override {}
    virtual bool ShouldAppRenderWithLowResources() override { return false; }
    virtual void ForceInterleavedReprojectionOn( bool bOverride ) override {}
    virtual void ForceReconnectProcess() override {}
    virtual void SuspendRendering( bool bSuspend ) override {}
    virtual vr::EVRCompositorError GetMirrorTextureD3D11( vr::EVREye eEye, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView ) override { return (EVRCompositorError)0; }
    virtual void ReleaseMirrorTextureD3D11( void *pD3D11ShaderResourceView ) override {}
    virtual vr::EVRCompositorError GetMirrorTextureGL( vr::EVREye eEye, vr::glUInt_t *pglTextureId, vr::glSharedTextureHandle_t *pglSharedTextureHandle ) override { return (EVRCompositorError)0; }
    virtual bool ReleaseSharedGLTexture( vr::glUInt_t glTextureId, vr::glSharedTextureHandle_t glSharedTextureHandle ) override { return false; }
    virtual void LockGLSharedTextureForAccess( vr::glSharedTextureHandle_t glSharedTextureHandle ) override {}
    virtual void UnlockGLSharedTextureForAccess( vr::glSharedTextureHandle_t glSharedTextureHandle ) override {}
    virtual uint32_t GetVulkanInstanceExtensionsRequired( VR_OUT_STRING() char *pchValue, uint32_t unBufferSize ) override { return {}; }
    virtual uint32_t GetVulkanDeviceExtensionsRequired( VkPhysicalDevice_T *pPhysicalDevice, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize ) override { return {}; }
    virtual void SetExplicitTimingMode( EVRCompositorTimingMode eTimingMode ) override {}
    virtual EVRCompositorError SubmitExplicitTimingData() override { return (EVRCompositorError)0; }
    virtual bool IsMotionSmoothingEnabled() override { return false; }
    virtual bool IsMotionSmoothingSupported() override { return false; }
    virtual bool IsCurrentSceneFocusAppLoading() override { return false; }
    virtual EVRCompositorError SetStageOverride_Async( const char *pchRenderModelPath, const HmdMatrix34_t *pTransform = 0, const Compositor_StageRenderSettings *pRenderSettings = 0, uint32_t nSizeOfRenderSettings = 0 ) override { return (EVRCompositorError)0; }
    virtual void ClearStageOverride() override {}
    virtual bool GetCompositorBenchmarkResults( Compositor_BenchmarkResults *pBenchmarkResults, uint32_t nSizeOfBenchmarkResults ) override { return false; }
    virtual EVRCompositorError GetLastPosePredictionIDs( uint32_t *pRenderPosePredictionID, uint32_t *pGamePosePredictionID ) override { return (EVRCompositorError)0; }
    virtual EVRCompositorError GetPosesForFrame( uint32_t unPosePredictionID, VR_ARRAY_COUNT( unPoseArrayCount ) TrackedDevicePose_t* pPoseArray, uint32_t unPoseArrayCount ) override { return (EVRCompositorError)0; }
};

class FakeIVRHeadsetView : public IVRHeadsetView {
public:
    virtual void SetHeadsetViewSize( uint32_t nWidth, uint32_t nHeight ) override {}
    virtual void GetHeadsetViewSize( uint32_t *pnWidth, uint32_t *pnHeight ) override {}
    virtual void SetHeadsetViewMode( HeadsetViewMode_t eHeadsetViewMode ) override {}
    virtual HeadsetViewMode_t GetHeadsetViewMode() override { return {}; }
    virtual void SetHeadsetViewCropped( bool bCropped ) override {}
    virtual bool GetHeadsetViewCropped() override { return false; }
    virtual float GetHeadsetViewAspectRatio() override { return 0.0f; }
    virtual void SetHeadsetViewBlendRange( float flStartPct, float flEndPct ) override {}
    virtual void GetHeadsetViewBlendRange( float *pStartPct, float *pEndPct ) override {}
};

class FakeIVRNotifications : public IVRNotifications {
public:
    virtual EVRNotificationError CreateNotification( VROverlayHandle_t ulOverlayHandle, uint64_t ulUserValue, EVRNotificationType type, const char *pchText, EVRNotificationStyle style, const NotificationBitmap_t *pImage, VRNotificationId *pNotificationId ) override { return (EVRNotificationError)0; }
    virtual EVRNotificationError RemoveNotification( VRNotificationId notificationId ) override { return (EVRNotificationError)0; }
};

class FakeIVROverlay : public IVROverlay {
public:
    virtual EVROverlayError FindOverlay( const char *pchOverlayKey, VROverlayHandle_t * pOverlayHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError CreateOverlay( const char *pchOverlayKey, const char *pchOverlayName, VROverlayHandle_t * pOverlayHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError CreateSubviewOverlay( VROverlayHandle_t parentOverlayHandle, const char *pchSubviewOverlayKey, const char *pchSubviewOverlayName, VROverlayHandle_t *pSubviewOverlayHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError DestroyOverlay( VROverlayHandle_t ulOverlayHandle ) override { return (EVROverlayError)0; }
    virtual uint32_t GetOverlayKey( VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L ) override { return {}; }
    virtual uint32_t GetOverlayName( VROverlayHandle_t ulOverlayHandle, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, EVROverlayError *pError = 0L ) override { return {}; }
    virtual EVROverlayError SetOverlayName( VROverlayHandle_t ulOverlayHandle, const char *pchName ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayImageData( VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unBufferSize, uint32_t *punWidth, uint32_t *punHeight ) override { return (EVROverlayError)0; }
    virtual const char *GetOverlayErrorNameFromEnum( EVROverlayError error ) override { return ""; }
    virtual EVROverlayError SetOverlayRenderingPid( VROverlayHandle_t ulOverlayHandle, uint32_t unPID ) override { return (EVROverlayError)0; }
    virtual uint32_t GetOverlayRenderingPid( VROverlayHandle_t ulOverlayHandle ) override { return {}; }
    virtual EVROverlayError SetOverlayFlag( VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool bEnabled ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayFlag( VROverlayHandle_t ulOverlayHandle, VROverlayFlags eOverlayFlag, bool *pbEnabled ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayFlags( VROverlayHandle_t ulOverlayHandle, uint32_t *pFlags ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayColor( VROverlayHandle_t ulOverlayHandle, float fRed, float fGreen, float fBlue ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayColor( VROverlayHandle_t ulOverlayHandle, float *pfRed, float *pfGreen, float *pfBlue ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayAlpha( VROverlayHandle_t ulOverlayHandle, float fAlpha ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayAlpha( VROverlayHandle_t ulOverlayHandle, float *pfAlpha ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTexelAspect( VROverlayHandle_t ulOverlayHandle, float fTexelAspect ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTexelAspect( VROverlayHandle_t ulOverlayHandle, float *pfTexelAspect ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlaySortOrder( VROverlayHandle_t ulOverlayHandle, uint32_t unSortOrder ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlaySortOrder( VROverlayHandle_t ulOverlayHandle, uint32_t *punSortOrder ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayWidthInMeters( VROverlayHandle_t ulOverlayHandle, float fWidthInMeters ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayWidthInMeters( VROverlayHandle_t ulOverlayHandle, float *pfWidthInMeters ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayCurvature( VROverlayHandle_t ulOverlayHandle, float fCurvature ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayCurvature( VROverlayHandle_t ulOverlayHandle, float *pfCurvature ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayPreCurvePitch( VROverlayHandle_t ulOverlayHandle, float fRadians ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayPreCurvePitch( VROverlayHandle_t ulOverlayHandle, float *pfRadians ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTextureColorSpace( VROverlayHandle_t ulOverlayHandle, EColorSpace eTextureColorSpace ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTextureColorSpace( VROverlayHandle_t ulOverlayHandle, EColorSpace *peTextureColorSpace ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTextureBounds( VROverlayHandle_t ulOverlayHandle, const VRTextureBounds_t *pOverlayTextureBounds ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTextureBounds( VROverlayHandle_t ulOverlayHandle, VRTextureBounds_t *pOverlayTextureBounds ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTransformType( VROverlayHandle_t ulOverlayHandle, VROverlayTransformType *peTransformType ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTransformAbsolute( VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToOverlayTransform ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTransformAbsolute( VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin *peTrackingOrigin, HmdMatrix34_t *pmatTrackingOriginToOverlayTransform ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceRelative( VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unTrackedDevice, const HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceRelative( VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punTrackedDevice, HmdMatrix34_t *pmatTrackedDeviceToOverlayTransform ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTransformTrackedDeviceComponent( VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t unDeviceIndex, const char *pchComponentName ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTransformTrackedDeviceComponent( VROverlayHandle_t ulOverlayHandle, TrackedDeviceIndex_t *punDeviceIndex, VR_OUT_STRING() char *pchComponentName, uint32_t unComponentNameSize ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTransformCursor( VROverlayHandle_t ulCursorOverlayHandle, const HmdVector2_t *pvHotspot ) override { return (EVROverlayError)0; }
    virtual vr::EVROverlayError GetOverlayTransformCursor( VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvHotspot ) override { return (EVROverlayError)0; }
    virtual vr::EVROverlayError SetOverlayTransformProjection( VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t* pmatTrackingOriginToOverlayTransform, const VROverlayProjection_t *pProjection, vr::EVREye eEye ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetSubviewPosition( VROverlayHandle_t ulOverlayHandle, float fX, float fY ) override { return (EVROverlayError)0; }
    virtual EVROverlayError ShowOverlay( VROverlayHandle_t ulOverlayHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError HideOverlay( VROverlayHandle_t ulOverlayHandle ) override { return (EVROverlayError)0; }
    virtual bool IsOverlayVisible( VROverlayHandle_t ulOverlayHandle ) override { return false; }
    virtual EVROverlayError GetTransformForOverlayCoordinates( VROverlayHandle_t ulOverlayHandle, ETrackingUniverseOrigin eTrackingOrigin, HmdVector2_t coordinatesInOverlay, HmdMatrix34_t *pmatTransform ) override { return (EVROverlayError)0; }
    virtual EVROverlayError WaitFrameSync( uint32_t nTimeoutMs ) override { return (EVROverlayError)0; }
    virtual bool PollNextOverlayEvent( VROverlayHandle_t ulOverlayHandle, VREvent_t *pEvent, uint32_t uncbVREvent ) override { return false; }
    virtual EVROverlayError GetOverlayInputMethod( VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod *peInputMethod ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayInputMethod( VROverlayHandle_t ulOverlayHandle, VROverlayInputMethod eInputMethod ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayMouseScale( VROverlayHandle_t ulOverlayHandle, HmdVector2_t *pvecMouseScale ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayMouseScale( VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvecMouseScale ) override { return (EVROverlayError)0; }
    virtual bool ComputeOverlayIntersection( VROverlayHandle_t ulOverlayHandle, const VROverlayIntersectionParams_t *pParams, VROverlayIntersectionResults_t *pResults ) override { return false; }
    virtual bool IsHoverTargetOverlay( VROverlayHandle_t ulOverlayHandle ) override { return false; }
    virtual EVROverlayError SetOverlayIntersectionMask( VROverlayHandle_t ulOverlayHandle, VROverlayIntersectionMaskPrimitive_t *pMaskPrimitives, uint32_t unNumMaskPrimitives, uint32_t unPrimitiveSize = sizeof( VROverlayIntersectionMaskPrimitive_t ) ) override { return (EVROverlayError)0; }
    virtual EVROverlayError TriggerLaserMouseHapticVibration( VROverlayHandle_t ulOverlayHandle, float fDurationSeconds, float fFrequency, float fAmplitude ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayCursor( VROverlayHandle_t ulOverlayHandle, VROverlayHandle_t ulCursorHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayCursorPositionOverride( VROverlayHandle_t ulOverlayHandle, const HmdVector2_t *pvCursor ) override { return (EVROverlayError)0; }
    virtual EVROverlayError ClearOverlayCursorPositionOverride( VROverlayHandle_t ulOverlayHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayTexture( VROverlayHandle_t ulOverlayHandle, const Texture_t *pTexture ) override { return (EVROverlayError)0; }
    virtual EVROverlayError ClearOverlayTexture( VROverlayHandle_t ulOverlayHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayRaw( VROverlayHandle_t ulOverlayHandle, void *pvBuffer, uint32_t unWidth, uint32_t unHeight, uint32_t unBytesPerPixel ) override { return (EVROverlayError)0; }
    virtual EVROverlayError SetOverlayFromFile( VROverlayHandle_t ulOverlayHandle, const char *pchFilePath ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTexture( VROverlayHandle_t ulOverlayHandle, void **pNativeTextureHandle, void *pNativeTextureRef, uint32_t *pWidth, uint32_t *pHeight, uint32_t *pNativeFormat, ETextureType *pAPIType, EColorSpace *pColorSpace, VRTextureBounds_t *pTextureBounds ) override { return (EVROverlayError)0; }
    virtual EVROverlayError ReleaseNativeOverlayHandle( VROverlayHandle_t ulOverlayHandle, void *pNativeTextureHandle ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetOverlayTextureSize( VROverlayHandle_t ulOverlayHandle, uint32_t *pWidth, uint32_t *pHeight ) override { return (EVROverlayError)0; }
    virtual EVROverlayError CreateDashboardOverlay( const char *pchOverlayKey, const char *pchOverlayFriendlyName, VROverlayHandle_t * pMainHandle, VROverlayHandle_t *pThumbnailHandle ) override { return (EVROverlayError)0; }
    virtual bool IsDashboardVisible() override { return false; }
    virtual bool IsActiveDashboardOverlay( VROverlayHandle_t ulOverlayHandle ) override { return false; }
    virtual EVROverlayError SetDashboardOverlaySceneProcess( VROverlayHandle_t ulOverlayHandle, uint32_t unProcessId ) override { return (EVROverlayError)0; }
    virtual EVROverlayError GetDashboardOverlaySceneProcess( VROverlayHandle_t ulOverlayHandle, uint32_t *punProcessId ) override { return (EVROverlayError)0; }
    virtual void ShowDashboard( const char *pchOverlayToShow ) override {}
    virtual vr::TrackedDeviceIndex_t GetPrimaryDashboardDevice() override { return {}; }
    virtual EVROverlayError ShowKeyboard( EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue ) override { return (EVROverlayError)0; }
    virtual EVROverlayError ShowKeyboardForOverlay( VROverlayHandle_t ulOverlayHandle, EGamepadTextInputMode eInputMode, EGamepadTextInputLineMode eLineInputMode, uint32_t unFlags, const char *pchDescription, uint32_t unCharMax, const char *pchExistingText, uint64_t uUserValue ) override { return (EVROverlayError)0; }
    virtual uint32_t GetKeyboardText( VR_OUT_STRING() char *pchText, uint32_t cchText ) override { return {}; }
    virtual void HideKeyboard() override {}
    virtual void SetKeyboardTransformAbsolute( ETrackingUniverseOrigin eTrackingOrigin, const HmdMatrix34_t *pmatTrackingOriginToKeyboardTransform ) override {}
    virtual void SetKeyboardPositionForOverlay( VROverlayHandle_t ulOverlayHandle, HmdRect2_t avoidRect ) override {}
    virtual VRMessageOverlayResponse ShowMessageOverlay( const char* pchText, const char* pchCaption, const char* pchButton0Text, const char* pchButton1Text = nullptr, const char* pchButton2Text = nullptr, const char* pchButton3Text = nullptr ) override { return {}; }
    virtual void CloseMessageOverlay() override {}
};

class FakeIVROverlayView : public IVROverlayView {
public:
    virtual EVROverlayError AcquireOverlayView(VROverlayHandle_t ulOverlayHandle, VRNativeDevice_t *pNativeDevice, VROverlayView_t *pOverlayView, uint32_t unOverlayViewSize ) override { return (EVROverlayError)0; }
    virtual EVROverlayError ReleaseOverlayView(VROverlayView_t *pOverlayView) override { return (EVROverlayError)0; }
    virtual void PostOverlayEvent(VROverlayHandle_t ulOverlayHandle, const VREvent_t *pvrEvent) override {}
    virtual bool IsViewingPermitted( VROverlayHandle_t ulOverlayHandle ) override { return false; }
};

class FakeIVRRenderModels : public IVRRenderModels {
public:
    virtual EVRRenderModelError LoadRenderModel_Async( const char *pchRenderModelName, RenderModel_t **ppRenderModel ) override { return (EVRRenderModelError)0; }
    virtual void FreeRenderModel( RenderModel_t *pRenderModel ) override {}
    virtual EVRRenderModelError LoadTexture_Async( TextureID_t textureId, RenderModel_TextureMap_t **ppTexture ) override { return (EVRRenderModelError)0; }
    virtual void FreeTexture( RenderModel_TextureMap_t *pTexture ) override {}
    virtual EVRRenderModelError LoadTextureD3D11_Async( TextureID_t textureId, void *pD3D11Device, void **ppD3D11Texture2D ) override { return (EVRRenderModelError)0; }
    virtual EVRRenderModelError LoadIntoTextureD3D11_Async( TextureID_t textureId, void *pDstTexture ) override { return (EVRRenderModelError)0; }
    virtual void FreeTextureD3D11( void *pD3D11Texture2D ) override {}
    virtual uint32_t GetRenderModelName( uint32_t unRenderModelIndex, VR_OUT_STRING() char *pchRenderModelName, uint32_t unRenderModelNameLen ) override { return {}; }
    virtual uint32_t GetRenderModelCount() override { return {}; }
    virtual uint32_t GetComponentCount( const char *pchRenderModelName ) override { return {}; }
    virtual uint32_t GetComponentName( const char *pchRenderModelName, uint32_t unComponentIndex, VR_OUT_STRING( ) char *pchComponentName, uint32_t unComponentNameLen ) override { return {}; }
    virtual uint64_t GetComponentButtonMask( const char *pchRenderModelName, const char *pchComponentName ) override { return {}; }
    virtual uint32_t GetComponentRenderModelName( const char *pchRenderModelName, const char *pchComponentName, VR_OUT_STRING( ) char *pchComponentRenderModelName, uint32_t unComponentRenderModelNameLen ) override { return {}; }
    virtual bool GetComponentStateForDevicePath( const char *pchRenderModelName, const char *pchComponentName, vr::VRInputValueHandle_t devicePath, const vr::RenderModel_ControllerMode_State_t *pState, vr::RenderModel_ComponentState_t *pComponentState ) override { return false; }
    virtual bool GetComponentState( const char *pchRenderModelName, const char *pchComponentName, const vr::VRControllerState_t *pControllerState, const RenderModel_ControllerMode_State_t *pState, RenderModel_ComponentState_t *pComponentState ) override { return false; }
    virtual bool RenderModelHasComponent( const char *pchRenderModelName, const char *pchComponentName ) override { return false; }
    virtual uint32_t GetRenderModelThumbnailURL( const char *pchRenderModelName, VR_OUT_STRING() char *pchThumbnailURL, uint32_t unThumbnailURLLen, vr::EVRRenderModelError *peError ) override { return {}; }
    virtual uint32_t GetRenderModelOriginalPath( const char *pchRenderModelName, VR_OUT_STRING() char *pchOriginalPath, uint32_t unOriginalPathLen, vr::EVRRenderModelError *peError ) override { return {}; }
    virtual const char *GetRenderModelErrorNameFromEnum( vr::EVRRenderModelError error ) override { return ""; }
};

class FakeIVRExtendedDisplay : public IVRExtendedDisplay {
public:
    virtual void GetWindowBounds( int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) override {}
    virtual void GetEyeOutputViewport( EVREye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight ) override {}
    virtual void GetDXGIOutputInfo( int32_t *pnAdapterIndex, int32_t *pnAdapterOutputIndex ) override {}
};

class FakeIVRTrackedCamera : public IVRTrackedCamera {
public:
    virtual const char *GetCameraErrorNameFromEnum( vr::EVRTrackedCameraError eCameraError ) override { return ""; }
    virtual vr::EVRTrackedCameraError HasCamera( vr::TrackedDeviceIndex_t nDeviceIndex, bool *pHasCamera ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetCameraFrameSize( vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, uint32_t *pnWidth, uint32_t *pnHeight, uint32_t *pnFrameBufferSize ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetCameraIntrinsics( vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::HmdVector2_t *pFocalLength, vr::HmdVector2_t *pCenter ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetCameraProjection( vr::TrackedDeviceIndex_t nDeviceIndex, uint32_t nCameraIndex, vr::EVRTrackedCameraFrameType eFrameType, float flZNear, float flZFar, vr::HmdMatrix44_t *pProjection ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError AcquireVideoStreamingService( vr::TrackedDeviceIndex_t nDeviceIndex, vr::TrackedCameraHandle_t *pHandle ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamingService( vr::TrackedCameraHandle_t hTrackedCamera ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetVideoStreamFrameBuffer( vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pFrameBuffer, uint32_t nFrameBufferSize, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureSize( vr::TrackedDeviceIndex_t nDeviceIndex, vr::EVRTrackedCameraFrameType eFrameType, vr::VRTextureBounds_t *pTextureBounds, uint32_t *pnWidth, uint32_t *pnHeight ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureD3D11( vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, void *pD3D11DeviceOrResource, void **ppD3D11ShaderResourceView, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError GetVideoStreamTextureGL( vr::TrackedCameraHandle_t hTrackedCamera, vr::EVRTrackedCameraFrameType eFrameType, vr::glUInt_t *pglTextureId, vr::CameraVideoStreamFrameHeader_t *pFrameHeader, uint32_t nFrameHeaderSize ) override { return (EVRTrackedCameraError)0; }
    virtual vr::EVRTrackedCameraError ReleaseVideoStreamTextureGL( vr::TrackedCameraHandle_t hTrackedCamera, vr::glUInt_t glTextureId ) override { return (EVRTrackedCameraError)0; }
    virtual void SetCameraTrackingSpace( vr::ETrackingUniverseOrigin eUniverse ) override {}
    virtual vr::ETrackingUniverseOrigin GetCameraTrackingSpace( ) override { return {}; }
};

class FakeIVRScreenshots : public IVRScreenshots {
public:
    virtual vr::EVRScreenshotError RequestScreenshot( vr::ScreenshotHandle_t *pOutScreenshotHandle, vr::EVRScreenshotType type, const char *pchPreviewFilename, const char *pchVRFilename ) override { return (EVRScreenshotError)0; }
    virtual vr::EVRScreenshotError HookScreenshot( VR_ARRAY_COUNT( numTypes ) const vr::EVRScreenshotType *pSupportedTypes, int numTypes ) override { return (EVRScreenshotError)0; }
    virtual vr::EVRScreenshotType GetScreenshotPropertyType( vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotError *pError ) override { return (EVRScreenshotType)0; }
    virtual uint32_t GetScreenshotPropertyFilename( vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotPropertyFilenames filenameType, VR_OUT_STRING() char *pchFilename, uint32_t cchFilename, vr::EVRScreenshotError *pError ) override { return {}; }
    virtual vr::EVRScreenshotError UpdateScreenshotProgress( vr::ScreenshotHandle_t screenshotHandle, float flProgress ) override { return (EVRScreenshotError)0; }
    virtual vr::EVRScreenshotError TakeStereoScreenshot( vr::ScreenshotHandle_t *pOutScreenshotHandle, const char *pchPreviewFilename, const char *pchVRFilename ) override { return (EVRScreenshotError)0; }
    virtual vr::EVRScreenshotError SubmitScreenshot( vr::ScreenshotHandle_t screenshotHandle, vr::EVRScreenshotType type, const char *pchSourcePreviewFilename, const char *pchSourceVRFilename ) override { return (EVRScreenshotError)0; }
};

class FakeIVRResources : public IVRResources {
public:
    virtual uint32_t LoadSharedResource( const char *pchResourceName, char *pchBuffer, uint32_t unBufferLen ) override { return {}; }
    virtual uint32_t GetResourceFullPath( const char *pchResourceName, const char *pchResourceTypeDirectory, VR_OUT_STRING() char *pchPathBuffer, uint32_t unBufferLen ) override { return {}; }
};

class FakeIVRDriverManager : public IVRDriverManager {
public:
    virtual uint32_t GetDriverCount() const override { return {}; }
    virtual uint32_t GetDriverName( vr::DriverId_t nDriver, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize ) override { return {}; }
    virtual DriverHandle_t GetDriverHandle( const char *pchDriverName ) override { return {}; }
    virtual bool IsEnabled( vr::DriverId_t nDriver ) const override { return false; }
};

class FakeIVRInput : public IVRInput {
public:
    virtual EVRInputError SetActionManifestPath( const char *pchActionManifestPath ) override { return (EVRInputError)0; }
    virtual EVRInputError GetActionSetHandle( const char *pchActionSetName, VRActionSetHandle_t *pHandle ) override { return (EVRInputError)0; }
    virtual EVRInputError GetActionHandle( const char *pchActionName, VRActionHandle_t *pHandle ) override { return (EVRInputError)0; }
    virtual EVRInputError GetInputSourceHandle( const char *pchInputSourcePath, VRInputValueHandle_t *pHandle ) override { return (EVRInputError)0; }
    virtual EVRInputError UpdateActionState( VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount ) override { return (EVRInputError)0; }
    virtual EVRInputError GetDigitalActionData( VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice ) override { return (EVRInputError)0; }
    virtual EVRInputError GetAnalogActionData( VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice ) override { return (EVRInputError)0; }
    virtual EVRInputError GetPoseActionDataRelativeToNow( VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice ) override { return (EVRInputError)0; }
    virtual EVRInputError GetPoseActionDataForNextFrame( VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice ) override { return (EVRInputError)0; }
    virtual EVRInputError GetSkeletalActionData( VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize ) override { return (EVRInputError)0; }
    virtual EVRInputError GetDominantHand( ETrackedControllerRole *peDominantHand ) override { return (EVRInputError)0; }
    virtual EVRInputError SetDominantHand( ETrackedControllerRole eDominantHand ) override { return (EVRInputError)0; }
    virtual EVRInputError GetEyeTrackingDataRelativeToNow( VRActionHandle_t action, vr::ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, vr::VREyeTrackingData_t *pEyeTrackingData, uint32_t ulEyeTrackingDataSize ) override { return (EVRInputError)0; }
    virtual EVRInputError GetEyeTrackingDataForNextFrame( VRActionHandle_t action, vr::ETrackingUniverseOrigin eOrigin, vr::VREyeTrackingData_t *pEyeTrackingData, uint32_t ulEyeTrackingDataSize ) override { return (EVRInputError)0; }
    virtual EVRInputError GetBoneCount( VRActionHandle_t action, uint32_t* pBoneCount ) override { return (EVRInputError)0; }
    virtual EVRInputError GetBoneHierarchy( VRActionHandle_t action, VR_ARRAY_COUNT( unIndexArayCount ) BoneIndex_t* pParentIndices, uint32_t unIndexArayCount ) override { return (EVRInputError)0; }
    virtual EVRInputError GetBoneName( VRActionHandle_t action, BoneIndex_t nBoneIndex, VR_OUT_STRING() char* pchBoneName, uint32_t unNameBufferSize ) override { return (EVRInputError)0; }
    virtual EVRInputError GetSkeletalReferenceTransforms( VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalReferencePose eReferencePose, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount ) override { return (EVRInputError)0; }
    virtual EVRInputError GetSkeletalTrackingLevel( VRActionHandle_t action, EVRSkeletalTrackingLevel* pSkeletalTrackingLevel ) override { return (EVRInputError)0; }
    virtual EVRInputError GetSkeletalBoneData( VRActionHandle_t action, EVRSkeletalTransformSpace eTransformSpace, EVRSkeletalMotionRange eMotionRange, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount ) override { return (EVRInputError)0; }
    virtual EVRInputError GetSkeletalSummaryData( VRActionHandle_t action, EVRSummaryType eSummaryType, VRSkeletalSummaryData_t * pSkeletalSummaryData ) override { return (EVRInputError)0; }
    virtual EVRInputError GetSkeletalBoneDataCompressed( VRActionHandle_t action, EVRSkeletalMotionRange eMotionRange, VR_OUT_BUFFER_COUNT( unCompressedSize ) void *pvCompressedData, uint32_t unCompressedSize, uint32_t *punRequiredCompressedSize ) override { return (EVRInputError)0; }
    virtual EVRInputError DecompressSkeletalBoneData( const void *pvCompressedBuffer, uint32_t unCompressedBufferSize, EVRSkeletalTransformSpace eTransformSpace, VR_ARRAY_COUNT( unTransformArrayCount ) VRBoneTransform_t *pTransformArray, uint32_t unTransformArrayCount ) override { return (EVRInputError)0; }
    virtual EVRInputError TriggerHapticVibrationAction( VRActionHandle_t action, float fStartSecondsFromNow, float fDurationSeconds, float fFrequency, float fAmplitude, VRInputValueHandle_t ulRestrictToDevice ) override { return (EVRInputError)0; }
    virtual EVRInputError GetActionOrigins( VRActionSetHandle_t actionSetHandle, VRActionHandle_t digitalActionHandle, VR_ARRAY_COUNT( originOutCount ) VRInputValueHandle_t *originsOut, uint32_t originOutCount ) override { return (EVRInputError)0; }
    virtual EVRInputError GetOriginLocalizedName( VRInputValueHandle_t origin, VR_OUT_STRING() char *pchNameArray, uint32_t unNameArraySize, int32_t unStringSectionsToInclude ) override { return (EVRInputError)0; }
    virtual EVRInputError GetOriginTrackedDeviceInfo( VRInputValueHandle_t origin, InputOriginInfo_t *pOriginInfo, uint32_t unOriginInfoSize ) override { return (EVRInputError)0; }
    virtual EVRInputError GetActionBindingInfo( VRActionHandle_t action, VR_ARRAY_COUNT( unBindingInfoCount ) InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, uint32_t *punReturnedBindingInfoCount ) override { return (EVRInputError)0; }
    virtual EVRInputError ShowActionOrigins( VRActionSetHandle_t actionSetHandle, VRActionHandle_t ulActionHandle ) override { return (EVRInputError)0; }
    virtual EVRInputError ShowBindingsForActionSet( VR_ARRAY_COUNT( unSetCount ) VRActiveActionSet_t *pSets, uint32_t unSizeOfVRSelectedActionSet_t, uint32_t unSetCount, VRInputValueHandle_t originToHighlight ) override { return (EVRInputError)0; }
    virtual EVRInputError GetComponentStateForBinding( const char *pchRenderModelName, const char *pchComponentName, const InputBindingInfo_t *pOriginInfo, uint32_t unBindingInfoSize, uint32_t unBindingInfoCount, vr::RenderModel_ComponentState_t *pComponentState ) override { return (EVRInputError)0; }
    virtual bool IsUsingLegacyInput() override { return false; }
    virtual EVRInputError OpenBindingUI( const char* pchAppKey, VRActionSetHandle_t ulActionSetHandle, VRInputValueHandle_t ulDeviceHandle, bool bShowOnDesktop ) override { return (EVRInputError)0; }
    virtual EVRInputError GetBindingVariant( vr::VRInputValueHandle_t ulDevicePath, VR_OUT_STRING() char *pchVariantArray, uint32_t unVariantArraySize ) override { return (EVRInputError)0; }
};

class FakeIVRIOBuffer : public IVRIOBuffer {
public:
    virtual vr::EIOBufferError Open( const char *pchPath, vr::EIOBufferMode mode, uint32_t unElementSize, uint32_t unElements, vr::IOBufferHandle_t *pulBuffer ) override { return (EIOBufferError)0; }
    virtual vr::EIOBufferError Close( vr::IOBufferHandle_t ulBuffer ) override { return (EIOBufferError)0; }
    virtual vr::EIOBufferError Read( vr::IOBufferHandle_t ulBuffer, void *pDst, uint32_t unBytes, uint32_t *punRead ) override { return (EIOBufferError)0; }
    virtual vr::EIOBufferError Write( vr::IOBufferHandle_t ulBuffer, void *pSrc, uint32_t unBytes ) override { return (EIOBufferError)0; }
    virtual vr::PropertyContainerHandle_t PropertyContainer( vr::IOBufferHandle_t ulBuffer ) override { return {}; }
    virtual bool HasReaders( vr::IOBufferHandle_t ulBuffer ) override { return false; }
};

class FakeIVRSpatialAnchors : public IVRSpatialAnchors {
public:
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromDescriptor( const char *pchDescriptor, SpatialAnchorHandle_t *pHandleOut ) override { return (EVRSpatialAnchorError)0; }
    virtual EVRSpatialAnchorError CreateSpatialAnchorFromPose( TrackedDeviceIndex_t unDeviceIndex, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPose, SpatialAnchorHandle_t *pHandleOut ) override { return (EVRSpatialAnchorError)0; }
    virtual EVRSpatialAnchorError GetSpatialAnchorPose( SpatialAnchorHandle_t unHandle, ETrackingUniverseOrigin eOrigin, SpatialAnchorPose_t *pPoseOut ) override { return (EVRSpatialAnchorError)0; }
    virtual EVRSpatialAnchorError GetSpatialAnchorDescriptor( SpatialAnchorHandle_t unHandle, VR_OUT_STRING() char *pchDescriptorOut, uint32_t *punDescriptorBufferLenInOut ) override { return (EVRSpatialAnchorError)0; }
};

class FakeIVRDebug : public IVRDebug {
public:
    virtual EVRDebugError EmitVrProfilerEvent( const char *pchMessage ) override { return (EVRDebugError)0; }
    virtual EVRDebugError BeginVrProfilerEvent( VrProfilerEventHandle_t *pHandleOut ) override { return (EVRDebugError)0; }
    virtual EVRDebugError FinishVrProfilerEvent( VrProfilerEventHandle_t hHandle, const char *pchMessage ) override { return (EVRDebugError)0; }
    virtual uint32_t DriverDebugRequest( vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, VR_OUT_STRING() char *pchResponseBuffer, uint32_t unResponseBufferSize ) override { return {}; }
};

class FakeIVRIPCResourceManagerClient : public IVRIPCResourceManagerClient {
public:
    virtual bool NewSharedVulkanImage( uint32_t nImageFormat, uint32_t nWidth, uint32_t nHeight, bool bRenderable, bool bMappable, bool bComputeAccess, uint32_t unMipLevels, uint32_t unArrayLayerCount, uint32_t unAdditionalVkCreateFlags, uint32_t unAdditionalVkUsageFlags, vr::SharedTextureHandle_t *pSharedHandle ) override { return false; }
    virtual bool NewSharedVulkanBuffer( uint32_t nSize, uint32_t nUsageFlags, vr::SharedTextureHandle_t *pSharedHandle ) override { return false; }
    virtual bool NewSharedVulkanSemaphore( bool bCounting, vr::SharedTextureHandle_t *pSharedHandle ) override { return false; }
    virtual bool RefResource( vr::SharedTextureHandle_t hSharedHandle, uint64_t *pNewIpcHandle ) override { return false; }
    virtual bool UnrefResource( vr::SharedTextureHandle_t hSharedHandle ) override { return false; }
    virtual bool GetDmabufFormats( uint32_t *pOutFormatCount, uint32_t *pOutFormats ) override { return false; }
    virtual bool GetDmabufModifiers( vr::EVRApplicationType eApplicationType, uint32_t unDRMFormat, uint32_t *pOutModifierCount, uint64_t *pOutModifiers ) override { return false; }
    virtual bool ImportDmabuf( vr::EVRApplicationType eApplicationType, vr::DmabufAttributes_t *pDmabufAttributes, vr::SharedTextureHandle_t *pSharedHandle ) override { return false; }
    virtual bool ReceiveSharedFd( uint64_t ulIpcHandle, int *pOutFd ) override { return false; }
};
} // namespace vr