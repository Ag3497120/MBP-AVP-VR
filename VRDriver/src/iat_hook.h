#pragma once
#include <windows.h>
#include <stdio.h>

inline bool HookIAT(const char* moduleName, const char* importModule, const char* functionName, void* newFunction, void** originalFunction) {
    HMODULE hModule = GetModuleHandleA(moduleName);
    if (!hModule) return false;

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);
    PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)hModule + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    while (importDesc->Name) {
        const char* currentImportModule = (const char*)((BYTE*)hModule + importDesc->Name);
        if (_stricmp(currentImportModule, importModule) == 0) {
            PIMAGE_THUNK_DATA thunkIL = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);
            PIMAGE_THUNK_DATA thunkIAT = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);

            while (thunkIL->u1.AddressOfData) {
                if (!(thunkIL->u1.Ordinal & IMAGE_ORDINAL_FLAG)) {
                    PIMAGE_IMPORT_BY_NAME importByName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule + thunkIL->u1.AddressOfData);
                    if (strcmp(importByName->Name, functionName) == 0) {
                        DWORD oldProtect;
                        VirtualProtect(&thunkIAT->u1.Function, sizeof(void*), PAGE_READWRITE, &oldProtect);
                        if (originalFunction) *originalFunction = (void*)thunkIAT->u1.Function;
                        thunkIAT->u1.Function = (ULONGLONG)newFunction;
                        VirtualProtect(&thunkIAT->u1.Function, sizeof(void*), oldProtect, &oldProtect);
                        return true;
                    }
                }
                thunkIL++;
                thunkIAT++;
            }
        }
        importDesc++;
    }
    return false;
}
