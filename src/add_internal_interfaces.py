import sys

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

dummy_classes = """
class FakeMailbox {
public:
    virtual void* dummy0() { return nullptr; }
    virtual void* dummy1() { return nullptr; }
    virtual void* dummy2() { return nullptr; }
    virtual void* dummy3() { return nullptr; }
    virtual void* dummy4() { return nullptr; }
    virtual void* dummy5() { return nullptr; }
    virtual void* dummy6() { return nullptr; }
    virtual void* dummy7() { return nullptr; }
    virtual void* dummy8() { return nullptr; }
    virtual void* dummy9() { return nullptr; }
    virtual void* dummy10() { return nullptr; }
    virtual void* dummy11() { return nullptr; }
    virtual void* dummy12() { return nullptr; }
    virtual void* dummy13() { return nullptr; }
    virtual void* dummy14() { return nullptr; }
    virtual void* dummy15() { return nullptr; }
    virtual void* dummy16() { return nullptr; }
    virtual void* dummy17() { return nullptr; }
    virtual void* dummy18() { return nullptr; }
    virtual void* dummy19() { return nullptr; }
};

class FakeInputInternal {
public:
    virtual void* dummy0() { return nullptr; }
    virtual void* dummy1() { return nullptr; }
    virtual void* dummy2() { return nullptr; }
    virtual void* dummy3() { return nullptr; }
    virtual void* dummy4() { return nullptr; }
    virtual void* dummy5() { return nullptr; }
    virtual void* dummy6() { return nullptr; }
    virtual void* dummy7() { return nullptr; }
    virtual void* dummy8() { return nullptr; }
    virtual void* dummy9() { return nullptr; }
    virtual void* dummy10() { return nullptr; }
    virtual void* dummy11() { return nullptr; }
    virtual void* dummy12() { return nullptr; }
    virtual void* dummy13() { return nullptr; }
    virtual void* dummy14() { return nullptr; }
    virtual void* dummy15() { return nullptr; }
    virtual void* dummy16() { return nullptr; }
    virtual void* dummy17() { return nullptr; }
    virtual void* dummy18() { return nullptr; }
    virtual void* dummy19() { return nullptr; }
};

class FakeXrProto {
public:
    virtual void* dummy0() { return nullptr; }
    virtual void* dummy1() { return nullptr; }
    virtual void* dummy2() { return nullptr; }
    virtual void* dummy3() { return nullptr; }
    virtual void* dummy4() { return nullptr; }
    virtual void* dummy5() { return nullptr; }
    virtual void* dummy6() { return nullptr; }
    virtual void* dummy7() { return nullptr; }
    virtual void* dummy8() { return nullptr; }
    virtual void* dummy9() { return nullptr; }
    virtual void* dummy10() { return nullptr; }
    virtual void* dummy11() { return nullptr; }
    virtual void* dummy12() { return nullptr; }
    virtual void* dummy13() { return nullptr; }
    virtual void* dummy14() { return nullptr; }
    virtual void* dummy15() { return nullptr; }
    virtual void* dummy16() { return nullptr; }
    virtual void* dummy17() { return nullptr; }
    virtual void* dummy18() { return nullptr; }
    virtual void* dummy19() { return nullptr; }
};

FakeMailbox g_VRMailbox;
FakeInputInternal g_VRInputInternal;
FakeXrProto g_VRXrProto;
"""

if "class FakeMailbox" not in content:
    content = content.replace("FakeSystem g_VRSystem;", dummy_classes + "\nFakeSystem g_VRSystem;")

interfaces = """
    else if (strncmp(pchInterfaceVersion, "IVRMailbox_", 11) == 0) pInterface = &g_VRMailbox;
    else if (strncmp(pchInterfaceVersion, "IVRInputInternal_", 17) == 0) pInterface = &g_VRInputInternal;
    else if (strncmp(pchInterfaceVersion, "IXrProto_", 9) == 0) pInterface = &g_VRXrProto;
"""

if "IVRMailbox_" not in content:
    content = content.replace("else if (strncmp(pchInterfaceVersion, \"IVRDebug_\", 9) == 0) pInterface = &g_VRDebug;", 
                              "else if (strncmp(pchInterfaceVersion, \"IVRDebug_\", 9) == 0) pInterface = &g_VRDebug;\n" + interfaces)

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)

print("Added missing internal interfaces to openvr_emulator.cpp")
