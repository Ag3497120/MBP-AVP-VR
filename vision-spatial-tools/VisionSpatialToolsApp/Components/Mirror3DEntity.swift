import RealityKit
import UIKit
import simd

// MARK: - Mirror3DEntity
// VLM判定結果に基づいて「疑似3Dオブジェクト」を構築するEntity。
//
// [対称 + thin]   → 表:元画像 / 裏:鏡写し / 側面:主要色 の薄いbox
// [対称 + thick]  → 表:元画像 / 裏:鏡写し / 側面:主要色グラデ
// [非対称 thin]   → dominantColorEdge: 表のみ + 側面に主要色帯
// [非対称 thick]  → billboard: 常にカメラ方向を向く2D板
//
// ユーザーが「裏返す」ジェスチャーをすると裏面に切り替わる。

class Mirror3DEntity: Entity {

    // MARK: - 設定
    struct Config {
        let image:      UIImage
        let judgment:   VLMJudgment
        let physWidth:  Float       // 物理横幅 (m)
        let depth:      Float       // 厚み (m) - flatなら0.004, otherwise 0.02
    }

    private let config:      Config
    private var frontPanel:  ModelEntity?
    private var backPanel:   ModelEntity?
    private var edgePanels:  [ModelEntity] = []
    private var isFrontSide: Bool = true

    // ビルボードモード (非対称thick)
    private var isBillboard = false

    // MARK: - ファクトリ
    static func build(config: Config) -> Mirror3DEntity {
        let e = Mirror3DEntity(config: config)
        e.construct()
        return e
    }

    init(config: Config) { self.config = config; super.init() }
    required init() { fatalError() }

    // MARK: - 構築
    private func construct() {
        let j = config.judgment

        switch (j.canMirror3D, j.fallback) {
        case (true, _):
            buildMirror3D()
        case (false, .dominantColorEdge):
            buildDominantEdge()
        case (false, .billboard):
            buildBillboard()
        case (false, .flipAnyway):
            buildMirror3D() // ユーザー確認後に呼ばれる
        case (false, .askUser):
            buildFlatPreview() // 仮表示
        }
    }

    // MARK: - A: 鏡写し疑似3D (対称物体)
    private func buildMirror3D() {
        let img     = config.image
        let imgFlip = mirrorImage(img, axis: config.judgment.axis)
        let w = config.physWidth
        let h = w * Float(img.size.height / img.size.width)
        let d = config.depth

        // 表面
        frontPanel = makePlane(image: img, w: w, h: h)
        frontPanel?.position = [0, 0, d/2 + 0.001]
        addChild(frontPanel!)

        // 裏面 (鏡写し)
        backPanel = makePlane(image: imgFlip, w: w, h: h)
        backPanel?.position = [0, 0, -(d/2 + 0.001)]
        backPanel?.orientation = simd_quatf(angle: .pi, axis: [0, 1, 0])
        addChild(backPanel!)

        // 側面エッジ (dominantColor)
        let dc = img.dominantColor()
        buildEdges(w: w, h: h, d: d, color: dc)

        // 衝突判定
        components.set(CollisionComponent(shapes: [
            .generateBox(width: w, height: h, depth: d)
        ]))
        components.set(InputTargetComponent(allowedInputTypes: [.direct, .indirect]))
        components.set(HoverEffectComponent())
    }

    // MARK: - B: 主要色エッジ (非対称・薄い物体)
    private func buildDominantEdge() {
        let img = config.image
        let w   = config.physWidth
        let h   = w * Float(img.size.height / img.size.width)
        let d   = max(config.depth, 0.003)

        // 表面のみ
        frontPanel = makePlane(image: img, w: w, h: h)
        frontPanel?.position = [0, 0, d/2]
        addChild(frontPanel!)

        // 裏面はグレーアウト (「この面は見えません」の表現)
        let grayImg = img.withGrayOverlay(alpha: 0.7)
        backPanel = makePlane(image: grayImg, w: w, h: h)
        backPanel?.position = [0, 0, -(d/2)]
        backPanel?.orientation = simd_quatf(angle: .pi, axis: [0, 1, 0])
        addChild(backPanel!)

        // 側面
        let dc = img.dominantColor()
        buildEdges(w: w, h: h, d: d, color: dc)

        // 非対称バッジ (オーバーレイ Entity)
        addWarningBadge(at: [w/2 + 0.02, h/2 + 0.02, d/2])

        components.set(CollisionComponent(shapes: [.generateBox(width: w, height: h, depth: d)]))
        components.set(InputTargetComponent(allowedInputTypes: [.direct, .indirect]))
    }

    // MARK: - C: ビルボード (非対称・厚い物体)
    private func buildBillboard() {
        isBillboard = true
        let img = config.image
        let w   = config.physWidth
        let h   = w * Float(img.size.height / img.size.width)

        frontPanel = makePlane(image: img, w: w, h: h)
        frontPanel?.position = [0, 0, 0]
        addChild(frontPanel!)

        // ビルボード: BillboardComponent で常にカメラ方向を向く
        components.set(BillboardComponent())

        addWarningBadge(at: [w/2 + 0.02, h/2 + 0.02, 0])

        components.set(CollisionComponent(shapes: [.generateBox(width: w, height: h, depth: 0.002)]))
        components.set(InputTargetComponent(allowedInputTypes: [.direct, .indirect]))
    }

    // MARK: - D: 仮プレビュー (confirmationダイアログ前)
    private func buildFlatPreview() {
        let img = config.image
        let w   = config.physWidth
        let h   = w * Float(img.size.height / img.size.width)
        frontPanel = makePlane(image: img, w: w, h: h)
        frontPanel?.position = [0, 0, 0]

        // 点線枠アニメーション (パルス)
        var t = frontPanel!.transform
        t.scale = [0.98, 0.98, 0.98]
        frontPanel?.move(to: t, relativeTo: frontPanel?.parent, duration: 0.8)
        addChild(frontPanel!)
    }

    // MARK: - 側面エッジ生成 (共通)
    private func buildEdges(w: Float, h: Float, d: Float, color: UIColor) {
        var mat = UnlitMaterial()
        mat.color = .init(tint: color)

        // 上下左右の4辺
        let edges: [(SIMD3<Float>, SIMD3<Float>)] = [
            ([ 0,  h/2, 0], [w, d, 0.001]),  // 上
            ([ 0, -h/2, 0], [w, d, 0.001]),  // 下
            ([ w/2, 0,  0], [0.001, h, d]),  // 右
            ([-w/2, 0,  0], [0.001, h, d]),  // 左
        ]
        for (pos, size) in edges {
            let mesh = MeshResource.generateBox(width: size.x == 0 ? 0.002 : size.x,
                                               height: size.y,
                                               depth: size.z == 0 ? d : size.z,
                                               cornerRadius: 0.001)
            let edge = ModelEntity(mesh: mesh, materials: [mat])
            edge.position = pos
            edgePanels.append(edge)
            addChild(edge)
        }
    }

    // MARK: - 警告バッジ (非対称を示す小アイコン)
    private func addWarningBadge(at pos: SIMD3<Float>) {
        let sphere = MeshResource.generateSphere(radius: 0.012)
        var mat = UnlitMaterial()
        mat.color = .init(tint: UIColor(red: 251/255, green: 191/255, blue: 36/255, alpha: 0.9))
        let badge = ModelEntity(mesh: sphere, materials: [mat])
        badge.position = pos
        badge.name = "WarningBadge"
        addChild(badge)
    }

    // MARK: - 平面生成 (UIImage → TextureResource)
    private func makePlane(image: UIImage, w: Float, h: Float) -> ModelEntity {
        let mesh = MeshResource.generatePlane(width: w, height: h)
        if let cgImg = image.cgImage,
           let texture = try? TextureResource.generate(from: cgImg, options: .init(semantic: .color)) {
            var mat = UnlitMaterial()
            mat.color = .init(texture: .init(texture))
            return ModelEntity(mesh: mesh, materials: [mat])
        }
        var mat = UnlitMaterial()
        mat.color = .init(tint: .gray)
        return ModelEntity(mesh: mesh, materials: [mat])
    }

    // MARK: - 画像ミラーリング
    private func mirrorImage(_ image: UIImage, axis: String) -> UIImage {
        let size = image.size
        UIGraphicsBeginImageContextWithOptions(size, false, image.scale)
        defer { UIGraphicsEndImageContext() }
        guard let ctx = UIGraphicsGetCurrentContext() else { return image }

        switch axis {
        case "vertical":
            ctx.translateBy(x: 0, y: size.height)
            ctx.scaleBy(x: 1, y: -1)
        case "both":
            ctx.translateBy(x: size.width, y: size.height)
            ctx.scaleBy(x: -1, y: -1)
        default: // horizontal (default for swords, etc.)
            ctx.translateBy(x: size.width, y: 0)
            ctx.scaleBy(x: -1, y: 1)
        }
        image.draw(in: CGRect(origin: .zero, size: size))
        return UIGraphicsGetImageFromCurrentImageContext() ?? image
    }

    // MARK: - 裏返しアニメーション (ジェスチャーから呼ばれる)
    func flipToBack(duration: TimeInterval = 0.4) {
        guard !isBillboard else { return }
        isFrontSide = !isFrontSide
        var t = self.transform
        let angle: Float = isFrontSide ? 0 : .pi
        t.rotation = simd_quatf(angle: angle, axis: [0, 1, 0])
        self.move(to: t, relativeTo: self.parent, duration: duration,
                  timingFunction: .easeInOut)
    }

    // MARK: - FlipAnyway実行 (非対称確認後にユーザーが了承した場合)
    func upgradeToMirror3D() {
        children.forEach { $0.removeFromParent() }
        edgePanels.removeAll()
        buildMirror3D()
    }
}

// MARK: - UIImage グレーオーバーレイ (裏面の非対称表現)
extension UIImage {
    func withGrayOverlay(alpha: CGFloat) -> UIImage {
        UIGraphicsBeginImageContextWithOptions(size, false, scale)
        defer { UIGraphicsEndImageContext() }
        draw(in: CGRect(origin: .zero, size: size))
        UIColor.black.withAlphaComponent(alpha).setFill()
        UIRectFillUsingBlendMode(CGRect(origin: .zero, size: size), .normal)
        return UIGraphicsGetImageFromCurrentImageContext() ?? self
    }
}
