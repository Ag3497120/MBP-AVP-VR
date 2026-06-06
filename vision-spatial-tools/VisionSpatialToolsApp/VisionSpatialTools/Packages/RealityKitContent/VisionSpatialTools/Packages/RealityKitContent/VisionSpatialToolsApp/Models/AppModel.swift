import SwiftUI
import RealityKit
import ARKit

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
        } catch {
            print("Failed to start ARKit session: \(error)")
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
    case trackpad  // New tool type
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
