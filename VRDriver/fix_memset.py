import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Zero-initialize the pose array elements in the fallback cases to prevent stack garbage from causing black screens
old_render_fallback = """                        // Identity fallback at 1.5m height
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f;"""
new_render_fallback = """                        // Identity fallback at 1.5m height
                        memset(&pRenderPoseArray[i].mDeviceToAbsoluteTracking, 0, sizeof(pRenderPoseArray[i].mDeviceToAbsoluteTracking));
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f;"""
content = content.replace(old_render_fallback, new_render_fallback)

old_game_fallback = """                        // Identity fallback at 1.5m height
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // 1.5 meters high"""
new_game_fallback = """                        // Identity fallback at 1.5m height
                        memset(&pGamePoseArray[i].mDeviceToAbsoluteTracking, 0, sizeof(pGamePoseArray[i].mDeviceToAbsoluteTracking));
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[0][0] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][1] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[2][2] = 1.0f;
                        pGamePoseArray[i].mDeviceToAbsoluteTracking.m[1][3] = 1.5f; // 1.5 meters high"""
content = content.replace(old_game_fallback, new_game_fallback)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Added memset to fallbacks!")
