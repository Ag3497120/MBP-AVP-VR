import sys

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

# Add new instances
instances = """
FakeHeadsetView g_VRHeadsetView;
FakeNotifications g_VRNotifications;
FakeOverlayView g_VROverlayView;
FakeExtendedDisplay g_VRExtendedDisplay;
FakeTrackedCamera g_VRTrackedCamera;
FakeScreenshots g_VRScreenshots;
FakeResources g_VRResources;
FakeDriverManager g_VRDriverManager;
FakeIOBuffer g_VRIOBuffer;
FakeSpatialAnchors g_VRSpatialAnchors;
FakeDebug g_VRDebug;
"""

if "FakeOverlayView g_VROverlayView;" not in content:
    content = content.replace("MyVRInput g_VRInput;", "MyVRInput g_VRInput;\n" + instances)

# Add interfaces
interfaces = """
    else if (strncmp(pchInterfaceVersion, "IVRHeadsetView_", 15) == 0) pInterface = &g_VRHeadsetView;
    else if (strncmp(pchInterfaceVersion, "IVRNotifications_", 17) == 0) pInterface = &g_VRNotifications;
    else if (strncmp(pchInterfaceVersion, "IVROverlayView_", 15) == 0) pInterface = &g_VROverlayView;
    else if (strncmp(pchInterfaceVersion, "IVRExtendedDisplay_", 19) == 0) pInterface = &g_VRExtendedDisplay;
    else if (strncmp(pchInterfaceVersion, "IVRTrackedCamera_", 17) == 0) pInterface = &g_VRTrackedCamera;
    else if (strncmp(pchInterfaceVersion, "IVRScreenshots_", 15) == 0) pInterface = &g_VRScreenshots;
    else if (strncmp(pchInterfaceVersion, "IVRResources_", 13) == 0) pInterface = &g_VRResources;
    else if (strncmp(pchInterfaceVersion, "IVRDriverManager_", 17) == 0) pInterface = &g_VRDriverManager;
    else if (strncmp(pchInterfaceVersion, "IVRIOBuffer_", 12) == 0) pInterface = &g_VRIOBuffer;
    else if (strncmp(pchInterfaceVersion, "IVRSpatialAnchors_", 18) == 0) pInterface = &g_VRSpatialAnchors;
    else if (strncmp(pchInterfaceVersion, "IVRDebug_", 9) == 0) pInterface = &g_VRDebug;
"""

if "IVROverlayView_" not in content:
    content = content.replace("else if (strncmp(pchInterfaceVersion, \"IVRInput_\", 9) == 0) pInterface = &g_VRInput;", 
                              "else if (strncmp(pchInterfaceVersion, \"IVRInput_\", 9) == 0) pInterface = &g_VRInput;\n" + interfaces)

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)

print("Added missing interfaces to openvr_emulator.cpp")
