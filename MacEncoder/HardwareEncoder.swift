import Foundation
setvbuf(stdout, nil, _IONBF, 0)
import VideoToolbox
import CoreMedia
import Network
import Accelerate

import simd

// MARK: - IPC and UDP Structures

struct SharedFrame {
    var sequenceNumber: UInt32
    var width: UInt32
    var height: UInt32
    var format: UInt32
}

struct UDPHeader {
    var magic: UInt32 // 0x5652414E ("VRAN")
    var frameSequence: UInt32
    var chunkIndex: UInt32
    var totalChunks: UInt32
    var chunkOffset: UInt32
    var payloadSize: UInt32
    var timestamp: Double
}

public struct JCrossNode_VR {
    public var nodeId: UInt16
    public var flags: UInt16
    public var position: simd_float3
    public var rotation: simd_quatf
    public var velocity: simd_float3
    public var angularVelocity: simd_float3
}

public struct JCrossZone_Input {
    public var parentNodeId: UInt16
    public var buttonMask: UInt32
    public var triggerValue: Float
    public var gripValue: Float
    public var thumbstick: simd_float2
}

public struct JCrossPayload {
    public var frameId: UInt32
    public var timestamp: Double
    public var headNode: JCrossNode_VR
    public var leftHandNode: JCrossNode_VR
    public var rightHandNode: JCrossNode_VR
    public var leftInput: JCrossZone_Input
    public var rightInput: JCrossZone_Input
}

struct JoyconPacket {
    var magic: UInt32 // 0x4A4F5943 ("JOYC")
    var rightButtons: UInt32
    var leftButtons: UInt32
    var rightStickX: Float
    var rightStickY: Float
    var leftStickX: Float
    var leftStickY: Float
    var rightVelocityX: Float
    var rightVelocityY: Float
    var rightVelocityZ: Float
    var leftVelocityX: Float
    var leftVelocityY: Float
    var leftVelocityZ: Float
}

// MARK: - Setup Network

var vrConnection: NWConnection?

// MARK: - Hand Tracking & Video Connection (AWDL Bonjour)
let netService = NetService(domain: "local.", type: "_verantyxvr._udp.", name: "VerantyxVR", port: 9999)
netService.includesPeerToPeer = true
netService.publish(options: [])
print("Published Bonjour _verantyxvr._udp on port 9999. Waiting for Vision Pro connection (AWDL enabled)...")

// MARK: - VideoToolbox Callback

var cachedVPS: Data?
var cachedSPS: Data?
var cachedPPS: Data?

func processSampleBuffer(sampleBuffer: CMSampleBuffer, isKeyFrame: Bool, frameSequence: UInt32) {
    guard let dataBuffer = CMSampleBufferGetDataBuffer(sampleBuffer) else { return }
    var length = 0
    var dataPointer: UnsafeMutablePointer<Int8>? = nil
    CMBlockBufferGetDataPointer(dataBuffer, atOffset: 0, lengthAtOffsetOut: nil, totalLengthOut: &length, dataPointerOut: &dataPointer)
    
    guard let data = dataPointer else { return }
    
    var frameData = Data()
    
    // 1. Extract and Cache SPS and PPS
    if let formatDesc = CMSampleBufferGetFormatDescription(sampleBuffer) {
        var vpsSize: Int = 0
        var vpsCount: Int = 0
        var vpsPtr: UnsafePointer<UInt8>? = nil
        if CMVideoFormatDescriptionGetHEVCParameterSetAtIndex(formatDesc, parameterSetIndex: 0, parameterSetPointerOut: &vpsPtr, parameterSetSizeOut: &vpsSize, parameterSetCountOut: &vpsCount, nalUnitHeaderLengthOut: nil) == noErr, let vps = vpsPtr {
            cachedVPS = Data(bytes: vps, count: vpsSize)
        }
        
        var spsSize: Int = 0
        var spsCount: Int = 0
        var spsPtr: UnsafePointer<UInt8>? = nil
        if CMVideoFormatDescriptionGetHEVCParameterSetAtIndex(formatDesc, parameterSetIndex: 1, parameterSetPointerOut: &spsPtr, parameterSetSizeOut: &spsSize, parameterSetCountOut: &spsCount, nalUnitHeaderLengthOut: nil) == noErr, let sps = spsPtr {
            cachedSPS = Data(bytes: sps, count: spsSize)
        }
        
        var ppsSize: Int = 0
        var ppsCount: Int = 0
        var ppsPtr: UnsafePointer<UInt8>? = nil
        if CMVideoFormatDescriptionGetHEVCParameterSetAtIndex(formatDesc, parameterSetIndex: 2, parameterSetPointerOut: &ppsPtr, parameterSetSizeOut: &ppsSize, parameterSetCountOut: &ppsCount, nalUnitHeaderLengthOut: nil) == noErr, let pps = ppsPtr {
            cachedPPS = Data(bytes: pps, count: ppsSize)
        }
    }
    
    // Prepend to EVERY frame so the Vision Pro can always decode instantly even if a keyframe is dropped
    let startCode: [UInt8] = [0, 0, 0, 1]
    if let vps = cachedVPS {
        frameData.append(contentsOf: startCode)
        frameData.append(vps)
    }
    if let sps = cachedSPS {
        frameData.append(contentsOf: startCode)
        frameData.append(sps)
    }
    if let pps = cachedPPS {
        frameData.append(contentsOf: startCode)
        frameData.append(pps)
    }
    
    // 2. AVCC to Annex-B Conversion
    var offset = 0
    while offset < length - 4 {
        var nalLength: UInt32 = 0
        memcpy(&nalLength, data + offset, 4)
        nalLength = UInt32(bigEndian: nalLength)
        
        let startCode: [UInt8] = [0, 0, 0, 1]
        frameData.append(contentsOf: startCode)
        frameData.append(data.advanced(by: offset + 4).withMemoryRebound(to: UInt8.self, capacity: Int(nalLength)) { $0 }, count: Int(nalLength))
        
        offset += 4 + Int(nalLength)
    }
    
    // 3. UDP Fragmentation and Transmission
    let mtu = 1400
    let totalBytes = frameData.count
    let numFragments = (totalBytes + mtu - 1) / mtu
    
    frameData.withUnsafeBytes { rawBuffer in
        guard let baseAddress = rawBuffer.bindMemory(to: UInt8.self).baseAddress else { return }
        
        for i in 0..<numFragments {
            let chunkOffset = i * mtu
            let fragSize = min(mtu, totalBytes - chunkOffset)
            
            let pts = CMSampleBufferGetPresentationTimeStamp(sampleBuffer)
            let timestamp = CMTimeGetSeconds(pts)
            
            var header = UDPHeader(
                magic: UInt32(0x5652414E).littleEndian, // "VRAN"
                frameSequence: frameSequence.littleEndian,
                chunkIndex: UInt32(i).littleEndian,
                totalChunks: UInt32(numFragments).littleEndian,
                chunkOffset: UInt32(chunkOffset).littleEndian,
                payloadSize: UInt32(fragSize).littleEndian,
                timestamp: timestamp
            )
            
            var packet = Data()
            withUnsafeBytes(of: &header) { pktHeader in
                packet.append(contentsOf: pktHeader)
            }
            packet.append(baseAddress + chunkOffset, count: fragSize)
            
            packet.withUnsafeBytes { rawBytes in
                if targetAddrLen > 0 {
                    let _ = sendto(sock, rawBytes.baseAddress, packet.count, 0, withUnsafePointer(to: &targetAddr) {
                        $0.withMemoryRebound(to: sockaddr.self, capacity: 1) { $0 }
                    }, targetAddrLen)
                }
            }
            
            // Smart Pacing disabled for Direct AP connection (Internet Sharing) to minimize latency
            // if i % 20 == 19 { usleep(1000) }
        }
    }
}

// MARK: - VideoToolbox Setup

var targetIP = ""

var sock = socket(AF_INET6, SOCK_DGRAM, 0)
var opt: Int32 = 1
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, socklen_t(MemoryLayout<Int32>.size))

var v6only: Int32 = 0
setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &v6only, socklen_t(MemoryLayout<Int32>.size))

var bindAddr = sockaddr_in6()
bindAddr.sin6_len = UInt8(MemoryLayout<sockaddr_in6>.size)
bindAddr.sin6_family = sa_family_t(AF_INET6)
bindAddr.sin6_port = in_port_t(9999).bigEndian
bindAddr.sin6_addr = in6addr_any

let bindResult = withUnsafePointer(to: &bindAddr) {
    $0.withMemoryRebound(to: sockaddr.self, capacity: 1) {
        bind(sock, $0, socklen_t(MemoryLayout<sockaddr_in6>.size))
    }
}

if bindResult < 0 {
    print("Failed to bind sock to 9999")
    exit(1)
}

var targetAddr = sockaddr_storage()
var targetAddrLen: socklen_t = 0

var compressionSession: VTCompressionSession?
var isEncoding = true
var totalBytesEncoded = 0

func setupEncoder(width: Int32, height: Int32) {
    let status = VTCompressionSessionCreate(allocator: kCFAllocatorDefault,
                                            width: width,
                                            height: height,
                                            codecType: kCMVideoCodecType_HEVC,
                                            encoderSpecification: nil,
                                            imageBufferAttributes: nil,
                                            compressedDataAllocator: nil,
                                            outputCallback: { (outputCallbackRefCon, sourceFrameRefCon, status, infoFlags, sampleBuffer) in
        var frameSequence: UInt32 = 0
        if let sourceFrameRefCon = sourceFrameRefCon {
            let refConPtr = sourceFrameRefCon.assumingMemoryBound(to: UInt32.self)
            frameSequence = refConPtr.pointee
            refConPtr.deinitialize(count: 1)
            refConPtr.deallocate()
        }
        
        if status != noErr {
            print("Encoding error: \(status)")
            return
        }
        guard let sampleBuffer = sampleBuffer else { return }
        let isKeyFrame = !infoFlags.contains(.frameDropped) && (CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, createIfNecessary: false) as? [[CFString: Any]] == nil || (CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, createIfNecessary: false) as? [[CFString: Any]])?.first?[kCMSampleAttachmentKey_NotSync] == nil)
        
        processSampleBuffer(sampleBuffer: sampleBuffer, isKeyFrame: isKeyFrame, frameSequence: frameSequence)
        
    },
                                            refcon: nil,
                                            compressionSessionOut: &compressionSession)
    
    if status != noErr {
        print("Failed to create VTCompressionSession")
        exit(1)
    }
    
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_RealTime, value: kCFBooleanTrue)
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_ProfileLevel, value: kVTProfileLevel_HEVC_Main_AutoLevel)
    
    // Critical settings for ultra-low latency VR streaming
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_AllowFrameReordering, value: kCFBooleanFalse) // Disable B-frames
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_PrioritizeEncodingSpeedOverQuality, value: kCFBooleanTrue)
    
    var bitrate: Int32 = 150_000_000 // Bump to 150 Mbps for ultra-high quality direct AP streaming
    let bitrateNum = CFNumberCreate(kCFAllocatorDefault, .sInt32Type, &bitrate)
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_AverageBitRate, value: bitrateNum)
    
    // Explicitly tell the encoder to pace itself at 90fps for buttery smoothness (native AVP refresh rate)
    var expectedFPS: Int32 = 90
    let expectedFPSNum = CFNumberCreate(kCFAllocatorDefault, .sInt32Type, &expectedFPS)
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_ExpectedFrameRate, value: expectedFPSNum)
    
    // Force an I-frame every 0.5 seconds (45 frames at 90fps) to ensure immediate recovery from dropped UDP packets!
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_MaxKeyFrameInterval, value: 45 as CFNumber)
    
    VTCompressionSessionPrepareToEncodeFrames(compressionSession!)
}

// MARK: - Main IPC Loop

let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let mapSize = 16 + 4096 * 4096 * 4 + 1024

print("Waiting for vr_shared_frame.dat...")
while true {
    if FileManager.default.fileExists(atPath: filePath) {
        do {
            let attr = try FileManager.default.attributesOfItem(atPath: filePath)
            if let fileSize = attr[FileAttributeKey.size] as? UInt64 {
                if fileSize >= mapSize {
                    break
                }
            }
        } catch {}
    }
    Thread.sleep(forTimeInterval: 0.5)
}

let fd = open(filePath, O_RDWR)
if fd < 0 {
    print("Failed to open file")
    exit(1)
}

let mapPtr = mmap(nil, mapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
if mapPtr == MAP_FAILED {
    print("mmap failed")
    exit(1)
}

let headerPtr = mapPtr!.bindMemory(to: SharedFrame.self, capacity: 1)
let pixelPtr = mapPtr! + MemoryLayout<SharedFrame>.size
let handsMapPtr = mapPtr! + 16 + 4096 * 4096 * 4

var lastSeq: UInt32 = 0
var pixelBuffer: CVPixelBuffer?
var framesEncoded = 0
var currentFrameSequence: UInt32 = 0
var currentWidth = 0
var currentHeight = 0

// MARK: - Joy-Con UDP Server
DispatchQueue.global(qos: .userInteractive).async {
    let jcSock = socket(AF_INET, SOCK_DGRAM, 0)
    var opt: Int32 = 1
    setsockopt(jcSock, SOL_SOCKET, SO_REUSEADDR, &opt, socklen_t(MemoryLayout<Int32>.size))
    
    var bindAddr = sockaddr_in()
    bindAddr.sin_len = UInt8(MemoryLayout<sockaddr_in>.size)
    bindAddr.sin_family = sa_family_t(AF_INET)
    bindAddr.sin_port = in_port_t(11002).bigEndian
    bindAddr.sin_addr.s_addr = INADDR_ANY
    
    bind(jcSock, withUnsafePointer(to: &bindAddr) {
        $0.withMemoryRebound(to: sockaddr.self, capacity: 1) { $0 }
    }, socklen_t(MemoryLayout<sockaddr_in>.size))
    
    var buffer = [UInt8](repeating: 0, count: 2048)
    while true {
        var senderAddr = sockaddr()
        var senderLen = socklen_t(MemoryLayout<sockaddr>.size)
        let bytesRead = recvfrom(jcSock, &buffer, buffer.count, 0, &senderAddr, &senderLen)
        
        var ipStr = [CChar](repeating: 0, count: Int(INET_ADDRSTRLEN))
        let senderAddrIn = withUnsafePointer(to: &senderAddr) {
            $0.withMemoryRebound(to: sockaddr_in.self, capacity: 1) { $0.pointee }
        }
        var sin_addr = senderAddrIn.sin_addr
        inet_ntop(AF_INET, &sin_addr, &ipStr, socklen_t(INET_ADDRSTRLEN))
        let senderIP = String(cString: ipStr)
        
        if senderIP == "127.0.0.1" && bytesRead == MemoryLayout<JoyconPacket>.size {
            buffer.withUnsafeBytes { rawBuffer in
                let jc = rawBuffer.load(as: JoyconPacket.self)
                if jc.magic == UInt32(0x4A4F5943) { // "JOYC"
                    // SharedHands offsets:
                    // 0-191: Matrices
                    // 192-195: Pinch & Triggers
                    // 196: rightButtons, 200: leftButtons
                    // 204: rightStickX, 208: rightStickY, 212: leftStickX, 216: leftStickY
                    // 220: rightVel, 232: leftVel
                    handsMapPtr.advanced(by: 196).storeBytes(of: jc.rightButtons, as: UInt32.self)
                    handsMapPtr.advanced(by: 200).storeBytes(of: jc.leftButtons, as: UInt32.self)
                    handsMapPtr.advanced(by: 204).storeBytes(of: jc.rightStickX, as: Float.self)
                    handsMapPtr.advanced(by: 208).storeBytes(of: jc.rightStickY, as: Float.self)
                    handsMapPtr.advanced(by: 212).storeBytes(of: jc.leftStickX, as: Float.self)
                    handsMapPtr.advanced(by: 216).storeBytes(of: jc.leftStickY, as: Float.self)
                    handsMapPtr.advanced(by: 220).storeBytes(of: jc.rightVelocityX, as: Float.self)
                    handsMapPtr.advanced(by: 224).storeBytes(of: jc.rightVelocityY, as: Float.self)
                    handsMapPtr.advanced(by: 228).storeBytes(of: jc.rightVelocityZ, as: Float.self)
                    handsMapPtr.advanced(by: 232).storeBytes(of: jc.leftVelocityX, as: Float.self)
                    handsMapPtr.advanced(by: 236).storeBytes(of: jc.leftVelocityY, as: Float.self)
                    handsMapPtr.advanced(by: 240).storeBytes(of: jc.leftVelocityZ, as: Float.self)
                }
            }
        }
    }
}

// removed duplicate netservice
print("Published Bonjour _verantyxvr._udp on port 9999. Waiting for Vision Pro connection (AWDL enabled)...")

struct VRPosePacket {
    var magic: UInt32 // 0x45534F50 ("POSE" little endian)
    var padding: UInt32
    var timestamp: Double
    var headTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var rightHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftPinch: UInt8
    var rightPinch: UInt8
}

// Receive loop using POSIX socket on port 9999
DispatchQueue.global(qos: .userInteractive).async {
    var buffer = [UInt8](repeating: 0, count: 2048)
    while true {
        var senderAddr = sockaddr_storage()
        var senderLen = socklen_t(MemoryLayout<sockaddr_storage>.size)
        
        let bytesRead = withUnsafeMutablePointer(to: &senderAddr) {
            $0.withMemoryRebound(to: sockaddr.self, capacity: 1) {
                recvfrom(sock, &buffer, buffer.count, 0, $0, &senderLen)
            }
        }
        
        if bytesRead >= 210 {
            buffer.withUnsafeBytes { rawBuffer in
                guard let baseAddr = rawBuffer.baseAddress else { return }
                var magic: UInt32 = 0
                memcpy(&magic, baseAddr, 4)

                if magic == 0x45534F50 || magic == 0x504F5345 { // "POSE"
                    // Vision Pro payload: magic(4) + pad(4) + ts(8) = 16 bytes offset
                    memcpy(handsMapPtr, baseAddr + 16, 64)       // Head (offset 0)
                    memcpy(handsMapPtr + 64, baseAddr + 80, 64)  // Left (offset 64)
                    memcpy(handsMapPtr + 128, baseAddr + 144, 64)// Right (offset 128)
                    
                    let visionLeftPinch = baseAddr.load(fromByteOffset: 208, as: UInt8.self)
                    let visionRightPinch = baseAddr.load(fromByteOffset: 209, as: UInt8.self)
                    let visionLeftTrigger = baseAddr.load(fromByteOffset: 210, as: UInt8.self)
                    let visionRightTrigger = baseAddr.load(fromByteOffset: 211, as: UInt8.self)
                    
                    handsMapPtr.advanced(by: 192).storeBytes(of: visionLeftPinch, as: UInt8.self)
                    handsMapPtr.advanced(by: 193).storeBytes(of: visionRightPinch, as: UInt8.self)
                    handsMapPtr.advanced(by: 194).storeBytes(of: visionLeftTrigger, as: UInt8.self)
                    handsMapPtr.advanced(by: 195).storeBytes(of: visionRightTrigger, as: UInt8.self)
                    
                    if bytesRead >= 236 {
                        let rightButtons = baseAddr.load(fromByteOffset: 212, as: UInt32.self)
                        let leftButtons = baseAddr.load(fromByteOffset: 216, as: UInt32.self)
                        let rightStickX = baseAddr.load(fromByteOffset: 220, as: Float.self)
                        let rightStickY = baseAddr.load(fromByteOffset: 224, as: Float.self)
                        let leftStickX = baseAddr.load(fromByteOffset: 228, as: Float.self)
                        let leftStickY = baseAddr.load(fromByteOffset: 232, as: Float.self)
                        
                        handsMapPtr.advanced(by: 196).storeBytes(of: rightButtons, as: UInt32.self)
                        handsMapPtr.advanced(by: 200).storeBytes(of: leftButtons, as: UInt32.self)
                        handsMapPtr.advanced(by: 204).storeBytes(of: rightStickX, as: Float.self)
                        handsMapPtr.advanced(by: 208).storeBytes(of: rightStickY, as: Float.self)
                        handsMapPtr.advanced(by: 212).storeBytes(of: leftStickX, as: Float.self)
                        handsMapPtr.advanced(by: 216).storeBytes(of: leftStickY, as: Float.self)
                    }
                    
                    var ipStr = [CChar](repeating: 0, count: Int(INET6_ADDRSTRLEN))
                    if senderAddr.ss_family == sa_family_t(AF_INET6) {
                        let senderAddrIn6 = withUnsafePointer(to: &senderAddr) {
                            $0.withMemoryRebound(to: sockaddr_in6.self, capacity: 1) { $0.pointee }
                        }
                        var sin6_addr = senderAddrIn6.sin6_addr
                        inet_ntop(AF_INET6, &sin6_addr, &ipStr, socklen_t(INET6_ADDRSTRLEN))
                    } else if senderAddr.ss_family == sa_family_t(AF_INET) {
                        let senderAddrIn = withUnsafePointer(to: &senderAddr) {
                            $0.withMemoryRebound(to: sockaddr_in.self, capacity: 1) { $0.pointee }
                        }
                        var sin_addr = senderAddrIn.sin_addr
                        inet_ntop(AF_INET, &sin_addr, &ipStr, socklen_t(INET_ADDRSTRLEN))
                    }
                    
                    let currentSenderIP = String(cString: ipStr)
                    
                    if targetIP != currentSenderIP && currentSenderIP != "127.0.0.1" && currentSenderIP != "::1" && currentSenderIP != "" {
                        targetIP = currentSenderIP
                        print("Auto-discovered Vision Pro IP: \(targetIP)")
                    }
                    
                    // Store the entire sockaddr_storage to respond exactly to the same address/port
                    targetAddr = senderAddr
                    targetAddrLen = senderLen
                }
            }
        }
    }
}
print("Published Bonjour _verantyxvr._udp. Waiting for Vision Pro connection (AWDL enabled)...")

print("Starting native read loop and UDP transmission on port 9999...")

while isEncoding {
    autoreleasepool {
        let currentSeq = headerPtr.pointee.sequenceNumber
        if currentSeq != lastSeq && currentSeq > 0 {
            let width = Int(headerPtr.pointee.width)
            let height = Int(headerPtr.pointee.height)
            
            if width != currentWidth || height != currentHeight {
                if compressionSession != nil {
                    VTCompressionSessionInvalidate(compressionSession!)
                    compressionSession = nil
                }
                pixelBuffer = nil
                currentWidth = width
                currentHeight = height
                print("Resolution changed to \(width)x\(height)")
            }
            
            if compressionSession == nil && width > 0 && height > 0 {
                print("Initializing VideoToolbox encoder for \(width)x\(height)...")
                fflush(stdout)
                setupEncoder(width: Int32(width), height: Int32(height))
            }
            
            if width > 0 && height > 0 {
                if pixelBuffer == nil {
                    CVPixelBufferCreate(kCFAllocatorDefault, width, height, kCVPixelFormatType_32BGRA, nil, &pixelBuffer)
                }
                
                if let pb = pixelBuffer {
                    CVPixelBufferLockBaseAddress(pb, [])
                    let dst = CVPixelBufferGetBaseAddress(pb)!
                    let bytesPerRow = CVPixelBufferGetBytesPerRow(pb)
                    
                    var srcBuffer = vImage_Buffer(data: UnsafeMutableRawPointer(mutating: pixelPtr), height: vImagePixelCount(height), width: vImagePixelCount(width), rowBytes: Int(width * 4))
                    var dstBuffer = vImage_Buffer(data: dst, height: vImagePixelCount(height), width: vImagePixelCount(width), rowBytes: bytesPerRow)
                    let map: [UInt8] = [2, 1, 0, 3] // Swap R (0) and B (2) -> BGRA
                    vImagePermuteChannels_ARGB8888(&srcBuffer, &dstBuffer, map, vImage_Flags(kvImageNoFlags))
                    
                    CVPixelBufferUnlockBaseAddress(pb, [])
                    
                    let presentationTime = CMTime(value: CMTimeValue(framesEncoded), timescale: 90)
                    currentFrameSequence = currentSeq
                    
                    let refConPtr = UnsafeMutablePointer<UInt32>.allocate(capacity: 1)
                    refConPtr.initialize(to: currentSeq)
                    
                    let status = VTCompressionSessionEncodeFrame(compressionSession!,
                                                                 imageBuffer: pb,
                                                                 presentationTimeStamp: presentationTime,
                                                                 duration: CMTime.invalid,
                                                                 frameProperties: nil,
                                                                 sourceFrameRefcon: UnsafeMutableRawPointer(refConPtr),
                                                                 infoFlagsOut: nil)
                    
                    if status != noErr {
                        print("VTCompressionSessionEncodeFrame failed: \(status)")
                        refConPtr.deinitialize(count: 1)
                        refConPtr.deallocate()
                    }
                    
                    framesEncoded += 1
                }
            }
            
            lastSeq = currentSeq
        }
    }
    usleep(1000)
}
