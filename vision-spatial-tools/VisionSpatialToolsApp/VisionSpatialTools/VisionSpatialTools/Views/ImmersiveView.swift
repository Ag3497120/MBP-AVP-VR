import SwiftUI
import RealityKit
import RealityKitContent
import WebKit
import ARKit

struct ImmersiveView: View {
    @EnvironmentObject var appModel: AppModel
    @Environment(\.dismissWindow) var dismissWindow
    @Environment(\.openWindow) var openWindow
    @Environment(\.scenePhase) var scenePhase
    @State private var rootEntity = Entity()
    @State private var scannedObjects: [ScannedObject] = []
    @State private var currentScanningEntity: Entity?
    /// ブラウザの Attachment IDリスト
    @State private var browserAttachmentIDs: [String] = []
    /// アクティブブラウザEntityリスト
    @State private var spatialBrowserEntities: [SpatialBrowserEntity] = []

    var body: some View {
        RealityView { content, attachments in
            content.add(rootEntity)

            // Add ambient lighting
            let ambientLight = DirectionalLight()
            ambientLight.light.intensity = 1000
            rootEntity.addChild(ambientLight)

            // 空中タップを受け取るための不可視な衝突スフィア（半径1.5m）
            let airTapCatcher = Entity()
            airTapCatcher.components.set(CollisionComponent(shapes: [.generateSphere(radius: 1.5)]))
            airTapCatcher.components.set(InputTargetComponent())
            airTapCatcher.name = "AirTapCatcher"
            rootEntity.addChild(airTapCatcher)
        } update: { content, attachments in
            // Update reality view when tools change
            updateMagneticAttachments()

            // ブラウザの SwiftUI Attachment を Entity にバインド
            for browser in spatialBrowserEntities {
                if let attachView = attachments.entity(for: browser.attachmentID) {
                    if attachView.parent == nil {
                        // パネル内部に配置 (Z方向に少し浮かせる)
                        attachView.position = [0, 0, 0.01]
                        browser.addChild(attachView)
                    }
                }
            }
        } attachments: {
            ForEach(browserAttachmentIDs, id: \.self) { attachID in
                Attachment(id: attachID) {
                    VxSpatialBrowserView()
                }
            }
        }
        .onChange(of: scenePhase) { oldPhase, newPhase in
            if newPhase == .active {
                openWindow(id: "mainWindow")
            }
        }
        .upperLimbVisibility(.hidden)
        .gesture(
            SpatialTapGesture()
                .targetedToAnyEntity()
                .onEnded { value in
                    handleSpatialTap(value)
                }
        )
        .gesture(
            DragGesture()
                .targetedToAnyEntity()
                .onChanged { value in
                    handleDrag(value)
                }
        )
        .task {
            await appModel.startTracking()
            await processWorldTracking()
            await processMagneticAttachments()
        }
        .onAppear {
            // 自動的にVRスクリーンをワールド座標の中心に配置する
            let vrScreen = VRScreenEntity(appModel: appModel)
            rootEntity.addChild(vrScreen)
            print("[ImmersiveView] Auto-placed VRScreenEntity in world coordinates!")
        }
    }

    private func handleSpatialTap(_ value: EntityTargetValue<SpatialTapGesture.Value>) {
        let location = value.convert(value.location3D, from: .local, to: .scene)
        let tappedEntity = value.entity

        // If tapped the background, dismiss the main menu window
        if tappedEntity.name == "AirTapCatcher" {
            dismissWindow(id: "mainWindow")
            return // これがないと空気をタップするたびにツールが無限に生成されてしまう
        }

        // --- ブラウザのナビゲーションボタン判定 ---
        for browser in spatialBrowserEntities {
            if let btnName = browser.navButtonName(for: tappedEntity) {
                browser.handleNavButtonTap(btnName)
                return
            }
        }

        // VRゲーム操作（ピンチやタップ）がツール配置として誤爆するのを完全に防ぐため、
        // 没入モードでのツール新規配置機能はここでリターンして無効化します。
        return
        
        /*
        // If scanning mode, scan object at location
        if appModel.isScanningMode {
            Task {
                await scanObjectAtLocation(location)
            }
            return
        }

        // Check for nearby scanned objects if magnetic snap enabled
        if appModel.magneticSnapEnabled && !scannedObjects.isEmpty {
            if let nearbyObject = findNearbyObject(to: location) {
                attachToolToObject(
                    toolType: appModel.selectedToolType,
                    object: nearbyObject,
                    at: location
                )
                return
            }
        }

        // Regular tool placement
        let anchor = AnchorEntity(world: location)
        rootEntity.addChild(anchor)

        createTool(type: appModel.selectedToolType, at: anchor, location: location)
        appModel.attachTool(type: appModel.selectedToolType, at: anchor)
        */
    }

    private func handleDrag(_ value: EntityTargetValue<DragGesture.Value>) {
        guard let entity = value.entity as? ModelEntity else { return }

        let location = value.convert(value.location3D, from: .local, to: .scene)

        // Check for magnetic attraction during drag
        if appModel.magneticSnapEnabled {
            checkMagneticAttraction(for: entity, at: location)
        }

        // Update entity position
        entity.setPosition(location, relativeTo: nil)
    }

    private func scanObjectAtLocation(_ location: SIMD3<Float>) async {
        print("Scanning object at: \(location)")

        // Create scanning indicator
        let scanIndicator = createScanIndicator(at: location)
        rootEntity.addChild(scanIndicator)
        currentScanningEntity = scanIndicator

        // Perform scan
        if let scannedObject = await appModel.objectRecognitionManager.scanObject(
            at: location,
            in: rootEntity
        ) {
            scannedObjects.append(scannedObject)

            // Start tracking the scanned object
            let trackingId = appModel.objectTrackingManager.trackObject(scannedObject)
            print("Tracking object: \(trackingId)")

            // Visualize scanned object
            visualizeScannedObject(scannedObject)
        }

        // Remove scanning indicator
        scanIndicator.removeFromParent()
        currentScanningEntity = nil
    }

    private func createScanIndicator(at position: SIMD3<Float>) -> Entity {
        let indicator = Entity()
        let mesh = MeshResource.generateSphere(radius: 0.05)
        var material = UnlitMaterial(color: .systemBlue.withAlphaComponent(0.5))
        let model = ModelEntity(mesh: mesh, materials: [material])
        indicator.addChild(model)
        indicator.position = position

        // Animate pulsing
        var transform = model.transform
        transform.scale = [0.5, 0.5, 0.5]

        return indicator
    }

    private func visualizeScannedObject(_ object: ScannedObject) {
        // Create bounding box visualization
        let boundingBox = Entity()
        let mesh = MeshResource.generateBox(
            width: object.boundingBox.x,
            height: object.boundingBox.y,
            depth: object.boundingBox.z
        )

        var material = UnlitMaterial(color: .systemGreen.withAlphaComponent(0.3))
        let model = ModelEntity(mesh: mesh, materials: [material])
        boundingBox.addChild(model)
        boundingBox.position = object.position
        boundingBox.orientation = object.orientation
        boundingBox.name = "ScannedObject_\(object.id)"

        rootEntity.addChild(boundingBox)
    }

    private func findNearbyObject(to position: SIMD3<Float>) -> ScannedObject? {
        let snapDistance: Float = 0.15

        for object in scannedObjects {
            let distance = distance(position, object.position)
            if distance < snapDistance {
                return object
            }
        }

        return nil
    }

    private func attachToolToObject(
        toolType: ToolType,
        object: ScannedObject,
        at position: SIMD3<Float>
    ) {
        print("Attaching \(toolType) to object magnetically")

        let anchor = AnchorEntity(world: position)
        rootEntity.addChild(anchor)

        let toolEntity = createToolEntity(type: toolType, at: anchor)

        // Attach to object with magnetic system
        let attachment = appModel.magneticAttachmentManager.attachToolToObject(
            tool: toolEntity,
            toolType: toolType,
            scannedObject: object
        )

        // Make tool follow object
        appModel.adaptiveFollower.makeToolFollowObject(
            tool: toolEntity,
            toolType: toolType,
            targetObjectId: object.id
        )

        var attachedTool = AttachedTool(type: toolType, anchor: anchor)
        attachedTool.attachedToObject = object.id
        attachedTool.isFollowing = true
        appModel.attachedTools.append(attachedTool)
    }

    private func checkMagneticAttraction(for entity: Entity, at position: SIMD3<Float>) {
        guard let nearbyObject = findNearbyObject(to: position) else { return }

        // Apply magnetic attraction force
        appModel.magneticAttachmentManager.attractToolToObject(
            tool: entity,
            targetObject: nearbyObject,
            strength: 1.0
        )
    }

    private func createTool(type: ToolType, at anchor: AnchorEntity, location: SIMD3<Float>) {
        let _ = createToolEntity(type: type, at: anchor)
    }

    private func createToolEntity(type: ToolType, at anchor: AnchorEntity) -> Entity {
        let toolEntity: Entity

        switch type {
        case .keyboard:
            toolEntity = createEnhancedKeyboard(at: anchor)
        case .trackpad:
            toolEntity = createTrackpad(at: anchor)
        case .memo:
            toolEntity = createMemo(at: anchor)
        case .drawing:
            toolEntity = createDrawingCanvas(at: anchor)
        case .calculator:
            toolEntity = createCalculator(at: anchor)
        case .browser:
            toolEntity = createSpatialBrowser(at: anchor)
        case .vrScreen:
            toolEntity = createVRScreen(at: anchor)
        }

        return toolEntity
    }

    private func createEnhancedKeyboard(at anchor: AnchorEntity) -> Entity {
        let keyboard = EnhancedKeyboardEntity()
        // 机の上面に平置 (少し浮かせてパッシブハプティクスの面に就かせる)
        keyboard.position = [0, 0.004, 0]
        anchor.addChild(keyboard)

        // FingerCollisionDetector に登録
        appModel.fingerCollisionDetector.registerKeyboard(
            keyboard,
            keys: keyboard.keyDescriptors
        )
        appModel.activeKeyboard = keyboard

        print("[ImmersiveView] EnhancedKeyboard placed and registered for collision detection")
        return keyboard
    }

    private func createSpatialBrowser(at anchor: AnchorEntity) -> Entity {
        let browser = SpatialBrowserEntity()
        // 壁に貴り付けるイメージの位置
        browser.position = [0, 0.2, 0]
        anchor.addChild(browser)

        // Attachment ID を登録 → body 再計算により SwiftUIビューが確保される
        spatialBrowserEntities.append(browser)
        browserAttachmentIDs.append(browser.attachmentID)

        print("[ImmersiveView] SpatialBrowser placed: \(browser.attachmentID)")
        return browser
    }

    private func createVRScreen(at anchor: AnchorEntity) -> Entity {
        let vrScreen = VRScreenEntity(appModel: appModel)
        anchor.addChild(vrScreen)
        print("[ImmersiveView] VRScreenEntity placed!")
        return vrScreen
    }

    private func createKeyboard(at anchor: AnchorEntity) -> Entity {
        let keyboard = KeyboardEntity()
        keyboard.position = [0, 0.1, 0]
        anchor.addChild(keyboard)
        return keyboard
    }

    private func createTrackpad(at anchor: AnchorEntity) -> Entity {
        let trackpad = TrackpadEntity()
        trackpad.position = [0, 0.1, 0]
        anchor.addChild(trackpad)
        return trackpad
    }

    private func createMemo(at anchor: AnchorEntity) -> Entity {
        let memo = MemoEntity()
        memo.position = [0, 0.1, 0]
        anchor.addChild(memo)
        return memo
    }

    private func createDrawingCanvas(at anchor: AnchorEntity) -> Entity {
        let canvas = DrawingCanvasEntity()
        canvas.position = [0, 0.1, 0]
        anchor.addChild(canvas)
        return canvas
    }

    private func createCalculator(at anchor: AnchorEntity) -> Entity {
        let calculator = CalculatorEntity()
        calculator.position = [0, 0.1, 0]
        anchor.addChild(calculator)
        return calculator
    }

    private func processWorldTracking() async {
        for await update in appModel.worldTrackingProvider.anchorUpdates {
            // Update scanned objects based on world tracking
            // This would update object positions in real-time
        }
    }

    private func processMagneticAttachments() async {
        while appModel.isImmersiveSpaceActive {
            // Update all magnetic attachments
            appModel.magneticAttachmentManager.updateAttachments(
                scannedObjects: scannedObjects
            )

            try? await Task.sleep(for: .milliseconds(16)) // 60 FPS
        }
    }

    private func updateMagneticAttachments() {
        // Called during RealityView update
        for object in scannedObjects {
            // Update object tracking
            if let trackingInfo = appModel.objectTrackingManager.getTrackingInfo(for: object.id) {
                // Update visualization
                if let visualEntity = rootEntity.findEntity(named: "ScannedObject_\(object.id)") {
                    visualEntity.position = trackingInfo.currentPosition
                    visualEntity.orientation = trackingInfo.currentOrientation
                }
            }
        }
    }
}
