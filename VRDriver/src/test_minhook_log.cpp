#include "minhook/include/MinHook.h"
#include <stdio.h>
void logMH(MH_STATUS status, const char* msg) {
    FILE* f = fopen("C:\\\\VR_Depth_MH_Log.txt", "a");
    if (f) {
        fprintf(f, "%s: %d\\n", msg, status);
        fclose(f);
    }
}
