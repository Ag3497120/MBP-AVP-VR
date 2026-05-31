with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "r") as f:
    content = f.read()

proxy_code = """
HMODULE g_hRealOpenVR = nullptr;
typedef uint32_t (*VR_InitInternal_t)(EVRInitError *peError, EVRApplicationType eType);
typedef uint32_t (*VR_InitInternal2_t)(EVRInitError *peError, EVRApplicationType eType, const char* pStartupInfo);
typedef void (*VR_ShutdownInternal_t)();
typedef void* (*VR_GetGenericInterface_t)(const char *pchInterfaceVersion, EVRInitError *peError);
typedef uint32_t (*VR_GetInitToken_t)();

VR_InitInternal_t Real_VR_InitInternal = nullptr;
VR_InitInternal2_t Real_VR_InitInternal2 = nullptr;
VR_ShutdownInternal_t Real_VR_ShutdownInternal = nullptr;
VR_GetGenericInterface_t Real_VR_GetGenericInterface = nullptr;
VR_GetInitToken_t Real_VR_GetInitToken = nullptr;

void LoadRealOpenVR() {
    if (g_hRealOpenVR) return;
    g_hRealOpenVR = LoadLibraryA("real_openvr_api.dll");
    if (g_hRealOpenVR) {
        Real_VR_InitInternal = (VR_InitInternal_t)GetProcAddress(g_hRealOpenVR, "VR_InitInternal");
        Real_VR_InitInternal2 = (VR_InitInternal2_t)GetProcAddress(g_hRealOpenVR, "VR_InitInternal2");
        Real_VR_ShutdownInternal = (VR_ShutdownInternal_t)GetProcAddress(g_hRealOpenVR, "VR_ShutdownInternal");
        Real_VR_GetGenericInterface = (VR_GetGenericInterface_t)GetProcAddress(g_hRealOpenVR, "VR_GetGenericInterface");
        Real_VR_GetInitToken = (VR_GetInitToken_t)GetProcAddress(g_hRealOpenVR, "VR_GetInitToken");
        
        FILE* f = fopen("C:\\\\vr_emulator.log", "a");
        if (f) { fprintf(f, "Successfully loaded real_openvr_api.dll!\\n"); fclose(f); }
    } else {
        FILE* f = fopen("C:\\\\vr_emulator.log", "a");
        if (f) { fprintf(f, "FAILED to load real_openvr_api.dll!\\n"); fclose(f); }
    }
}
"""

content = content.replace("extern \"C\" {", proxy_code + "\nextern \"C\" {\n")

init1_replacement = """
__declspec(dllexport) uint32_t VR_InitInternal(EVRInitError *peError, EVRApplicationType eType) {
    LoadRealOpenVR();
    uint32_t token = 1;
    if (Real_VR_InitInternal) {
        token = Real_VR_InitInternal(peError, eType);
    } else {
        if (peError) *peError = VRInitError_None;
    }
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_InitInternal called! Proxy Token: %u\\n", token); fclose(f); }
    return token;
}
"""
content = content.replace("""__declspec(dllexport) uint32_t VR_InitInternal(EVRInitError *peError, EVRApplicationType eType) {
    if (peError) *peError = VRInitError_None;
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_InitInternal called!\\n"); fclose(f); }
    return 1;
}""", init1_replacement.strip())

init2_replacement = """
__declspec(dllexport) uint32_t VR_InitInternal2(EVRInitError *peError, EVRApplicationType eType, const char* pStartupInfo) {
    LoadRealOpenVR();
    uint32_t token = 1;
    if (Real_VR_InitInternal2) {
        token = Real_VR_InitInternal2(peError, eType, pStartupInfo);
    } else {
        if (peError) *peError = VRInitError_None;
    }
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_InitInternal2 called! Proxy Token: %u\\n", token); fclose(f); }
    return token;
}
"""
content = content.replace("""__declspec(dllexport) uint32_t VR_InitInternal2(EVRInitError *peError, EVRApplicationType eType, const char* pStartupInfo) {
    if (peError) *peError = VRInitError_None;
    FILE* f = fopen("C:\\\\vr_emulator.log", "a");
    if (f) { fprintf(f, "VR_InitInternal2 called!\\n"); fclose(f); }
    return 1;
}""", init2_replacement.strip())

get_init_token_replacement = """
__declspec(dllexport) uint32_t VR_GetInitToken() {
    LoadRealOpenVR();
    if (Real_VR_GetInitToken) return Real_VR_GetInitToken();
    return 1;
}
"""
content = content.replace("__declspec(dllexport) uint32_t VR_GetInitToken() { return 1; }", get_init_token_replacement.strip())

content = content.replace("    if (strncmp(pchInterfaceVersion, \"IVRMailbox\", 10) == 0) return &g_DummyMailbox;\n    return &g_DummyInterface;", """
    LoadRealOpenVR();
    if (Real_VR_GetGenericInterface) {
        void* real_interface = Real_VR_GetGenericInterface(pchInterfaceVersion, peError);
        FILE* log = fopen("C:\\\\vr_emulator.log", "a");
        if (log) { fprintf(log, "Forwarded %s to REAL OpenVR: %p\\n", pchInterfaceVersion, real_interface); fclose(log); }
        return real_interface;
    }
    
    if (peError) *peError = VRInitError_Init_InterfaceNotFound;
    return nullptr;
""")

with open("/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp", "w") as f:
    f.write(content)
