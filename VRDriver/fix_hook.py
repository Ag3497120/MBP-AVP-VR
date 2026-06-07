import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Fix GetCurrentFadeColor to properly zero out the struct without corrupting VTable
content = content.replace(
    'virtual void GetCurrentFadeColor(HmdColor_t *pRet, bool bBackground = false) override { /* Do nothing to prevent GCC ABI VTable corruption! */ }',
    'virtual void GetCurrentFadeColor(HmdColor_t *pRet, bool bBackground = false) override { if(pRet) { memset(pRet, 0, sizeof(*pRet)); } }'
)

# Remove the D3D11 Shared flag stripping from CreateTexture2D, because it strips KEYEDMUTEX and breaks rendering
old_hook = """    D3D11_TEXTURE2D_DESC newDesc = *pDesc;
    if (newDesc.MiscFlags & D3D11_RESOURCE_MISC_SHARED) {
        newDesc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED;
        FILE* _f = fopen("vr_emulator_log.txt", "a");
        if (_f) { fprintf(_f, "[Verantyx] Stripped D3D11_RESOURCE_MISC_SHARED from Texture2D\\n"); fclose(_f); }
    }
    if (newDesc.MiscFlags & D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX) {
        newDesc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
    }
    return g_pOriginalCreateTexture2D(This, &newDesc, pInitialData, ppTexture2D);"""

new_hook = """    return g_pOriginalCreateTexture2D(This, pDesc, pInitialData, ppTexture2D);"""

content = content.replace(old_hook, new_hook)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Fixed openvr_emulator.cpp!")
