#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <objc/runtime.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <malloc/malloc.h>
#include <mach/mach.h>

// Original write
static ssize_t (*orig_write)(int fd, const void *buf, size_t count);

static void scanTextureMemory(uint64_t* ptr) {
    printf("[Mac Dylib] Scanning texture memory at %p\n", ptr);
    for (int i = 0; i < 64; i++) {
        uint64_t val = ptr[i];
        if (val > 0x100000000ULL) { // Plausible pointer
            // Safely read the isa pointer using mach_vm_read
            uintptr_t isa = 0;
            mach_msg_type_number_t read_size = sizeof(isa);
            if (vm_read_overwrite(mach_task_self(), (vm_address_t)val, sizeof(isa), (vm_address_t)&isa, &read_size) == KERN_SUCCESS) {
                if (isa > 0x7ff000000000ULL || isa > 0x0000000100000000ULL) {
                    const char* name = object_getClassName((__bridge id)(void*)val);
                    if (name && strlen(name) > 0) {
                        printf("[Mac Dylib] Offset %d: Pointer %p is ObjC Class '%s'\n", i * 8, (void*)val, name);
                        if (strstr(name, "Texture") != NULL || strstr(name, "AGX") != NULL) {
                            id<MTLTexture> tex = (__bridge id<MTLTexture>)(void*)val;
                            printf("[Mac Dylib] FOUND MTLTexture! width=%lu, height=%lu\n", (unsigned long)tex.width, (unsigned long)tex.height);
                            if ([tex respondsToSelector:@selector(iosurface)]) {
                                IOSurfaceRef surf = [tex iosurface];
                                printf("[Mac Dylib] IOSurfaceID: %d\n", IOSurfaceGetID(surf));
                            }
                        }
                    }
                }
            }
        }
    }
}

// Hook write using dlsym (DYLD_INSERT_LIBRARIES will interpose it if we export it)
__attribute__((visibility("default")))
ssize_t write(int fd, const void *buf, size_t count) {
    if (!orig_write) {
        orig_write = dlsym(RTLD_NEXT, "write");
    }
    
    if (fd == 1 || fd == 2) {
        char temp[512] = {0};
        size_t copy_len = count < 511 ? count : 511;
        memcpy(temp, buf, copy_len);
        
        const char* prefix = "Texture created at: ";
        const char* match = strstr(temp, prefix);
        if (match) {
            uint64_t addr = 0;
            sscanf(match + strlen(prefix), "%llx", &addr);
            if (addr != 0) {
                // Call scan immediately
                scanTextureMemory((uint64_t*)addr);
            }
        }
    }
    
    return orig_write(fd, buf, count);
}

__attribute__((constructor))
static void custom_init() {
    printf("[Mac Dylib] Loaded into process! Write hook installed.\n");
}
