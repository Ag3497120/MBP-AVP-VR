import ARKit
import RealityKit
import simd
import Combine

// MARK: - FlipGestureDetector
// ハンドトラッキングで「手首をクルッと裏返す」ジェスチャーを検知する。
// Y軸まわりの角速度がthresholdを超えた後、
// 累積回転角がflipAngleThreshold (デフォルト150°) を超えたら onFlip() を発火。
//
// AIは使わない。人間が意図的に「裏返す」という身体操作が唯一のトリガー。

@MainActor
class FlipGestureDetector: ObservableObject {

    // MARK: - 設定
    /// 手首が何°以上回転したら flip と見なすか (default: 150°)
    var flipAngleThreshold: Float = 150 * .pi / 180

    /// 回転速度の最小しきい値 (rad/s) - ゆっくりとした回転は誤検知しない
    var angularVelocityThreshold: Float = 2.0

    /// フリップ後のクールダウン (秒)
    var cooldown: TimeInterval = 1.2

    // MARK: - 出力
    /// フリップ検知時に呼ばれる (どちらの手か、位置)
    var onFlip: ((Chirality, SIMD3<Float>) -> Void)?

    // MARK: - 内部状態
    enum Chirality { case left, right }

    private struct HandState {
        var prevOrientation: simd_quatf?
        var prevTime:        TimeInterval = 0
        var cumulativeYAngle: Float       = 0
        var isTracking:      Bool         = false
        var lastFlipTime:    TimeInterval = 0
        var wristPos:        SIMD3<Float> = .zero
    }

    private var leftHand  = HandState()
    private var rightHand = HandState()

    // MARK: - ARKit Hand Tracking 更新
    // ImmersiveViewのhandTrackingループから毎フレーム呼ぶ
    func update(handAnchors: [HandAnchor]) {
        let now = Date.timeIntervalSinceReferenceDate
        for anchor in handAnchors {
            switch anchor.chirality {
            case .left:  updateHand(&leftHand,  anchor: anchor, chirality: .left,  now: now)
            case .right: updateHand(&rightHand, anchor: anchor, chirality: .right, now: now)
            }
        }
    }

    private func updateHand(
        _ state: inout HandState,
        anchor: HandAnchor,
        chirality: Chirality,
        now: TimeInterval
    ) {
        // 手首ジョイントを取得
        guard let wristJoint = anchor.handSkeleton?.joint(.wrist),
              wristJoint.isTracked else {
            state.isTracking = false
            state.prevOrientation = nil
            return
        }

        // 手首のワールド座標での向き
        let wristTransform = matrix_multiply(anchor.originFromAnchorTransform, wristJoint.anchorFromJointTransform)
        let orientation    = simd_quatf(wristTransform)
        let wristPos       = SIMD3<Float>(wristTransform.columns.3.x,
                                          wristTransform.columns.3.y,
                                          wristTransform.columns.3.z)
        state.wristPos = wristPos

        defer {
            state.prevOrientation = orientation
            state.prevTime = now
            state.isTracking = true
        }

        guard let prev = state.prevOrientation, state.isTracking else { return }

        let dt = Float(now - state.prevTime)
        guard dt > 0.001 else { return }

        // 前フレームからの差分回転
        let deltaQ = orientation * prev.inverse
        let axis   = deltaQ.axis   // 回転軸
        let angle  = deltaQ.angle  // 回転角 (rad)

        // Y軸成分の角速度 (rad/s)
        let yAngularVelocity = abs(axis.y) * angle / dt

        // 速度がthreshold以下なら累積リセット (意図的な高速フリップのみ検知)
        if yAngularVelocity < angularVelocityThreshold {
            state.cumulativeYAngle = 0
            return
        }

        // Y軸回転を符号付きで累積
        let signedY = axis.y * angle
        state.cumulativeYAngle += signedY

        // 累積角がflipThresholdを超えたらフリップ確定
        if abs(state.cumulativeYAngle) >= flipAngleThreshold {
            let timeSinceLast = now - state.lastFlipTime
            guard timeSinceLast > cooldown else { return } // クールダウン

            state.cumulativeYAngle = 0
            state.lastFlipTime = now

            // メインスレッドで発火
            onFlip?(chirality, wristPos)
        }
    }

    // MARK: - リセット
    func reset() {
        leftHand  = HandState()
        rightHand = HandState()
    }

    // MARK: - デバッグ情報
    var debugDescription: String {
        "L: \(Int(leftHand.cumulativeYAngle * 180 / .pi))°  R: \(Int(rightHand.cumulativeYAngle * 180 / .pi))°"
    }
}

// MARK: - GrabDetector
// どの3D Entityをユーザーが「掴んでいる」かを追跡するシンプルな状態管理。
// ピンチジェスチャー + 近傍Entity の組み合わせで判定。

@MainActor
class GrabDetector: ObservableObject {

    // 現在掴まれているEntity
    @Published var grabbedEntity: Entity?
    @Published var grabbedImage:  UIImage?
    @Published var grabChirality: FlipGestureDetector.Chirality?

    // 掴める距離のしきい値 (m)
    var grabRadius: Float = 0.12

    // MARK: - ピンチ検知 (HandAnchorから)
    func updateGrab(handAnchors: [HandAnchor], candidates: [Entity], images: [Entity: UIImage]) {
        for anchor in handAnchors {
            guard let indexTip  = anchor.handSkeleton?.joint(.indexFingerTip),
                  let thumbTip  = anchor.handSkeleton?.joint(.thumbTip),
                  indexTip.isTracked, thumbTip.isTracked else { continue }

            let idx = matrix_multiply(anchor.originFromAnchorTransform, indexTip.anchorFromJointTransform)
            let thm = matrix_multiply(anchor.originFromAnchorTransform, thumbTip.anchorFromJointTransform)

            let indexPos = SIMD3<Float>(idx.columns.3.x, idx.columns.3.y, idx.columns.3.z)
            let thumbPos = SIMD3<Float>(thm.columns.3.x, thm.columns.3.y, thm.columns.3.z)
            let pinchDist = distance(indexPos, thumbPos)

            // ピンチ距離 < 2cm = ピンチ中
            let isPinching = pinchDist < 0.02
            let midPoint   = (indexPos + thumbPos) / 2

            if isPinching {
                // 最も近いEntityを掴む
                if grabbedEntity == nil {
                    let nearest = candidates.min(by: {
                        distance($0.position(relativeTo: nil), midPoint) <
                        distance($1.position(relativeTo: nil), midPoint)
                    })
                    if let e = nearest, distance(e.position(relativeTo: nil), midPoint) < grabRadius {
                        grabbedEntity = e
                        grabbedImage  = images[e]
                        grabChirality = anchor.chirality == .left ? .left : .right
                    }
                } else {
                    // 掴み中: Entityをピンチ位置に追従
                    grabbedEntity?.position = midPoint
                }
            } else {
                // ピンチ解放
                if grabChirality == (anchor.chirality == .left ? .left : .right) {
                    grabbedEntity = nil
                    grabbedImage  = nil
                    grabChirality = nil
                }
            }
        }
    }
}
