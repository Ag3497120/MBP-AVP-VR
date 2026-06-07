#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    char exePath[4096] = {0};
    GetModuleFileNameA(NULL, exePath, 4000);
    
    char targetExe[4096] = {0};
    strcpy(targetExe, exePath);
    
    char* lastSlash = strrchr(targetExe, '\\');
    if (!lastSlash) lastSlash = strrchr(targetExe, '/');
    if (lastSlash) {
        *lastSlash = '\0';
    }
    
    strcat(targetExe, "\\dxvk\\vrcompositor.exe");
    
    char* cmdBuf = (char*)malloc(32768);
    if (!cmdBuf) return 1;
    cmdBuf[0] = '\0';
    
    char* origCmd = GetCommandLineA();
    if (origCmd) {
        // Just pass the entire original command line directly.
        // It starts with the wrapper's executable path, but Windows programs usually
        // ignore argv[0] anyway. We'll just replace the first token with targetExe.
        int inQuotes = 0;
        char* p = origCmd;
        while (*p) {
            if (*p == '"') inQuotes = !inQuotes;
            else if (*p == ' ' && !inQuotes) break;
            p++;
        }
        if (*p == ' ') p++;
        
        strcpy(cmdBuf, "\"");
        strcat(cmdBuf, targetExe);
        strcat(cmdBuf, "\" ");
        strcat(cmdBuf, p);
    } else {
        strcpy(cmdBuf, "\"");
        strcat(cmdBuf, targetExe);
        strcat(cmdBuf, "\"");
    }
    
    STARTUPINFOA si = {0};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {0};
    
    if (CreateProcessA(targetExe, cmdBuf, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode = 0;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        free(cmdBuf);
        return exitCode;
    }
    
    free(cmdBuf);
    return 1;
}
