with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

mailbox_class = """
class MyDummyMailbox {
public:
    virtual intptr_t m0() { return 0; }
    virtual intptr_t m1() { return 0; }
    virtual intptr_t m2() { return 0; }
    virtual intptr_t m3() { return 0; }
    virtual intptr_t m4() { return 0; }
    virtual intptr_t m5() { return 10; } // Return some error (e.g., empty)
    virtual intptr_t m6() { return 10; }
    virtual intptr_t m7() { return 10; }
    virtual intptr_t m8() { return 10; }
    virtual intptr_t m9() { return 10; }
    virtual intptr_t m10() { return 10; }
};
MyDummyMailbox g_DummyMailbox;
"""

content = content.replace("class MyFakeVRSystem", mailbox_class + "class MyFakeVRSystem")
content = content.replace("if (strncmp(pchInterfaceVersion, \"IVRMailbox\", 10) == 0) return &g_DummyInterface;", "if (strncmp(pchInterfaceVersion, \"IVRMailbox\", 10) == 0) return &g_DummyMailbox;")
# Just in case we didn't have IVRMailbox explicitly in the ifs before, let's add it before returning g_DummyInterface
if "IVRMailbox" not in content:
    content = content.replace("return &g_DummyInterface;", "if (strncmp(pchInterfaceVersion, \"IVRMailbox\", 10) == 0) return &g_DummyMailbox;\n    return &g_DummyInterface;")


poll_event_impl = """
    virtual bool PollNextEvent( VREvent_t *pEvent, uint32_t uncbVREvent ) override {
        static int eventCount = 0;
        if (eventCount == 0) {
            pEvent->eventType = VREvent_TrackedDeviceActivated;
            pEvent->trackedDeviceIndex = 0;
            pEvent->eventAgeSeconds = 0.0f;
            eventCount++;
            return true;
        }
        return false;
    }
"""

content = content.replace("virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel( vr::TrackedDeviceIndex_t unDeviceId ) override {", poll_event_impl + "\n    virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel( vr::TrackedDeviceIndex_t unDeviceId ) override {")

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)
