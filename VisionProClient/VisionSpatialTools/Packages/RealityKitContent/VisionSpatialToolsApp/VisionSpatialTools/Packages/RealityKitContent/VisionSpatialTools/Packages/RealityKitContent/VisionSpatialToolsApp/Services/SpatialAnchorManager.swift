import Foundation
import ARKit
import RealityKit

@MainActor
class SpatialAnchorManager: ObservableObject {
    @Published var anchors: [UUID: AnchorEntity] = [:]
    @Published var planeAnchors: [PlaneAnchor] = []

    private let worldTrackingProvider = WorldTrackingProvider()
    private let planeDetectionProvider = PlaneDetectionProvider(alignments: [.horizontal, .vertical])
    private var arkitSession: ARKitSession?

    func startTracking(session: ARKitSession) async {
        self.arkitSession = session

        do {
            try await session.run([worldTrackingProvider, planeDetectionProvider])
            print("Spatial anchor tracking started successfully")
            await processPlaneUpdates()
        } catch {
            print("Failed to start spatial tracking: \(error)")
        }
    }

    private func processPlaneUpdates() async {
        for await update in planeDetectionProvider.anchorUpdates {
            let anchor = update.anchor

            switch update.event {
            case .added:
                planeAnchors.append(anchor)
                print("Plane detected: \(anchor.classification)")

            case .updated:
                if let index = planeAnchors.firstIndex(where: { $0.id == anchor.id }) {
                    planeAnchors[index] = anchor
                }

            case .removed:
                planeAnchors.removeAll { $0.id == anchor.id }
            }
        }
    }

    func createAnchor(at position: SIMD3<Float>, in scene: Entity) -> AnchorEntity {
        let anchor = AnchorEntity(world: position)
        scene.addChild(anchor)
        anchors[anchor.id] = anchor
        return anchor
    }

    func createAnchorOnPlane(at position: SIMD3<Float>, in scene: Entity) -> AnchorEntity? {
        // Find nearest plane
        guard let nearestPlane = findNearestPlane(to: position) else {
            return nil
        }

        // Project position onto plane
        let projectedPosition = projectOntoPlane(position, plane: nearestPlane)

        let anchor = AnchorEntity(world: projectedPosition)
        scene.addChild(anchor)
        anchors[anchor.id] = anchor

        return anchor
    }

    private func findNearestPlane(to position: SIMD3<Float>) -> PlaneAnchor? {
        var nearestPlane: PlaneAnchor?
        var minDistance: Float = .infinity

        for plane in planeAnchors {
            let planePosition = SIMD3<Float>(
                plane.originFromAnchorTransform.columns.3.x,
                plane.originFromAnchorTransform.columns.3.y,
                plane.originFromAnchorTransform.columns.3.z
            )

            let dist = distance(position, planePosition)
            if dist < minDistance {
                minDistance = dist
                nearestPlane = plane
            }
        }

        return nearestPlane
    }

    private func projectOntoPlane(_ position: SIMD3<Float>, plane: PlaneAnchor) -> SIMD3<Float> {
        let planeTransform = plane.originFromAnchorTransform
        let planePosition = SIMD3<Float>(
            planeTransform.columns.3.x,
            planeTransform.columns.3.y,
            planeTransform.columns.3.z
        )

        // Get plane normal (y-axis for horizontal, z-axis for vertical)
        let planeNormal: SIMD3<Float>
        if plane.alignment == .horizontal {
            planeNormal = SIMD3<Float>(
                planeTransform.columns.1.x,
                planeTransform.columns.1.y,
                planeTransform.columns.1.z
            )
        } else {
            planeNormal = SIMD3<Float>(
                planeTransform.columns.2.x,
                planeTransform.columns.2.y,
                planeTransform.columns.2.z
            )
        }

        // Project position onto plane
        let toPoint = position - planePosition
        let distanceToPlane = dot(toPoint, planeNormal)
        return position - planeNormal * distanceToPlane
    }

    func removeAnchor(id: UUID) {
        if let anchor = anchors[id] {
            anchor.removeFromParent()
            anchors.removeValue(forKey: id)
        }
    }

    func moveAnchor(id: UUID, to position: SIMD3<Float>) {
        if let anchor = anchors[id] {
            anchor.position = position
        }
    }

    func getAnchorTransform(id: UUID) -> simd_float4x4? {
        guard let anchor = anchors[id] else { return nil }
        return anchor.transformMatrix(relativeTo: nil)
    }

    func clearAllAnchors() {
        for (_, anchor) in anchors {
            anchor.removeFromParent()
        }
        anchors.removeAll()
    }
}
