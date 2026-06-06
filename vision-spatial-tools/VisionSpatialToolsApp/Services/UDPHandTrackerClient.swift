import Foundation
import Network
import simd

class UDPHandTrackerClient {
    static let shared = UDPHandTrackerClient()
    
    private var connection: NWConnection?
    private let queue = DispatchQueue(label: "UDPHandTrackerClientQueue")
    
    // Config
    var targetIP: String = "192.168.1.80" // Update this to Mac's IP
    var targetPort: NWEndpoint.Port = 9998
    
    func start() {
        let endpoint = NWEndpoint.hostPort(host: NWEndpoint.Host(targetIP), port: targetPort)
        connection = NWConnection(to: endpoint, using: .udp)
        
        connection?.stateUpdateHandler = { state in
            print("UDPHandTrackerClient state: \(state)")
        }
        
        connection?.start(queue: queue)
    }
    
    func stop() {
        connection?.cancel()
        connection = nil
    }
    
    func sendHandData(leftTransform: simd_float4x4, rightTransform: simd_float4x4, leftPinch: Bool, rightPinch: Bool) {
        guard let connection = connection, connection.state == .ready else { return }
        
        var packet = Data()
        
        // Serialize left transform (16 floats)
        var left = leftTransform
        withUnsafeBytes(of: &left) { packet.append(contentsOf: $0) }
        
        // Serialize right transform (16 floats)
        var right = rightTransform
        withUnsafeBytes(of: &right) { packet.append(contentsOf: $0) }
        
        // Serialize pinch states (1 byte each)
        var lp: UInt8 = leftPinch ? 1 : 0
        var rp: UInt8 = rightPinch ? 1 : 0
        packet.append(&lp, count: 1)
        packet.append(&rp, count: 1)
        
        connection.send(content: packet, completion: .contentProcessed { error in
            if let error = error {
                print("Failed to send hand data: \(error)")
            }
        })
    }
}
