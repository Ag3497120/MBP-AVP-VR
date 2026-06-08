import RealityKit
import UIKit
import simd
import Combine

// MARK: - VirtualMirrorEntity
// 空間に配置される「仮想の鏡ポータル」。
// ユーザーが掴んだ画像Entityがこのポータル平面を通過した瞬間に
// 裏面鏡写しテクスチャを生成・実体化する（人間の判断がトリガー）。
//
// AIによる確率的判定は一切行わない。
// 人間が「通す」という行為 = 「左右対称である」という構造的推論の明示的入力。

class VirtualMirrorEntity: Entity {

    // MARK: - コールバック
    // 画像が鏡平面を通過した時に呼ばれる (onMainActor)
    var onImageCrossed: ((UIImage, SIMD3<Float>) -> Void)?

    // MARK: - 設定
    let mirrorWidth:  Float = 0.50   // 50cm × 70cm の鏡
    let mirrorHeight: Float = 0.70
    private var glowIntensity: Float = 0.0
    private var glowTimer: Float     = 0.0

    // 平面の法線ベクトル (ワールド座標)
    var planeNormal: SIMD3<Float> { transform.matrix.columns.2.xyz }
    var planePoint:  SIMD3<Float> { position }

    // 通過済みEntityのIDセット (二重検知防止)
    private var crossedIDs: Set<UInt64> = []

    // MARK: - 構築
    required init() {
        super.init()
        buildMirrorFrame()
        buildGlassPane()
        buildParticleRing()
        setupCollision()
    }

    // MARK: - フレーム (外枠)
    private func buildMirrorFrame() {
        let frameW = mirrorWidth  + 0.014
        let frameH = mirrorHeight + 0.014
        let thick: Float = 0.008

        // 上下左右の4本フレーム
        let bars: [(SIMD3<Float>, SIMD3<Float>)] = [
            ([0,  frameH/2, 0], [frameW, thick, thick]),
            ([0, -frameH/2, 0], [frameW, thick, thick]),
            ([ frameW/2, 0, 0], [thick, frameH, thick]),
            ([-frameW/2, 0, 0], [thick, frameH, thick]),
        ]
        for (pos, size) in bars {
            let mesh = MeshResource.generateBox(width: size.x, height: size.y, depth: size.z, cornerRadius: 0.003)
            var mat = PhysicallyBasedMaterial()
            mat.baseColor = .init(tint: UIColor(red: 180/255, green: 180/255, blue: 220/255, alpha: 1))
            mat.metallic   = 0.85
            mat.roughness  = 0.10
            let bar = ModelEntity(mesh: mesh, materials: [mat])
            bar.position = pos
            addChild(bar)
        }
    }

    // MARK: - ガラス面 (半透明 + 発光)
    private func buildGlassPane() {
        let mesh = MeshResource.generatePlane(width: mirrorWidth, height: mirrorHeight)
        var mat = PhysicallyBasedMaterial()
        mat.baseColor    = .init(tint: UIColor(red: 160/255, green: 180/255, blue: 255/255, alpha: 0.18))
        mat.roughness    = 0.0
        mat.metallic     = 0.0
        mat.blending     = .transparent(opacity: .init(floatLiteral: 0.18))
        let glass = ModelEntity(mesh: mesh, materials: [mat])
        glass.name = "MirrorGlass"
        // XZ面に立てる
        glass.orientation = simd_quatf(angle: .pi/2, axis: [1,0,0])
        addChild(glass)
    }

    // MARK: - パーティクルリング (鏡の縁を光らせる)
    private func buildParticleRing() {
        // コーナーの4箇所にglow球を置く
        let corners: [SIMD3<Float>] = [
            [ mirrorWidth/2,  mirrorHeight/2, 0],
            [-mirrorWidth/2,  mirrorHeight/2, 0],
            [ mirrorWidth/2, -mirrorHeight/2, 0],
            [-mirrorWidth/2, -mirrorHeight/2, 0],
        ]
        for pos in corners {
            let sphere = MeshResource.generateSphere(radius: 0.010)
            var mat = UnlitMaterial()
            mat.color = .init(tint: UIColor(red: 120/255, green: 160/255, blue: 255/255, alpha: 0.9))
            let glow = ModelEntity(mesh: sphere, materials: [mat])
            glow.position = pos
            glow.name = "GlowCorner"
            addChild(glow)
        }
    }

    // MARK: - 衝突判定面 (薄いトリガーボックス)
    private func setupCollision() {
        // ポータルの「入口」となる薄いボックス
        components.set(CollisionComponent(shapes: [
            .generateBox(width: mirrorWidth, height: mirrorHeight, depth: 0.04)
        ], mode: .trigger))
        components.set(InputTargetComponent(allowedInputTypes: .indirect))
        components.set(HoverEffectComponent())
    }

    // MARK: - 平面交差判定
    // grabbed EntityがこのMirror平面を通過したか判定する。
    // ImmersiveViewのupdateループからフレームごとに呼ぶ。
    func checkCrossing(grabbedEntity: Entity, image: UIImage) {
        guard !crossedIDs.contains(grabbedEntity.id.hashValue) else { return }

        let entityPos = grabbedEntity.position(relativeTo: nil)
        let mirrorPos = position(relativeTo: nil)
        let normal    = planeNormal.normalized

        // 平面の方程式: dot(normal, point - planePoint) > 0 なら前面
        let signedDist = dot(normal, entityPos - mirrorPos)

        // 閾値内で平面を通過 = ±2cm以内
        if abs(signedDist) < 0.02 {
            crossedIDs.insert(grabbedEntity.id.hashValue)
            pulseGlow()
            onImageCrossed?(image, entityPos)
        }
    }

    // MARK: - 通過済みIDリセット (新しい画像をドラッグし直した時)
    func resetCrossed() { crossedIDs.removeAll() }

    // MARK: - 発光パルスアニメーション
    func pulseGlow() {
        // コーナーglow を一時的に明るくする
        children.filter { $0.name == "GlowCorner" }.forEach { glow in
            var t = glow.transform
            t.scale = [2.0, 2.0, 2.0]
            glow.move(to: t, relativeTo: glow.parent, duration: 0.1)
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.15) {
                var t2 = glow.transform; t2.scale = [1, 1, 1]
                glow.move(to: t2, relativeTo: glow.parent, duration: 0.2)
            }
        }
    }

    // MARK: - Hover時のガラス輝度アップ
    func setHoverGlow(active: Bool) {
        guard let glass = children.first(where: { $0.name == "MirrorGlass" }) as? ModelEntity else { return }
        var mat = UnlitMaterial()
        mat.color = .init(tint: active
            ? UIColor(red: 160/255, green: 200/255, blue: 255/255, alpha: 0.45)
            : UIColor(red: 160/255, green: 180/255, blue: 255/255, alpha: 0.18))
        glass.model?.materials = [mat]
    }
}

// MARK: - SIMD Helpers
private extension SIMD4 {
    var xyz: SIMD3<Scalar> { SIMD3(x, y, z) }
}

private extension SIMD3 where Scalar == Float {
    var normalized: SIMD3<Float> {
        let l = sqrt(x*x + y*y + z*z)
        return l > 0 ? self / l : self
    }
}
