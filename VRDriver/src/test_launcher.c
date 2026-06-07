#include <windows.h>
#include <stdio.h>

int main() {
    STARTUPINFOA si = {0};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi = {0};
    if (CreateProcessA(NULL, (LPSTR)"test_script.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        printf("Success\n");
    } else {
        printf("Failed: %lu\n", GetLastError());
    }
    return 0;
}
