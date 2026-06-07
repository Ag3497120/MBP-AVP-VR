import re
with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

old_fallback_render = """                    } else {
                        // Identity fallback at 1.5m height
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f;
                    }"""

new_fallback_render = """                    } else {
                        // Safe offset fallback
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        if (i == 0) { pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; }
                        else if (i == 1) { pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][3] = -0.2f; pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.3f; pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][3] = -0.3f; }
                        else if (i == 2) { pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][3] = 0.2f; pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.3f; pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][3] = -0.3f; }
                    }"""
content = content.replace(old_fallback_render, new_fallback_render)

old_fallback_game = """                    } else {
                        // Identity fallback at 1.5m height
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // 1.5 meters high
                    }"""

new_fallback_game = """                    } else {
                        // Safe offset fallback
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        if (i == 0) { pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; }
                        else if (i == 1) { pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][3] = -0.2f; pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.3f; pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][3] = -0.3f; }
                        else if (i == 2) { pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][3] = 0.2f; pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.3f; pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][3] = -0.3f; }
                    }"""
content = content.replace(old_fallback_game, new_fallback_game)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Fixed Fallback!")
