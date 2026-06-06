import Foundation
import RealityKit
import ARKit

@MainActor
class MagneticAttachmentManager: ObservableObject {
    @Published var attachments: [MagneticAttachment] = []
    @Published var nearbyObjects: [NearbyObject] = []

    // Magnetic snap settings
    private let snapDistance: Float = 0.15 // 15cm
    private let snapStrength: Float = 0.8
    private let adaptiveScaling: Bool = true

    // Animation settings
    private let attractionSpeed: Float = 0.1
    private let rotationSpeed: Float = 0.05

    func checkForNearbyObjects(
        tool: Entity,
        toolType: ToolType,
        scannedObjects: [ScannedObject]
    ) -> NearbyObject? {
        let toolPosition = tool.position(relativeTo: nil)

        for scannedObject in scannedObjects {
            let distance = distance(toolPosition, scannedObject.position)

            if distance < snapDistance {
                return NearbyObject(
                    scannedObject: scannedObject,
                    distance: distance,
                    isInSnapRange: true
                )
            }
        }

        return nil
    }

    func attachToolToObject(
        tool: Entity,
        toolType: ToolType,
        scannedObject: ScannedObject
    ) -> MagneticAttachment {
        print("Attaching \(toolType) to object \(scannedObject.id)")

        // Calculate optimal placement
        let placementData = calculateOptimalPlacement(
            tool: tool,
            object: scannedObject,
            toolType: toolType
        )

        // Create attachment
        let attachment = MagneticAttachment(
            id: UUID(),
            toolEntity: tool,
            toolType: toolType,
            targetObject: scannedObject,
            offsetPosition: placementData.offset,
            offsetOrientation: placementData.orientation,
            isLocked: false
        )

        attachments.append(attachment)

        // Apply initial transformation
        applyAttachmentTransform(attachment)

        return attachment
    }

    private func calculateOptimalPlacement(
        tool: Entity,
        object: ScannedObject,
        toolType: ToolType
    ) -> (offset: SIMD3<Float>, orientation: simd_quatf) {
        // Calculate the best position and orientation for the tool

        // For keyboard/trackpad, align to the top surface
        var offset = SIMD3<Float>(0, object.boundingBox.y / 2 + 0.005, 0)
        var orientation = object.orientation

        // Adjust based on tool type
        switch toolType {
        case .keyboard:
            // Keyboard should be slightly angled for ergonomics
            let tiltAngle: Float = .pi / 36 // 5 degrees
            orientation = simd_quatf(angle: tiltAngle, axis: [1, 0, 0]) * orientation

        case .trackpad:
            // Trackpad should be flat on surface
            offset.y += 0.002 // Slightly elevated

        case .memo, .drawing:
            // Upright position
            offset.y += 0.1
            orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0]) * orientation

        case .calculator:
            // Slight angle
            let tiltAngle: Float = .pi / 12
            orientation = simd_quatf(angle: tiltAngle, axis: [1, 0, 0]) * orientation
        }

        return (offset, orientation)
    }

    func updateAttachments(scannedObjects: [ScannedObject]) {
        for index in attachments.indices {
            guard let updatedObject = scannedObjects.first(where: {
                $0.id == attachments[index].targetObject.id
            }) else {
                continue
            }

            // Update target object data
            attachments[index].targetObject = updatedObject

            // Update tool transformation if object moved
            if !attachments[index].isLocked {
                applyAttachmentTransform(attachments[index])
            }
        }
    }

    private func applyAttachmentTransform(_ attachment: MagneticAttachment) {
        let targetPosition = attachment.targetObject.position
        let targetOrientation = attachment.targetObject.orientation

        // Calculate world position
        let worldPosition = targetPosition + attachment.offsetPosition
        let worldOrientation = targetOrientation * attachment.offsetOrientation

        // Apply adaptive scaling
        if adaptiveScaling {
            let scale = calculateAdaptiveScale(
                toolType: attachment.toolType,
                objectSize: attachment.targetObject.boundingBox
            )
            attachment.toolEntity.scale = [scale, scale, scale]
        }

        // Smoothly interpolate to target
        animateToTransform(
            entity: attachment.toolEntity,
            targetPosition: worldPosition,
            targetOrientation: worldOrientation
        )
    }

    private func calculateAdaptiveScale(
        toolType: ToolType,
        objectSize: SIMD3<Float>
    ) -> Float {
        // Scale tool based on object size
        let objectWidth = objectSize.x

        switch toolType {
        case .keyboard:
            // Keyboard should be 80% of object width
            return min(objectWidth / 0.4 * 0.8, 1.5) // Max 150% scale

        case .trackpad:
            // Trackpad should be 40% of object width
            return min(objectWidth / 0.15 * 0.4, 1.2)

        default:
            return 1.0
        }
    }

    private func animateToTransform(
        entity: Entity,
        targetPosition: SIMD3<Float>,
        targetOrientation: simd_quatf
    ) {
        // Smooth animation using interpolation
        let currentPosition = entity.position(relativeTo: nil)
        let currentOrientation = entity.orientation(relativeTo: nil)

        // Linear interpolation for position
        let newPosition = mix(currentPosition, targetPosition, t: attractionSpeed)

        // Spherical interpolation for rotation
        let newOrientation = simd_slerp(currentOrientation, targetOrientation, rotationSpeed)

        entity.setPosition(newPosition, relativeTo: nil)
        entity.setOrientation(newOrientation, relativeTo: nil)
    }

    func attractToolToObject(
        tool: Entity,
        targetObject: ScannedObject,
        strength: Float = 1.0
    ) {
        let toolPosition = tool.position(relativeTo: nil)
        let objectPosition = targetObject.position

        let direction = normalize(objectPosition - toolPosition)
        let distance = distance(toolPosition, objectPosition)

        // Apply magnetic force (stronger when closer)
        let force = direction * (snapStrength * strength) * (1.0 / max(distance, 0.1))

        let newPosition = toolPosition + force * attractionSpeed
        tool.setPosition(newPosition, relativeTo: nil)

        // Rotate to match object orientation
        let targetOrientation = targetObject.orientation
        let currentOrientation = tool.orientation(relativeTo: nil)
        let newOrientation = simd_slerp(currentOrientation, targetOrientation, rotationSpeed)
        tool.setOrientation(newOrientation, relativeTo: nil)
    }

    func detachTool(_ attachmentId: UUID) {
        attachments.removeAll { $0.id == attachmentId }
    }

    func toggleLock(_ attachmentId: UUID) {
        if let index = attachments.firstIndex(where: { $0.id == attachmentId }) {
            attachments[index].isLocked.toggle()
            print("Attachment \(attachmentId) lock: \(attachments[index].isLocked)")
        }
    }

    func isToolAttached(_ tool: Entity) -> Bool {
        return attachments.contains { $0.toolEntity.id == tool.id }
    }

    func getAttachment(for tool: Entity) -> MagneticAttachment? {
        return attachments.first { $0.toolEntity.id == tool.id }
    }
}

// MARK: - Models

struct MagneticAttachment: Identifiable {
    let id: UUID
    let toolEntity: Entity
    let toolType: ToolType
    var targetObject: ScannedObject
    var offsetPosition: SIMD3<Float>
    var offsetOrientation: simd_quatf
    var isLocked: Bool
    var attachmentTime: Date = Date()
}

struct NearbyObject {
    let scannedObject: ScannedObject
    let distance: Float
    let isInSnapRange: Bool
}

// MARK: - Helper Functions

func mix(_ a: SIMD3<Float>, _ b: SIMD3<Float>, t: Float) -> SIMD3<Float> {
    return a + (b - a) * t
}
