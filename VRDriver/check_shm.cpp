#include <windows.h>
#include <stdio.h>
#include <stdint.h>

int main() {
    HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 16, NULL);
        if (hMap) {
            void* pBuf = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 16);
            if (pBuf) {
                uint32_t* pSeq = (uint32_t*)pBuf;
                printf("Sequence Number: %u\n", pSeq[0]);
                printf("Width: %u\n", pSeq[1]);
                printf("Height: %u\n", pSeq[2]);
                printf("Format: %u\n", pSeq[3]);
                UnmapViewOfFile(pBuf);
            }
            CloseHandle(hMap);
        }
        CloseHandle(hFile);
    } else {
        printf("Failed to open C:\\vr_shared_frame.dat\n");
    }
    return 0;
}
