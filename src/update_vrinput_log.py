import sys
import re

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

new_input_class = r"""class MyVRInput : public FakeInput {
    uint64_t HashStr(const char* str) {
        if (!str) return 1;
        uint64_t hash = 5381;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        }
        return hash == 0 ? 1 : hash; // Ensure it's never 0 (k_ulInvalidActionHandle)
    }
    void LogCall(const char* name) {
        FILE* _f = fopen("C:\\vr_emulator.log", "a");
        if (_f) { fprintf(_f, "MyVRInput::%s\n", name); fclose(_f); }
    }
public:
    virtual EVRInputError SetActionManifestPath(const char *pchActionManifestPath) override {
        LogCall("SetActionManifestPath");
        return VRInputError_None;
    }
    virtual EVRInputError GetActionSetHandle(const char *pchActionSetName, VRActionSetHandle_t *pHandle) override {
        LogCall("GetActionSetHandle");
        if (pHandle) *pHandle = HashStr(pchActionSetName);
        return VRInputError_None;
    }
    virtual EVRInputError GetActionHandle(const char *pchActionName, VRActionHandle_t *pHandle) override {
        LogCall("GetActionHandle");
        if (pHandle) *pHandle = HashStr(pchActionName);
        return VRInputError_None;
    }
    virtual EVRInputError GetInputSourceHandle(const char *pchInputSourcePath, VRInputValueHandle_t *pHandle) override {
        LogCall("GetInputSourceHandle");
        if (pHandle) *pHandle = HashStr(pchInputSourcePath);
        return VRInputError_None;
    }
    virtual EVRInputError GetDigitalActionData(VRActionHandle_t action, InputDigitalActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        LogCall("GetDigitalActionData");
        if (pActionData && unActionDataSize == sizeof(InputDigitalActionData_t)) {
            memset(pActionData, 0, sizeof(InputDigitalActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetAnalogActionData(VRActionHandle_t action, InputAnalogActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        LogCall("GetAnalogActionData");
        if (pActionData && unActionDataSize == sizeof(InputAnalogActionData_t)) {
            memset(pActionData, 0, sizeof(InputAnalogActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataRelativeToNow(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, float fPredictedSecondsFromNow, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        LogCall("GetPoseActionDataRelativeToNow");
        if (pActionData && unActionDataSize == sizeof(InputPoseActionData_t)) {
            memset(pActionData, 0, sizeof(InputPoseActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetPoseActionDataForNextFrame(VRActionHandle_t action, ETrackingUniverseOrigin eOrigin, InputPoseActionData_t *pActionData, uint32_t unActionDataSize, VRInputValueHandle_t ulRestrictToDevice) override {
        LogCall("GetPoseActionDataForNextFrame");
        if (pActionData && unActionDataSize == sizeof(InputPoseActionData_t)) {
            memset(pActionData, 0, sizeof(InputPoseActionData_t));
        }
        return VRInputError_None;
    }
    virtual EVRInputError GetSkeletalActionData(VRActionHandle_t action, InputSkeletalActionData_t *pActionData, uint32_t unActionDataSize) override {
        LogCall("GetSkeletalActionData");
        if (pActionData && unActionDataSize == sizeof(InputSkeletalActionData_t)) {
            memset(pActionData, 0, sizeof(InputSkeletalActionData_t));
        }
        return VRInputError_None;
    }
};"""

# Use string replace instead of re.sub to avoid backslash issues
start_marker = "class MyVRInput : public FakeInput {"
end_marker = "};"
start_idx = content.find(start_marker)

if start_idx != -1:
    # Find the matching closing brace for the class
    brace_count = 0
    in_class = False
    end_idx = -1
    for i in range(start_idx, len(content)):
        if content[i] == '{':
            brace_count += 1
            in_class = True
        elif content[i] == '}':
            brace_count -= 1
            if in_class and brace_count == 0:
                # found end of class
                # skip until semicolon
                for j in range(i, len(content)):
                    if content[j] == ';':
                        end_idx = j + 1
                        break
                break
    if end_idx != -1:
        content = content[:start_idx] + new_input_class + content[end_idx:]

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)

print("Updated MyVRInput with logging safely")
