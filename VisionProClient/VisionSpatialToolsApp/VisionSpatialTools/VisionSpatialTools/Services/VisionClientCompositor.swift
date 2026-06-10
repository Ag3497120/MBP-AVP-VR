import Foundation
import Network
import CoreBluetooth
import VideoToolbox
import ARKit
import RealityKit
import QuartzCore
import CompositorServices
import Metal
import MetalKit
import MetalFX
/// Phase 14: VisionOS Drop-in SDK (Client Side)
/// ユーザーが保有する「vision spatial tools」にドロップインで組み込める、
/// Mac(Verantyx)からのゼロコピー映像のデコードと、JCrossトラッキング情報の送信を行うコアモジュールです。
public class VisionClientCompositor: NSObject, HEVCVideoDecoderDelegate {
    public var onFrameDecoded: ((CVPixelBuffer, Double, simd_float4x4) -> Void)?
    private var listener: NWListener?
    private var trackingConnection: NWConnection?
    private var isTrackingConnectionReady = false
    
    // --- VideoToolbox (Hardware Decoder) ---
    private var decoder = HEVCVideoDecoder()
    
    // --- Chunk Assembly State ---
    private var currentSeq: UInt32 = 0
    private var lastDecodedSeq: UInt32 = 0
    private var readyToDecodeQueue: [UInt32: Data] = [:]
    
    private struct FrameAssembler {
        var chunksReceived: UInt32 = 0
        var expectedChunks: UInt32 = 0
        var maxFrameSize: Int = 0
        var frameBuffer: Data
        var timestamp: Double = 0.0
        var transform: simd_float4x4 = matrix_identity_float4x4
    }
    private var assemblers: [UInt32: FrameAssembler] = [:]
    
    private var dummyBrowser: NWBrowser?
    private let networkingQueue = DispatchQueue(label: "com.verantyx.networking", qos: .userInteractive)
    
    public override init() {
        super.init()
        decoder.delegate = self
        
        // Force trigger Local Network Permission prompt
        let descriptor = NWBrowser.Descriptor.bonjour(type: "_verantyxvr._udp", domain: "local")
        dummyBrowser = NWBrowser(for: descriptor, using: .udp)
        dummyBrowser?.stateUpdateHandler = { state in
            print("[VisionClient] Browser state: \(state)")
        }
        dummyBrowser?.start(queue: .main)
        
        reconnect(to: "")
    }
    
    private var joyconConnection: NWConnection?
    
    public func reconnect(to ip: String) {
        trackingConnection?.cancel()
        trackingConnection = nil
        
        let udpOptionsTracking = NWProtocolUDP.Options()
        // Increase UDP receive window to 8MB to absorb high-bitrate video bursts and prevent dropped frames
        udpOptionsTracking.receiveWindowSize = 8 * 1024 * 1024
        
        let trackingParams = NWParameters(dtls: nil, udp: udpOptionsTracking)
        trackingParams.includePeerToPeer = true
        
        let udpOptionsJoyc = NWProtocolUDP.Options()
        let joyconParams = NWParameters(dtls: nil, udp: udpOptionsJoyc)
        joyconParams.includePeerToPeer = true
        
        let targetIP = ip
        let endpoint: NWEndpoint
        if targetIP.isEmpty {
            endpoint = NWEndpoint.service(name: "VerantyxVR", type: "_verantyxvr._udp", domain: "local", interface: nil)
        } else {
            endpoint = NWEndpoint.hostPort(host: NWEndpoint.Host(targetIP), port: NWEndpoint.Port(rawValue: 9999)!)
        }
        
        trackingConnection = NWConnection(to: endpoint, using: trackingParams)
        
        let joyconEndpoint: NWEndpoint
        if targetIP.isEmpty {
            joyconEndpoint = NWEndpoint.service(name: "VerantyxJoyc", type: "_verantyxjoyc._udp", domain: "local", interface: nil)
        } else {
            joyconEndpoint = NWEndpoint.hostPort(host: NWEndpoint.Host(targetIP), port: NWEndpoint.Port(rawValue: 11002)!)
        }
        
        joyconConnection?.cancel()
        joyconConnection = NWConnection(to: joyconEndpoint, using: joyconParams)
        
        if targetIP.isEmpty {
            print("[VisionClient] Searching for Mac via AWDL Bonjour (_verantyxvr._udp)...")
        } else {
            print("[VisionClient] Connecting directly to Mac at \(targetIP):9999 and 11002...")
        }
        
        self.packetCount = 0
        var hasStartedReceiving = false
        
        trackingConnection?.stateUpdateHandler = { [weak self] state in
            guard let self = self else { return }
            print("[VisionClient] Connection state: \(state)")
            if state == .ready {
                self.isTrackingConnectionReady = true
                if !hasStartedReceiving {
                    hasStartedReceiving = true
                    self.receiveMessage()
                }
            } else {
                self.isTrackingConnectionReady = false
            }
        }
        
        trackingConnection?.start(queue: networkingQueue)
        joyconConnection?.start(queue: networkingQueue)
    }
    
    private var packetCount = 0
    private func receiveMessage() {
        trackingConnection?.receiveMessage { [weak self] (content, context, isComplete, error) in
            if let data = content {
                self?.packetCount += 1
                if let count = self?.packetCount {
                    if count <= 5 || count % 300 == 0 {
                        print("[VisionClient] RAW PACKET RECEIVED! size: \(data.count) (packet #\(count))")
                    }
                }
                self?.processUDPPacket(data)
            }
            if error == nil && self?.trackingConnection?.state == .ready {
                self?.receiveMessage()
            } else {
                print("[VisionClient] Receive error or disconnected: \(String(describing: error))")
            }
        }
    }
    
    private func processUDPPacket(_ data: Data) {
        guard data.count >= 96 else { return }
        
        let magic = data[0..<4].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        guard magic == 0x5652414E else { return } // Match HardwareEncoder's 0x5652414E
        
        let seq = data[4..<8].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let chunkIdx = data[8..<12].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let totalChunks = data[12..<16].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let chunkOffset = data[16..<20].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let payloadSize = data[20..<24].withUnsafeBytes { $0.load(as: UInt32.self).littleEndian }
        let timestamp = data[24..<32].withUnsafeBytes { $0.load(as: Double.self) }
        let transform = data[32..<96].withUnsafeBytes { $0.load(as: simd_float4x4.self) }
        
        // Print the first few packets unconditionally to verify reception
        if seq < 10 && chunkIdx == 0 {
            print("[VisionClient] Received UDP packet: seq=\\(seq) chunk=\\(chunkIdx)/\\(totalChunks) ts=\\(timestamp)")
        } else if chunkIdx == 0 && seq % 60 == 0 {
            print("[VisionClient] Received UDP packet: seq=\\(seq) chunk=\\(chunkIdx)/\\(totalChunks) ts=\\(timestamp)")
        }
        
        if currentSeq > 5 && seq < currentSeq - 5 {
            // Ignore very late packets from older frames
            return
        }
        
        var assembler = assemblers[seq] ?? FrameAssembler(frameBuffer: Data(count: Int(totalChunks * 1500)))
        assembler.expectedChunks = totalChunks
        assembler.timestamp = timestamp
        assembler.transform = transform
        
        let payloadData = data.subdata(in: 96..<(96 + Int(payloadSize)))
        let endOffset = Int(chunkOffset) + Int(payloadSize)
        
        if endOffset > assembler.maxFrameSize {
            assembler.maxFrameSize = endOffset
        }
        
        if endOffset <= assembler.frameBuffer.count {
            assembler.frameBuffer.replaceSubrange(Int(chunkOffset)..<endOffset, with: payloadData)
        } else {
            let padding = Data(count: endOffset - assembler.frameBuffer.count)
            assembler.frameBuffer.append(padding)
            assembler.frameBuffer.replaceSubrange(Int(chunkOffset)..<endOffset, with: payloadData)
        }
        
        assembler.chunksReceived += 1
        assemblers[seq] = assembler
        
        if assembler.chunksReceived == assembler.expectedChunks {
            // print("[VisionClient] SUCCESS: Assembled FULL frame \(seq) with \(totalChunks) chunks!")
            
            if seq > currentSeq {
                currentSeq = seq
            }
            
            if lastDecodedSeq == 0 || (lastDecodedSeq > seq + 10000) {
                lastDecodedSeq = seq - 1
            }
            
            // 1. Normal Decode: Process contiguous frames in STRICT order
            while let readyAssembler = assemblers[lastDecodedSeq + 1], readyAssembler.chunksReceived == readyAssembler.expectedChunks {
                let completeFrame = readyAssembler.frameBuffer.subdata(in: 0..<readyAssembler.maxFrameSize)
                decoder.decodeAnnexBFrame(completeFrame, sequence: lastDecodedSeq + 1, timestamp: readyAssembler.timestamp, transform: readyAssembler.transform)
                lastDecodedSeq += 1
                assemblers.removeValue(forKey: lastDecodedSeq)
            }
            
            // 2. Timeout/Skip Logic: If a packet is permanently lost, wait up to 15 frames (~166ms) before forcing a skip.
            // This deep jitter buffer ensures that out-of-order packets due to Wi-Fi jitter do NOT corrupt the HEVC stream!
            // Corrupting the stream causes 500ms freezes ("hitching"), so waiting 166ms is vastly superior.
            if currentSeq >= lastDecodedSeq + 15 {
                print("[VisionClient] LOST FRAME DETECTED! Force skipping. currentSeq=\(currentSeq), lastDecodedSeq=\(lastDecodedSeq)")
                let sortedKeys = assemblers.keys.sorted()
                for k in sortedKeys {
                    if k > lastDecodedSeq {
                        if let a = assemblers[k], a.chunksReceived == a.expectedChunks {
                            lastDecodedSeq = k - 1
                            break
                        }
                    }
                }
                
                // Flush the newly found contiguous frames
                while let readyAssembler = assemblers[lastDecodedSeq + 1], readyAssembler.chunksReceived == readyAssembler.expectedChunks {
                    let completeFrame = readyAssembler.frameBuffer.subdata(in: 0..<readyAssembler.maxFrameSize)
                    decoder.decodeAnnexBFrame(completeFrame, sequence: lastDecodedSeq + 1, timestamp: readyAssembler.timestamp, transform: readyAssembler.transform)
                    lastDecodedSeq += 1
                    assemblers.removeValue(forKey: lastDecodedSeq)
                }
            }
            
            // Clean up old assemblers to save memory. 
            let keysToRemove = assemblers.keys.filter { $0 < currentSeq && (currentSeq - $0 > 60) }
            for k in keysToRemove { assemblers.removeValue(forKey: k) }
        }
    }
    
    // MARK: - HEVCVideoDecoderDelegate
    public func didDecodeFrame(_ pixelBuffer: CVPixelBuffer, timestamp: Double, transform: simd_float4x4) {
        onFrameDecoded?(pixelBuffer, timestamp, transform)
    }
    
    public func sendPose(head: simd_float4x4, leftHand: simd_float4x4, rightHand: simd_float4x4, leftPinch: UInt8, rightPinch: UInt8, leftTrigger: UInt8, rightTrigger: UInt8,
                         rightButtons: UInt32 = 0, leftButtons: UInt32 = 0,
                         rightStickX: Float = 0, rightStickY: Float = 0,
                         leftStickX: Float = 0, leftStickY: Float = 0) {
        guard isTrackingConnectionReady, let trackingConnection = trackingConnection else { return }
        
        var packetData = Data()
        
        // magic (0x504F5345 = "POSE")
        var magic: UInt32 = 0x45534F50 // "POSE" in little endian
        packetData.append(Data(bytes: &magic, count: 4))
        
        // 4 bytes padding to align the Double to 8-byte boundary!
        var padding: UInt32 = 0
        packetData.append(Data(bytes: &padding, count: 4))
        
        // timestamp (CACurrentMediaTime)
        var ts: Double = CACurrentMediaTime()
        packetData.append(Data(bytes: &ts, count: 8))
        
        func appendMatrix(_ matrix: simd_float4x4) {
            var m = matrix
            // Copy matrix columns as a contiguous float array
            let data = Data(bytes: &m, count: 64)
            packetData.append(data)
        }
        
        // Debug print the translation to prove we are sending active tracking data
        if Int.random(in: 0...60) == 0 {
            let pos = head.columns.3
            print("[VisionClient] Sending POSE: x=\\(pos.x) y=\\(pos.y) z=\\(pos.z)")
        }
        
        appendMatrix(head)
        appendMatrix(leftHand)
        appendMatrix(rightHand)
        
        var lPinch: UInt8 = leftPinch
        var rPinch: UInt8 = rightPinch
        var lTrig: UInt8 = leftTrigger
        var rTrig: UInt8 = rightTrigger
        
        packetData.append(Data(bytes: &lPinch, count: 1))
        packetData.append(Data(bytes: &rPinch, count: 1))
        packetData.append(Data(bytes: &lTrig, count: 1))
        packetData.append(Data(bytes: &rTrig, count: 1))
        
        var rBtn = rightButtons
        var lBtn = leftButtons
        packetData.append(Data(bytes: &rBtn, count: 4))
        packetData.append(Data(bytes: &lBtn, count: 4))
        
        var rsX = rightStickX, rsY = rightStickY
        var lsX = leftStickX, lsY = leftStickY
        packetData.append(Data(bytes: &rsX, count: 4))
        packetData.append(Data(bytes: &rsY, count: 4))
        packetData.append(Data(bytes: &lsX, count: 4))
        packetData.append(Data(bytes: &lsY, count: 4))
        
        trackingConnection.send(content: packetData, completion: .contentProcessed({ error in
            if let error = error {
                print("[VisionClient] Failed to send POSE packet: \(error)")
            }
        }))
    }
    
    public func sendJoycon(rightButtons: UInt32, leftButtons: UInt32,
                           rightStickX: Float, rightStickY: Float,
                           leftStickX: Float, leftStickY: Float,
                           rightVelX: Float, rightVelY: Float, rightVelZ: Float,
                           leftVelX: Float, leftVelY: Float, leftVelZ: Float) {
        guard let joyconConnection = joyconConnection else { return }
        
        var packetData = Data()
        var magic: UInt32 = 0x4A4F5943 // Match HardwareEncoder's 0x4A4F5943
        packetData.append(Data(bytes: &magic, count: 4))
        
        var rBtn = rightButtons
        var lBtn = leftButtons
        packetData.append(Data(bytes: &rBtn, count: 4))
        packetData.append(Data(bytes: &lBtn, count: 4))
        
        var rsX = rightStickX, rsY = rightStickY
        var lsX = leftStickX, lsY = leftStickY
        packetData.append(Data(bytes: &rsX, count: 4))
        packetData.append(Data(bytes: &rsY, count: 4))
        packetData.append(Data(bytes: &lsX, count: 4))
        packetData.append(Data(bytes: &lsY, count: 4))
        
        var rvX = rightVelX, rvY = rightVelY, rvZ = rightVelZ
        var lvX = leftVelX, lvY = leftVelY, lvZ = leftVelZ
        packetData.append(Data(bytes: &rvX, count: 4))
        packetData.append(Data(bytes: &rvY, count: 4))
        packetData.append(Data(bytes: &rvZ, count: 4))
        packetData.append(Data(bytes: &lvX, count: 4))
        packetData.append(Data(bytes: &lvY, count: 4))
        packetData.append(Data(bytes: &lvZ, count: 4))
        
        joyconConnection.send(content: packetData, completion: .contentProcessed({ error in
            if let error = error {
                print("[VisionClient] Failed to send JOYC packet: \(error)")
            }
        }))
    }
}

class CompositorRenderer {
    private var renderFrameCount: Int = 0
    let layerRenderer: LayerRenderer
    let worldTrackingProvider: WorldTrackingProvider
    let device: MTLDevice
    let commandQueue: MTLCommandQueue
    var pipelineState: MTLRenderPipelineState?
    var pipelineStateNoVideo: MTLRenderPipelineState?
    var samplerState: MTLSamplerState?
    var textureCache: CVMetalTextureCache?
    
    // --- MetalFX Upscaling ---
    private var intermediateTexture: MTLTexture?
    private var mfxScalerLeft: MTLFXSpatialScaler?
    private var mfxScalerRight: MTLFXSpatialScaler?
    private var mfxInputWidth: Int = 0
    private var mfxInputHeight: Int = 0
    
    // インスタンスに担当しない。AppModel共有バッファを参照する。
    // CompositorLayerクロージャが何度啇び出されても、
    // 常に同じ AppModelのバッファを読む。
    private weak var appModel: AppModel?
    
    init(_ layerRenderer: LayerRenderer, appModel: AppModel) {
        self.layerRenderer = layerRenderer
        self.worldTrackingProvider = appModel.worldTrackingProvider
        self.appModel = appModel          // ← 共有バッファへの参照を保持
        self.device = layerRenderer.device
        self.commandQueue = self.device.makeCommandQueue()!
        
        CVMetalTextureCacheCreate(kCFAllocatorDefault, nil, self.device, nil, &textureCache)
        let samplerDesc = MTLSamplerDescriptor()
        samplerDesc.minFilter = .linear
        samplerDesc.magFilter = .linear
        samplerDesc.sAddressMode = .clampToEdge
        samplerDesc.tAddressMode = .clampToEdge
        self.samplerState = self.device.makeSamplerState(descriptor: samplerDesc)
        
        setupPipeline()
    }
    
    func setupPipeline() {
        let shaderSource = """
        #include <metal_stdlib>
        using namespace metal;

        struct VertexOut {
            float4 position [[position]];
            float2 texCoord;
            uint eyeIndex;
        };

        vertex VertexOut vrVertexShader(uint vertexID [[vertex_id]],
                                      uint ampID [[amplification_id]],
                                      constant float4x4 *projectionMatrices [[buffer(0)]],
                                      constant float4 *tangentsArray [[buffer(1)]],
                                      constant float4x4 *headMatrices [[buffer(2)]]) {
            
            // Fallback to exactly 1.0 to prevent Metal from clipping the quad or causing zero-area intersections
            float left = tangentsArray[ampID].x;
            float right = tangentsArray[ampID].y;
            float top = tangentsArray[ampID].z;
            float bottom = tangentsArray[ampID].w;
            
            // Quad perfectly sized to the view frustum at Z = -1
            float2 positions[6] = {
                float2(left, bottom),
                float2(right, bottom),
                float2(left, top),
                float2(left, top),
                float2(right, bottom),
                float2(right, top)
            };
            
            float2 texCoords[6] = {
                float2(0.0, 1.0),
                float2(1.0, 1.0),
                float2(0.0, 0.0),
                float2(0.0, 0.0),
                float2(1.0, 1.0),
                float2(1.0, 0.0)
            };
            
            // Quad perfectly sized to the view frustum at Z = -1
            float4 viewSpacePos = float4(positions[vertexID], -1.0, 1.0);
            
            VertexOut out;
            // Project into OS clip space directly from view space (deviceAnchor handles the 6-DOF ATW)
            out.position = projectionMatrices[ampID] * viewSpacePos;
            out.texCoord = texCoords[vertexID];
            out.eyeIndex = ampID;
            return out;
        }
        fragment float4 vrFragmentShader(VertexOut in [[stage_in]],
                                       texture2d<float, access::sample> videoTexture [[texture(0)]],
                                       sampler videoSampler [[sampler(0)]]) {
            float2 uv = in.texCoord;
            
            // SteamVR sends Side-by-Side (SBS) video. 
            // Left eye (eyeIndex == 0) reads the left half of the texture.
            // Right eye (eyeIndex == 1) reads the right half.
            if (in.eyeIndex == 0) {
                uv.x = uv.x * 0.5;
            } else {
                uv.x = uv.x * 0.5 + 0.5;
            }
            
            float4 color = videoTexture.sample(videoSampler, uv);
            
            // If the texture is YUV (r8Unorm), G and B channels are 0. Make it grayscale.
            if (color.g == 0.0 && color.b == 0.0 && color.r > 0.0) {
                color = float4(color.r, color.r, color.r, 1.0);
            }
            
            return color;
        }

        fragment float4 vrFragmentShaderNoVideo(VertexOut in [[stage_in]]) {
            // Just return green when there is no video
            return float4(0.0, 1.0, 0.0, 1.0);
        }
        """

        do {
            let library = try device.makeLibrary(source: shaderSource, options: nil)
            print("[CompositorRenderer] Successfully compiled MTLLibrary from source")
            
            let vertexFunction = library.makeFunction(name: "vrVertexShader")
            let fragmentFunction = library.makeFunction(name: "vrFragmentShader")
            let fragmentFunctionNoVideo = library.makeFunction(name: "vrFragmentShaderNoVideo")
            
            let pipelineDescriptor = MTLRenderPipelineDescriptor()
            pipelineDescriptor.vertexFunction = vertexFunction
            pipelineDescriptor.fragmentFunction = fragmentFunction
            pipelineDescriptor.colorAttachments[0].pixelFormat = layerRenderer.configuration.colorFormat
            pipelineDescriptor.depthAttachmentPixelFormat = layerRenderer.configuration.depthFormat
            
            // Required by Vision Pro when using render_target_array_index
            pipelineDescriptor.inputPrimitiveTopology = .triangle
            
            // Critical for stereoscopic layered rendering
            pipelineDescriptor.maxVertexAmplificationCount = layerRenderer.configuration.layout == .layered ? 2 : 1
            
            self.pipelineState = try device.makeRenderPipelineState(descriptor: pipelineDescriptor)
            
            // Create the No Video pipeline
            let pipelineDescriptorNoVideo = MTLRenderPipelineDescriptor()
            pipelineDescriptorNoVideo.vertexFunction = vertexFunction
            pipelineDescriptorNoVideo.fragmentFunction = fragmentFunctionNoVideo
            pipelineDescriptorNoVideo.colorAttachments[0].pixelFormat = layerRenderer.configuration.colorFormat
            pipelineDescriptorNoVideo.depthAttachmentPixelFormat = layerRenderer.configuration.depthFormat
            pipelineDescriptorNoVideo.inputPrimitiveTopology = .triangle
            pipelineDescriptorNoVideo.maxVertexAmplificationCount = layerRenderer.configuration.layout == .layered ? 2 : 1
            
            self.pipelineStateNoVideo = try device.makeRenderPipelineState(descriptor: pipelineDescriptorNoVideo)
            
            let samplerDesc = MTLSamplerDescriptor()
            samplerDesc.minFilter = .linear
            samplerDesc.magFilter = .linear
            samplerDesc.sAddressMode = .clampToEdge
            samplerDesc.tAddressMode = .clampToEdge
            self.samplerState = device.makeSamplerState(descriptor: samplerDesc)
            
            print("[CompositorRenderer] Pipelines created successfully")
            
        } catch {
            print("[CompositorRenderer] Failed to create pipeline state: \(String(describing: error))")
        }
    }
    
    private var depthState: MTLDepthStencilState?

    func startRenderLoop() {
        let depthDesc = MTLDepthStencilDescriptor()
        depthDesc.depthCompareFunction = .always
        depthDesc.isDepthWriteEnabled = true
        self.depthState = device.makeDepthStencilState(descriptor: depthDesc)
        
        let thread = Thread {
            do {
                try self.renderLoop()
            } catch {
                print("[CompositorRenderer] Render loop error: \(error)")
            }
        }
        thread.name = "com.verantyx.renderloop"
        thread.start()
    }
    private var lastValidAnchor: DeviceAnchor? = nil
    
    private func renderLoop() throws {
        
        while true {
            guard layerRenderer.state != .invalidated else {
                print("[CompositorRenderer] Layer invalidated, stopping loop")
                break
            }
            guard layerRenderer.state != .paused else {
                if renderFrameCount % 60 == 0 { print("[CompositorRenderer] Layer is paused...") }
                Thread.sleep(forTimeInterval: 0.1)
                continue
            }
            
            guard let frame = layerRenderer.queryNextFrame() else { continue }
            frame.startUpdate()
            frame.endUpdate()
            
            guard let timing = frame.predictTiming() else { continue }
            LayerRenderer.Clock().wait(until: timing.optimalInputTime)
            
            // 映像フレームを取得 (This pops the latest frame and updates latestTimestamp/Transform)
            let pb = self.appModel?.latestPixelBuffer
            
            // In visionOS, we cannot use plain CACurrentMediaTime for the device anchor.
            // We must predict the exact target time using the Compositor's Clock Instant.
            // We now use the exact past timestamp when the frame was rendered by SteamVR
            let ts = self.appModel?.latestTimestamp ?? 0.0
            let effectiveTs = ts > 0 ? ts : CACurrentMediaTime()
            
            // 過去のタイムスタンプを使ってATWの相殺を行う
            var validAnchor = worldTrackingProvider.state == .running
                ? worldTrackingProvider.queryDeviceAnchor(atTimestamp: effectiveTs)
                : nil
                
            // 履歴バッファから漏れた等でアンカーが取れなかった場合は、前回成功したアンカーを維持する。
            // 絶対に CACurrentMediaTime()（現在時刻）にはフォールバックしない！
            // もし現在時刻にフォールバックすると、ATWがゼロになり映像が顔に張り付いて「ガクガク・ブルブル」の原因になるため。
            if validAnchor == nil {
                validAnchor = lastValidAnchor
            } else {
                lastValidAnchor = validAnchor
            }
            
            if renderFrameCount % 300 == 0 {
                let hasPB = pb != nil
                let anchorStatus = validAnchor == nil ? "NIL (World Origin)" : (predictedAnchor != nil ? "VALID (Predicted)" : "VALID (Fallback/Cached)")
                let lag = CACurrentMediaTime() - effectiveTs
                print("[CompositorRenderer] renderFrame=\(renderFrameCount) hasPixelBuffer=\(hasPB) anchor=\(anchorStatus) lag=\(lag*1000)ms ts=\(ts)")
            }
            
            guard let drawable = frame.queryDrawable() else { continue }
            
            frame.startSubmission()
            
            if let anchor = validAnchor {
                drawable.deviceAnchor = anchor
            }
            
            // Render the frame, passing the already popped pixel buffer
            let currentHeadT = validAnchor?.originFromAnchorTransform ?? matrix_identity_float4x4
            renderFrame(drawable: drawable, headT: currentHeadT, pb: pb)
            
            frame.endSubmission()
            renderFrameCount += 1
        }
    }
    
    private func renderFrame(drawable: LayerRenderer.Drawable, headT: simd_float4x4, pb: CVPixelBuffer?) {
        let isLayered = layerRenderer.configuration.layout == .layered
        let viewCount = drawable.views.count
        
        guard let commandBuffer = commandQueue.makeCommandBuffer() else { return }
        
        var metalTexture: MTLTexture? = nil
        
        if let pb = pb, let tc = self.textureCache {
            let actualFmt = CVPixelBufferGetPixelFormatType(pb)
            var cvTextureOut: CVMetalTexture?
            var metalPixelFormat: MTLPixelFormat = .bgra8Unorm
            var planeIndex: Int = 0
            
            let kBGRA: OSType = kCVPixelFormatType_32BGRA
            let k420v: OSType = kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange
            let k420f: OSType = kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
            
            if actualFmt == kBGRA {
                metalPixelFormat = .bgra8Unorm
            } else if actualFmt == k420v || actualFmt == k420f {
                metalPixelFormat = .r8Unorm
                planeIndex = 0
            }
            
            let texWidth = planeIndex == 0 ? CVPixelBufferGetWidth(pb) : CVPixelBufferGetWidthOfPlane(pb, planeIndex)
            let texHeight = planeIndex == 0 ? CVPixelBufferGetHeight(pb) : CVPixelBufferGetHeightOfPlane(pb, planeIndex)
            
            let status = CVMetalTextureCacheCreateTextureFromImage(
                kCFAllocatorDefault, tc, pb, nil,
                metalPixelFormat,
                texWidth, texHeight, planeIndex,
                &cvTextureOut
            )
            
            if status == kCVReturnSuccess, let cvt = cvTextureOut {
                metalTexture = CVMetalTextureGetTexture(cvt)
            }
        }
        
        let rpdesc = MTLRenderPassDescriptor()
        
        // --- MetalFX Intermediate Texture Setup ---
        // Determine native output resolution from the drawable
        let outWidth = drawable.colorTextures[0].width
        let outHeight = drawable.colorTextures[0].height
        
        // Target rendering resolution (Mac side resolution)
        let inWidth = 1440
        let inHeight = 1620
        
        if intermediateTexture == nil || intermediateTexture?.width != inWidth || intermediateTexture?.height != inHeight {
            let texDesc = MTLTextureDescriptor()
            texDesc.textureType = isLayered ? .type2DArray : .type2D
            texDesc.pixelFormat = layerRenderer.configuration.colorFormat
            texDesc.width = inWidth
            texDesc.height = inHeight
            texDesc.arrayLength = isLayered ? 2 : 1
            texDesc.usage = [.renderTarget, .shaderRead]
            texDesc.storageMode = .private
            intermediateTexture = device.makeTexture(descriptor: texDesc)
            
            // Re-create MetalFX Scalers
            let scalerDesc = MTLFXSpatialScalerDescriptor()
            scalerDesc.colorTextureFormat = layerRenderer.configuration.colorFormat
            scalerDesc.outputTextureFormat = layerRenderer.configuration.colorFormat
            scalerDesc.inputWidth = inWidth
            scalerDesc.inputHeight = inHeight
            scalerDesc.outputWidth = outWidth
            scalerDesc.outputHeight = outHeight
            scalerDesc.colorProcessingMode = .perceptual
            
            do {
                mfxScalerLeft = try scalerDesc.makeSpatialScaler(device: device)
                if isLayered {
                    mfxScalerRight = try scalerDesc.makeSpatialScaler(device: device)
                }
            } catch {
                print("[MetalFX] Error creating scaler: \(error)")
            }
        }
        
        rpdesc.colorAttachments[0].texture    = intermediateTexture ?? drawable.colorTextures[0]
        rpdesc.colorAttachments[0].loadAction  = .clear
        rpdesc.colorAttachments[0].storeAction = .store
        // 🔥 If video is missing or invalid, clear to BLUE. If video is present, clear to BLACK.
        rpdesc.colorAttachments[0].clearColor  = metalTexture == nil 
            ? MTLClearColor(red: 0.0, green: 0.0, blue: 0.5, alpha: 1.0) 
            : MTLClearColor(red: 0.0, green: 0.0, blue: 0.0, alpha: 1.0)
        
        if drawable.depthTextures.count > 0 {
            rpdesc.depthAttachment.texture = drawable.depthTextures[0]
            rpdesc.depthAttachment.loadAction = .clear
            rpdesc.depthAttachment.storeAction = .store
            rpdesc.depthAttachment.clearDepth = 0.0
        }
        
        if isLayered { rpdesc.renderTargetArrayLength = viewCount }
        
        if let enc = commandBuffer.makeRenderCommandEncoder(descriptor: rpdesc) {
            enc.setRenderPipelineState(pipelineState!)
            enc.setCullMode(.none)
            if let ds = self.depthState {
                enc.setDepthStencilState(ds)
            }
            
            if isLayered {
                var mappings = (0..<viewCount).map { i in
                    MTLVertexAmplificationViewMapping(
                        viewportArrayIndexOffset: UInt32(i),
                        renderTargetArrayIndexOffset: UInt32(i)
                    )
                }
                enc.setVertexAmplificationCount(viewCount, viewMappings: &mappings)
            }
            enc.setViewports(drawable.views.map { $0.textureMap.viewport })
            
            // 🔥 OSから提供された完璧なプロジェクション行列（視界情報）をMetalに渡す
            // VisionOSでは tangents (left, right, top, bottom) からプロジェクション行列を自力で構築する
            func makeProj(_ tangents: simd_float4) -> simd_float4x4 {
                let left = tangents[0]
                let right = tangents[1]
                let top = tangents[2]
                let bottom = tangents[3]
                let nearZ: Float = 0.01
                let farZ: Float = 100.0
                
                let x = 2.0 / (right - left)
                let y = 2.0 / (top - bottom)
                let a = (right + left) / (right - left)
                let b = (top + bottom) / (top - bottom)
                // Reverse-Z mapping for visionOS
                let c = nearZ / (farZ - nearZ)
                let d = (farZ * nearZ) / (farZ - nearZ)
                
                return simd_float4x4(
                    simd_float4(x, 0, 0, 0),
                    simd_float4(0, y, 0, 0),
                    simd_float4(a, b, c, -1),
                    simd_float4(0, 0, d, 0)
                )
            }
            
            var proj0 = makeProj(drawable.views[0].tangents)
            var proj1 = isLayered && viewCount > 1 ? makeProj(drawable.views[1].tangents) : proj0
            var uniforms: [simd_float4x4] = [proj0, proj1]
            enc.setVertexBytes(&uniforms, length: MemoryLayout<simd_float4x4>.stride * 2, index: 0)
            
            var tan0 = drawable.views[0].tangents
            var tan1 = isLayered && viewCount > 1 ? drawable.views[1].tangents : tan0
            var tangentsArray: [simd_float4] = [tan0, tan1]
            enc.setVertexBytes(&tangentsArray, length: MemoryLayout<simd_float4>.stride * 2, index: 1)
            
            var headArray: [simd_float4x4] = [headT, headT]
            enc.setVertexBytes(&headArray, length: MemoryLayout<simd_float4x4>.stride * 2, index: 2)
            
            if let tex = metalTexture, let sampler = self.samplerState {
                enc.setFragmentTexture(tex, index: 0)
                enc.setFragmentSamplerState(sampler, index: 0)
                enc.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
            }
            
            enc.endEncoding()
        }
        
        // --- MetalFX Upscaling Pass ---
        if let interTex = intermediateTexture, let outTex = drawable.colorTextures[0] as MTLTexture? {
            if isLayered, let scalerL = mfxScalerLeft, let scalerR = mfxScalerRight {
                // Upscale Left Eye (Slice 0)
                if let inViewL = interTex.makeTextureView(pixelFormat: interTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 0..<1),
                   let outViewL = outTex.makeTextureView(pixelFormat: outTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 0..<1) {
                    scalerL.colorTexture = inViewL
                    scalerL.outputTexture = outViewL
                    scalerL.encode(commandBuffer: commandBuffer)
                }
                
                // Upscale Right Eye (Slice 1)
                if viewCount > 1, let inViewR = interTex.makeTextureView(pixelFormat: interTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 1..<2),
                   let outViewR = outTex.makeTextureView(pixelFormat: outTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 1..<2) {
                    scalerR.colorTexture = inViewR
                    scalerR.outputTexture = outViewR
                    scalerR.encode(commandBuffer: commandBuffer)
                }
            } else if let scalerL = mfxScalerLeft {
                scalerL.colorTexture = interTex
                scalerL.outputTexture = outTex
                scalerL.encode(commandBuffer: commandBuffer)
            }
        }

        drawable.encodePresent(commandBuffer: commandBuffer)
        commandBuffer.commit()
        if let tc = self.textureCache {
            CVMetalTextureCacheFlush(tc, 0)
        }
    }
    
    private func renderFallback(drawable: LayerRenderer.Drawable, isLayered: Bool, viewCount: Int, color: MTLClearColor) {
        guard let commandBuffer = commandQueue.makeCommandBuffer() else { return }
        
        let rpdesc = MTLRenderPassDescriptor()
        
        // --- MetalFX Intermediate Texture Setup ---
        // Determine native output resolution from the drawable
        let outWidth = drawable.colorTextures[0].width
        let outHeight = drawable.colorTextures[0].height
        
        // Target rendering resolution (Mac side resolution)
        let inWidth = 1440
        let inHeight = 1620
        
        if intermediateTexture == nil || intermediateTexture?.width != inWidth || intermediateTexture?.height != inHeight {
            let texDesc = MTLTextureDescriptor()
            texDesc.textureType = isLayered ? .type2DArray : .type2D
            texDesc.pixelFormat = layerRenderer.configuration.colorFormat
            texDesc.width = inWidth
            texDesc.height = inHeight
            texDesc.arrayLength = isLayered ? 2 : 1
            texDesc.usage = [.renderTarget, .shaderRead]
            texDesc.storageMode = .private
            intermediateTexture = device.makeTexture(descriptor: texDesc)
            
            // Re-create MetalFX Scalers
            let scalerDesc = MTLFXSpatialScalerDescriptor()
            scalerDesc.colorTextureFormat = layerRenderer.configuration.colorFormat
            scalerDesc.outputTextureFormat = layerRenderer.configuration.colorFormat
            scalerDesc.inputWidth = inWidth
            scalerDesc.inputHeight = inHeight
            scalerDesc.outputWidth = outWidth
            scalerDesc.outputHeight = outHeight
            scalerDesc.colorProcessingMode = .perceptual
            
            do {
                mfxScalerLeft = try scalerDesc.makeSpatialScaler(device: device)
                if isLayered {
                    mfxScalerRight = try scalerDesc.makeSpatialScaler(device: device)
                }
            } catch {
                print("[MetalFX] Error creating scaler: \(error)")
            }
        }
        
        rpdesc.colorAttachments[0].texture    = intermediateTexture ?? drawable.colorTextures[0]
        rpdesc.colorAttachments[0].loadAction  = .clear
        rpdesc.colorAttachments[0].storeAction = .store
        rpdesc.colorAttachments[0].clearColor  = color
        
        if drawable.depthTextures.count > 0 {
            rpdesc.depthAttachment.texture = drawable.depthTextures[0]
            rpdesc.depthAttachment.loadAction = .clear
            rpdesc.depthAttachment.storeAction = .store
            rpdesc.depthAttachment.clearDepth = 0.0
        }
        
        if isLayered { rpdesc.renderTargetArrayLength = viewCount }
        
        if let enc = commandBuffer.makeRenderCommandEncoder(descriptor: rpdesc) {
            if let pipeline = pipelineStateNoVideo {
                enc.setRenderPipelineState(pipeline)
                if isLayered {
                    var mappings = (0..<viewCount).map { i in
                        MTLVertexAmplificationViewMapping(
                            viewportArrayIndexOffset: UInt32(i),
                            renderTargetArrayIndexOffset: UInt32(i)
                        )
                    }
                    enc.setVertexAmplificationCount(viewCount, viewMappings: &mappings)
                }
                enc.setViewports(drawable.views.map { $0.textureMap.viewport })
                enc.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 6)
            }
            enc.endEncoding()
        }
        
        // --- MetalFX Upscaling Pass ---
        if let interTex = intermediateTexture, let outTex = drawable.colorTextures[0] as MTLTexture? {
            if isLayered, let scalerL = mfxScalerLeft, let scalerR = mfxScalerRight {
                // Upscale Left Eye (Slice 0)
                if let inViewL = interTex.makeTextureView(pixelFormat: interTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 0..<1),
                   let outViewL = outTex.makeTextureView(pixelFormat: outTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 0..<1) {
                    scalerL.colorTexture = inViewL
                    scalerL.outputTexture = outViewL
                    scalerL.encode(commandBuffer: commandBuffer)
                }
                
                // Upscale Right Eye (Slice 1)
                if viewCount > 1, let inViewR = interTex.makeTextureView(pixelFormat: interTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 1..<2),
                   let outViewR = outTex.makeTextureView(pixelFormat: outTex.pixelFormat, textureType: .type2D, levels: 0..<1, slices: 1..<2) {
                    scalerR.colorTexture = inViewR
                    scalerR.outputTexture = outViewR
                    scalerR.encode(commandBuffer: commandBuffer)
                }
            } else if let scalerL = mfxScalerLeft {
                scalerL.colorTexture = interTex
                scalerL.outputTexture = outTex
                scalerL.encode(commandBuffer: commandBuffer)
            }
        }

        drawable.encodePresent(commandBuffer: commandBuffer)
        commandBuffer.commit()
    }
}
