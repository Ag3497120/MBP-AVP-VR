#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

struct VRPosePacket {
    uint32_t magic;
    float headTransform[16];
    float leftHandTransform[16];
    float rightHandTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
};

int main() {
    printf("Size: %zu\n", sizeof(struct VRPosePacket));
    printf("Offset of headTransform: %zu\n", offsetof(struct VRPosePacket, headTransform));
    printf("Offset of leftHandTransform: %zu\n", offsetof(struct VRPosePacket, leftHandTransform));
    return 0;
}
