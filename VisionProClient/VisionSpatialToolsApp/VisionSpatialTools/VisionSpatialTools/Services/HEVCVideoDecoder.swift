import Foundation
import VideoToolbox
import CoreMedia

import simd

protocol HEVCVideoDecoderDelegate: AnyObject {
    func didDecodeFrame(_ pixelBuffer: CVPixelBuffer, timestamp: Double, transform: simd_float4x4)
}

class HEVCVideoDecoder {
    
    weak var delegate: HEVCVideoDecoderDelegate?
    
    private var decompressionSession: VTDecompressionSession?
    private var formatDescription: CMVideoFormatDescription?
    
    private var vps: Data?
    private var sps: Data?
    private var pps: Data?
    
    private var isH264 = false
    var decodeTimestampQueue = [UInt32: (Double, simd_float4x4)]()
    
    // Parses Annex B stream (0x00 0x00 0x00 0x01) and feeds it to VideoToolbox
    func decodeAnnexBFrame(_ frameData: Data, sequence: UInt32, timestamp: Double, transform: simd_float4x4) {
        self.decodeTimestampQueue[sequence] = (timestamp, transform)
        
        let nals = extractNALUnits(from: frameData)
        var vclNals = [Data]()
        
        for nal in nals {
            if nal.isEmpty { continue }
            
            // Check HEVC first
            let hevcNalType = (nal[0] & 0x7E) >> 1
            // Check H264
            let h264NalType = nal[0] & 0x1F
            
            if hevcNalType == 32 { 
                if vps != nal { vps = nal; formatDescription = nil }
                isH264 = false 
            }
            else if hevcNalType == 33 { 
                if sps != nal { sps = nal; formatDescription = nil }
                isH264 = false 
            }
            else if hevcNalType == 34 { 
                if pps != nal { pps = nal; formatDescription = nil }
                isH264 = false 
            }
            else if vps == nil && h264NalType == 7 { 
                if sps != nal { sps = nal; formatDescription = nil }
                isH264 = true 
            }
            else if vps == nil && h264NalType == 8 { 
                if pps != nal { pps = nal; formatDescription = nil }
                isH264 = true 
            }
            else { vclNals.append(nal) }
        }
        
        if isH264, formatDescription == nil {
            print("[HEVCDecoder] Found H264 NALs. SPS: \(sps != nil), PPS: \(pps != nil)")
        }
        
        if formatDescription == nil {
            print("[HEVCDecoder] Attempting to create format desc for frame \(sequence). size: \(frameData.count). isH264: \(isH264), vps: \(vps != nil), sps: \(sps != nil), pps: \(pps != nil)")
        }
        
        // Try to create format description
        if formatDescription == nil {
            if let session = decompressionSession {
                VTDecompressionSessionInvalidate(session)
                decompressionSession = nil
            }
            if isH264, let sps = sps, let pps = pps {
                createH264FormatDescription(sps: sps, pps: pps)
            } else if !isH264, let vps = vps, let sps = sps, let pps = pps {
                createHEVCFormatDescription(vps: vps, sps: sps, pps: pps)
            }
        }
        
        guard let _ = formatDescription, !vclNals.isEmpty else { return }
        
        // Convert to AVCC (length-prefixed)
        var avccData = Data()
        for nal in vclNals {
            var length = UInt32(nal.count).bigEndian
            avccData.append(Data(bytes: &length, count: 4))
            avccData.append(nal)
        }
        
        decodeAVCCData(avccData, sequence: sequence)
    }
    
    private func extractNALUnits(from data: Data) -> [Data] {
        var nals = [Data]()
        let startCode: [UInt8] = [0, 0, 0, 1]
        
        var searchIndex = 0
        while searchIndex < data.count - 4 {
            guard let nextStartRange = data.range(of: Data(startCode), options: [], in: searchIndex..<data.count) else {
                nals.append(data.subdata(in: searchIndex..<data.count))
                break
            }
            
            if nextStartRange.lowerBound > searchIndex {
                nals.append(data.subdata(in: searchIndex..<nextStartRange.lowerBound))
            }
            
            searchIndex = nextStartRange.upperBound
        }
        return nals
    }
    
    private func createH264FormatDescription(sps: Data, pps: Data) {
        sps.withUnsafeBytes { spsBuf in
            pps.withUnsafeBytes { ppsBuf in
                guard let spsPtr = spsBuf.baseAddress?.assumingMemoryBound(to: UInt8.self),
                      let ppsPtr = ppsBuf.baseAddress?.assumingMemoryBound(to: UInt8.self) else { return }
                
                let paramSets = [spsPtr, ppsPtr]
                let paramSizes = [sps.count, pps.count]
                
                var newFormatDesc: CMVideoFormatDescription?
                let status = CMVideoFormatDescriptionCreateFromH264ParameterSets(
                    allocator: kCFAllocatorDefault,
                    parameterSetCount: 2,
                    parameterSetPointers: paramSets,
                    parameterSetSizes: paramSizes,
                    nalUnitHeaderLength: 4,
                    formatDescriptionOut: &newFormatDesc
                )
                
                print("[HEVCDecoder] CMVideoFormatDescriptionCreateFromH264ParameterSets returned \(status)")
                
                if status == noErr {
                    self.formatDescription = newFormatDesc
                    createDecompressionSession()
                } else {
                    print("[VideoDecoder] Failed to create H.264 Format Description: \(status)")
                }
            }
        }
    }
    
    private func createHEVCFormatDescription(vps: Data, sps: Data, pps: Data) {
        vps.withUnsafeBytes { vpsBuf in
            sps.withUnsafeBytes { spsBuf in
                pps.withUnsafeBytes { ppsBuf in
                    guard let vpsPtr = vpsBuf.baseAddress?.assumingMemoryBound(to: UInt8.self),
                          let spsPtr = spsBuf.baseAddress?.assumingMemoryBound(to: UInt8.self),
                          let ppsPtr = ppsBuf.baseAddress?.assumingMemoryBound(to: UInt8.self) else { return }
                    
                    let paramSets = [vpsPtr, spsPtr, ppsPtr]
                    let paramSizes = [vps.count, sps.count, pps.count]
                    
                    var newFormatDesc: CMVideoFormatDescription?
                    let status = CMVideoFormatDescriptionCreateFromHEVCParameterSets(
                        allocator: kCFAllocatorDefault,
                        parameterSetCount: 3,
                        parameterSetPointers: paramSets,
                        parameterSetSizes: paramSizes,
                        nalUnitHeaderLength: 4,
                        extensions: nil,
                        formatDescriptionOut: &newFormatDesc
                    )
                    
                    if status == noErr {
                        self.formatDescription = newFormatDesc
                        createDecompressionSession()
                    } else {
                        print("[VideoDecoder] Failed to create HEVC Format Description: \(status)")
                    }
                }
            }
        }
    }
    
    private func createDecompressionSession() {
        guard let formatDesc = formatDescription else { return }
        
        let destinationImageBufferAttributes: [String: Any] = [
            kCVPixelBufferMetalCompatibilityKey as String: true,
            kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_32BGRA,
            kCVPixelBufferIOSurfacePropertiesKey as String: [:]
        ]
        
        var callbackRecord = VTDecompressionOutputCallbackRecord(
            decompressionOutputCallback: decompressionSessionCallback,
            decompressionOutputRefCon: Unmanaged.passUnretained(self).toOpaque()
        )
        
        var session: VTDecompressionSession?
        let status = VTDecompressionSessionCreate(
            allocator: kCFAllocatorDefault,
            formatDescription: formatDesc,
            decoderSpecification: nil,
            imageBufferAttributes: destinationImageBufferAttributes as CFDictionary,
            outputCallback: &callbackRecord,
            decompressionSessionOut: &session
        )
        
        if status == noErr, let session = session {
            VTSessionSetProperty(session, key: kVTDecompressionPropertyKey_RealTime, value: kCFBooleanTrue)
            self.decompressionSession = session
            print("[HEVCDecoder] Hardware Decompression Session Created!")
        } else {
            print("[HEVCDecoder] Failed to create Decompression Session: \(status)")
        }
    }
    
    private func decodeAVCCData(_ data: Data, sequence: UInt32) {
        guard let session = decompressionSession else { return }
        
        var blockBuffer: CMBlockBuffer?
        let dataLength = data.count
        
        data.withUnsafeBytes { rawBuffer in
            let ptr = rawBuffer.baseAddress!
            CMBlockBufferCreateWithMemoryBlock(
                allocator: kCFAllocatorDefault,
                memoryBlock: nil,
                blockLength: dataLength,
                blockAllocator: nil,
                customBlockSource: nil,
                offsetToData: 0,
                dataLength: dataLength,
                flags: 0,
                blockBufferOut: &blockBuffer
            )
            
            if let blockBuffer = blockBuffer {
                CMBlockBufferReplaceDataBytes(
                    with: ptr,
                    blockBuffer: blockBuffer,
                    offsetIntoDestination: 0,
                    dataLength: dataLength
                )
            }
        }
        
        guard let bb = blockBuffer, let formatDesc = formatDescription else { return }
        
        var sampleBuffer: CMSampleBuffer?
        var sampleSizeArray = [dataLength]
        
        CMSampleBufferCreateReady(
            allocator: kCFAllocatorDefault,
            dataBuffer: bb,
            formatDescription: formatDesc,
            sampleCount: 1,
            sampleTimingEntryCount: 0,
            sampleTimingArray: nil,
            sampleSizeEntryCount: 1,
            sampleSizeArray: &sampleSizeArray,
            sampleBufferOut: &sampleBuffer
        )
        
        guard let sb = sampleBuffer else { return }
        
        var flagsOut = VTDecodeInfoFlags()
        
        var refConSeq = sequence
        let ptrSeq = UnsafeMutableRawPointer(bitPattern: UInt(refConSeq))
        
        VTDecompressionSessionDecodeFrame(
            session,
            sampleBuffer: sb,
            flags: [._EnableAsynchronousDecompression],
            frameRefcon: ptrSeq,
            infoFlagsOut: &flagsOut
        )
    }
}

// Global C callback for VideoToolbox
func decompressionSessionCallback(
    decompressionOutputRefCon: UnsafeMutableRawPointer?,
    sourceFrameRefCon: UnsafeMutableRawPointer?,
    status: OSStatus,
    infoFlags: VTDecodeInfoFlags,
    imageBuffer: CVImageBuffer?,
    presentationTimeStamp: CMTime,
    presentationDuration: CMTime
) {
    guard status == noErr, let pixelBuffer = imageBuffer else {
        print("[HEVCDecoder] Decode failed! status: \(status)")
        return
    }
    let decoder = Unmanaged<HEVCVideoDecoder>.fromOpaque(decompressionOutputRefCon!).takeUnretainedValue()
    
    let seq = UInt32(UInt(bitPattern: sourceFrameRefCon))
    let payload = decoder.decodeTimestampQueue[seq] ?? (0.0, matrix_identity_float4x4)
    decoder.decodeTimestampQueue.removeValue(forKey: seq)
    
    decoder.delegate?.didDecodeFrame(pixelBuffer, timestamp: payload.0, transform: payload.1)
}
