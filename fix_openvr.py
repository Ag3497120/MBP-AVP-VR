import re

with open('/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Fix mock methods returning struct pointers to return void and take the pointer as first arg
# Based on the error: conflicting return type specified for 'virtual vr::HmdColor_t* Mock_IVRCompositor::GetCurrentFadeColor(vr::HmdColor_t*, bool)'
# Note: overridden function is 'virtual void vr::IVRCompositor::GetCurrentFadeColor(vr::HmdColor_t*, bool)'

# Replace `virtual vr::HmdColor_t* GetCurrentFadeColor` -> `virtual void GetCurrentFadeColor`
content = re.sub(r'virtual\s+vr::[A-Za-z0-9_]+_t\*\s+([A-Za-z0-9_]+)\(', r'virtual void \1(', content)
content = re.sub(r'virtual\s+[A-Za-z0-9_]+_t\*\s+([A-Za-z0-9_]+)\(', r'virtual void \1(', content)

# And inside those functions, they probably do `return pRet;`. We should replace `return pRet;` with `return;` inside those specific methods if needed.
# Since it's C++, returning `pRet` in a void function is a compiler error.
# Actually, the user's codebase might just need `void` instead of `Type*`.
# Let's just fix all of them.

with open('/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
