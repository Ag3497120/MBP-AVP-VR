import Foundation
import simd

public struct JCrossNode_VR {
    public var nodeId: UInt16
    public var flags: UInt16
    public var position: simd_float3
    public var rotation: simd_quatf
    public var velocity: simd_float3
    public var angularVelocity: simd_float3
    
    public init(nodeId: UInt16) {
        self.nodeId = nodeId
        self.flags = 1
        self.position = simd_make_float3(0, 0, 0)
        self.rotation = simd_quaternion(0, 0, 0, 1)
        self.velocity = simd_make_float3(0, 0, 0)
        self.angularVelocity = simd_make_float3(0, 0, 0)
    }
}

public struct JCrossZone_Input {
    public var parentNodeId: UInt16
    public var buttonMask: UInt32
    public var triggerValue: Float
    public var gripValue: Float
    public var thumbstick: simd_float2
    
    public init(parentNodeId: UInt16) {
        self.parentNodeId = parentNodeId
        self.buttonMask = 0
        self.triggerValue = 0
        self.gripValue = 0
        self.thumbstick = simd_make_float2(0, 0)
    }
}

public struct JCrossPayload {
    public var frameId: UInt32 = 0
    public var timestamp: Double = 0
    public var headNode: JCrossNode_VR = JCrossNode_VR(nodeId: 1)
    public var leftHandNode: JCrossNode_VR = JCrossNode_VR(nodeId: 2)
    public var rightHandNode: JCrossNode_VR = JCrossNode_VR(nodeId: 3)
    public var leftInput: JCrossZone_Input = JCrossZone_Input(parentNodeId: 2)
    public var rightInput: JCrossZone_Input = JCrossZone_Input(parentNodeId: 3)
    
    public init() {}
    
    public func serialize() -> Data {
        var copy = self
        return Data(bytes: &copy, count: MemoryLayout<JCrossPayload>.size)
    }
    
    public static func deserialize(from data: Data) -> JCrossPayload? {
        guard data.count == MemoryLayout<JCrossPayload>.size else { return nil }
        return data.withUnsafeBytes { $0.load(as: JCrossPayload.self) }
    }
}
