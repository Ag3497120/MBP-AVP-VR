import re
with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

dummy_class = """
class MyDummyInterface {
public:
"""
for i in range(100):
    dummy_class += f"    virtual void* dummy{i}() {{ return nullptr; }}\n"
dummy_class += """};
MyDummyInterface g_DummyInterface;
"""

# Insert dummy class before MyFakeVRSystem
content = content.replace("class MyFakeVRSystem", dummy_class + "class MyFakeVRSystem")

# In VR_GetGenericInterface, add fallback for any other string to return &g_DummyInterface
content = content.replace("    if (peError) *peError = VRInitError_Init_InterfaceNotFound;\n    return nullptr;", "    if (peError) *peError = VRInitError_None;\n    return &g_DummyInterface;")

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)
