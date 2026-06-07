#include <windows.h>
#include <math.h>

#pragma pack(push, 1)
struct SharedHands {
    double poseTimestamp;
    double renderedTimestamp;
    float headTransform[16];
    float leftTransform[16];
    float rightTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
    uint8_t leftTrigger;
    uint8_t rightTrigger;
    uint32_t rightButtons;
    uint32_t leftButtons;
    float rightStickX;
    float rightStickY;
    float leftStickX;
    float leftStickY;
    float rightVelocity[3];
    float leftVelocity[3];
};
#pragma pack(pop)

static HANDLE hMapFile = NULL;
static SharedHands* pSharedHands = NULL;

void InitSharedMemoryDriver() {
    if (hMapFile) return;
    
    // We only need to map the header, since the driver doesn't read the pixels
    const int mapSize = 16 + 4096 * 4096 * 4 + 194;
    
    hMapFile = OpenFileMappingA(FILE_MAP_READ, FALSE, "VerantyxVRSharedHands");
    if (!hMapFile) {
        // Fallback to CreateFileMapping if vrcompositor hasn't created it yet
        HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, mapSize, "VerantyxVRSharedHands");
            CloseHandle(hFile);
        }
    }
    
    if (hMapFile) {
        void* pBuf = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, mapSize);
        if (pBuf) {
            pSharedHands = (SharedHands*)((uint8_t*)pBuf + 16 + 4096 * 4096 * 4);
        }
    }
}

vr::HmdQuaternion_t ExtractQuaternion(const float* m) {
    vr::HmdQuaternion_t q;
    float t = m[0] + m[5] + m[10];
    if (t > 0.0f) {
        float S = sqrtf(1.0f + t) * 2.0f;
        q.w = 0.25f * S;
        q.x = (m[6] - m[9]) / S;
        q.y = (m[8] - m[2]) / S;
        q.z = (m[1] - m[4]) / S;
    } else if ((m[0] > m[5]) && (m[0] > m[10])) {
        float S = sqrtf(1.0f + m[0] - m[5] - m[10]) * 2.0f;
        q.w = (m[6] - m[9]) / S;
        q.x = 0.25f * S;
        q.y = (m[1] + m[4]) / S;
        q.z = (m[8] + m[2]) / S;
    } else if (m[5] > m[10]) {
        float S = sqrtf(1.0f + m[5] - m[0] - m[10]) * 2.0f;
        q.w = (m[8] - m[2]) / S;
        q.x = (m[1] + m[4]) / S;
        q.y = 0.25f * S;
        q.z = (m[6] + m[9]) / S;
    } else {
        float S = sqrtf(1.0f + m[10] - m[0] - m[5]) * 2.0f;
        q.w = (m[1] - m[4]) / S;
        q.x = (m[8] + m[2]) / S;
        q.y = (m[6] + m[9]) / S;
        q.z = 0.25f * S;
    }
    return q;
}

