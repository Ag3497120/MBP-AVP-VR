import sys
import re

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

new_input_class = """class MyVRInput : public FakeInput {
    uint64_t HashStr(const char* str) {
        if (!str) return 1;
        uint64_t hash = 5381;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash == 0 ? 1 : hash; // Ensure it's never 0 (k_ulInvalidActionHandle)
    }
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        return VRInputError_None;
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        if (pHandle) *pHandle = HashStr(pchActionSetName);
        return VRInputError_None;
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        if (pHandle) *pHandle = HashStr(pchActionName);
        return VRInputError_None;
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        if (pHandle) *pHandle = HashStr(pchInputSourcePath);
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
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        if (pActionData && unActionDataSize == sizeof(InputPoseActionData_t)) {
            memset(pActionData, 0, sizeof(InputPoseActionData_t));
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

print("Updated MyVRInput with HashStr in openvr_emulator.cpp")
