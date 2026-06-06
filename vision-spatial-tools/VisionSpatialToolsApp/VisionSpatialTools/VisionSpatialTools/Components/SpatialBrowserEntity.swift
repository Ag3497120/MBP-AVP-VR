import RealityKit
import SwiftUI

// MARK: - SpatialBrowserEntity
// verantyx-browserのダークサイバーパンク美学をvisionOS 3D空間に移植。
// sunset gradient + ガラスモーフィズム + タブ管理 + Bridge IPC設計を踏襲。

class SpatialBrowserEntity: Entity {

    // MARK: - Layout (60cm × 42cm パネル)
    let panelWidth:  Float = 0.60
    let panelHeight: Float = 0.42

    // MARK: - verantyx sunset palette (openclaude_ui.rsより移植)
    static let accentColor   = UIColor(red: 240/255, green: 148/255, blue: 100/255, alpha: 1)   // #F09464
    static let creamColor    = UIColor(red: 220/255, green: 195/255, blue: 170/255, alpha: 1)   // #DCC3AA
    static let borderColor   = UIColor(red: 100/255, green: 80/255,  blue: 65/255,  alpha: 0.9) // #644041
    static let bgDeepColor   = UIColor(red: 15/255,  green: 23/255,  blue: 42/255,  alpha: 0.97)// #0F172A
    static let bgCardColor   = UIColor(red: 30/255,  green: 41/255,  blue: 59/255,  alpha: 0.95)// #1E293B
    static let glowColor     = UIColor(red: 217/255, green: 119/255, blue: 87/255,  alpha: 0.4) // sunset glow

    // MARK: - Attachment ID (RealityView Attachment用)
    let attachmentID: String = "VxBrowser_\(UUID().uuidString)"

    // MARK: - Nav button entity map
    private var navButtonMap: [String: Entity] = [:]

    // MARK: - Init
    required init() {
        super.init()
        buildPanel()
        buildURLBar()
        buildNavButtons()
        buildTabBar()
        buildInteractiveOverlay()
    }

    // MARK: - Panel (ダークガラス本体)
    private func buildPanel() {
        let mesh = MeshResource.generateBox(
            width: panelWidth, height: panelHeight,
            depth: 0.008, cornerRadius: 0.014
        )
        var mat = PhysicallyBasedMaterial()
        mat.baseColor  = .init(tint: Self.bgDeepColor)
        mat.metallic   = 0.12
        mat.roughness  = 0.10
        mat.blending   = .transparent(opacity: .init(floatLiteral: 0.97))

        let panel = ModelEntity(mesh: mesh, materials: [mat])
        panel.name = "VxPanel"
        panel.components.set(CollisionComponent(shapes: [
            .generateBox(width: panelWidth, height: panelHeight, depth: 0.008)
        ]))
        panel.components.set(InputTargetComponent(allowedInputTypes: [.indirect, .direct]))
        panel.components.set(HoverEffectComponent())
        addChild(panel)

        // sunset accent border (細いリング)
        let borderMesh = MeshResource.generateBox(
            width: panelWidth + 0.002, height: panelHeight + 0.002,
            depth: 0.006, cornerRadius: 0.015
        )
        var borderMat = UnlitMaterial()
        borderMat.color = .init(tint: Self.glowColor)
        let border = ModelEntity(mesh: borderMesh, materials: [borderMat])
        border.position = [0, 0, -0.001]
        addChild(border)
    }

    // MARK: - URL Bar (sunset accent背景)
    private func buildURLBar() {
        let barMesh = MeshResource.generateBox(
            width: panelWidth - 0.10, height: 0.034,
            depth: 0.010, cornerRadius: 0.008
        )
        var barMat = PhysicallyBasedMaterial()
        barMat.baseColor = .init(tint: Self.bgCardColor)
        barMat.roughness = 0.85

        let bar = ModelEntity(mesh: barMesh, materials: [barMat])
        bar.position = [0.02, panelHeight/2 - 0.028, 0.005]
        bar.name = "URLBar"
        addChild(bar)
    }

    // MARK: - Nav Buttons (戻る/進む/リロード/ホーム)
    private func buildNavButtons() {
        let defs: [(String, Float)] = [
            ("back",    -(panelWidth/2) + 0.025),
            ("forward", -(panelWidth/2) + 0.058),
            ("reload",  -(panelWidth/2) + 0.091),
            ("home",    -(panelWidth/2) + 0.124),
        ]
        for (id, x) in defs {
            let btn = buildNavButton(id: id, x: x)
            navButtonMap[id] = btn
            addChild(btn)
        }
    }

    private func buildNavButton(id: String, x: Float) -> Entity {
        let sphere = MeshResource.generateSphere(radius: 0.011)
        var mat = PhysicallyBasedMaterial()
        mat.baseColor = .init(tint: Self.bgCardColor)
        mat.metallic  = 0.5
        mat.roughness = 0.3
        let model = ModelEntity(mesh: sphere, materials: [mat])

        let container = Entity()
        container.addChild(model)
        container.position = [x, panelHeight/2 - 0.028, 0.006]
        container.name = "NavBtn_\(id)"
        container.components.set(InputTargetComponent(allowedInputTypes: .indirect))
        container.components.set(CollisionComponent(shapes: [.generateSphere(radius: 0.013)]))
        container.components.set(HoverEffectComponent())
        return container
    }

    // MARK: - Tab Bar (マルチタブ表示エリア)
    private func buildTabBar() {
        let barMesh = MeshResource.generateBox(
            width: panelWidth, height: 0.022,
            depth: 0.007, cornerRadius: 0.004
        )
        var mat = PhysicallyBasedMaterial()
        mat.baseColor = .init(tint: Self.bgCardColor)
        mat.roughness = 0.9

        let tabBar = ModelEntity(mesh: barMesh, materials: [mat])
        tabBar.position = [0, panelHeight/2 - 0.057, 0.004]
        tabBar.name = "TabBar"
        addChild(tabBar)

        // accent line (sunset gradient下線)
        let lineMesh = MeshResource.generateBox(
            width: panelWidth * 0.3, height: 0.002, depth: 0.008
        )
        var lineMat = UnlitMaterial()
        lineMat.color = .init(tint: Self.accentColor)
        let line = ModelEntity(mesh: lineMesh, materials: [lineMat])
        line.position = [-(panelWidth/2) + panelWidth * 0.15, panelHeight/2 - 0.068, 0.005]
        addChild(line)
    }

    // MARK: - Interactive Overlay (WebView領域)
    private func buildInteractiveOverlay() {
        let h: Float = panelHeight - 0.090
        let mesh = MeshResource.generatePlane(width: panelWidth - 0.01, height: h)
        var mat = UnlitMaterial()
        mat.color = .init(tint: UIColor.clear)

        let overlay = ModelEntity(mesh: mesh, materials: [mat])
        overlay.position = [0, -0.020, 0.005]
        overlay.orientation = simd_quatf(angle: .pi/2, axis: [1,0,0])
        overlay.name = "WebOverlay"
        overlay.components.set(InputTargetComponent(allowedInputTypes: [.indirect,.direct]))
        overlay.components.set(CollisionComponent(shapes: [
            .generateBox(width: panelWidth - 0.01, height: 0.001, depth: h)
        ]))
        addChild(overlay)
    }

    // MARK: - Public API

    func navButtonName(for entity: Entity) -> String? {
        for (name, btn) in navButtonMap {
            if btn.id == entity.id || entity.parent?.id == btn.id { return name }
        }
        return nil
    }

    func handleNavButtonTap(_ name: String) {
        print("[VxBrowser] nav: \(name)")
    }

    /// パネル上のタップ位置 → WebView正規化座標 (0...1)
    func toWebPoint(_ localPos: SIMD3<Float>) -> CGPoint {
        let x = CGFloat((localPos.x + panelWidth/2) / panelWidth).clamped(0...1)
        let y = CGFloat(1 - (localPos.y + panelHeight/2) / panelHeight).clamped(0...1)
        return CGPoint(x: x, y: y)
    }

    // Pulse animation on key press
    func pulseAccent() {
        Task {
            try? await Task.sleep(for: .milliseconds(80))
        }
    }
}

private extension CGFloat {
    func clamped(_ r: ClosedRange<CGFloat>) -> CGFloat { Swift.min(Swift.max(self, r.lowerBound), r.upperBound) }
}
