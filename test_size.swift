import Foundation

struct VRPosePacket {
    var magic: UInt32
    var headTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var rightHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftPinch: UInt8
    var rightPinch: UInt8
}

var pkt = VRPosePacket(magic: 0x12345678, headTransform: (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0), leftHandTransform: (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0), rightHandTransform: (0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0), leftPinch: 0, rightPinch: 0)
withUnsafePointer(to: &pkt) { ptr in
    withUnsafePointer(to: &pkt.headTransform) { headPtr in
        let offset = Int(bitPattern: headPtr) - Int(bitPattern: ptr)
        print("Offset:", offset)
    }
}
print("Stride: \(MemoryLayout<VRPosePacket>.stride)")


