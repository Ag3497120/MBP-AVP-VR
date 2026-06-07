import re

with open('/Users/motonishikoudai/verantyx-cli/cli/VRDriver/src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Fix D3D11 shared stripping
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
content = content.replace(old_create, new_create)

# Fix srcTransform[0] to srcTransform[15] (homogenous 1.0f check)
content = content.replace("if (srcTransform && srcTransform[0] != 0.0f) {", "if (srcTransform && srcTransform[15] != 0.0f) {")

# Add the 1.5m fallback
old_fallback = """                        // Identity fallback
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;"""
new_fallback = """                        // Identity fallback at 1.5m height
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f;"""
content = content.replace(old_fallback, new_fallback)

old_fallback_game = """                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;"""
new_fallback_game = """                        // Identity fallback at 1.5m height
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // 1.5 meters high"""
content = content.replace(old_fallback_game, new_fallback_game)


with open('/Users/motonishikoudai/verantyx-cli/cli/VRDriver/src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
