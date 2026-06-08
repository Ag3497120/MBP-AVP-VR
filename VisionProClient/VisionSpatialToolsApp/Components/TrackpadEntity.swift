import RealityKit
import SwiftUI

class TrackpadEntity: Entity {
    private var touchPosition: SIMD2<Float> = [0, 0]
    private var isTouching = false
    private var lastTouchTime: Date?

    // Trackpad dimensions
    private let width: Float = 0.15
    private let height: Float = 0.10
    private let depth: Float = 0.01

    // Cursor tracking
    private var cursorEntity: Entity?
    private var clickIndicator: ModelEntity?

    // Multi-touch gesture tracking
    private var touchPoints: [SIMD3<Float>] = []
    private var scrollVelocity: SIMD2<Float> = [0, 0]

    enum GestureState {
        case none
        case singleTouch
        case doubleTouch
        case threeFingerSwipe
        case pinchZoom
    }

    private var currentGesture: GestureState = .none

    required init() {
        super.init()
        setupTrackpad()
    }

    private func setupTrackpad() {
        // Create trackpad base (glass-like material)
        let baseMesh = MeshResource.generateBox(
            width: width,
            height: depth,
            depth: height,
            cornerRadius: 0.008
        )

        var baseMaterial = PhysicallyBasedMaterial()
        baseMaterial.baseColor = PhysicallyBasedMaterial.BaseColor(
            tint: .systemGray6.withAlphaComponent(0.95)
        )
        baseMaterial.metallic = 0.8
        baseMaterial.roughness = 0.2

        let baseModel = ModelEntity(mesh: baseMesh, materials: [baseMaterial])
        baseModel.position = [0, 0, 0]
        addChild(baseModel)

        // Create touch surface (slightly elevated)
        let surfaceMesh = MeshResource.generatePlane(
            width: width - 0.01,
            height: height - 0.01,
            cornerRadius: 0.006
        )

        var surfaceMaterial = PhysicallyBasedMaterial()
        surfaceMaterial.baseColor = PhysicallyBasedMaterial.BaseColor(
            tint: .white.withAlphaComponent(0.9)
        )
        surfaceMaterial.roughness = 0.1

        let surfaceModel = ModelEntity(mesh: surfaceMesh, materials: [surfaceMaterial])
        surfaceModel.position = [0, depth / 2 + 0.001, 0]
        surfaceModel.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])
        surfaceModel.name = "TouchSurface"
        addChild(surfaceModel)

        // Create cursor indicator
        createCursor()

        // Create click feedback indicator
        createClickIndicator()

        // Add gesture zone indicators (subtle guides)
        createGestureZones()

        // Enable interaction
        components.set(InputTargetComponent(allowedInputTypes: [.indirect, .direct]))
        components.set(CollisionComponent(shapes: [.generateBox(
            width: width,
            height: depth,
            depth: height
        )]))

        // Add haptic feedback component
        components.set(HoverEffectComponent())
    }

    private func createCursor() {
        cursorEntity = Entity()

        // Cursor circle
        let cursorMesh = MeshResource.generateSphere(radius: 0.003)
        var cursorMaterial = UnlitMaterial(color: .systemBlue.withAlphaComponent(0.8))
        let cursorModel = ModelEntity(mesh: cursorMesh, materials: [cursorMaterial])

        cursorEntity?.addChild(cursorModel)
        cursorEntity?.position = [0, depth / 2 + 0.002, 0]
        cursorEntity?.isEnabled = false // Hidden until touch

        addChild(cursorEntity!)
    }

    private func createClickIndicator() {
        let indicatorMesh = MeshResource.generateSphere(radius: 0.005)
        var indicatorMaterial = UnlitMaterial(color: .systemGreen.withAlphaComponent(0.6))
        clickIndicator = ModelEntity(mesh: indicatorMesh, materials: [indicatorMaterial])
        clickIndicator?.position = [0, depth / 2 + 0.003, 0]
        clickIndicator?.isEnabled = false
        addChild(clickIndicator!)
    }

    private func createGestureZones() {
        // Left and right edge indicators for swipe gestures
        let edgeWidth: Float = 0.008

        // Left edge
        let leftEdge = createEdgeIndicator(
            position: [-(width / 2 - edgeWidth / 2), depth / 2 + 0.0005, 0],
            size: [edgeWidth, height - 0.02]
        )
        addChild(leftEdge)

        // Right edge
        let rightEdge = createEdgeIndicator(
            position: [(width / 2 - edgeWidth / 2), depth / 2 + 0.0005, 0],
            size: [edgeWidth, height - 0.02]
        )
        addChild(rightEdge)

        // Bottom edge for three-finger gestures
        let bottomEdge = createEdgeIndicator(
            position: [0, depth / 2 + 0.0005, -(height / 2 - edgeWidth / 2)],
            size: [width - 0.02, edgeWidth]
        )
        addChild(bottomEdge)
    }

    private func createEdgeIndicator(position: SIMD3<Float>, size: [Float]) -> Entity {
        let edge = Entity()
        let mesh = MeshResource.generatePlane(width: size[0], height: size[1])
        var material = UnlitMaterial(color: .systemBlue.withAlphaComponent(0.1))
        let model = ModelEntity(mesh: mesh, materials: [material])
        model.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])
        edge.addChild(model)
        edge.position = position
        return edge
    }

    // MARK: - Touch Handling

    func handleTouch(at localPosition: SIMD3<Float>) {
        // Convert 3D position to 2D trackpad coordinates (-1 to 1)
        let x = (localPosition.x / (width / 2)).clamped(to: -1...1)
        let z = (localPosition.z / (height / 2)).clamped(to: -1...1)

        touchPosition = [x, z]
        isTouching = true
        lastTouchTime = Date()

        // Update cursor position
        updateCursor(x: x, z: z)

        // Detect gesture based on touch
        detectGesture(at: [x, z])
    }

    func handleTouchMove(at localPosition: SIMD3<Float>) {
        guard isTouching else { return }

        let x = (localPosition.x / (width / 2)).clamped(to: -1...1)
        let z = (localPosition.z / (height / 2)).clamped(to: -1...1)

        let deltaX = x - touchPosition.x
        let deltaZ = z - touchPosition.y

        touchPosition = [x, z]

        // Update cursor
        updateCursor(x: x, z: z)

        // Handle scroll or swipe
        handleScroll(delta: [deltaX, deltaZ])
    }

    func handleTouchEnd() {
        isTouching = false
        cursorEntity?.isEnabled = false

        // Check for tap/click
        if let lastTime = lastTouchTime,
           Date().timeIntervalSince(lastTime) < 0.2 {
            performClick()
        }

        currentGesture = .none
        touchPoints.removeAll()
    }

    private func updateCursor(x: Float, z: Float) {
        cursorEntity?.isEnabled = true
        let worldX = x * (width / 2) * 0.9 // Scale down slightly
        let worldZ = z * (height / 2) * 0.9
        cursorEntity?.position = [worldX, depth / 2 + 0.002, worldZ]
    }

    private func detectGesture(at position: SIMD2<Float>) {
        // Edge detection for swipe gestures
        if abs(position.x) > 0.8 {
            currentGesture = .singleTouch
            print("Edge swipe detected")
        } else if abs(position.y) < -0.7 {
            currentGesture = .threeFingerSwipe
            print("Three-finger gesture area")
        } else {
            currentGesture = .singleTouch
        }
    }

    private func handleScroll(delta: SIMD2<Float>) {
        scrollVelocity = delta * 10.0 // Scale for sensitivity

        switch currentGesture {
        case .singleTouch:
            // Single finger drag - cursor movement
            print("Cursor move: \(scrollVelocity)")

        case .doubleTouch:
            // Two finger scroll
            print("Scroll: \(scrollVelocity)")

        case .threeFingerSwipe:
            // Three finger swipe - app switching
            print("Three-finger swipe: \(scrollVelocity)")

        default:
            break
        }
    }

    private func performClick() {
        // Visual feedback
        clickIndicator?.isEnabled = true
        clickIndicator?.position = cursorEntity?.position ?? [0, depth / 2 + 0.003, 0]

        // Animate click indicator
        Task {
            try? await Task.sleep(for: .milliseconds(150))
            await MainActor.run {
                clickIndicator?.isEnabled = false
            }
        }

        print("Click at position: \(touchPosition)")
    }

    // MARK: - Multi-touch Support

    func handleMultiTouch(points: [SIMD3<Float>]) {
        touchPoints = points

        if points.count == 2 {
            currentGesture = .doubleTouch
            handleTwoFingerGesture(points)
        } else if points.count >= 3 {
            currentGesture = .threeFingerSwipe
            handleThreeFingerGesture(points)
        }
    }

    private func handleTwoFingerGesture(_ points: [SIMD3<Float>]) {
        // Two-finger scroll or pinch/zoom
        let distance = distance(points[0], points[1])

        // Detect pinch vs scroll based on distance change
        print("Two-finger gesture, distance: \(distance)")
    }

    private func handleThreeFingerGesture(_ points: [SIMD3<Float>]) {
        // Three-finger swipe for app switching or expose
        let avgX = points.map { $0.x }.reduce(0, +) / Float(points.count)
        let avgZ = points.map { $0.z }.reduce(0, +) / Float(points.count)

        print("Three-finger swipe at: [\(avgX), \(avgZ)]")
    }

    // MARK: - Haptic Feedback

    func triggerHaptic(intensity: Float = 1.0) {
        // Trigger haptic feedback for trackpad events
        // This would integrate with visionOS haptic system
        print("Haptic feedback: \(intensity)")
    }

    // MARK: - Public Properties

    var currentTouchPosition: SIMD2<Float> {
        return touchPosition
    }

    var isCurrentlyTouching: Bool {
        return isTouching
    }

    var currentScrollVelocity: SIMD2<Float> {
        return scrollVelocity
    }
}

extension Comparable {
    func clamped(to range: ClosedRange<Self>) -> Self {
        return min(max(self, range.lowerBound), range.upperBound)
    }
}
