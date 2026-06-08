import SwiftUI
import RealityKit
import ARKit
import Combine

@MainActor
class AppModel: ObservableObject {
    @Published var isImmersiveSpaceActive = false
    @Published var attachedTools: [AttachedTool] = []
    @Published var selectedToolType: ToolType = .keyboard
    @Published var isScanningMode = false
    @Published var magneticSnapEnabled = true

    // ARKit session for spatial tracking
    let arkitSession = ARKitSession()
    let worldTrackingProvider = WorldTrackingProvider()
    let planeDetectionProvider = PlaneDetectionProvider(alignments: [.horizontal, .vertical])
    let handTrackingProvider = HandTrackingProvider()

    // New managers for advanced features
    let objectRecognitionManager = ObjectRecognitionManager()
    let magneticAttachmentManager = MagneticAttachmentManager()
    let objectTrackingManager = ObjectTrackingManager()
    lazy var adaptiveFollower = AdaptiveToolFollower(
        trackingManager: objectTrackingManager,
        magneticManager: magneticAttachmentManager
    )

    // Finger collision detector
    let fingerCollisionDetector = FingerCollisionDetector()
    
    // VR UDP Sender for Hand/Head Tracking
    let vrUDPSender = VRUDPSender()
    @Published var macIPAddress: String = ""
    @Published var framesReceived: Int = 0
    @Published var isSendingTracking: Bool = false


    // vx-browser 移植版 ViewModel
    let vxTabManager       = VxTabManager()
    let vxBrowserViewModel = VxBrowserViewModel()

    // 画像Pinch→AI分析→空間配置
    let spatialObjectBuilder = SpatialObjectBuilder()

    // AIプロバイダ設定
    @Published var aiProvider: AIImageAnalyzer.AIProvider = .openAI
    @Published var aiApiKey:   String = ""

    // ローカルVLM疑似3D化
    let mirror3DBuilder = Mirror3DBuilder()
    let localVLM        = LocalVLMAnalyzer()

    // Active keyboard entity
    weak var activeKeyboard: EnhancedKeyboardEntity?

    private var cancellables = Set<AnyCancellable>()

    // MARK: - Init
    init() {
        Task { @MainActor in
            for await event in self.fingerCollisionDetector.$keyPressEvent.values {
                guard let event else { continue }
                self.vxBrowserViewModel.injectKey(event.key)
                self.activeKeyboard?.animateKeyPress(letter: event.key)
            }
        }
    }

    func startTracking() async {
        do {
            try await arkitSession.run([
                worldTrackingProvider,
                planeDetectionProvider,
                handTrackingProvider
            ])
            print("ARKit tracking started successfully")

            // Start object recognition and tracking
            await objectRecognitionManager.startObjectRecognition(session: arkitSession)
            await objectTrackingManager.startTracking(session: arkitSession)

            // 指先衝突判定ループを並列起動
            Task { await processHandTrackingForCollision() }
            
            // VRストリーミング用のトラッキングループ
            Task { await processTrackingForVR() }
        } catch {
            print("Failed to start ARKit session: \(error)")
        }
    }

    /// HandTrackingProvider のアップデートを FingerCollisionDetector へ流す
    private func processHandTrackingForCollision() async {
        for await update in handTrackingProvider.anchorUpdates {
            fingerCollisionDetector.processHandAnchor(update.anchor)
        }
    }
    
    private func processTrackingForVR() async {
        while !Task.isCancelled {
            try? await Task.sleep(nanoseconds: 16_000_000) // ~60fps
            
            guard !macIPAddress.isEmpty else { continue }
            
            if vrUDPSender.targetIP != macIPAddress {
                vrUDPSender.stop()
                vrUDPSender.start(ip: macIPAddress)
            }
            
            guard let deviceAnchor = worldTrackingProvider.queryDeviceAnchor(atTimestamp: CACurrentMediaTime()) else { continue }
            
            let headT = deviceAnchor.originFromAnchorTransform
            var leftT = matrix_identity_float4x4
            var rightT = matrix_identity_float4x4
            
            let hands = handTrackingProvider.latestAnchors
            if let left = hands.leftHand, left.isTracked {
                leftT = left.originFromAnchorTransform
            }
            if let right = hands.rightHand, right.isTracked {
                rightT = right.originFromAnchorTransform
            }
            
            vrUDPSender.sendPose(
                head: headT,
                leftHand: leftT,
                rightHand: rightT,
                leftPinch: hands.leftHand?.isGeometryTracked ?? false, // Basic approximation
                rightPinch: hands.rightHand?.isGeometryTracked ?? false
            )
            
            DispatchQueue.main.async { [weak self] in
                if self?.isSendingTracking == false {
                    self?.isSendingTracking = true
                }
            }
        }
    }

    func attachTool(type: ToolType, at anchor: AnchorEntity) {
        let tool = AttachedTool(type: type, anchor: anchor)
        attachedTools.append(tool)
    }

    func removeTool(_ tool: AttachedTool) {
        attachedTools.removeAll { $0.id == tool.id }
    }

    func toggleScanningMode() {
        isScanningMode.toggle()
        print("Scanning mode: \(isScanningMode)")
    }

    func toggleMagneticSnap() {
        magneticSnapEnabled.toggle()
        print("Magnetic snap: \(magneticSnapEnabled)")
    }
}

enum ToolType {
    case keyboard
    case memo
    case drawing
    case calculator
    case trackpad   // トラックパッド
    case browser    // 仮想ブラウザ (WKWebView)
    case vrScreen   // MacからのVRストリーミング画面
}

struct AttachedTool: Identifiable {
    let id = UUID()
    let type: ToolType
    let anchor: AnchorEntity
    var position: SIMD3<Float> = [0, 0, 0]
    var isActive: Bool = true
    var attachedToObject: UUID? = nil  // Track which object it's attached to
    var isFollowing: Bool = false  // Whether it follows object movement
}
