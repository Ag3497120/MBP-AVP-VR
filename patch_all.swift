import Foundation
import CoreMedia
import VideoToolbox
import Network

// MARK: - Protocol Definitions

struct SharedFrame {
    var sequenceNumber: UInt32
    var width: UInt32
    var height: UInt32
    var format: UInt32
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

struct VRPosePacket {
    var magic: UInt32 // 0x504F5345 ("POSE")
    var timestamp: Double
    var headTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var rightHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftPinch: UInt8
    var rightPinch: UInt8
}
