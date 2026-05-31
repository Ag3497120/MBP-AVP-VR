import sys
import re

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

new_input_class = """class MyVRInput : public FakeInput {
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        FILE* f = fopen("C:\\\\\\\\vr_emulator.log", "a"); if(f){ fprintf(f, "MyVRInput::SetActionManifestPath(%s)\\\\n", pchActionManifestPath); fclose(f); }
        return VRInputError_None;
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        static VRActionSetHandle_t next_handle = 1000;
        if (pHandle) *pHandle = next_handle++;
        FILE* f = fopen("C:\\\\\\\\vr_emulator.log", "a"); if(f){ fprintf(f, "MyVRInput::GetActionSetHandle(%s) -> %llu\\\\n", pchActionSetName, (unsigned long long)*pHandle); fclose(f); }
        return VRInputError_None;
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        static VRActionHandle_t next_handle = 2000;
        if (pHandle) *pHandle = next_handle++;
        FILE* f = fopen("C:\\\\\\\\vr_emulator.log", "a"); if(f){ fprintf(f, "MyVRInput::GetActionHandle(%s) -> %llu\\\\n", pchActionName, (unsigned long long)*pHandle); fclose(f); }
        return VRInputError_None;
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        static VRInputValueHandle_t next_handle = 3000;
        if (pHandle) *pHandle = next_handle++;
        FILE* f = fopen("C:\\\\\\\\vr_emulator.log", "a"); if(f){ fprintf(f, "MyVRInput::GetInputSourceHandle(%s) -> %llu\\\\n", pchInputSourcePath, (unsigned long long)*pHandle); fclose(f); }
        return VRInputError_None;
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputDigitalActionData_t)) {
            memset(pActionData, 0, sizeof(InputDigitalActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputAnalogActionData_t)) {
            memset(pActionData, 0, sizeof(InputAnalogActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataRelativeToNow(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputPoseActionData_t)) {
            memset(pActionData, 0, sizeof(InputPoseActionData_t));
            pActionData->bActive = true;
            pActionData->pose.bPoseIsValid = true;
            pActionData->pose.bDeviceIsConnected = true;
            pActionData->pose.mDeviceToAbsoluteTracking = Identity34_Gen();
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputPoseActionData_t)) {
            memset(pActionData, 0, sizeof(InputPoseActionData_t));
            pActionData->bActive = true;
            pActionData->pose.bPoseIsValid = true;
            pActionData->pose.bDeviceIsConnected = true;
            pActionData->pose.mDeviceToAbsoluteTracking = Identity34_Gen();
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetSkeletalActionData(VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize) override {
        if (pActionData && unActionDataSize == sizeof(InputSkeletalActionData_t)) {
            memset(pActionData, 0, sizeof(InputSkeletalActionData_t));
        }
        return VRInputError_None;
    }
};"""

content = re.sub(r'class MyVRInput : public FakeInput \{.*?\};', new_input_class, content, flags=re.DOTALL)

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)

print("Updated MyVRInput with tracing and unique handles in openvr_emulator.cpp")
