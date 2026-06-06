import Foundation
import Network
import CoreBluetooth
import VideoToolbox
import ARKit
import RealityKit
import QuartzCore

/// Phase 14: VisionOS Drop-in SDK (Client Side)
/// ユーザーが保有する「vision spatial tools」にドロップインで組み込める、
/// Mac(Verantyx)からのゼロコピー映像のデコードと、JCrossトラッキング情報の送信を行うコアモジュールです。
public class VisionClientCompositor: NSObject, CBCentralManagerDelegate, HEVCVideoDecoderDelegate {
    
    public var onFrameDecoded: ((CVPixelBuffer) -> Void)?
    
    // --- Network.framework (P2P UDP) ---
    private var listenerConnection: NWConnection?
    private let targetPort: NWEndpoint.Port = 9090
    
    // --- VideoToolbox (Hardware Decoder) ---
    private var decoder = HEVCVideoDecoder()
    
    // --- Chunk Assembly State ---
    private var currentSeq: UInt32 = 0
    private var chunksReceived: UInt32 = 0
    private var expectedChunks: UInt32 = 0
    private var frameBuffer = Data()
    
    // --- CoreBluetooth (Discovery) ---
    private var centralManager: CBCentralManager?
    private let serviceUUID = CBUUID(string: "A1B2C3D4-1234-5678-90AB-CDEF12345678")
    
    // --- ARKit Tracking ---
    // (実際のvisionOSアプリでは ARKitSession と WorldTrackingProvider, HandTrackingProvider を使用します)
    
    public override init() {
        super.init()
        decoder.delegate = self
        setupBluetoothDiscovery()
    }
    
    // MARK: - Discovery & Connection (AirDrop Style)
    private func setupBluetoothDiscovery() {
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    public func centralManagerDidUpdateState(_ central: CBCentralManager) {
        if central.state == .poweredOn {
            print("[VisionClient] Bluetooth Powered On. Scanning for Mac Host...")
            centralManager?.scanForPeripherals(withServices: [serviceUUID], options: nil)
        }
    }
    
    public func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral, advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print("[VisionClient] Discovered Mac Host! Initiating Wi-Fi 6 P2P (AWDL) Connection...")
        centralManager?.stopScan()
        
        // P2P UDP 接続の確立
        establishP2PConnection(to: peripheral.identifier.uuidString)
    }
    
    private func establishP2PConnection(to hostId: String) {
        let parameters = NWParameters.udp
        parameters.includePeerToPeer = true
        
        // 実際の運用ではホストの解決が必要ですが、ここではローカルマルチキャスト等を想定
        let endpoint = NWEndpoint.hostPort(host: "verantyx-mac.local", port: targetPort)
        listenerConnection = NWConnection(to: endpoint, using: parameters)
        
        listenerConnection?.stateUpdateHandler = { [weak self] state in
            switch state {
            case .ready:
                print("[VisionClient] Connected to Mac via P2P UDP!")
                self?.startReceivingNALUnits()
                
                // トラッキング送信ループの開始（例: 90Hz または 250Hz）
                self?.startTrackingUploadLoop()
            case .failed(let error):
                print("[VisionClient] Connection failed: \(error)")
            default:
                break
            }
        }
        
        listenerConnection?.start(queue: .main)
    }
    
    // MARK: - NAL Unit Hardware Decoding
    
    private func startReceivingNALUnits() {
        listenerConnection?.receiveMessage { [weak self] (content, context, isComplete, error) in
            if let data = content {
                self?.processUDPPacket(data)
            }
            if error == nil {
                self?.startReceivingNALUnits() // ループ
            }
        }
    }
    
    private func processUDPPacket(_ data: Data) {
        guard data.count >= 21 else { return }
        
        // Custom Header parsing: [Magic:4][Seq:4][ChunkIdx:4][TotalChunks:4][IsKey:1][PayloadSize:4]
        let magic = data[0..<4].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        guard magic == 0x48455643 else { return } // "HEVC"
        
        let seq = data[4..<8].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let chunkIdx = data[8..<12].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let totalChunks = data[12..<16].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let payloadSize = data[17..<21].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        
        if currentSeq != seq {
            currentSeq = seq
            chunksReceived = 0
            expectedChunks = totalChunks
            
            // Reallocate / reset buffer if needed. We assume chunks arrive in order for simplicity.
            // In a real robust implementation, we'd use chunkIdx to place data at the correct offset.
            frameBuffer.removeAll(keepingCapacity: true)
        }
        
        let payloadData = data.subdata(in: 21..<(21 + Int(payloadSize)))
        frameBuffer.append(payloadData)
        chunksReceived += 1
        
        if chunksReceived == expectedChunks {
            decoder.decodeAnnexBFrame(frameBuffer)
        }
    }
    
    // MARK: - HEVCVideoDecoderDelegate
    public func didDecodeFrame(_ pixelBuffer: CVPixelBuffer) {
        onFrameDecoded?(pixelBuffer)
    }
    
    // MARK: - JCross Pose Upload
    
    private func startTrackingUploadLoop() {
        // Timer等を使って高頻度で呼ばれる想定
        let timer = Timer.scheduledTimer(withTimeInterval: 1.0 / 90.0, repeats: true) { [weak self] _ in
            self?.uploadJCrossPose()
        }
        RunLoop.main.add(timer, forMode: .common)
    }
    
    // VRPosePacket Closure
    public var getTrackingData: (() -> (head: simd_float4x4, left: simd_float4x4, right: simd_float4x4, leftPinch: Bool, rightPinch: Bool)?)?
    
    private func uploadJCrossPose() {
        guard let td = getTrackingData?() else { return }
        
        var payload = JCrossPayload()
        payload.timestamp = CACurrentMediaTime()
        
        // Convert matrix to position/rotation
        func extract(matrix: simd_float4x4, to node: inout JCrossNode_VR) {
            node.position = simd_make_float3(matrix.columns.3.x, matrix.columns.3.y, matrix.columns.3.z)
            node.rotation = simd_quatf(matrix)
        }
        
        extract(matrix: td.head, to: &payload.headNode)
        extract(matrix: td.left, to: &payload.leftHandNode)
        extract(matrix: td.right, to: &payload.rightHandNode)
        
        payload.leftInput.triggerValue = td.leftPinch ? 1.0 : 0.0
        payload.rightInput.triggerValue = td.rightPinch ? 1.0 : 0.0
        
        let packet = payload.serialize()
        
        listenerConnection?.send(content: packet, completion: .contentProcessed({ error in
            if let error = error {
                print("[VisionClient] Failed to send JCrossPayload: \(error)")
            }
        }))
    }
}
