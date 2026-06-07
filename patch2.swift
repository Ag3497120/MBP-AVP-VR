import Foundation
import CoreMedia
import VideoToolbox
import Network

// MARK: - Protocol Definitions

struct SharedFrame {
    var sequenceNumber: UInt32
    var width: UInt32
    var height: UInt32
    var renderedTimestamp: Double
}

struct UDPHeader {
    var magic: UInt32
    var frameSequence: UInt32
    var chunkIndex: UInt32
    var totalChunks: UInt32
    var chunkOffset: UInt32
    var payloadSize: UInt32
    var timestamp: Double
}

class RefConDataClass {
    let seq: UInt32
    let timestamp: Double
    init(seq: UInt32, timestamp: Double) {
        self.seq = seq
        self.timestamp = timestamp
    }
}
