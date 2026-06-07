import SwiftUI
import RealityKit
import ARKit
import Combine
import GameController

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
    
    // VR Client Compositor (handles Video receive + Pose send via AWDL)
    let compositor = VisionClientCompositor()
    @Published var macIPAddress: String = "" {
        didSet {
            UserDefaults.standard.set(macIPAddress, forKey: "macIPAddress")
            compositor.reconnect(to: macIPAddress)
        }
    }
    @Published var framesReceived: Int = 0
    @Published var isSendingTracking: Bool = false
    
    // デコード済みフレームの共有バッファ
    // nonisolated(unsafe) でアクター隔離から外し、NSLock でスレッド安全を保証する
    // Metal レンダーループ（バックグラウンドスレッド）と
    // ネットワークコールバックの両方から安全にアクセスできる
    nonisolated(unsafe) private let _pixelBufferLock = NSLock()
    nonisolated(unsafe) private var _presentationQueue: [CVPixelBuffer] = []
    nonisolated(unsafe) private var _lastPoppedPixelBuffer: CVPixelBuffer? = nil
    nonisolated(unsafe) private var _latestTimestamp: Double = 0.0
    
    nonisolated func setLatestPixelBuffer(_ pb: CVPixelBuffer, timestamp: Double) {
        _pixelBufferLock.lock()
        _presentationQueue.append(pb)
        _latestTimestamp = timestamp
        // Keep latency low by dropping older frames if the queue grows too large (e.g. > 3 frames)
        if _presentationQueue.count > 3 {
            _presentationQueue.removeFirst(_presentationQueue.count - 3)
        }
        _pixelBufferLock.unlock()
    }
    
    nonisolated var latestPixelBuffer: CVPixelBuffer? {
        _pixelBufferLock.lock()
        defer { _pixelBufferLock.unlock() }
        if !_presentationQueue.isEmpty {
            _lastPoppedPixelBuffer = _presentationQueue.removeFirst()
        }
        return _lastPoppedPixelBuffer
    }
    
    nonisolated var latestTimestamp: Double {
        _pixelBufferLock.lock()
        defer { _pixelBufferLock.unlock() }
        return _latestTimestamp
    }

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

    // Gamepad state
    @Published var gamepadPinch: Bool = false

    private var cancellables = Set<AnyCancellable>()

    // MARK: - Init
    init() {
        if let savedIP = UserDefaults.standard.string(forKey: "macIPAddress") {
            self.macIPAddress = savedIP
            self.compositor.reconnect(to: savedIP)
        }
        
        // デコードされたフレームを AppModel の共有バッファに保存する
        // CompositorRenderer はこのバッファを直接参照するため、
        // インスタンスが複数存在しても必ず最新フレームが使われる
        self.compositor.onFrameDecoded = { [weak self] pixelBuffer, timestamp in
            guard let self = self else { return }
            self.setLatestPixelBuffer(pixelBuffer, timestamp: timestamp)
            DispatchQueue.main.async {
                self.framesReceived += 1
            }
        }
        
        Task { @MainActor in
            for await event in self.fingerCollisionDetector.$keyPressEvent.values {
                guard let event else { continue }
                self.vxBrowserViewModel.injectKey(event.key)
                self.activeKeyboard?.animateKeyPress(letter: event.key)
            }
        }
        
        Task {
            await self.processGamepadInput()
        }
        
        NotificationCenter.default.addObserver(forName: .GCControllerDidConnect, object: nil, queue: .main) { [weak self] notification in
            guard let controller = notification.object as? GCController else { return }
            self?.setupGamepad(controller)
        }
        
        // Handle controllers that are already connected before the app started
        for controller in GCController.controllers() {
            setupGamepad(controller)
        }
    }
    
    private func setupGamepad(_ controller: GCController) {
        print("[GameController] Connected/Setup: \(controller.vendorName ?? "Unknown")")
        
        // Map any face button or trigger to gamepadPinch
        controller.extendedGamepad?.buttonA.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        controller.extendedGamepad?.buttonB.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        controller.extendedGamepad?.buttonX.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        controller.extendedGamepad?.buttonY.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        controller.extendedGamepad?.rightTrigger.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        controller.extendedGamepad?.leftTrigger.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        
        controller.microGamepad?.buttonA.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
        controller.microGamepad?.buttonX.valueChangedHandler = { _, _, pressed in self.gamepadPinch = pressed }
    }

    func startTracking() async {
        do {
            var providers: [DataProvider] = []
            if WorldTrackingProvider.isSupported { providers.append(worldTrackingProvider) }
            if PlaneDetectionProvider.isSupported { providers.append(planeDetectionProvider) }
            if HandTrackingProvider.isSupported { providers.append(handTrackingProvider) }
            
            if !providers.isEmpty {
                // Request authorization before running the session
                _ = try await arkitSession.requestAuthorization(for: [.handTracking])
                
                try await arkitSession.run(providers)
                print("ARKit tracking started successfully")
            } else {
                print("No ARKit providers supported on this device/simulator")
            }

            // Start object recognition and tracking
            await objectRecognitionManager.startObjectRecognition(session: arkitSession)
            await objectTrackingManager.startTracking(session: arkitSession)

            // 指先衝突判定ループを並列起動
            Task {
                await processHandTrackingForCollision()
            }
            
            // VRストリーミング用のトラッキングループ
            Task { await processTrackingForVR() }
        } catch {
            print("Failed to start ARKit session: \(error)")
        }
    }

    private func processHandTrackingForCollision() async {
        for await update in handTrackingProvider.anchorUpdates {
            fingerCollisionDetector.processHandAnchor(update.anchor)
        }
    }

    private func processGamepadInput() async {
        while true {
            do {
                try await Task.sleep(nanoseconds: 11_111_111) // ~90fps
                
                guard let controller = GCController.controllers().first,
                      let pad = controller.extendedGamepad else {
                    continue
                }
                
                var rightButtons: UInt32 = 0
                if pad.buttonA.isPressed { rightButtons |= (1 << 0) }
                if pad.buttonB.isPressed { rightButtons |= (1 << 1) }
                if pad.rightTrigger.isPressed { rightButtons |= (1 << 2) }
                if pad.rightThumbstickButton?.isPressed == true { rightButtons |= (1 << 3) }
                if pad.buttonOptions?.isPressed == true || pad.buttonMenu.isPressed { rightButtons |= (1 << 4) }
                if pad.rightShoulder.isPressed { rightButtons |= (1 << 5) }
                
                var leftButtons: UInt32 = 0
                if pad.dpad.up.isPressed { leftButtons |= (1 << 0) }
                if pad.dpad.down.isPressed { leftButtons |= (1 << 1) }
                if pad.dpad.left.isPressed { leftButtons |= (1 << 2) }
                if pad.dpad.right.isPressed { leftButtons |= (1 << 3) }
                if pad.leftTrigger.isPressed { leftButtons |= (1 << 4) }
                if pad.leftThumbstickButton?.isPressed == true { leftButtons |= (1 << 5) }
                if pad.buttonHome?.isPressed == true { leftButtons |= (1 << 6) }
                if pad.leftShoulder.isPressed { leftButtons |= (1 << 7) }
                
                // Map X and Y to extra bits if needed
                if pad.buttonX.isPressed { leftButtons |= (1 << 8) }
                if pad.buttonY.isPressed { leftButtons |= (1 << 9) }
                
                let rsX = pad.rightThumbstick.xAxis.value
                let rsY = pad.rightThumbstick.yAxis.value
                let lsX = pad.leftThumbstick.xAxis.value
                let lsY = pad.leftThumbstick.yAxis.value
                
                self.compositor.sendJoycon(
                    rightButtons: rightButtons,
                    leftButtons: leftButtons,
                    rightStickX: rsX, rightStickY: rsY,
                    leftStickX: lsX, leftStickY: lsY,
                    rightVelX: 0, rightVelY: 0, rightVelZ: 0,
                    leftVelX: 0, leftVelY: 0, leftVelZ: 0
                )
            } catch {
                break
            }
        }
    }


    
    private func processTrackingForVR() async {
        while !Task.isCancelled {
            try? await Task.sleep(nanoseconds: 11_111_111) // ~90fps
            
            // Predict ~45ms into the future to account for network, render, encode, and decode latency.
            // This allows the PC to render the frame for where the head WILL be, eliminating black pull-in on fast head turns.
            let targetTs = CACurrentMediaTime() + 0.045
            let deviceAnchor = worldTrackingProvider.state == .running ? worldTrackingProvider.queryDeviceAnchor(atTimestamp: targetTs) : nil
            let headT = deviceAnchor?.originFromAnchorTransform ?? matrix_identity_float4x4
            // If hands are not tracked, put them far below the player (Y = -2.0) so they don't block the camera!
            var hiddenT = matrix_identity_float4x4
            hiddenT.columns.3.y = -2.0
            
            var leftT = hiddenT
            var rightT = hiddenT
            
            let hands = handTrackingProvider.state == .running ? handTrackingProvider.latestAnchors : (leftHand: nil, rightHand: nil)
            
            func getPalmTransform(hand: HandAnchor?) -> simd_float4x4? {
                guard let hand = hand, hand.isTracked else { return nil }
                
                if let skeleton = hand.handSkeleton {
                    let knuckle = skeleton.joint(.middleFingerKnuckle)
                    if knuckle.isTracked {
                        return matrix_multiply(hand.originFromAnchorTransform, knuckle.anchorFromJointTransform)
                    }
                }
                
                // Fallback to wrist if knuckle isn't tracked (e.g. holding Joy-Con)
                return hand.originFromAnchorTransform
            }
            
            if let palmT = getPalmTransform(hand: hands.leftHand) {
                leftT = palmT
            }
            if let palmT = getPalmTransform(hand: hands.rightHand) {
                rightT = palmT
            }
            
            func getHandInputs(hand: HandAnchor?, isRight: Bool) -> (buttons: UInt32, stickX: Float, stickY: Float) {
                guard let hand = hand, hand.isTracked, let skeleton = hand.handSkeleton else {
                    return (0, 0, 0)
                }
                
                var buttons: UInt32 = 0
                
                let thumb = skeleton.joint(.thumbTip)
                let index = skeleton.joint(.indexFingerTip)
                let middle = skeleton.joint(.middleFingerTip)
                let ring = skeleton.joint(.ringFingerTip)
                let pinky = skeleton.joint(.littleFingerTip)
                let wrist = skeleton.joint(.wrist)
                
                guard thumb.isTracked && index.isTracked && middle.isTracked && ring.isTracked && pinky.isTracked && wrist.isTracked else {
                    return (0, 0, 0)
                }
                
                let thumbPos = thumb.anchorFromJointTransform.columns.3
                let indexPos = index.anchorFromJointTransform.columns.3
                let middlePos = middle.anchorFromJointTransform.columns.3
                let ringPos = ring.anchorFromJointTransform.columns.3
                let pinkyPos = pinky.anchorFromJointTransform.columns.3
                let wristPos = wrist.anchorFromJointTransform.columns.3
                
                func dist(_ a: simd_float4, _ b: simd_float4) -> Float {
                    return simd_distance(simd_make_float3(a.x, a.y, a.z), simd_make_float3(b.x, b.y, b.z))
                }
                
                // 1. Trigger: Thumb to Index
                if dist(thumbPos, indexPos) < 0.025 {
                    buttons |= isRight ? (1 << 2) : (1 << 4) // ZR / ZL
                }
                
                // 2. Grip: Middle, Ring, Pinky curled (close to wrist)
                if dist(middlePos, wristPos) < 0.08 && dist(ringPos, wristPos) < 0.08 {
                    buttons |= isRight ? (1 << 5) : (1 << 7) // R / L
                }
                
                // 3. A Button / D-Pad Right: Thumb to Ring
                if dist(thumbPos, ringPos) < 0.025 {
                    buttons |= (1 << 0)
                }
                
                // 4. B Button / D-Pad Down: Thumb to Pinky
                if dist(thumbPos, pinkyPos) < 0.025 {
                    buttons |= (1 << 1)
                }
                
                // 5. Joystick Mode: Thumb to Middle
                var stickX: Float = 0.0
                var stickY: Float = 0.0
                if dist(thumbPos, middlePos) < 0.03 {
                    let matrix = hand.originFromAnchorTransform
                    let upVector = matrix.columns.1
                    
                    stickY = Float(-upVector.z * 2.0)
                    stickX = Float(upVector.x * 2.0)
                    
                    stickX = max(-1.0, min(1.0, stickX))
                    stickY = max(-1.0, min(1.0, stickY))
                }
                
                return (buttons, stickX, stickY)
            }
            
            let leftInputs = getHandInputs(hand: hands.leftHand, isRight: false)
            let rightInputs = getHandInputs(hand: hands.rightHand, isRight: true)
            
            let leftGrip = (leftInputs.buttons & (1 << 7)) != 0
            let rightGrip = (rightInputs.buttons & (1 << 5)) != 0
            
            compositor.sendPose(
                head: headT,
                leftHand: leftT,
                rightHand: rightT,
                leftPinch: leftGrip,
                rightPinch: rightGrip
            )
            
            compositor.sendJoycon(
                rightButtons: rightInputs.buttons,
                leftButtons: leftInputs.buttons,
                rightStickX: rightInputs.stickX,
                rightStickY: rightInputs.stickY,
                leftStickX: leftInputs.stickX,
                leftStickY: leftInputs.stickY,
                rightVelX: 0, rightVelY: 0, rightVelZ: 0,
                leftVelX: 0, leftVelY: 0, leftVelZ: 0
            )
            
            DispatchQueue.main.async { [weak self] in
                self?.isSendingTracking = true
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
