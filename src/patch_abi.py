import sys

with open("/Users/motonishikoudai/Verantyx-Mirage/src/official_openvr.h", "r") as f:
    content = f.read()

replacements = [
    ("virtual HmdMatrix44_t GetProjectionMatrix( EVREye eEye, float fNearZ, float fFarZ ) = 0;",
     "virtual void GetProjectionMatrix( HmdMatrix44_t* ret, EVREye eEye, float fNearZ, float fFarZ ) = 0;"),
     
    ("virtual HmdMatrix34_t GetEyeToHeadTransform( EVREye eEye ) = 0;",
     "virtual void GetEyeToHeadTransform( HmdMatrix34_t* ret, EVREye eEye ) = 0;"),
     
    ("virtual HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() = 0;",
     "virtual void GetSeatedZeroPoseToStandingAbsoluteTrackingPose( HmdMatrix34_t* ret ) = 0;"),
     
    ("virtual HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() = 0;",
     "virtual void GetRawZeroPoseToStandingAbsoluteTrackingPose( HmdMatrix34_t* ret ) = 0;"),
     
    ("virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty( vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) = 0;",
     "virtual void GetMatrix34TrackedDeviceProperty( HmdMatrix34_t* ret, vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L ) = 0;"),
     
    ("virtual HiddenAreaMesh_t GetHiddenAreaMesh( EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard ) = 0;",
     "virtual void GetHiddenAreaMesh( HiddenAreaMesh_t* ret, EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard ) = 0;"),
     
    ("virtual HmdColor_t GetCurrentFadeColor( bool bBackground = false ) = 0;",
     "virtual void GetCurrentFadeColor( HmdColor_t* ret, bool bBackground = false ) = 0;")
]

for old, new in replacements:
    content = content.replace(old, new)

with open("/Users/motonishikoudai/Verantyx-Mirage/src/official_openvr.h", "w") as f:
    f.write(content)

print("Patched official_openvr.h to fix MinGW struct-return ABI mismatch")
