struct VRPosePacket {
    var magic: UInt32
    var timestamp: Double
    var headTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var rightHandTransform: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float)
    var leftPinch: UInt8
    var rightPinch: UInt8
}
print(MemoryLayout<VRPosePacket>.size)
print(MemoryLayout<VRPosePacket>.stride)
