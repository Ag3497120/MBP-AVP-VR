import Foundation
struct VRPosePacket {
    var magic: UInt32 // 4
    var timestamp: Double // 8
    var headTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float) // 64
    var leftHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float) // 64
    var rightHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float) // 64
    var leftPinch: UInt8 // 1
    var rightPinch: UInt8 // 1
}
var data = Data(count: 210)
data.withUnsafeBytes { rawBuffer in
    let pkt = rawBuffer.load(as: VRPosePacket.self)
    print("Loaded packet with magic: \\(pkt.magic)")
}
