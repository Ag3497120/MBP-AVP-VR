import re
with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Replace GetCurrentFadeColor
old1 = "virtual void GetCurrentFadeColor(HmdColor_t *pRet, bool bBackground = false) override {"
new1 = "virtual void GetCurrentFadeColor() {"
content = content.replace(old1, new1)

# Replace GetFadeColor
old2 = "virtual void GetFadeColor(HmdColor_t *pRet, bool bBackground = false) override {"
new2 = "virtual void GetFadeColor() {"
content = content.replace(old2, new2)

# Remove the memset inside them!
# Actually it's easier to just regex the whole function
content = re.sub(r'virtual void GetCurrentFadeColor\([^)]*\)\s*(?:override\s*)?{\s*if\(pRet\)\s*{\s*memset\(pRet,\s*0,\s*sizeof\(\*pRet\)\);\s*}\s*}', 'virtual void GetCurrentFadeColor() { }', content)
content = re.sub(r'virtual void GetFadeColor\([^)]*\)\s*(?:override\s*)?{\s*if\(pRet\)\s*{\s*memset\(pRet,\s*0,\s*sizeof\(\*pRet\)\);\s*}\s*}', 'virtual void GetFadeColor() { }', content)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Fixed ABI!")
