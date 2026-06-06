import Foundation
import AVFoundation
import VideoToolbox
import CoreMedia
import Combine

/// VRVideoDecoder — HEVC (H.265) 対応デコーダー
///
/// Mac側 HardwareEncoder.swift は kCMVideoCodecType_HEVC でエンコードしているため、
/// Vision Pro側も HEVC の VPS/SPS/PPS NALUタイプ (32/33/34) でパラメータセットを
/// 収集し、CMVideoFormatDescriptionCreateFromHEVCParameterSets で
/// フォーマット記述子を生成する必要がある。
/// H.264 (type 7/8) として扱っていたのが真っ暗の根本原因。
class VRVideoDecoder: ObservableObject {
    let displayLayer = AVSampleBufferDisplayLayer()

    private var formatDescription: CMVideoFormatDescription?
    private var vpsData: [UInt8]?
    private var spsData: [UInt8]?
    private var ppsData: [UInt8]?

    // 表示タイムスタンプ管理
    private var frameCounter: Int64 = 0
    // 90000 Hz timebase (典型的なビデオ)
    private let timeScale: CMTimeScale = 90000
    private let frameDuration: CMTime

    init() {
        // 60fps想定。Macの送信側も timescale=90, value=framesEncoded なので
        // 1/90000秒を単位として使う。
        frameDuration = CMTime(value: 1500, timescale: timeScale) // ~60fps

        displayLayer.videoGravity = .resizeAspect

        // controlTimebase を設定しないと displayLayer が表示を行わない
        var timebase: CMTimebase?
        CMTimebaseCreateWithSourceClock(
            allocator: kCFAllocatorDefault,
            sourceClock: CMClockGetHostTimeClock(),
            timebaseOut: &timebase
        )
        if let tb = timebase {
            CMTimebaseSetTime(tb, time: .zero)
            CMTimebaseSetRate(tb, rate: 1.0)
            displayLayer.controlTimebase = tb
        }
    }

    func decodeFrame(_ data: Data) {
        let nals = extractNALUs(from: data)
        var vclNALUs: [Data] = []
        var parameterSetsUpdated = false

        for nal in nals {
            guard !nal.isEmpty else { continue }

            // HEVC NALUタイプ: 先頭バイトの上位6ビット (>> 1 & 0x3F)
            // H.264では &0x1F だったが HEVC では違う
            let nalType = (nal[0] >> 1) & 0x3F

            switch nalType {
            case 32: // VPS (Video Parameter Set)
                if vpsData == nil || Array(nal) != vpsData! {
                    vpsData = Array(nal)
                    parameterSetsUpdated = true
                    print("[VRVideoDecoder] VPS received (\(nal.count) bytes)")
                }
            case 33: // SPS (Sequence Parameter Set)
                if spsData == nil || Array(nal) != spsData! {
                    spsData = Array(nal)
                    parameterSetsUpdated = true
                    print("[VRVideoDecoder] SPS received (\(nal.count) bytes)")
                }
            case 34: // PPS (Picture Parameter Set)
                if ppsData == nil || Array(nal) != ppsData! {
                    ppsData = Array(nal)
                    parameterSetsUpdated = true
                    print("[VRVideoDecoder] PPS received (\(nal.count) bytes)")
                }
            default:
                // IDR (19/20), P/B frame (1/2) など実際の映像データ
                vclNALUs.append(nal)
            }
        }

        // パラメータセットが更新されたら formatDescription を再生成
        if parameterSetsUpdated || (formatDescription == nil) {
            createFormatDescription()
        }

        guard formatDescription != nil, !vclNALUs.isEmpty else { return }

        // AVCC形式に変換（4バイトビッグエンディアン長さプレフィックス）
        var avccData = Data()
        for nal in vclNALUs {
            var length = UInt32(nal.count).bigEndian
            withUnsafeBytes(of: &length) { avccData.append(contentsOf: $0) }
            avccData.append(nal)
        }

        if let sampleBuffer = createSampleBuffer(from: avccData) {
            DispatchQueue.main.async { [weak self] in
                guard let self = self else { return }
                // エラー状態から回復
                if self.displayLayer.status == .failed {
                    self.displayLayer.flush()
                }
                self.displayLayer.enqueue(sampleBuffer)
            }
        }
    }

    // MARK: - Private

    private func extractNALUs(from data: Data) -> [Data] {
        var nals: [Data] = []

        var searchIndex = 0
        let count = data.count

        // Annex B スタートコード (0x00 0x00 0x00 0x01) を探す
        func findNextStartCode(from startIdx: Int) -> Int? {
            guard startIdx + 3 < count else { return nil }
            for i in startIdx..<(count - 3) {
                if data[i] == 0 && data[i+1] == 0 && data[i+2] == 0 && data[i+3] == 1 {
                    return i
                }
            }
            return nil
        }

        // 最初のスタートコードを探す
        guard var currentStart = findNextStartCode(from: 0) else { return [] }
        searchIndex = currentStart + 4

        while searchIndex < count {
            if let nextStart = findNextStartCode(from: searchIndex) {
                let nalData = data.subdata(in: (currentStart + 4)..<nextStart)
                if !nalData.isEmpty {
                    nals.append(nalData)
                }
                currentStart = nextStart
                searchIndex = nextStart + 4
            } else {
                // 最後のNALU
                let nalData = data.subdata(in: (currentStart + 4)..<count)
                if !nalData.isEmpty {
                    nals.append(nalData)
                }
                break
            }
        }

        return nals
    }

    private func createFormatDescription() {
        guard let vps = vpsData, let sps = spsData, let pps = ppsData else {
            print("[VRVideoDecoder] Waiting for VPS/SPS/PPS...")
            return
        }

        formatDescription = nil

        vps.withUnsafeBufferPointer { vpsBuf in
            sps.withUnsafeBufferPointer { spsBuf in
                pps.withUnsafeBufferPointer { ppsBuf in
                    let parameterSetPointers: [UnsafePointer<UInt8>] = [
                        vpsBuf.baseAddress!,
                        spsBuf.baseAddress!,
                        ppsBuf.baseAddress!
                    ]
                    let parameterSetSizes: [Int] = [vps.count, sps.count, pps.count]

                    let status = CMVideoFormatDescriptionCreateFromHEVCParameterSets(
                        allocator: kCFAllocatorDefault,
                        parameterSetCount: 3,
                        parameterSetPointers: parameterSetPointers,
                        parameterSetSizes: parameterSetSizes,
                        nalUnitHeaderLength: 4,
                        extensions: nil,
                        formatDescriptionOut: &formatDescription
                    )

                    if status == noErr {
                        print("[VRVideoDecoder] ✅ HEVC CMVideoFormatDescription created successfully!")
                    } else {
                        print("[VRVideoDecoder] ❌ Failed to create HEVC format description: \(status)")
                        formatDescription = nil
                    }
                }
            }
        }
    }

    private func createSampleBuffer(from avccData: Data) -> CMSampleBuffer? {
        guard let formatDesc = formatDescription else { return nil }

        // CMBlockBuffer 作成
        var blockBuffer: CMBlockBuffer?
        let blockStatus: OSStatus = avccData.withUnsafeBytes { bufPtr -> OSStatus in
            guard let baseAddress = bufPtr.baseAddress else { return kCMBlockBufferBadCustomBlockSourceErr }
            return CMBlockBufferCreateWithMemoryBlock(
                allocator: kCFAllocatorDefault,
                memoryBlock: UnsafeMutableRawPointer(mutating: baseAddress),
                blockLength: avccData.count,
                blockAllocator: kCFAllocatorNull, // メモリ解放はSwiftに任せる
                customBlockSource: nil,
                offsetToData: 0,
                dataLength: avccData.count,
                flags: 0,
                blockBufferOut: &blockBuffer
            )
        }

        guard blockStatus == kCMBlockBufferNoErr, let blockBuffer = blockBuffer else {
            print("[VRVideoDecoder] Failed to create CMBlockBuffer: \(blockStatus)")
            return nil
        }

        // タイムスタンプを単調増加させる（表示が確実に行われるよう）
        frameCounter += 1
        let pts = CMTime(value: frameCounter * 1500, timescale: timeScale)

        var timingInfo = CMSampleTimingInfo(
            duration: frameDuration,
            presentationTimeStamp: pts,
            decodeTimeStamp: .invalid
        )

        var sampleSizeArray = [avccData.count]
        var sampleBuffer: CMSampleBuffer?
        let sampleStatus = CMSampleBufferCreateReady(
            allocator: kCFAllocatorDefault,
            dataBuffer: blockBuffer,
            formatDescription: formatDesc,
            sampleCount: 1,
            sampleTimingEntryCount: 1,
            sampleTimingArray: &timingInfo,
            sampleSizeEntryCount: 1,
            sampleSizeArray: &sampleSizeArray,
            sampleBufferOut: &sampleBuffer
        )

        guard sampleStatus == noErr, let sampleBuffer = sampleBuffer else {
            print("[VRVideoDecoder] Failed to create CMSampleBuffer: \(sampleStatus)")
            return nil
        }

        // 即時表示フラグを付ける
        if let attachments = CMSampleBufferGetSampleAttachmentsArray(sampleBuffer, createIfNecessary: true) as? [[CFString: Any]],
           let dict = CFArrayGetValueAtIndex(attachments as CFArray, 0) {
            let mutableDict = dict as! CFMutableDictionary
            CFDictionarySetValue(
                mutableDict,
                Unmanaged.passUnretained(kCMSampleAttachmentKey_DisplayImmediately).toOpaque(),
                Unmanaged.passUnretained(kCFBooleanTrue).toOpaque()
            )
        }

        return sampleBuffer
    }
}
