import re
with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Replace GetCurrentFadeColor back to exact signature but empty body
old1 = "virtual void GetCurrentFadeColor() { }"
new1 = "virtual void GetCurrentFadeColor(HmdColor_t *pRet, bool bBackground = false) override { /* Do nothing to prevent GCC ABI VTable corruption! */ }"
content = content.replace(old1, new1)

# Replace GetFadeColor back to exact signature but empty body
old2 = "virtual void GetFadeColor() { }"
new2 = "virtual void GetFadeColor(HmdColor_t *pRet, bool bBackground = false) override { /* Do nothing to prevent GCC ABI VTable corruption! */ }"
content = content.replace(old2, new2)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Fixed ABI Abstract Class Error!")
