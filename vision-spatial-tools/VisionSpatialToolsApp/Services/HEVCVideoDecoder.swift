import Foundation
import VideoToolbox
import CoreMedia

protocol HEVCVideoDecoderDelegate: AnyObject {
    func didDecodeFrame(_ pixelBuffer: CVPixelBuffer)
}

class HEVCVideoDecoder {
    
    weak var delegate: HEVCVideoDecoderDelegate?
    
    private var decompressionSession: VTDecompressionSession?
    private var formatDescription: CMVideoFormatDescription?
    
    private var vps: Data?
    private var sps: Data?
    private var pps: Data?
    
    // Parses Annex B stream (0x00 0x00 0x00 0x01) and feeds it to VideoToolbox
    func decodeAnnexBFrame(_ frameData: Data) {
        let nals = extractNALUnits(from: frameData)
        
        var vclNals = [Data]()
        
        for nal in nals {
            if nal.isEmpty { continue }
            let nalType = (nal[0] & 0x7E) >> 1
            
            if nalType == 32 { vps = nal }
            else if nalType == 33 { sps = nal }
            else if nalType == 34 { pps = nal }
            else { vclNals.append(nal) }
        }
        
        // If we don't have format description, try to create it
        if formatDescription == nil {
            if let vps = vps, let sps = sps, let pps = pps {
                createFormatDescription(vps: vps, sps: sps, pps: pps)
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
        
        decodeAVCCData(avccData)
    }
    
    private func extractNALUnits(from data: Data) -> [Data] {
        var nals = [Data]()
        let startCode: [UInt8] = [0, 0, 0, 1]
        
        var searchIndex = 0
        while searchIndex < data.count - 4 {
            // Find next start code
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
    
    private func createFormatDescription(vps: Data, sps: Data, pps: Data) {
        let paramSets = [
            vps.withUnsafeBytes { $0.baseAddress!.assumingMemoryBound(to: UInt8.self) },
            sps.withUnsafeBytes { $0.baseAddress!.assumingMemoryBound(to: UInt8.self) },
            pps.withUnsafeBytes { $0.baseAddress!.assumingMemoryBound(to: UInt8.self) }
        ]
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
            print("[HEVCDecoder] Failed to create Format Description: \(status)")
        }
    }
    
    private func createDecompressionSession() {
        guard let formatDesc = formatDescription else { return }
        
        let destinationImageBufferAttributes: [String: Any] = [
            kCVPixelBufferMetalCompatibilityKey as String: true,
            kCVPixelBufferPixelFormatTypeKey as String: kCVPixelFormatType_32BGRA
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
        
        if status == noErr {
            self.decompressionSession = session
            print("[HEVCDecoder] Hardware Decompression Session Created!")
        } else {
            print("[HEVCDecoder] Failed to create Decompression Session: \(status)")
        }
    }
    
    private func decodeAVCCData(_ data: Data) {
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
        VTDecompressionSessionDecodeFrame(
            session,
            sampleBuffer: sb,
            flags: [._EnableAsynchronousDecompression],
            frameRefcon: nil,
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
    guard status == noErr, let pixelBuffer = imageBuffer else { return }
    let decoder = Unmanaged<HEVCVideoDecoder>.fromOpaque(decompressionOutputRefCon!).takeUnretainedValue()
    decoder.delegate?.didDecodeFrame(pixelBuffer)
}
