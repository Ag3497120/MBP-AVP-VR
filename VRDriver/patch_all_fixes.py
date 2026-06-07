import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# 1. D3D11 Strip
old_create = "return g_pOriginalCreateTexture2D(This, pDesc, pInitialData, ppTexture2D);"
new_create = """D3D11_TEXTURE2D_DESC newDesc = *pDesc;
    if (newDesc.MiscFlags & D3D11_RESOURCE_MISC_SHARED) {
        newDesc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED;
        FILE* _f = fopen("vr_emulator_log.txt", "a");
        if (_f) { fprintf(_f, "[Verantyx] Stripped D3D11_RESOURCE_MISC_SHARED from Texture2D\\n"); fclose(_f); }
    }
    if (newDesc.MiscFlags & D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX) {
        newDesc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
    }
    return g_pOriginalCreateTexture2D(This, &newDesc, pInitialData, ppTexture2D);"""
if old_create in content:
    content = content.replace(old_create, new_create)
    print("Applied D3D11 Strip fix")

# 2. Matrix Transpose
if 'c*4 + r' in content:
    content = content.replace('c*4 + r', 'r*4 + c')
    print("Applied Matrix Transpose fix")

# 3. srcTransform zero check
if 'if (srcTransform && srcTransform[0] != 0.0f) {' in content:
    content = content.replace('if (srcTransform && srcTransform[0] != 0.0f) {', 'if (srcTransform && srcTransform[15] != 0.0f) {')
    print("Applied srcTransform zero check fix")

# 4. ABI Fixes
if 'virtual vr::HmdColor_t* GetCurrentFadeColor(HmdColor_t *pRet, bool bBackground = false) override {' in content:
    content = content.replace('virtual vr::HmdColor_t* GetCurrentFadeColor', 'virtual void GetCurrentFadeColor')
    print("Applied ABI fix for GetCurrentFadeColor")
if 'virtual vr::HmdColor_t* GetFadeColor(HmdColor_t *pRet, bool bBackground = false) override {' in content:
    content = content.replace('virtual vr::HmdColor_t* GetFadeColor', 'virtual void GetFadeColor')
    print("Applied ABI fix for GetFadeColor")

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)

print("Patching complete.")
