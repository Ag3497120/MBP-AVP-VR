import Foundation
setvbuf(stdout, nil, _IONBF, 0)
import VideoToolbox
import CoreMedia
import Network

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
}

struct VRPosePacket {
    var magic: UInt32 // 0x504F5345 ("POSE")
    var headTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var rightHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftPinch: UInt8
    var rightPinch: UInt8
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

let parameters = NWParameters.udp
parameters.includePeerToPeer = true
parameters.allowLocalEndpointReuse = true

let port = NWEndpoint.Port(rawValue: 9999)!
let vrListener = try! NWListener(using: parameters, on: port)
vrListener.service = NWListener.Service(name: "VerantyxVR", type: "_verantyxvr._udp")

// MARK: - VideoToolbox Callback

func processSampleBuffer(sampleBuffer: CMSampleBuffer, isKeyFrame: Bool, frameSequence: UInt32) {
    guard let dataBuffer = CMSampleBufferGetDataBuffer(sampleBuffer) else { return }
    var length = 0
    var dataPointer: UnsafeMutablePointer<Int8>? = nil
    CMBlockBufferGetDataPointer(dataBuffer, atOffset: 0, lengthAtOffsetOut: nil, totalLengthOut: &length, dataPointerOut: &dataPointer)
    
    guard let data = dataPointer else { return }
    
    var frameData = Data()
    
    // 1. Extract SPS and PPS for Keyframes
    if isKeyFrame {
        if let formatDesc = CMSampleBufferGetFormatDescription(sampleBuffer) {
            var vpsSize: Int = 0
            var vpsCount: Int = 0
            var vpsPtr: UnsafePointer<UInt8>? = nil
            CMVideoFormatDescriptionGetHEVCParameterSetAtIndex(formatDesc, parameterSetIndex: 0, parameterSetPointerOut: &vpsPtr, parameterSetSizeOut: &vpsSize, parameterSetCountOut: &vpsCount, nalUnitHeaderLengthOut: nil)
            
            var spsSize: Int = 0
            var spsCount: Int = 0
            var spsPtr: UnsafePointer<UInt8>? = nil
            CMVideoFormatDescriptionGetHEVCParameterSetAtIndex(formatDesc, parameterSetIndex: 1, parameterSetPointerOut: &spsPtr, parameterSetSizeOut: &spsSize, parameterSetCountOut: &spsCount, nalUnitHeaderLengthOut: nil)
            
            var ppsSize: Int = 0
            var ppsCount: Int = 0
            var ppsPtr: UnsafePointer<UInt8>? = nil
            CMVideoFormatDescriptionGetHEVCParameterSetAtIndex(formatDesc, parameterSetIndex: 2, parameterSetPointerOut: &ppsPtr, parameterSetSizeOut: &ppsSize, parameterSetCountOut: &ppsCount, nalUnitHeaderLengthOut: nil)
            
            let startCode: [UInt8] = [0, 0, 0, 1]
            if let vps = vpsPtr {
                frameData.append(contentsOf: startCode)
                frameData.append(vps, count: vpsSize)
            }
            if let sps = spsPtr {
                frameData.append(contentsOf: startCode)
                frameData.append(sps, count: spsSize)
            }
            if let pps = ppsPtr {
                frameData.append(contentsOf: startCode)
                frameData.append(pps, count: ppsSize)
            }
        }
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
    let mtu = 1300 // Reduced from 1400 to prevent MTU fragmentation (1300 + 96 header + 28 IP/UDP <= 1500 MTU)
    let totalBytes = frameData.count
    let numFragments = (totalBytes + mtu - 1) / mtu
    
    frameData.withUnsafeBytes { rawBuffer in
        guard let baseAddress = rawBuffer.bindMemory(to: UInt8.self).baseAddress else { return }
        
        for i in 0..<numFragments {
            let chunkOffset = i * mtu
            let fragSize = min(mtu, totalBytes - chunkOffset)
            
            var header = UDPHeader(
                magic: UInt32(0x5652414E).littleEndian, // "VRAN"
                frameSequence: frameSequence.littleEndian,
                chunkIndex: UInt32(i).littleEndian,
                totalChunks: UInt32(numFragments).littleEndian,
                chunkOffset: UInt32(chunkOffset).littleEndian,
                payloadSize: UInt32(fragSize).littleEndian
            )
            
            var packet = Data()
            withUnsafeBytes(of: &header) { pktHeader in
                packet.append(contentsOf: pktHeader)
            }
            packet.append(baseAddress + chunkOffset, count: fragSize)
            
            if let conn = vrConnection, conn.state == .ready {
                conn.send(content: packet, completion: .contentProcessed { error in
                    if let error = error {
                        print("Send error: \(error)")
                    }
                })
            }
            print("Sent UDP packet for frame \(frameSequence) frag \(i)/\(numFragments) payload: \(fragSize)")
            fflush(stdout)
        }
    }
}

// MARK: - VideoToolbox Setup

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
        print("VideoToolbox Callback FIRED! Status: \(status)")
        fflush(stdout)
        if status != noErr {
            print("Encoding error: \(status)")
            return
        }
        guard let sampleBuffer = sampleBuffer else { return }
        let isKeyFrame = !infoFlags.contains(.frameDropped) && (CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, createIfNecessary: false) as? [[CFString: Any]] == nil || (CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, createIfNecessary: false) as? [[CFString: Any]])?.first?[kCMSampleAttachmentKey_NotSync] == nil)
        
        let frameSequence = sourceFrameRefCon!.assumingMemoryBound(to: UInt32.self).pointee
        
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
    
    // Force a keyframe every 30 frames (0.5 seconds at 60fps) to quickly recover from UDP packet loss
    VTSessionSetProperty(compressionSession!, key: kVTCompressionPropertyKey_MaxKeyFrameInterval, value: 30 as CFNumber)
    
    VTCompressionSessionPrepareToEncodeFrames(compressionSession!)
}

// MARK: - Main IPC Loop

let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let mapSize = 16 + 4096 * 4096 * 4 + 194

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
let joyconListener = try? NWListener(using: parameters, on: 11002)
joyconListener?.service = NWListener.Service(name: "VerantyxJoyc", type: "_verantyxjoyc._udp")
joyconListener?.newConnectionHandler = { connection in
    connection.start(queue: .global())
    func receiveNext() {
        connection.receiveMessage { data, context, isComplete, error in
            if let data = data, data.count == MemoryLayout<JoyconPacket>.size {
                data.withUnsafeBytes { rawBuffer in
                    let jc = rawBuffer.load(as: JoyconPacket.self)
                    if jc.magic == UInt32(0x4A4F5943) { // "JOYC"
                        handsMapPtr.advanced(by: 194).storeBytes(of: jc.rightButtons, as: UInt32.self)
                        handsMapPtr.advanced(by: 198).storeBytes(of: jc.leftButtons, as: UInt32.self)
                        handsMapPtr.advanced(by: 202).storeBytes(of: jc.rightStickX, as: Float.self)
                        handsMapPtr.advanced(by: 206).storeBytes(of: jc.rightStickY, as: Float.self)
                        handsMapPtr.advanced(by: 210).storeBytes(of: jc.leftStickX, as: Float.self)
                        handsMapPtr.advanced(by: 214).storeBytes(of: jc.leftStickY, as: Float.self)
                        handsMapPtr.advanced(by: 218).storeBytes(of: jc.rightVelocityX, as: Float.self)
                        handsMapPtr.advanced(by: 222).storeBytes(of: jc.rightVelocityY, as: Float.self)
                        handsMapPtr.advanced(by: 226).storeBytes(of: jc.rightVelocityZ, as: Float.self)
                        handsMapPtr.advanced(by: 230).storeBytes(of: jc.leftVelocityX, as: Float.self)
                        handsMapPtr.advanced(by: 234).storeBytes(of: jc.leftVelocityY, as: Float.self)
                        handsMapPtr.advanced(by: 238).storeBytes(of: jc.leftVelocityZ, as: Float.self)
                    }
                }
            }
            if error == nil { receiveNext() }
        }
    }
    receiveNext()
}
joyconListener?.start(queue: .global())

// MARK: - Hand Tracking & Video Connection (AWDL Bonjour)
vrListener.stateUpdateHandler = { state in
    print("AWDL VRListener State: \(state)")
}
vrListener.newConnectionHandler = { connection in
    print("New AWDL Connection from Vision Pro!")
    vrConnection = connection
    connection.start(queue: .global())
    
    func receiveNext() {
        connection.receiveMessage { data, context, isComplete, error in
            if let data = data, data.count == MemoryLayout<VRPosePacket>.size {
                data.withUnsafeBytes { rawBuffer in
                    let posePkt = rawBuffer.load(as: VRPosePacket.self)
                    if posePkt.magic == UInt32(0x504F5345) { // "POSE"
                        _ = withUnsafePointer(to: posePkt.headTransform) { headPtr in
                            memcpy(handsMapPtr, headPtr, 64)
                        }
                        _ = withUnsafePointer(to: posePkt.leftHandTransform) { leftPtr in
                            memcpy(handsMapPtr + 64, leftPtr, 64)
                        }
                        _ = withUnsafePointer(to: posePkt.rightHandTransform) { rightPtr in
                            memcpy(handsMapPtr + 128, rightPtr, 64)
                        }
                        handsMapPtr.advanced(by: 192).storeBytes(of: posePkt.leftPinch, as: UInt8.self)
                        handsMapPtr.advanced(by: 193).storeBytes(of: posePkt.rightPinch, as: UInt8.self)
                    }
                }
            }
            if error == nil {
                receiveNext()
            }
        }
    }
    receiveNext()
}
vrListener.start(queue: .global())
print("Published Bonjour _verantyxvr._udp. Waiting for Vision Pro connection (AWDL enabled)...")

print("Starting native read loop and UDP transmission on port 9999...")

while isEncoding {
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
                
                if bytesPerRow == width * 4 {
                    memcpy(dst, pixelPtr, width * height * 4)
                } else {
                    for y in 0..<height {
                        memcpy(dst + y * bytesPerRow, pixelPtr + y * width * 4, width * 4)
                    }
                }
                
                CVPixelBufferUnlockBaseAddress(pb, [])
                
                let presentationTime = CMTime(value: CMTimeValue(framesEncoded), timescale: 90)
                currentFrameSequence = currentSeq
                
                var refConVal = currentSeq
                
                let status = VTCompressionSessionEncodeFrame(compressionSession!,
                                                             imageBuffer: pb,
                                                             presentationTimeStamp: presentationTime,
                                                             duration: CMTime.invalid,
                                                             frameProperties: nil,
                                                             sourceFrameRefcon: &refConVal,
                                                             infoFlagsOut: nil)
                if status == noErr {
                    framesEncoded += 1
                } else {
                    print("VTCompressionSessionEncodeFrame failed with status: \(status)")
                    fflush(stdout)
                }
            }
        }
        lastSeq = currentSeq
    }
    
    usleep(1000)
}
