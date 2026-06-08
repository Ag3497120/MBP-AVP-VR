import Foundation
import Network
import Combine

struct UDPHeader {
    let magic: UInt32
    let frameSequence: UInt32
    let chunkIndex: UInt32
    let totalChunks: UInt32
    let chunkOffset: UInt32
    let payloadSize: UInt32
}

class VRUDPReceiver: ObservableObject {
    private var browser: NWBrowser?
    private var connection: NWConnection?
    
    // Publishes complete H.264 Annex B frames
    let framePublisher = PassthroughSubject<Data, Never>()
    
    private var frames: [UInt32: [UInt32: Data]] = [:]
    private var receivedCounts: [UInt32: UInt32] = [:]
    private var totalFragments: [UInt32: UInt32] = [:]
    private var lastPlayedFrame: UInt32 = 0
    
    init() {}
    
    func start(port: NWEndpoint.Port = 9999) {
        let descriptor = NWBrowser.Descriptor.bonjour(type: "_verantyxvr._udp", domain: "local.")
        let params = NWParameters.udp
        params.allowFastOpen = true
        params.includePeerToPeer = true
        
        browser = NWBrowser(for: descriptor, using: params)
        browser?.stateUpdateHandler = { state in
            print("VRUDPBrowser State: \(state)")
        }
        browser?.browseResultsChangedHandler = { [weak self] results, changes in
            guard let self = self else { return }
            for result in results {
                print("Found Mac VR service: \(result.endpoint)")
                self.connect(to: result.endpoint)
                self.browser?.cancel()
                break
            }
        }
        browser?.start(queue: .global())
        print("Browsing for Mac HardwareEncoder (_verantyxvr._udp)...")
    }
    
    private func connect(to endpoint: NWEndpoint) {
        let params = NWParameters.udp
        params.allowFastOpen = true
        params.includePeerToPeer = true
        
        let conn = NWConnection(to: endpoint, using: params)
        conn.stateUpdateHandler = { state in
            print("VRUDPConnection State: \(state)")
            if state == .ready {
                // Send a wake-up packet to trigger Mac's listener
                let dummy = Data("HELLO_VR".utf8)
                conn.send(content: dummy, completion: .idempotent)
            }
        }
        
        self.connection = conn
        conn.start(queue: .global())
        self.receive(on: conn)
    }
    
    func stop() {
        browser?.cancel()
        browser = nil
        connection?.cancel()
        connection = nil
    }
    
    private func receive(on connection: NWConnection) {
        connection.receiveMessage { [weak self] content, context, isComplete, error in
            guard let self = self else { return }
            
            if let data = content, data.count >= 24 {
                self.processPacket(data)
            }
            
            if error == nil {
                self.receive(on: connection)
            } else {
                print("UDP connection error: \(error!)")
            }
        }
    }
    
    private func processPacket(_ data: Data) {
        let magic = data[0..<4].withUnsafeBytes { $0.load(as: UInt32.self) }
        let frameSequence = data[4..<8].withUnsafeBytes { $0.load(as: UInt32.self) }
        let chunkIndex = data[8..<12].withUnsafeBytes { $0.load(as: UInt32.self) }
        let totalChunks = data[12..<16].withUnsafeBytes { $0.load(as: UInt32.self) }
        let chunkOffset = data[16..<20].withUnsafeBytes { $0.load(as: UInt32.self) }
        let payloadSize = data[20..<24].withUnsafeBytes { $0.load(as: UInt32.self) }
        
        // HardwareEncoder.swift は magic を `.littleEndian` で書き込んでいるため、
        // 受信側も `littleEndian` として解釈する必要がある。
        // Apple Silicon (little-endian) ではそのまま一致するが、明示する。
        let expectedMagic = UInt32(0x5652414E).littleEndian
        if magic != expectedMagic {
            return
        }
        
        if frameSequence < lastPlayedFrame {
            return
        }
        
        let payload = data.subdata(in: 24..<(24 + Int(payloadSize)))
        
        if frames[frameSequence] == nil {
            frames[frameSequence] = [:]
            receivedCounts[frameSequence] = 0
            totalFragments[frameSequence] = totalChunks
        }
        
        if frames[frameSequence]?[chunkIndex] == nil {
            frames[frameSequence]?[chunkIndex] = payload
            receivedCounts[frameSequence]! += 1
        }
        
        if receivedCounts[frameSequence] == totalChunks {
            // Frame complete!
            var fullFrame = Data()
            for i in 0..<totalChunks {
                if let frag = frames[frameSequence]?[i] {
                    fullFrame.append(frag)
                }
            }
            
            lastPlayedFrame = frameSequence
            
            // Clean up
            let keys = frames.keys
            for k in keys {
                if k <= frameSequence {
                    frames.removeValue(forKey: k)
                    receivedCounts.removeValue(forKey: k)
                    totalFragments.removeValue(forKey: k)
                }
            }
            
            // Publish the frame
            framePublisher.send(fullFrame)
        }
    }
}
