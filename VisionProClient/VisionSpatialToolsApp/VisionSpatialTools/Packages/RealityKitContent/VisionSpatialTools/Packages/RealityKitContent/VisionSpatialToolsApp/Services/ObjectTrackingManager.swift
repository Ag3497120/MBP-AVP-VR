import Foundation
import RealityKit
import ARKit

@MainActor
class ObjectTrackingManager: ObservableObject {
    @Published var trackedObjects: [UUID: TrackedObjectInfo] = [:]
    @Published var isTracking = false

    private var arkitSession: ARKitSession?
    private let updateFrequency: TimeInterval = 1.0 / 60.0 // 60 Hz

    // Tracking settings
    private let positionSmoothingFactor: Float = 0.15
    private let rotationSmoothingFactor: Float = 0.1
    private let predictionTime: TimeInterval = 0.05 // 50ms ahead

    func startTracking(session: ARKitSession) async {
        self.arkitSession = session
        isTracking = true

        print("Object tracking started")
        await continuousTracking()
    }

    private func continuousTracking() async {
        while isTracking {
            updateAllTrackedObjects()

            try? await Task.sleep(for: .milliseconds(Int(updateFrequency * 1000)))
        }
    }

    func trackObject(_ scannedObject: ScannedObject) -> UUID {
        let trackingId = scannedObject.id

        let trackingInfo = TrackedObjectInfo(
            id: trackingId,
            currentPosition: scannedObject.position,
            currentOrientation: scannedObject.orientation,
            velocity: [0, 0, 0],
            angularVelocity: simd_quatf(angle: 0, axis: [0, 1, 0]),
            lastUpdateTime: Date(),
            confidence: 1.0,
            boundingBox: scannedObject.boundingBox
        )

        trackedObjects[trackingId] = trackingInfo

        print("Started tracking object: \(trackingId)")
        return trackingId
    }

    func updateTrackedObject(
        id: UUID,
        newPosition: SIMD3<Float>,
        newOrientation: simd_quatf
    ) {
        guard var info = trackedObjects[id] else { return }

        let now = Date()
        let deltaTime = Float(now.timeIntervalSince(info.lastUpdateTime))

        // Calculate velocity
        let velocity = (newPosition - info.currentPosition) / deltaTime

        // Calculate angular velocity (simplified)
        let deltaRotation = newOrientation * info.currentOrientation.inverse
        let angularVelocity = deltaRotation

        // Apply smoothing
        let smoothedPosition = mix(
            info.currentPosition,
            newPosition,
            t: positionSmoothingFactor
        )

        let smoothedOrientation = simd_slerp(
            info.currentOrientation,
            newOrientation,
            rotationSmoothingFactor
        )

        // Update tracking info
        info.currentPosition = smoothedPosition
        info.currentOrientation = smoothedOrientation
        info.velocity = velocity
        info.angularVelocity = angularVelocity
        info.lastUpdateTime = now

        trackedObjects[id] = info
    }

    func predictPosition(for id: UUID) -> SIMD3<Float>? {
        guard let info = trackedObjects[id] else { return nil }

        // Predict future position based on current velocity
        let predictedPosition = info.currentPosition + info.velocity * Float(predictionTime)
        return predictedPosition
    }

    func predictOrientation(for id: UUID) -> simd_quatf? {
        guard let info = trackedObjects[id] else { return nil }

        // Predict future orientation
        // Simplified: apply angular velocity
        let predictedOrientation = info.angularVelocity * info.currentOrientation
        return predictedOrientation
    }

    func getTrackingInfo(for id: UUID) -> TrackedObjectInfo? {
        return trackedObjects[id]
    }

    func stopTracking(id: UUID) {
        trackedObjects.removeValue(forKey: id)
        print("Stopped tracking object: \(id)")
    }

    func stopAllTracking() {
        isTracking = false
        trackedObjects.removeAll()
        print("All tracking stopped")
    }

    private func updateAllTrackedObjects() {
        // Update confidence based on time since last update
        let now = Date()

        for (id, var info) in trackedObjects {
            let timeSinceUpdate = now.timeIntervalSince(info.lastUpdateTime)

            // Decrease confidence if no updates
            if timeSinceUpdate > 1.0 {
                info.confidence = max(0, info.confidence - Float(timeSinceUpdate * 0.1))
                trackedObjects[id] = info
            }

            // Remove if confidence too low
            if info.confidence < 0.2 {
                stopTracking(id: id)
                print("Lost tracking for object: \(id)")
            }
        }
    }

    func isObjectVisible(_ id: UUID) -> Bool {
        guard let info = trackedObjects[id] else { return false }
        return info.confidence > 0.5
    }

    func getObjectVelocity(_ id: UUID) -> SIMD3<Float>? {
        return trackedObjects[id]?.velocity
    }
}

// MARK: - Tracked Object Info

struct TrackedObjectInfo {
    let id: UUID
    var currentPosition: SIMD3<Float>
    var currentOrientation: simd_quatf
    var velocity: SIMD3<Float>
    var angularVelocity: simd_quatf
    var lastUpdateTime: Date
    var confidence: Float
    var boundingBox: SIMD3<Float>

    // Additional tracking data
    var isFlipped: Bool = false
    var surfaceNormal: SIMD3<Float> = [0, 1, 0]

    mutating func updateFlipState(newOrientation: simd_quatf) {
        // Detect if object has been flipped
        let upVector = newOrientation.act([0, 1, 0])
        let dotProduct = dot(upVector, [0, 1, 0])

        // If dot product is negative, object is flipped
        isFlipped = dotProduct < 0
    }
}

// MARK: - Adaptive Tool Follower

@MainActor
class AdaptiveToolFollower: ObservableObject {
    private let trackingManager: ObjectTrackingManager
    private let magneticManager: MagneticAttachmentManager

    @Published var followingTools: [UUID: FollowingTool] = [:]

    init(
        trackingManager: ObjectTrackingManager,
        magneticManager: MagneticAttachmentManager
    ) {
        self.trackingManager = trackingManager
        self.magneticManager = magneticManager
    }

    func makeToolFollowObject(
        tool: Entity,
        toolType: ToolType,
        targetObjectId: UUID
    ) {
        let followingTool = FollowingTool(
            toolEntity: tool,
            toolType: toolType,
            targetObjectId: targetObjectId,
            followPosition: true,
            followOrientation: true,
            adaptiveSize: true,
            stickOnFlip: true
        )

        followingTools[tool.id] = followingTool

        print("Tool \(tool.id) now following object \(targetObjectId)")

        // Start update loop
        Task {
            await updateFollowingTool(toolId: tool.id)
        }
    }

    private func updateFollowingTool(toolId: UUID) async {
        while let followingTool = followingTools[toolId] {
            guard let trackingInfo = trackingManager.getTrackingInfo(
                for: followingTool.targetObjectId
            ) else {
                // Object lost, stop following
                stopFollowing(toolId: toolId)
                return
            }

            // Update tool position
            if followingTool.followPosition {
                updateToolPosition(followingTool, trackingInfo: trackingInfo)
            }

            // Update tool orientation
            if followingTool.followOrientation {
                updateToolOrientation(followingTool, trackingInfo: trackingInfo)
            }

            // Update tool size
            if followingTool.adaptiveSize {
                updateToolSize(followingTool, trackingInfo: trackingInfo)
            }

            // Handle flip detection
            if followingTool.stickOnFlip {
                handleFlip(followingTool, trackingInfo: trackingInfo)
            }

            try? await Task.sleep(for: .milliseconds(16)) // ~60 FPS
        }
    }

    private func updateToolPosition(_ tool: FollowingTool, trackingInfo: TrackedObjectInfo) {
        // Use predicted position for smoother tracking
        let predictedPosition = trackingManager.predictPosition(
            for: tool.targetObjectId
        ) ?? trackingInfo.currentPosition

        // Calculate offset based on tool type
        let offset = calculateToolOffset(
            toolType: tool.toolType,
            objectSize: trackingInfo.boundingBox,
            isFlipped: trackingInfo.isFlipped
        )

        let finalPosition = predictedPosition + offset
        tool.toolEntity.setPosition(finalPosition, relativeTo: nil)
    }

    private func updateToolOrientation(_ tool: FollowingTool, trackingInfo: TrackedObjectInfo) {
        let predictedOrientation = trackingManager.predictOrientation(
            for: tool.targetObjectId
        ) ?? trackingInfo.currentOrientation

        // Apply tool-specific orientation adjustment
        var finalOrientation = predictedOrientation

        if trackingInfo.isFlipped && tool.stickOnFlip {
            // If object is flipped, keep tool readable
            finalOrientation = predictedOrientation * simd_quatf(angle: .pi, axis: [0, 0, 1])
        }

        tool.toolEntity.setOrientation(finalOrientation, relativeTo: nil)
    }

    private func updateToolSize(_ tool: FollowingTool, trackingInfo: TrackedObjectInfo) {
        let objectWidth = trackingInfo.boundingBox.x
        let objectDepth = trackingInfo.boundingBox.z

        var scale: Float = 1.0

        switch tool.toolType {
        case .keyboard:
            // Keyboard scales to 80% of object width
            scale = (objectWidth / 0.4) * 0.8

        case .trackpad:
            // Trackpad scales to 40% of object width
            scale = (objectWidth / 0.15) * 0.4

        case .memo:
            // Memo scales to object depth
            scale = (objectDepth / 0.3) * 0.9

        case .drawing:
            // Drawing canvas maintains aspect ratio
            scale = min(objectWidth / 0.45, objectDepth / 0.35)

        case .calculator:
            // Calculator stays relatively small
            scale = (objectWidth / 0.25) * 0.3
        }

        // Clamp scale to reasonable range
        scale = max(0.3, min(scale, 2.0))

        tool.toolEntity.scale = [scale, scale, scale]
    }

    private func calculateToolOffset(
        toolType: ToolType,
        objectSize: SIMD3<Float>,
        isFlipped: Bool
    ) -> SIMD3<Float> {
        var offset = SIMD3<Float>(0, objectSize.y / 2 + 0.01, 0)

        // If flipped, attach to bottom instead
        if isFlipped {
            offset.y = -(objectSize.y / 2 + 0.01)
        }

        return offset
    }

    private func handleFlip(_ tool: FollowingTool, trackingInfo: TrackedObjectInfo) {
        // Detect flip and adjust tool accordingly
        if trackingInfo.isFlipped {
            print("Object flipped! Adjusting tool \(tool.toolEntity.id)")

            // Tool stays attached but may need special handling
            // For example, keyboard keys should still face up even when flipped
        }
    }

    func stopFollowing(toolId: UUID) {
        followingTools.removeValue(forKey: toolId)
        print("Tool \(toolId) stopped following")
    }

    func isToolFollowing(_ toolId: UUID) -> Bool {
        return followingTools[toolId] != nil
    }
}

struct FollowingTool {
    let toolEntity: Entity
    let toolType: ToolType
    let targetObjectId: UUID
    var followPosition: Bool
    var followOrientation: Bool
    var adaptiveSize: Bool
    var stickOnFlip: Bool
}
