import RealityKit
import SwiftUI

// MARK: - Enhanced KeyboardEntity with Finger Collision Support
/// キーボードエンティティを指先衝突検出に対応させるため、
/// KeyDescriptor リストを外部に公開し、FingerCollisionDetector へ登録できるようにした。

class EnhancedKeyboardEntity: Entity {

    // MARK: - Key Layout Definition

    /// QWERTY 配列 (row 0 = 上段, row 2 = 下段)
    private let keyLayout: [[String]] = [
        ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"],
        ["A", "S", "D", "F", "G", "H", "J", "K", "L"],
        ["Z", "X", "C", "V", "B", "N", "M"]
    ]
    private let functionRow: [String] = ["⌫"]  // バックスペース

    // MARK: - Physical Dimensions
    private let keyWidth:   Float = 0.036
    private let keyDepth:   Float = 0.036
    private let keySpacing: Float = 0.004
    private let keyThickness: Float = 0.010  // キーの厚み (Y方向)
    private let baseThickness: Float = 0.008

    // MARK: - State
    /// 衝突検出用に公開するキー記述子リスト (ローカル座標)
    private(set) var keyDescriptors: [KeyDescriptor] = []

    /// 現在押されているキーのハイライト状態
    private var keyEntities: [String: ModelEntity] = [:]

    // MARK: - Visual State
    private let normalKeyColor   = UIColor(white: 0.92, alpha: 1.0)
    private let pressedKeyColor  = UIColor(red: 0.3, green: 0.6, blue: 1.0, alpha: 1.0)
    private let baseKeyColor     = UIColor(white: 0.85, alpha: 0.95)

    // MARK: - Init

    required init() {
        super.init()
        buildKeyboard()
    }

    // MARK: - Build

    private func buildKeyboard() {
        // --- ベースプレート ---
        let totalWidth = Float(10) * (keyWidth + keySpacing) + 0.01
        let totalDepth = 3.5 * (keyDepth + keySpacing) + 0.06  // 3行 + スペースバー

        let baseMesh = MeshResource.generateBox(
            width: totalWidth,
            height: baseThickness,
            depth: totalDepth,
            cornerRadius: 0.006
        )
        var baseMat = PhysicallyBasedMaterial()
        baseMat.baseColor = .init(tint: UIColor(white: 0.78, alpha: 0.97))
        baseMat.metallic  = 0.1
        baseMat.roughness = 0.7

        let base = ModelEntity(mesh: baseMesh, materials: [baseMat])
        base.name = "KeyboardBase"
        addChild(base)

        // Collision for full keyboard (InputTarget)
        base.components.set(InputTargetComponent(allowedInputTypes: [.indirect, .direct]))
        base.components.set(
            CollisionComponent(shapes: [
                .generateBox(width: totalWidth, height: baseThickness + keyThickness, depth: totalDepth)
            ])
        )

        // --- キー配置 ---
        let topY = baseThickness / 2 + keyThickness / 2   // キーボードベース上面 + キー厚みの半分

        var allKeyDescs: [KeyDescriptor] = []

        for (rowIndex, row) in keyLayout.enumerated() {
            let rowZ = 0.09 - Float(rowIndex) * (keyDepth + keySpacing)
            let rowStartX = -Float(row.count - 1) * (keyWidth + keySpacing) / 2

            for (colIndex, letter) in row.enumerated() {
                let x = rowStartX + Float(colIndex) * (keyWidth + keySpacing)
                let pos = SIMD3<Float>(x, topY, rowZ)

                let (entity, _) = makeKey(letter: letter, width: keyWidth, position: pos)
                addChild(entity)
                keyEntities[letter] = entity

                // 衝突検出用記述子を追加
                allKeyDescs.append(KeyDescriptor(
                    letter: letter,
                    localPosition: pos,
                    halfWidth: keyWidth / 2,
                    halfHeight: keyThickness / 2,
                    halfDepth: keyDepth / 2
                ))
            }
        }

        // スペースバー
        let spaceWidth = keyWidth * 5
        let spacePos = SIMD3<Float>(0, topY, 0.09 - 3 * (keyDepth + keySpacing))
        let (spaceEntity, _) = makeKey(letter: "SPACE", width: spaceWidth, position: spacePos)
        addChild(spaceEntity)
        keyEntities["SPACE"] = spaceEntity
        allKeyDescs.append(KeyDescriptor(
            letter: "SPACE",
            localPosition: spacePos,
            halfWidth: spaceWidth / 2,
            halfHeight: keyThickness / 2,
            halfDepth: keyDepth / 2
        ))

        // バックスペースキー
        let bsX = Float(keyLayout[0].count - 1) * (keyWidth + keySpacing) / 2 + keyWidth + keySpacing
        let bsPos = SIMD3<Float>(bsX, topY, 0.09)
        let (bsEntity, _) = makeKey(letter: "⌫", width: keyWidth * 1.5, position: bsPos, accent: true)
        addChild(bsEntity)
        keyEntities["⌫"] = bsEntity
        allKeyDescs.append(KeyDescriptor(
            letter: "⌫",
            localPosition: bsPos,
            halfWidth: keyWidth * 1.5 / 2,
            halfHeight: keyThickness / 2,
            halfDepth: keyDepth / 2
        ))

        // Enterキー
        let enterX = Float(keyLayout[1].count - 1) * (keyWidth + keySpacing) / 2 + keyWidth * 0.75 + keySpacing
        let enterPos = SIMD3<Float>(enterX, topY, 0.09 - (keyDepth + keySpacing))
        let (enterEntity, _) = makeKey(letter: "⏎", width: keyWidth * 1.5, position: enterPos, accent: true)
        addChild(enterEntity)
        keyEntities["⏎"] = enterEntity
        allKeyDescs.append(KeyDescriptor(
            letter: "⏎",
            localPosition: enterPos,
            halfWidth: keyWidth * 1.5 / 2,
            halfHeight: keyThickness / 2,
            halfDepth: keyDepth / 2
        ))

        keyDescriptors = allKeyDescs
        print("[EnhancedKeyboard] Built \(allKeyDescs.count) keys")
    }

    /// キーの ModelEntity を作成する
    private func makeKey(
        letter: String,
        width: Float,
        position: SIMD3<Float>,
        accent: Bool = false
    ) -> (Entity, ModelEntity) {
        let container = Entity()

        let keyMesh = MeshResource.generateBox(
            width: width,
            height: keyThickness,
            depth: keyDepth,
            cornerRadius: 0.003
        )

        var keyMat = PhysicallyBasedMaterial()
        keyMat.baseColor = .init(tint: accent ? UIColor(white: 0.55, alpha: 1.0) : normalKeyColor)
        keyMat.metallic  = 0.05
        keyMat.roughness = 0.6

        let keyModel = ModelEntity(mesh: keyMesh, materials: [keyMat])
        keyModel.name = "Key_\(letter)"
        container.addChild(keyModel)
        container.position = position

        // キーごとに当たり判定とホバーエフェクト
        container.components.set(
            CollisionComponent(shapes: [
                .generateBox(width: width, height: keyThickness + 0.006, depth: keyDepth)
            ])
        )
        container.components.set(InputTargetComponent(allowedInputTypes: [.indirect, .direct]))
        container.components.set(HoverEffectComponent())

        return (container, keyModel)
    }

    // MARK: - Visual Feedback

    /// キー押下時のアニメーション (色変更 + 沈み込み)
    func animateKeyPress(letter: String) {
        guard let entity = keyEntities[letter] else { return }

        // 色をハイライトに変更
        var pressedMat = PhysicallyBasedMaterial()
        pressedMat.baseColor = .init(tint: pressedKeyColor)
        pressedMat.emissiveColor = .init(color: UIColor(red: 0.1, green: 0.3, blue: 0.9, alpha: 1))
        pressedMat.emissiveIntensity = 0.5
        entity.model?.materials = [pressedMat]

        // 0.12秒後に元に戻す
        Task {
            try? await Task.sleep(for: .milliseconds(120))
            await MainActor.run {
                var normalMat = PhysicallyBasedMaterial()
                normalMat.baseColor = .init(tint: self.normalKeyColor)
                entity.model?.materials = [normalMat]
            }
        }
    }
}
