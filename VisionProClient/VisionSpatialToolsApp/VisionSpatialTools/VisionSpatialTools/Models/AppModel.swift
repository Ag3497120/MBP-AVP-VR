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
    nonisolated(unsafe) private var _lastPoppedPixelBuffer: CVPixelBuffer? = nil
    nonisolated(unsafe) private var _presentationQueue: [(CVPixelBuffer, Double, simd_float4x4)] = []
    
    nonisolated(unsafe) private var _latestTimestamp: Double = 0.0
    nonisolated(unsafe) private var _latestTransform: simd_float4x4 = matrix_identity_float4x4
    
    nonisolated(unsafe) var latestLeftTangents: simd_float4 = simd_float4(0,0,0,0)
    nonisolated(unsafe) var latestRightTangents: simd_float4 = simd_float4(0,0,0,0)
    
    nonisolated func setLatestPixelBuffer(_ pb: CVPixelBuffer, timestamp: Double, transform: simd_float4x4) {
        _pixelBufferLock.lock()
        _presentationQueue.append((pb, timestamp, transform))
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
            let popped = _presentationQueue.removeFirst()
            _lastPoppedPixelBuffer = popped.0
            _latestTimestamp = popped.1 // Update ATW timestamp ONLY when the frame is presented
            _latestTransform = popped.2
        }
        return _lastPoppedPixelBuffer
    }
    
    nonisolated var latestTimestamp: Double {
        _pixelBufferLock.lock()
        defer { _pixelBufferLock.unlock() }
        return _latestTimestamp
    }
    
    nonisolated var latestTransform: simd_float4x4 {
        _pixelBufferLock.lock()
        defer { _pixelBufferLock.unlock() }
        return _latestTransform
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
        self.compositor.onFrameDecoded = { [weak self] pixelBuffer, timestamp, transform in
            guard let self = self else { return }
            self.setLatestPixelBuffer(pixelBuffer, timestamp: timestamp, transform: transform)
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


    

    // Joystick neutral anchors
    private var leftJoystickNeutralAnchor: simd_float4x4? = nil
    private var rightJoystickNeutralAnchor: simd_float4x4? = nil
    
    private func processTrackingForVR() async {

        while !Task.isCancelled {
            // Poll at 250Hz (4ms) to ensure we always grab the very latest anchor as soon as ARKit produces it!
            try? await Task.sleep(nanoseconds: 4_000_000)
            
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
            
            func getPalmTransform(hand: HandAnchor?, isRight: Bool) -> simd_float4x4? {
                guard let hand = hand, hand.isTracked else { return nil }
                
                var transform = hand.originFromAnchorTransform
                
                // Vision Pro HandAnchor orientation:
                // +Z points out of the back of the hand. +Y points up (towards thumb). +X points right.
                // OpenVR Controller (Valve Index) orientation:
                // -Z points forward (laser pointer). +Y points up (from button face). +X points right.
                
                // 腕の位置（肘から手首へのベクトル）を算出して手の向きをさらに安定させる
                if let skeleton = hand.handSkeleton {
                   let elbow = skeleton.joint(.forearmArm)
                   let forearmWrist = skeleton.joint(.forearmWrist)
                   
                   if elbow.isTracked && forearmWrist.isTracked {
                    
                    // 肘と手首の位置をワールド座標に変換
                    let elbowPos = matrix_multiply(transform, elbow.anchorFromJointTransform).columns.3
                    let wristPos = matrix_multiply(transform, forearmWrist.anchorFromJointTransform).columns.3
                    
                    // 腕の方向ベクトル（肘から手首へ）
                    let forearmDir = simd_normalize(simd_make_float3(wristPos.x - elbowPos.x, wristPos.y - elbowPos.y, wristPos.z - elbowPos.z))
                    
                    // 手の位置を少し腕側に寄せることで、手首の微細なブレを吸収して位置を固定する
                    let positionOffset = forearmDir * -0.05 // 手首から5cm腕側にコントローラーの原点を置く
                    transform.columns.3.x += positionOffset.x
                    transform.columns.3.y += positionOffset.y
                    transform.columns.3.z += positionOffset.z
                   }
                }
                
                // Adjust rotation to perfectly align the real hand with the VR game hand
                let angleX = Float.pi / 2.0 // Pitch
                let angleY = isRight ? -Float.pi / 2.0 : Float.pi / 2.0 // Yaw
                
                let rotX = simd_float4x4(
                    simd_float4(1, 0, 0, 0),
                    simd_float4(0, cos(angleX), sin(angleX), 0),
                    simd_float4(0, -sin(angleX), cos(angleX), 0),
                    simd_float4(0, 0, 0, 1)
                )
                let rotY = simd_float4x4(
                    simd_float4(cos(angleY), 0, -sin(angleY), 0),
                    simd_float4(0, 1, 0, 0),
                    simd_float4(sin(angleY), 0, cos(angleY), 0),
                    simd_float4(0, 0, 0, 1)
                )
                
                transform = matrix_multiply(transform, rotY)
                transform = matrix_multiply(transform, rotX)
                
                return transform
            }
            
            if let palmT = getPalmTransform(hand: hands.leftHand, isRight: false) {
                leftT = palmT
            }
            if let palmT = getPalmTransform(hand: hands.rightHand, isRight: true) {
                rightT = palmT
            }
            

            struct HandState {
                var trigger: UInt8 = 0
                var grip: UInt8 = 0
                var buttons: UInt32 = 0
                var stickX: Float = 0
                var stickY: Float = 0
            }

            func getHandInputs(hand: HandAnchor?, isRight: Bool) -> HandState {
                var state = HandState()
                guard let hand = hand, hand.isTracked, let skeleton = hand.handSkeleton else { return state }
                
                let thumb = skeleton.joint(.thumbTip)
                let index = skeleton.joint(.indexFingerTip)
                let middle = skeleton.joint(.middleFingerTip)
                let ring = skeleton.joint(.ringFingerTip)
                let pinky = skeleton.joint(.littleFingerTip)
                let wrist = skeleton.joint(.wrist)
                let palm = skeleton.joint(.middleFingerKnuckle) // use as center of palm
                
                guard thumb.isTracked && index.isTracked && middle.isTracked && ring.isTracked && pinky.isTracked && wrist.isTracked else { return state }
                
                func dist(_ a: simd_float4, _ b: simd_float4) -> Float {
                    return simd_distance(simd_make_float3(a.x, a.y, a.z), simd_make_float3(b.x, b.y, b.z))
                }
                
                let thumbPos = thumb.anchorFromJointTransform.columns.3
                let indexPos = index.anchorFromJointTransform.columns.3
                let middlePos = middle.anchorFromJointTransform.columns.3
                let ringPos = ring.anchorFromJointTransform.columns.3
                let pinkyPos = pinky.anchorFromJointTransform.columns.3
                let palmPos = palm.anchorFromJointTransform.columns.3
                
                let pinchThreshold: Float = 0.025 // 2.5cm
                
                // Trigger (Index + Thumb)
                let indexPinch = dist(thumbPos, indexPos)
                if indexPinch < pinchThreshold { state.trigger = 255 }
                
                // Grip (Middle, Ring, Pinky curled to palm)
                let gripDist = (dist(middlePos, palmPos) + dist(ringPos, palmPos) + dist(pinkyPos, palmPos)) / 3.0
                if gripDist < 0.06 { state.grip = 255 }
                
                // A Button (Thumb + Ring)
                if dist(thumbPos, ringPos) < pinchThreshold {
                    state.buttons |= isRight ? (1 << 0) : (1 << 1) // BTN_A or BTN_DPAD_DOWN
                }
                
                // B Button (Thumb + Pinky)
                if dist(thumbPos, pinkyPos) < pinchThreshold {
                    state.buttons |= isRight ? (1 << 1) : (1 << 3) // BTN_B or BTN_DPAD_RIGHT
                }
                
                // Virtual Joystick (Thumb + Middle)
                if dist(thumbPos, middlePos) < pinchThreshold {
                    let currentRot = hand.originFromAnchorTransform
                    if isRight {
                        if rightJoystickNeutralAnchor == nil { rightJoystickNeutralAnchor = currentRot }
                        if let neutral = rightJoystickNeutralAnchor {
                            // Extract relative pitch/roll
                            let relative = matrix_multiply(simd_inverse(neutral), currentRot)
                            state.stickX = max(-1.0, min(1.0, relative.columns.0.y * 3.0)) // Roll
                            state.stickY = max(-1.0, min(1.0, -relative.columns.2.y * 3.0)) // Pitch
                        }
                    } else {
                        if leftJoystickNeutralAnchor == nil { leftJoystickNeutralAnchor = currentRot }
                        if let neutral = leftJoystickNeutralAnchor {
                            let relative = matrix_multiply(simd_inverse(neutral), currentRot)
                            state.stickX = max(-1.0, min(1.0, relative.columns.0.y * 3.0)) // Roll
                            state.stickY = max(-1.0, min(1.0, -relative.columns.2.y * 3.0)) // Pitch
                        }
                    }
                } else {
                    // Reset anchor when released
                    if isRight { rightJoystickNeutralAnchor = nil }
                    else { leftJoystickNeutralAnchor = nil }
                }
                
                return state
            }
            
            let leftInputs = getHandInputs(hand: hands.leftHand, isRight: false)
            let rightInputs = getHandInputs(hand: hands.rightHand, isRight: true)
            
            compositor.sendPose(
                head: headT,
                leftHand: leftT,
                rightHand: rightT,
                leftPinch: leftInputs.grip,
                rightPinch: rightInputs.grip,
                leftTrigger: leftInputs.trigger,
                rightTrigger: rightInputs.trigger,
                rightButtons: rightInputs.buttons,
                leftButtons: leftInputs.buttons,
                rightStickX: rightInputs.stickX,
                rightStickY: rightInputs.stickY,
                leftStickX: leftInputs.stickX,
                leftStickY: leftInputs.stickY,
                leftTangents: self.latestLeftTangents,
                rightTangents: self.latestRightTangents
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
