#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

struct SharedFrame {
    uint32_t sequenceNumber;
    uint32_t width;
    uint32_t height;
    uint32_t format;
};

#pragma pack(push, 1)
struct SharedHands {
    float headTransform[16];
    float leftTransform[16];
    float rightTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
};

struct UDPPacket {
    uint32_t magic; // 0x5652414E ("VRAN")
    uint32_t sequenceNumber;
    uint32_t chunkIndex;
    uint32_t totalChunks;
    uint32_t chunkOffset;
    uint32_t payloadSize;
    uint8_t payload[60000];
};

struct VRPosePacket {
    uint32_t magic; // 0x504F5345 ("POSE")
    float headTransform[16];
    float leftHandTransform[16];
    float rightHandTransform[16];
    uint8_t leftPinch;
    uint8_t rightPinch;
};
#pragma pack(pop)

void TrackingThread(SharedHands* pSharedHands) {
    SOCKET recvSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (recvSock == INVALID_SOCKET) {
        printf("Tracking socket creation failed\n");
        return;
    }
    
    sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(11001);
    inet_pton(AF_INET, "127.0.0.1", &bindAddr.sin_addr);
    
    if (bind(recvSock, (sockaddr*)&bindAddr, sizeof(bindAddr)) == SOCKET_ERROR) {
        printf("Tracking socket bind failed\n");
        closesocket(recvSock);
        return;
    }
    
    printf("Tracking thread listening on 127.0.0.1:11001...\n");
    
    VRPosePacket posePkt;
    sockaddr_in senderAddr;
    int senderLen = sizeof(senderAddr);
    
    while (true) {
        int bytes = recvfrom(recvSock, (char*)&posePkt, sizeof(VRPosePacket), 0, (sockaddr*)&senderAddr, &senderLen);
        if (bytes == sizeof(VRPosePacket) && posePkt.magic == 0x504F5345) {
            if (pSharedHands) {
                memcpy(pSharedHands->headTransform, posePkt.headTransform, 64);
                memcpy(pSharedHands->leftTransform, posePkt.leftHandTransform, 64);
                memcpy(pSharedHands->rightTransform, posePkt.rightHandTransform, 64);
                pSharedHands->leftPinch = posePkt.leftPinch;
                pSharedHands->rightPinch = posePkt.rightPinch;
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: tb_streamer.exe <target_ip> <port>\n");
        printf("Example: tb_streamer.exe 192.168.100.2 11000\n");
        return 1;
    }
    
    const char* targetIp = argv[1];
    int targetPort = atoi(argv[2]);
    
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }
    
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }
    
    int bufSize = 8 * 1024 * 1024; // 8MB send buffer
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&bufSize, sizeof(bufSize));
    
    sockaddr_in targetAddr;
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetIp, &targetAddr.sin_addr);
    
    const int mapSize = 16 + 1920 * 1080 * 4 + 200;
    HANDLE hMapFile = NULL;
    void* pBuf = NULL;
    
    printf("Creating or opening VerantyxVRSharedFrame...\n");
    while (true) {
        HANDLE hFile = CreateFileA("C:\\vr_shared_frame.dat", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            LARGE_INTEGER fsize;
            GetFileSizeEx(hFile, &fsize);
            if (fsize.QuadPart < mapSize) {
                SetFilePointer(hFile, mapSize - 1, NULL, FILE_BEGIN);
                DWORD written;
                WriteFile(hFile, "", 1, &written, NULL);
            }
            hMapFile = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, mapSize, NULL);
            if (hMapFile) {
                pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, mapSize);
                if (pBuf) {
                    printf("Successfully connected to shared memory!\n");
                    break;
                }
                CloseHandle(hMapFile);
            }
            CloseHandle(hFile);
        }
        Sleep(1000);
    }
    
    SharedFrame* pHeader = (SharedFrame*)pBuf;
    uint8_t* pPixelData = (uint8_t*)pBuf + sizeof(SharedFrame);
    SharedHands* pSharedHands = (SharedHands*)((uint8_t*)pBuf + 16 + 1920 * 1080 * 4);
    
    std::thread trackThread(TrackingThread, pSharedHands);
    trackThread.detach();
    
    uint32_t lastSeq = 0;
    UDPPacket packet;
    packet.magic = 0x5652414E;
    
    printf("Streaming frames to %s:%d...\n", targetIp, targetPort);
    
    while (true) {
        uint32_t currentSeq = pHeader->sequenceNumber;
        if (currentSeq != lastSeq && currentSeq != 0) {
            lastSeq = currentSeq;
            
            uint32_t frameSize = pHeader->width * pHeader->height * 4;
            if (frameSize == 0 || frameSize > 1920 * 1080 * 4) {
                // Invalid or uninitialized frame, wait
                Sleep(1);
                continue;
            }
            
            uint32_t chunkSize = 60000;
            uint32_t totalChunks = (frameSize + chunkSize - 1) / chunkSize;
            
            packet.sequenceNumber = currentSeq;
            packet.totalChunks = totalChunks;
            
            for (uint32_t i = 0; i < totalChunks; i++) {
                packet.chunkIndex = i;
                packet.chunkOffset = i * chunkSize;
                
                uint32_t remaining = frameSize - packet.chunkOffset;
                packet.payloadSize = (remaining < chunkSize) ? remaining : chunkSize;
                
                memcpy(packet.payload, pPixelData + packet.chunkOffset, packet.payloadSize);
                
                int packetSize = sizeof(UDPPacket) - 60000 + packet.payloadSize;
                
                sendto(sock, (char*)&packet, packetSize, 0, (sockaddr*)&targetAddr, sizeof(targetAddr));
            }
            // Add a small delay to avoid overwhelming the UDP stack buffer
            Sleep(1); 
        } else {
            // Spinwait with yield to avoid 100% CPU on 1 core, but keep latency low
            Sleep(1);
        }
    }
    
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    closesocket(sock);
    WSACleanup();
    return 0;
}
