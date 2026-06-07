#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <IOSurface/IOSurface.h>
#import <objc/runtime.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mach/mach.h>
#include <pthread.h>
#include <mach-o/dyld.h>

#define THUNK_PORT 49191

static int extract_iosurface_id(uint64_t* ptr) {
    if (!ptr) return 0;
    
    // First, check offset 48 (index 6) for the nested resource object
    uint64_t nested_ptr = 0;
    vm_size_t read_size = sizeof(nested_ptr);
    if (vm_read_overwrite(mach_task_self(), (vm_address_t)(ptr + 6), sizeof(nested_ptr), (vm_address_t)&nested_ptr, &read_size) == KERN_SUCCESS) {
        if (nested_ptr > 0x100000000ULL) {
            printf("[mac_thunk] Scanning nested object at offset 48: %p\n", (void*)nested_ptr);
            fflush(stdout);
            
            for (int j = 0; j < 64; j++) {
                uint64_t val = 0;
                vm_size_t val_size = sizeof(val);
                if (vm_read_overwrite(mach_task_self(), (vm_address_t)(nested_ptr + j*8), sizeof(val), (vm_address_t)&val, &val_size) == KERN_SUCCESS) {
                    if (val > 0x100000000ULL) {
                        uintptr_t isa = 0;
                        vm_size_t isa_size = sizeof(isa);
                        if (vm_read_overwrite(mach_task_self(), (vm_address_t)val, sizeof(isa), (vm_address_t)&isa, &isa_size) == KERN_SUCCESS) {
                            if (isa > 0x7ff000000000ULL || isa > 0x0000000100000000ULL) {
                                // VERY unsafe heuristic: if it has a valid-looking ISA, check if it's an MTLTexture by trying to call conformsToProtocol
                                // Instead of object_getClassName which can crash, we'll use a safer approach for this targeted object
                                const char* name = "?";
                                @try {
                                    name = object_getClassName((__bridge id)(void*)val);
                                    if (name && (strstr(name, "Texture") != NULL || strstr(name, "AGX") != NULL)) {
                                        id<MTLTexture> tex = (__bridge id<MTLTexture>)(void*)val;
                                        if ([tex respondsToSelector:@selector(width)]) {
                                            printf("[mac_thunk] FOUND MTLTexture (class %s) in nested offset %d. Size: %lux%lu\n", name, j * 8, (unsigned long)tex.width, (unsigned long)tex.height);
                                            fflush(stdout);
                                            if ([tex respondsToSelector:@selector(iosurface)]) {
                                                IOSurfaceRef surf = (__bridge IOSurfaceRef)([tex performSelector:@selector(iosurface)]);
                                                if (surf) {
                                                    int iosid = IOSurfaceGetID(surf);
                                                    printf("[mac_thunk] -> Has IOSurfaceID: %d\n", iosid);
                                                    fflush(stdout);
                                                    return iosid;
                                                }
                                            }
                                            printf("[mac_thunk] -> No IOSurface attached to this texture.\n");
                                            fflush(stdout);
                                            return 9999;
                                        }
                                    }
                                } @catch (NSException *e) {
                                    // Ignore
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    printf("[mac_thunk] No MTLTexture found in nested object.\n");
    fflush(stdout);
    return 0;
}

static void* file_poller_thread(void* arg) {
    printf("[mac_thunk] File poller started.\n");
    fflush(stdout);
    
    // We poll /tmp/mac_thunk_ptr.bin
    uint64_t last_ptr = 0;
    while (1) {
        FILE* f = fopen("/tmp/mac_thunk_ptr.bin", "rb");
        if (f) {
            uint64_t ptr_val = 0;
            if (fread(&ptr_val, 1, sizeof(ptr_val), f) == sizeof(ptr_val)) {
                if (ptr_val != last_ptr && ptr_val != 0) {
                    printf("[mac_thunk] Read new pointer from file: 0x%llx\n", ptr_val);
                    fflush(stdout);
                    last_ptr = ptr_val;
                    
                    int iosid = extract_iosurface_id((uint64_t*)ptr_val);
                    
                    FILE* out = fopen("/tmp/mac_thunk_iosid.bin", "wb");
                    if (out) {
                        fwrite(&iosid, 1, sizeof(iosid), out);
                        fclose(out);
                    }
                }
            }
            fclose(f);
        }
        usleep(50000); // 50ms
    }
    return NULL;
}

__attribute__((constructor))
static void custom_init() {
    NSArray *args = [[NSProcessInfo processInfo] arguments];
    BOOL is_target = NO;
    for (NSString *arg in args) {
        if ([arg containsString:@"test_d3dmetal.exe"] || [arg containsString:@"hlvr.exe"]) {
            is_target = YES;
            break;
        }
    }
    
    if (!is_target) {
        return;
    }
    
    printf("[mac_thunk] Loaded into target process!\n");
    fflush(stdout);
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        file_poller_thread(NULL);
    });
}
