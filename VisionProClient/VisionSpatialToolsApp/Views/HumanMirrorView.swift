import SwiftUI
import RealityKit
import ARKit

// MARK: - HumanMirrorView
// 「人間が論理ノード」アーキテクチャの中核ビュー。
// - 仮想鏡ポータル + フリップジェスチャーの両アプローチを統合
// - 全処理がオンデバイス・決定論的 (AIゼロ)

struct HumanMirrorView: View {

    @EnvironmentObject var appModel: AppModel
    @StateObject private var flipDetector = FlipGestureDetector()
    @StateObject private var grabDetector = GrabDetector()
    @State private var rootEntity         = Entity()
    @State private var mirrorEntity:      VirtualMirrorEntity?
    @State private var floatingEntities:  [Entity: UIImage] = [:]  // Entity→元画像マップ
    @State private var placedObjects:     [Mirror3DEntity]  = []
    @State private var showMirror:        Bool = false
    @State private var lastFlipPosition:  SIMD3<Float>?
    @State private var feedbackMessage:   String = ""
    @State private var showFeedback:      Bool = false

    var body: some View {
        ZStack(alignment: .bottom) {
            // MARK: - 3D空間
            RealityView { content in
                content.add(rootEntity)
                setupVirtualMirror()
                appModel.mirror3DBuilder.rootEntity = rootEntity
            } update: { _ in }
            .task { await runInteractionLoop() }

            // MARK: - HUD
            VStack(spacing: 12) {
                // フリップデバッグバー
                #if DEBUG
                Text(flipDetector.debugDescription)
                    .font(.system(size: 10, design: .monospaced))
                    .foregroundColor(.white.opacity(0.4))
                #endif

                // フィードバックメッセージ
                if showFeedback {
                    HumanMirrorFeedback(message: feedbackMessage)
                        .transition(.scale.combined(with: .opacity))
                }

                // コントロールバー
                HumanMirrorControlBar(
                    showMirror: $showMirror,
                    onToggleMirror: toggleMirror,
                    onClearAll: clearAll
                )
            }
            .padding(.bottom, 24)
            .animation(.spring(response: 0.35), value: showFeedback)
        }
    }

    // MARK: - 仮想鏡ポータルの配置
    private func setupVirtualMirror() {
        let mirror = VirtualMirrorEntity()
        // ユーザーの正面1m先、目の高さ付近に縦に配置
        mirror.position = [0, 0, -1.0]
        mirror.orientation = simd_quatf(angle: 0, axis: [0, 1, 0])
        mirror.isEnabled = false // 最初は非表示

        mirror.onImageCrossed = { [self] image, position in
            // ポータル通過 → 即時Mirror3D化 (AIゼロ、完全決定論的)
            Task { @MainActor in
                triggerMirror3D(image: image, at: position, trigger: .portal)
            }
        }

        rootEntity.addChild(mirror)
        mirrorEntity = mirror
    }

    // MARK: - ポータル ON/OFF
    private func toggleMirror() {
        showMirror.toggle()
        mirrorEntity?.isEnabled = showMirror
        if showMirror {
            mirrorEntity?.pulseGlow()
            showTemporaryFeedback("🪟 仮想鏡ポータルを配置しました。画像をドラッグして通過させてください。")
        }
    }

    // MARK: - 全削除
    private func clearAll() {
        placedObjects.forEach { $0.removeFromParent() }
        placedObjects.removeAll()
        floatingEntities.removeAll()
        mirrorEntity?.resetCrossed()
    }

    // MARK: - ハンドトラッキングループ
    private func runInteractionLoop() async {
        // FlipDetector コールバック設定
        flipDetector.onFlip = { chirality, pos in
            Task { @MainActor in
                // 掴んでいる画像があればフリップ
                if let entity = grabDetector.grabbedEntity,
                   let image  = grabDetector.grabbedImage {
                    // フリップジェスチャー → Mirror3D化
                    triggerMirror3D(image: image, at: pos, trigger: .flip)
                    // 元の浮遊Entityを削除
                    floatingEntities.removeValue(forKey: entity)
                    entity.removeFromParent()
                    grabDetector.grabbedEntity = nil
                } else {
                    showTemporaryFeedback("⚠ 画像を掴んでからフリップしてください。")
                }
            }
        }

        // ARKitセッション
        let session = ARKitSession()
        let handTracking = HandTrackingProvider()
        guard HandTrackingProvider.isSupported else { return }

        do {
            try await session.run([handTracking])
            for await update in handTracking.anchorUpdates {
                let anchor = update.anchor
                guard anchor.isTracked else { continue }

                // 両手のアンカーを収集 (簡易: 1フレーム1アンカー更新)
                let anchors = [anchor]

                await MainActor.run {
                    // フリップ検知
                    flipDetector.update(handAnchors: anchors)

                    // グラブ追跡 (浮遊中の画像Entity)
                    let candidates = Array(floatingEntities.keys)
                    grabDetector.updateGrab(
                        handAnchors: anchors,
                        candidates:  candidates,
                        images:      floatingEntities
                    )

                    // ポータル交差チェック
                    if showMirror,
                       let mirror = mirrorEntity,
                       let entity = grabDetector.grabbedEntity,
                       let image  = grabDetector.grabbedImage {
                        mirror.checkCrossing(grabbedEntity: entity, image: image)
                    }
                }
            }
        } catch {
            print("[HumanMirrorView] HandTracking error: \(error)")
        }
    }

    // MARK: - Mirror3D化の共通処理 (ポータル & フリップ 両トリガー共用)
    enum MirrorTrigger { case portal, flip }

    private func triggerMirror3D(image: UIImage, at position: SIMD3<Float>, trigger: MirrorTrigger) {
        // 鏡写し画像生成 (CGContext, 完全ローカル・ゼロレイテンシ)
        let mirroredImage = mirrorHorizontal(image)

        // Mirror3DEntity を構築
        // 人間が「通した/裏返した」 = 対称と判断 → isSymmetrical: true で固定
        let judgment = VLMJudgment(
            isSymmetrical: true,
            isFlat:        true,
            isOpaque:      true,
            category:      "object",
            axis:          "horizontal",
            confidence:    1.0,       // 人間判断なので100%
            fallbackHint:  "flip_anyway"
        )
        let config = Mirror3DEntity.Config(
            image:     image,
            judgment:  judgment,
            physWidth: 0.28,
            depth:     0.004
        )
        let entity = Mirror3DEntity.build(config: config)
        entity.position = position
        entity.position.z -= 0.05 // 少し奥に置く
        rootEntity.addChild(entity)
        placedObjects.append(entity)

        // フィードバック表示
        let msg: String
        switch trigger {
        case .portal: msg = "✨ ポータル通過 → 疑似3D化完了"
        case .flip:   msg = "↩️ フリップ検知 → 疑似3D化完了"
        }
        showTemporaryFeedback(msg)

        // 入場アニメーション (スケール0→1)
        var t = entity.transform
        t.scale = [0.01, 0.01, 0.01]
        entity.transform = t
        var t2 = entity.transform
        t2.scale = [1, 1, 1]
        entity.move(to: t2, relativeTo: entity.parent, duration: 0.35, timingFunction: .easeOut)
    }

    // MARK: - 水平鏡写し (Core Graphics, ゼロレイテンシ)
    private func mirrorHorizontal(_ image: UIImage) -> UIImage {
        let size = image.size
        UIGraphicsBeginImageContextWithOptions(size, false, image.scale)
        defer { UIGraphicsEndImageContext() }
        guard let ctx = UIGraphicsGetCurrentContext() else { return image }
        ctx.translateBy(x: size.width, y: 0)
        ctx.scaleBy(x: -1, y: 1)
        image.draw(in: CGRect(origin: .zero, size: size))
        return UIGraphicsGetImageFromCurrentImageContext() ?? image
    }

    // MARK: - 一時フィードバック表示
    private func showTemporaryFeedback(_ message: String) {
        feedbackMessage = message
        withAnimation { showFeedback = true }
        DispatchQueue.main.asyncAfter(deadline: .now() + 2.5) {
            withAnimation { showFeedback = false }
        }
    }

    // MARK: - 浮遊画像の追加 (ブラウザからドラッグ時に呼ばれる)
    func addFloatingImage(_ image: UIImage, at position: SIMD3<Float>) {
        let w: Float = 0.25
        let h = w * Float(image.size.height / image.size.width)
        let mesh = MeshResource.generatePlane(width: w, height: h)

        if let cgImg = image.cgImage,
           let texture = try? TextureResource.generate(from: cgImg, options: .init(semantic: .color)) {
            var mat = UnlitMaterial()
            mat.color = .init(texture: .init(texture))
            let entity = ModelEntity(mesh: mesh, materials: [mat])
            entity.orientation = simd_quatf(angle: .pi/2, axis: [1, 0, 0])
            entity.position = position
            entity.components.set(CollisionComponent(shapes: [
                .generateBox(width: w, height: 0.005, depth: h)
            ]))
            entity.components.set(InputTargetComponent(allowedInputTypes: [.direct, .indirect]))
            entity.components.set(HoverEffectComponent())
            rootEntity.addChild(entity)
            floatingEntities[entity] = image
        }
    }
}

// MARK: - HumanMirrorFeedback (フィードバックHUD)
struct HumanMirrorFeedback: View {
    let message: String

    var body: some View {
        HStack(spacing: 10) {
            Text(message)
                .font(.system(size: 13, weight: .semibold))
                .foregroundColor(.white)
        }
        .padding(.horizontal, 20)
        .padding(.vertical, 12)
        .background(.ultraThinMaterial)
        .clipShape(Capsule())
        .shadow(color: Color(red: 99/255, green: 102/255, blue: 241/255).opacity(0.4), radius: 12)
    }
}

// MARK: - HumanMirrorControlBar (操作バー)
struct HumanMirrorControlBar: View {
    @Binding var showMirror: Bool
    let onToggleMirror: () -> Void
    let onClearAll:     () -> Void

    var body: some View {
        HStack(spacing: 12) {
            // ポータルON/OFF
            Button(action: onToggleMirror) {
                HStack(spacing: 8) {
                    Image(systemName: showMirror ? "rectangle.on.rectangle.slash" : "rectangle.on.rectangle")
                        .font(.system(size: 14))
                    Text(showMirror ? "鏡を片付ける" : "🪟 鏡ポータル")
                        .font(.system(size: 13, weight: .semibold))
                }
                .foregroundColor(.white)
                .padding(.horizontal, 16).padding(.vertical, 10)
                .background(showMirror
                    ? Color(red: 99/255, green: 102/255, blue: 241/255).opacity(0.8)
                    : Color.white.opacity(0.12))
                .clipShape(Capsule())
            }
            .buttonStyle(.plain)

            // 使い方ヒント
            VStack(alignment: .leading, spacing: 2) {
                HStack(spacing: 6) {
                    Image(systemName: "hand.pinch").font(.system(size: 10))
                    Text("画像を掴んで鏡に通す").font(.system(size: 10))
                }
                HStack(spacing: 6) {
                    Image(systemName: "hand.raised.fingers.spread").font(.system(size: 10))
                    Text("掴んだまま手首を返す").font(.system(size: 10))
                }
            }
            .foregroundColor(.white.opacity(0.6))
            .padding(.horizontal, 12)

            Spacer()

            // 全削除
            Button(action: onClearAll) {
                Image(systemName: "trash")
                    .font(.system(size: 14))
                    .foregroundColor(.white.opacity(0.5))
                    .frame(width: 36, height: 36)
                    .background(.white.opacity(0.08))
                    .clipShape(Circle())
            }
            .buttonStyle(.plain)
        }
        .padding(.horizontal, 20)
        .padding(.vertical, 14)
        .background(.ultraThinMaterial)
        .clipShape(RoundedRectangle(cornerRadius: 20))
    }
}
