import Combine
import Foundation
import ARKit
import RealityKit

@MainActor
class HandTrackingManager: ObservableObject {
    @Published var leftHandPosition: SIMD3<Float>?
    @Published var rightHandPosition: SIMD3<Float>?
    @Published var leftHandTransform: simd_float4x4?
    @Published var rightHandTransform: simd_float4x4?
    
    @Published var isLeftPinching: Bool = false
    @Published var isRightPinching: Bool = false
    
    @Published var isPinching: Bool = false
    @Published var gestureType: GestureType = .none

    private let handTrackingProvider = HandTrackingProvider()
    private var arkitSession: ARKitSession?

    enum GestureType {
        case none
        case pinch
        case point
        case grab
        case swipe
    }

    enum Hand {
        case left
        case right
    }

    func startTracking(session: ARKitSession) async {
        self.arkitSession = session

        do {
            if HandTrackingProvider.isSupported {
                try await session.run([handTrackingProvider])
                print("Hand tracking started successfully")
                UDPHandTrackerClient.shared.start()
                await processHandUpdates()
            } else {
                print("Hand tracking is not supported on this device")
            }
        } catch {
            print("Failed to start hand tracking: \(error)")
        }
    }

    private func processHandUpdates() async {
        for await update in handTrackingProvider.anchorUpdates {
            let anchor = update.anchor

            guard anchor.isTracked else { continue }

            if anchor.chirality == .left {
                leftHandTransform = getHandTransform(from: anchor)
                if let t = leftHandTransform {
                    leftHandPosition = SIMD3<Float>(t.columns.3.x, t.columns.3.y, t.columns.3.z)
                }
            } else if anchor.chirality == .right {
                rightHandTransform = getHandTransform(from: anchor)
                if let t = rightHandTransform {
                    rightHandPosition = SIMD3<Float>(t.columns.3.x, t.columns.3.y, t.columns.3.z)
                }
            }

            detectGestures(from: anchor)
            
            // Send UDP Update
            let lTrans = leftHandTransform ?? matrix_identity_float4x4
            let rTrans = rightHandTransform ?? matrix_identity_float4x4
            UDPHandTrackerClient.shared.sendHandData(
                leftTransform: lTrans, rightTransform: rTrans,
                leftPinch: isLeftPinching, rightPinch: isRightPinching
            )
        }
    }

    private func getHandTransform(from anchor: HandAnchor) -> simd_float4x4? {
        guard let wristJoint = anchor.handSkeleton?.joint(.wrist) else { return nil }
        return anchor.originFromAnchorTransform * wristJoint.anchorFromJointTransform
    }
    
    private func getHandPosition(from anchor: HandAnchor) -> SIMD3<Float> {
        guard let transform = getHandTransform(from: anchor) else { return [0, 0, 0] }
        return SIMD3<Float>(transform.columns.3.x, transform.columns.3.y, transform.columns.3.z)
    }

    private func detectGestures(from anchor: HandAnchor) {
        guard let handSkeleton = anchor.handSkeleton else { return }

        // Get key joints
        let thumbTip = handSkeleton.joint(.thumbTip)
        let indexTip = handSkeleton.joint(.indexFingerTip)
        let middleTip = handSkeleton.joint(.middleFingerTip)
        let wrist = handSkeleton.joint(.wrist)

        // Calculate positions in anchor space
        let thumbPos = getJointPosition(thumbTip, anchor: anchor)
        let indexPos = getJointPosition(indexTip, anchor: anchor)
        let middlePos = getJointPosition(middleTip, anchor: anchor)

        // Pinch detection (thumb and index finger close together)
        let pinchDistance = distance(thumbPos, indexPos)
        let isPinchingNow = pinchDistance < 0.03 // 3cm threshold
        
        if anchor.chirality == .left {
            isLeftPinching = isPinchingNow
        } else {
            isRightPinching = isPinchingNow
        }
        
        let wasPinching = isPinching
        isPinching = isLeftPinching || isRightPinching

        if isPinching && !wasPinching {
            gestureType = .pinch
            print("Pinch detected at distance: \(pinchDistance)")
        } else if !isPinching && wasPinching {
            gestureType = .none
        }

        // Point detection (index finger extended, others curled)
        if isFingerExtended(indexTip, wrist: wrist, anchor: anchor) &&
           !isFingerExtended(middleTip, wrist: wrist, anchor: anchor) {
            if gestureType != .pinch {
                gestureType = .point
            }
        }

        // Grab detection (all fingers curled)
        if !isFingerExtended(indexTip, wrist: wrist, anchor: anchor) &&
           !isFingerExtended(middleTip, wrist: wrist, anchor: anchor) {
            if gestureType != .pinch && gestureType != .point {
                gestureType = .grab
            }
        }
    }

    private func getJointPosition(_ joint: HandSkeleton.Joint, anchor: HandAnchor) -> SIMD3<Float> {
        let transform = anchor.originFromAnchorTransform * joint.anchorFromJointTransform
        return SIMD3<Float>(transform.columns.3.x, transform.columns.3.y, transform.columns.3.z)
    }

    private func isFingerExtended(_ fingerTip: HandSkeleton.Joint, wrist: HandSkeleton.Joint, anchor: HandAnchor) -> Bool {
        let tipPos = getJointPosition(fingerTip, anchor: anchor)
        let wristPos = getJointPosition(wrist, anchor: anchor)

        let distance = distance(tipPos, wristPos)
        return distance > 0.10 // 10cm threshold for extended finger
    }

    func getPinchPosition(for hand: Hand) -> SIMD3<Float>? {
        guard isPinching else { return nil }

        switch hand {
        case .left:
            return leftHandPosition
        case .right:
            return rightHandPosition
        }
    }

    func isHandVisible(_ hand: Hand) -> Bool {
        switch hand {
        case .left:
            return leftHandPosition != nil
        case .right:
            return rightHandPosition != nil
        }
    }

    func resetGesture() {
        gestureType = .none
        isPinching = false
    }
}
