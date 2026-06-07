#include <windows.h>
#include <stdio.h>
#include <string>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string basePath = exePath;
    size_t lastSlash = basePath.find_last_of("\\/");
    if (lastSlash != std::string::npos) {
        basePath = basePath.substr(0, lastSlash);
    }

    std::string targetExe = basePath + "\\dxvk\\vrcompositor.exe";
    std::string args = lpCmdLine ? lpCmdLine : "";
    std::string cmdLine = "\"" + targetExe + "\" " + args;
    char cmdBuf[32768];
    snprintf(cmdBuf, sizeof(cmdBuf), "%s", cmdLine.c_str());

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };

    if (CreateProcessA(targetExe.c_str(), cmdBuf, NULL, NULL, FALSE, 0, NULL, basePath.c_str(), &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode = 0;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return exitCode;
    }

    return 1;
}
