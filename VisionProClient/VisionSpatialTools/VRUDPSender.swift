import Foundation
import Network
import simd
import Combine

struct VRPosePacket {
    var magic: UInt32 = 0x504F5345 // "POSE"
    var headTransform: simd_float4x4
    var leftHandTransform: simd_float4x4
    var rightHandTransform: simd_float4x4
    var leftPinch: UInt8
    var rightPinch: UInt8
}

class VRUDPSender: ObservableObject {
    private var connection: NWConnection?
    private let queue = DispatchQueue(label: "com.verantyx.vrudpsender")
    
    var targetIP: String = "192.168.0.1" // Needs to be configurable
    
    init() {}
    
    func start(ip: String, port: NWEndpoint.Port = 11001) {
        self.targetIP = ip
        let host = NWEndpoint.Host(ip)
        connection = NWConnection(host: host, port: port, using: .udp)
        
        connection?.stateUpdateHandler = { state in
            print("VRUDPSender State: \(state)")
        }
        
        connection?.start(queue: queue)
        print("VRUDPSender started for \(ip):\(port)")
    }
    
    func stop() {
        connection?.cancel()
        connection = nil
    }
    
    func sendPose(head: simd_float4x4, leftHand: simd_float4x4, rightHand: simd_float4x4, leftPinch: Bool, rightPinch: Bool) {
        guard let connection = connection, connection.state == .ready else { return }
        
        var packetData = Data()
        
        // magic (0x45534F50 is "POSE" in little endian)
        var magic: UInt32 = 0x45534F50
        packetData.append(Data(bytes: &magic, count: 4))
        
        func appendMatrix(_ matrix: simd_float4x4) {
            var m = matrix
            let data = Data(bytes: &m, count: 64)
            packetData.append(data)
        }
        
        appendMatrix(head)
        appendMatrix(leftHand)
        appendMatrix(rightHand)
        
        var lPinch: UInt8 = leftPinch ? 1 : 0
        var rPinch: UInt8 = rightPinch ? 1 : 0
        
        packetData.append(Data(bytes: &lPinch, count: 1))
        packetData.append(Data(bytes: &rPinch, count: 1))
        
        connection.send(content: packetData, completion: .contentProcessed { error in
            if let error = error {
                print("VRUDPSender error: \(error)")
            }
        })
    }
}
