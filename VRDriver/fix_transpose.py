import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Revert my broken transpose fix which scrambled the camera matrices
content = content.replace(
    'pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = srcTransform[r*4 + c];',
    'pRenderPoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = srcTransform[c*4 + r];'
)
content = content.replace(
    'pGamePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = srcTransform[r*4 + c];',
    'pGamePoseArray[i].mDeviceToAbsoluteTracking.m[r][c] = srcTransform[c*4 + r];'
)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Reverted transpose!")
