import Foundation
import ARKit
import RealityKit
import Combine

// MARK: - Finger Collision Detector
/// 指先が仮想キーボードのキー（現実の机の面）に触れたかを判定する。
/// パッシブ・ハプティクス原理: 指先が現実の机の硬さを感じる瞬間 = キーストローク検出。

@MainActor
class FingerCollisionDetector: ObservableObject {

    // MARK: - Published State

    /// 最後に押されたキー文字
    @Published var lastPressedKey: String? = nil
    /// キーが押されたイベント (キー文字, タイムスタンプ)
    @Published var keyPressEvent: KeyPressEvent? = nil
    /// 現在のタイプされたテキスト
    @Published var typedText: String = ""
    /// デバッグ用: 左人差し指の現在3D位置
    @Published var leftIndexTipPosition: SIMD3<Float>? = nil
    /// デバッグ用: 右人差し指の現在3D位置
    @Published var rightIndexTipPosition: SIMD3<Float>? = nil

    // MARK: - Private State

    /// 各キーの「今フレームで押されているか」= デバウンス用
    private var keyPressStates: [String: Bool] = [:]

    /// 前フレームの指先Y座標 (急降下 = キーストローク判定に使う)
    private var prevLeftIndexY: Float? = nil
    private var prevRightIndexY: Float? = nil

    /// 指先の降下速度でストロークを判断するための閾値 (m/s)
    /// 机の高さ (planeY) に対して指先がこの値以下にあるとき「着地」とみなす
    private let surfaceContactThreshold: Float = 0.012  // 12mm

    /// デバウンス: 同じキーを連続して検出するインターバル (秒)
    private let debounceDuration: TimeInterval = 0.18

    /// キーごとのクールダウン管理
    private var lastPressTime: [String: Date] = [:]

    // MARK: - Keyboard Registration

    /// 登録済みキーボード: キー文字 → ワールド空間の AABB
    private var registeredKeyboards: [ObjectIdentifier: KeyboardLayout] = [:]

    // MARK: - Public API

    /// キーボードEntityを登録する
    /// - Parameters:
    ///   - keyboard: `KeyboardEntity` インスタンス
    ///   - keyData: キー文字とその位置リスト
    func registerKeyboard(_ keyboard: Entity, keys: [KeyDescriptor]) {
        let layout = KeyboardLayout(entity: keyboard, keys: keys)
        registeredKeyboards[ObjectIdentifier(keyboard)] = layout
        print("[FingerCollision] Keyboard registered with \(keys.count) keys")
    }

    /// キーボードEntityの登録を解除する
    func unregisterKeyboard(_ keyboard: Entity) {
        registeredKeyboards.removeValue(forKey: ObjectIdentifier(keyboard))
    }

    /// ハンドトラッキングアップデートを受けて衝突判定を実行する
    /// `ImmersiveView` の handAnchor ループからフレームごとに呼ぶ
    func processHandAnchor(_ anchor: HandAnchor) {
        guard anchor.isTracked, let skeleton = anchor.handSkeleton else { return }

        // 人差し指先端のワールド座標を取得
        let indexTipPos = getWorldPosition(of: .indexFingerTip, from: anchor, skeleton: skeleton)
        // 中指先端 (追加検出用)
        let middleTipPos = getWorldPosition(of: .middleFingerTip, from: anchor, skeleton: skeleton)

        // 左右で公開プロパティを更新
        if anchor.chirality == .left {
            leftIndexTipPosition = indexTipPos
        } else {
            rightIndexTipPosition = indexTipPos
        }

        guard let fingerPos = indexTipPos else { return }

        // すべての登録済みキーボードに対して判定
        for (_, layout) in registeredKeyboards {
            checkCollision(fingerPos: fingerPos, layout: layout, chirality: anchor.chirality)
        }
    }

    // MARK: - Private Helpers

    private func getWorldPosition(
        of jointName: HandSkeleton.JointName,
        from anchor: HandAnchor,
        skeleton: HandSkeleton
    ) -> SIMD3<Float>? {
        let joint = skeleton.joint(jointName)
        guard joint.isTracked else { return nil }
        let worldMatrix = anchor.originFromAnchorTransform * joint.anchorFromJointTransform
        return SIMD3<Float>(worldMatrix.columns.3.x, worldMatrix.columns.3.y, worldMatrix.columns.3.z)
    }

    private func checkCollision(
        fingerPos: SIMD3<Float>,
        layout: KeyboardLayout,
        chirality: HandAnchor.Chirality
    ) {
        // キーボードEntityのワールド変換行列を取得
        guard let keyboardWorldTransform = layout.entity.transformMatrix(relativeTo: nil) as simd_float4x4? else {
            return
        }

        // 指先をキーボードのローカル座標に変換
        let inverseTransform = keyboardWorldTransform.inverse
        let fingerLocal4 = inverseTransform * SIMD4<Float>(fingerPos.x, fingerPos.y, fingerPos.z, 1.0)
        let fingerLocal = SIMD3<Float>(fingerLocal4.x, fingerLocal4.y, fingerLocal4.z)

        // XZの範囲でキーを絞り込み、Yで接触判定
        for key in layout.keys {
            // XZ平面での当たり判定（2Dバウンディングボックス）
            let dx = abs(fingerLocal.x - key.localPosition.x)
            let dz = abs(fingerLocal.z - key.localPosition.z)

            if dx <= key.halfWidth && dz <= key.halfDepth {
                // Y方向: 指先がキー上面 ± threshold 内にあるか
                let keyTopY = key.localPosition.y + key.halfHeight
                let contactY = keyTopY + surfaceContactThreshold

                if fingerLocal.y <= contactY && fingerLocal.y >= (keyTopY - 0.005) {
                    triggerKeyPress(key: key.letter, at: fingerPos)
                }
            }
        }
    }

    private func triggerKeyPress(key: String, at position: SIMD3<Float>) {
        let now = Date()

        // デバウンス: 前回押してから debounceDuration 以内はスキップ
        if let lastTime = lastPressTime[key],
           now.timeIntervalSince(lastTime) < debounceDuration {
            return
        }

        lastPressTime[key] = now
        lastPressedKey = key

        // テキスト更新
        switch key {
        case "⌫", "BACKSPACE":
            if !typedText.isEmpty {
                typedText.removeLast()
            }
        case "SPACE", "SPC":
            typedText += " "
        case "⏎", "RETURN", "ENTER":
            typedText += "\n"
        default:
            typedText += key
        }

        // イベント発行 (WebView等が購読)
        keyPressEvent = KeyPressEvent(key: key, timestamp: now, position: position)

        print("[FingerCollision] Key pressed: '\(key)' | Text: \(typedText.suffix(20))")
    }

    func clearText() {
        typedText = ""
        lastPressedKey = nil
    }
}

// MARK: - Supporting Types

/// キーボードのレイアウト情報（ローカル座標系）
struct KeyboardLayout {
    let entity: Entity
    let keys: [KeyDescriptor]
}

/// 個別キーの記述子
struct KeyDescriptor {
    let letter: String        // 文字 ("A", "SPACE", "⌫" など)
    let localPosition: SIMD3<Float>  // キーボードローカル座標における中心位置
    let halfWidth: Float             // X方向の半サイズ
    let halfHeight: Float            // Y方向の半サイズ (キーの厚み/2)
    let halfDepth: Float             // Z方向の半サイズ
}

/// キー押下イベント
struct KeyPressEvent: Identifiable, Equatable {
    let id = UUID()
    let key: String
    let timestamp: Date
    let position: SIMD3<Float>

    static func == (lhs: KeyPressEvent, rhs: KeyPressEvent) -> Bool {
        lhs.id == rhs.id
    }
}
