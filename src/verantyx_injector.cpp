#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

void InjectDLL(HANDLE hProcess, const char* dllPath) {
    size_t len = strlen(dllPath) + 1;
    LPVOID pRemoteMem = VirtualAllocEx(hProcess, NULL, len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteMem) {
        printf("VirtualAllocEx failed: %lu\n", GetLastError());
        return;
    }
    WriteProcessMemory(hProcess, pRemoteMem, dllPath, len, NULL);
    
    // Get absolute address of LoadLibraryA in kernel32.dll
    // Since kernel32.dll is loaded at the same base address across processes in Windows/Wine, this works.
    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC pLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
    
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteMem, 0, NULL);
    if (hThread) {
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
    } else {
        printf("CreateRemoteThread failed: %lu\n", GetLastError());
    }
    VirtualFreeEx(hProcess, pRemoteMem, 0, MEM_RELEASE);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: verantyx_injector.exe <executable> [args...]\n");
        return 1;
    }
    
    char cmdLine[4096] = {0};
    for(int i = 1; i < argc; i++) {
        strcat(cmdLine, argv[i]);
        if (i < argc - 1) strcat(cmdLine, " ");
    }
    
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    printf("Starting process suspended: %s\n", cmdLine);
    if (CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        char dllPath[MAX_PATH];
        GetFullPathNameA("verantyx_hook.dll", MAX_PATH, dllPath, NULL);
        printf("Injecting %s into PID %lu\n", dllPath, pi.dwProcessId);
        
        InjectDLL(pi.hProcess, dllPath);
        
        printf("Resuming process...\n");
        ResumeThread(pi.hThread);
        
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        printf("Launch successful.\n");
    } else {
        printf("Failed to create process: %lu\n", GetLastError());
    }
    return 0;
}
