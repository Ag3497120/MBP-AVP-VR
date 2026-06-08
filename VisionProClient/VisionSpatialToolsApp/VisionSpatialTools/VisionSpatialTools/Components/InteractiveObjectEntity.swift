import RealityKit
import UIKit
import simd

// MARK: - InteractiveObjectEntity
// AIの分析結果 + クロップ画像 + テンプレートを組み合わせて
// 空間上に配置されるインタラクティブエンティティ。
// 指の衝突でSpatialAudioEngineを通じて音を鳴らす。

class InteractiveObjectEntity: Entity {

    // MARK: - 構成要素
    let region:    InteractiveRegion
    let soundSet:  String
    private(set) var panelEntity: ModelEntity?

    // 衝突面のサイズ (メートル)
    let physicalWidth:  Float
    let physicalHeight: Float

    // 分割された当たり判定ゾーン (鍵盤列など)
    private var hitZones: [HitZone] = []

    struct HitZone {
        let entity:   Entity
        let uvStart:  Double   // 0.0-1.0
        let uvEnd:    Double
        let noteHint: String   // "C4", "D4", etc.
    }

    // MARK: - ファクトリ
    static func build(
        region:  InteractiveRegion,
        image:   UIImage,
        scaleM:  Float = 0.4      // 物理サイズ (メートル)
    ) -> InteractiveObjectEntity {
        let aspect = Float(image.size.width / image.size.height)
        let w = scaleM * (aspect > 1 ? 1 : aspect)
        let h = scaleM / (aspect > 1 ? aspect : 1)

        let entity = InteractiveObjectEntity(
            region: region,
            soundSet: region.soundSet,
            width: w, height: h
        )
        entity.buildPanel(image: image)
        entity.buildHitZones()
        return entity
    }

    init(region: InteractiveRegion, soundSet: String, width: Float, height: Float) {
        self.region = region
        self.soundSet = soundSet
        self.physicalWidth  = width
        self.physicalHeight = height
        super.init()
        SpatialAudioEngine.shared.load(soundSet: soundSet)
    }

    required init() { fatalError() }

    // MARK: - パネル生成 (クロップ画像をテクスチャに)
    private func buildPanel(image: UIImage) {
        let mesh = MeshResource.generatePlane(width: physicalWidth, height: physicalHeight)

        // UIImage → TextureResource
        if let cgImage = image.cgImage,
           let texture = try? TextureResource.generate(from: cgImage, options: .init(semantic: .color)) {
            var mat = UnlitMaterial()
            mat.color = .init(texture: .init(texture))
            let panel = ModelEntity(mesh: mesh, materials: [mat])
            panel.name = "Panel_\(region.id)"
            panel.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])

            // ガラス枠 (accent border)
            let borderMesh = MeshResource.generateBox(
                width: physicalWidth + 0.008,
                height: 0.005,
                depth: physicalHeight + 0.008,
                cornerRadius: 0.006
            )
            var borderMat = UnlitMaterial()
            borderMat.color = .init(tint: UIColor(red: 99/255, green: 102/255, blue: 241/255, alpha: 0.7))
            let border = ModelEntity(mesh: borderMesh, materials: [borderMat])
            border.position = [0, -0.003, 0]
            addChild(border)

            addChild(panel)
            panelEntity = panel
        }

        // 全体の衝突判定 (InputTarget)
        components.set(CollisionComponent(shapes: [
            .generateBox(width: physicalWidth, height: 0.02, depth: physicalHeight)
        ]))
        components.set(InputTargetComponent(allowedInputTypes: [.direct, .indirect]))
        components.set(HoverEffectComponent())
    }

    // MARK: - ヒットゾーン構築 (テンプレート別)
    private func buildHitZones() {
        switch region.type {
        case .keyboard:
            buildKeyboardZones()
        case .button:
            buildSingleZone()
        case .slider:
            buildSliderZone()
        case .string:
            buildStringZones()
        }
    }

    /// 鍵盤列: 横方向をkeyCount分割して各ゾーンにEntityを作成
    private func buildKeyboardZones() {
        let count    = region.keyCount ?? 12
        let octave   = region.octave  ?? 4
        let noteNames = ["C","C#","D","D#","E","F","F#","G","G#","A","A#","B"]
        let zoneW    = physicalWidth / Float(count)

        for i in 0..<count {
            let uvStart = Double(i) / Double(count)
            let uvEnd   = Double(i + 1) / Double(count)
            let noteName = "\(noteNames[i % 12])\(octave + i / 12)"
            let isBlack  = [1,3,6,8,10].contains(i % 12)

            let mesh = MeshResource.generateBox(
                width: zoneW - 0.001,
                height: 0.008,
                depth: physicalHeight * (isBlack ? 0.6 : 1.0),
                cornerRadius: 0.002
            )
            var mat = UnlitMaterial()
            mat.color = .init(tint: UIColor.clear) // 透明 (画像テクスチャが見える)
            let zone = ModelEntity(mesh: mesh, materials: [mat])
            zone.position = [
                -physicalWidth/2 + zoneW * (Float(i) + 0.5),
                0.005,
                isBlack ? physicalHeight * 0.1 : 0
            ]
            zone.name = "Key_\(noteName)"
            zone.components.set(CollisionComponent(shapes: [
                .generateBox(width: zoneW - 0.001, height: 0.012, depth: physicalHeight * (isBlack ? 0.6 : 1.0))
            ]))
            zone.components.set(InputTargetComponent(allowedInputTypes: .direct))

            hitZones.append(HitZone(entity: zone, uvStart: uvStart, uvEnd: uvEnd, noteHint: noteName))
            addChild(zone)
        }
    }

    /// 単一打面: パネル全体が1ゾーン
    private func buildSingleZone() {
        hitZones.append(HitZone(entity: self, uvStart: 0, uvEnd: 1, noteHint: "hit"))
    }

    /// スライダー: 横方向に連続ゾーン
    private func buildSliderZone() {
        let sliderMesh = MeshResource.generateBox(
            width: physicalWidth, height: 0.015, depth: 0.015, cornerRadius: 0.007
        )
        var mat = UnlitMaterial()
        mat.color = .init(tint: UIColor(red: 99/255, green: 102/255, blue: 241/255, alpha: 0.3))
        let slider = ModelEntity(mesh: sliderMesh, materials: [mat])
        slider.position = [0, 0.008, 0]
        slider.name = "Slider_\(region.id)"
        slider.components.set(CollisionComponent(shapes: [
            .generateBox(width: physicalWidth, height: 0.02, depth: 0.02)
        ]))
        slider.components.set(InputTargetComponent(allowedInputTypes: .direct))
        hitZones.append(HitZone(entity: slider, uvStart: 0, uvEnd: 1, noteHint: "slide"))
        addChild(slider)
    }

    /// 弦: 縦方向に6本 (ギター弦イメージ)
    private func buildStringZones() {
        let stringCount = 6
        let spacing = physicalHeight / Float(stringCount)
        for i in 0..<stringCount {
            let mesh = MeshResource.generateCylinder(height: physicalWidth, radius: 0.001)
            var mat = UnlitMaterial()
            mat.color = .init(tint: UIColor(white: 0.8, alpha: 0.6))
            let str = ModelEntity(mesh: mesh, materials: [mat])
            str.orientation = simd_quatf(angle: .pi/2, axis: [0,0,1])
            str.position = [0, 0.004, -physicalHeight/2 + spacing * (Float(i) + 0.5)]
            str.name = "String_\(i)"
            str.components.set(CollisionComponent(shapes: [
                .generateBox(width: physicalWidth, height: 0.01, depth: 0.01)
            ]))
            str.components.set(InputTargetComponent(allowedInputTypes: .direct))
            let uv = Double(i) / Double(stringCount)
            hitZones.append(HitZone(entity: str, uvStart: uv, uvEnd: uv + 1.0/Double(stringCount), noteHint: "str_\(i)"))
            addChild(str)
        }
    }

    // MARK: - 衝突検知 → 音再生
    // ImmersiveViewから呼ばれる (DirectTouch)
    func handleTouch(localPosition pos: SIMD3<Float>) {
        let uvX = Double((pos.x + physicalWidth/2) / physicalWidth).clamped(0...1)
        let uvZ = Double((pos.z + physicalHeight/2) / physicalHeight).clamped(0...1)

        let audio = SpatialAudioEngine.shared
        switch region.type {
        case .keyboard:
            let count  = region.keyCount ?? 12
            let octave = region.octave   ?? 4
            audio.playKeyboard(uvX: uvX, keyCount: count, octave: octave)
            pulseKey(uvX: uvX)
        case .button:
            audio.playButton(soundSet: soundSet)
            pulsePanel()
        case .slider:
            audio.playSlider(uvX: uvX, soundSet: soundSet)
        case .string:
            audio.playString(uvX: uvZ)
            pulsePanel()
        }
    }

    // MARK: - ビジュアルフィードバック
    private func pulseKey(uvX: Double) {
        let idx = Int(uvX * Double(hitZones.count))
        guard idx < hitZones.count else { return }
        let zone = hitZones[idx].entity
        var transform = zone.transform
        let orig = transform.scale
        transform.scale = orig * 1.15
        zone.move(to: transform, relativeTo: zone.parent, duration: 0.05)
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
            var t2 = zone.transform; t2.scale = orig
            zone.move(to: t2, relativeTo: zone.parent, duration: 0.05)
        }
    }

    private func pulsePanel() {
        guard let p = panelEntity else { return }
        var t = p.transform
        t.scale = [1.05, 1.05, 1.05]
        p.move(to: t, relativeTo: p.parent, duration: 0.05)
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.1) {
            var t2 = p.transform; t2.scale = [1,1,1]
            p.move(to: t2, relativeTo: p.parent, duration: 0.05)
        }
    }
}

private extension Double {
    func clamped(_ r: ClosedRange<Double>) -> Double { min(max(self, r.lowerBound), r.upperBound) }
}
