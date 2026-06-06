import Combine
import SwiftUI
import RealityKit

// MARK: - Mirror3DBuilder (オーケストレーター)
// ブラウザから画像URL受信 → LocalVLM判定 → Mirror3DEntity生成 → フォールバックUI制御

@MainActor
class Mirror3DBuilder: ObservableObject {

    enum BuildPhase: Equatable {
        case idle
        case downloading
        case judging
        case building
        case awaitingConfirmation(fallback: FallbackStrategy, message: String)
        case placed(category: String, mode: String)
        case error(String)
    }

    @Published var phase: BuildPhase = .idle
    @Published var judgment: VLMJudgment?
    @Published var placedEntities: [Mirror3DEntity] = []
    @Published var showConfirmationSheet = false
    @Published var confirmationMessage   = ""
    @Published var confirmationFallback: FallbackStrategy = .billboard

    private let vlm = LocalVLMAnalyzer()
    weak var rootEntity: Entity?

    // 確認待ち中の保留データ
    private var pendingImage:    UIImage?
    private var pendingPosition: SIMD3<Float>?

    // MARK: - エントリーポイント
    func buildFrom(imageURL: String, at position: SIMD3<Float>) async {
        phase = .downloading

        // 画像ダウンロード
        guard let url = URL(string: imageURL),
              let data = try? await URLSession.shared.data(from: url).0,
              let image = UIImage(data: data) else {
            phase = .error("Download failed: \(imageURL)"); return
        }

        // VLM判定
        phase = .judging
        guard let j = await vlm.judge(image: image) else {
            phase = .error(vlm.errorMessage ?? "VLM error"); return
        }
        judgment = j
        phase = .building

        // フォールバック確認が必要か判断
        if case .askUser(let msg) = j.fallback {
            pendingImage    = image
            pendingPosition = position
            confirmationMessage = msg
            confirmationFallback = j.fallback
            phase = .awaitingConfirmation(fallback: j.fallback, message: msg)
            showConfirmationSheet = true
            return
        }

        // 直接ビルド
        placeEntity(image: image, judgment: j, at: position)
    }

    // MARK: - ユーザー確認後の処理
    func userApprovedFlipAnyway() {
        guard let img = pendingImage, let pos = pendingPosition, var j = judgment else { return }
        // flipAnywayとして強制ビルド
        j = VLMJudgment(
            isSymmetrical: true, isFlat: j.isFlat, isOpaque: j.isOpaque,
            category: j.category, axis: j.axis,
            confidence: 0.5, fallbackHint: "flip_anyway"
        )
        placeEntity(image: img, judgment: j, at: pos)
        showConfirmationSheet = false
    }

    func userChoseBillboard() {
        guard let img = pendingImage, let pos = pendingPosition, let j = judgment else { return }
        let bj = VLMJudgment(
            isSymmetrical: false, isFlat: false, isOpaque: j.isOpaque,
            category: j.category, axis: "none",
            confidence: j.confidence, fallbackHint: "billboard_only"
        )
        placeEntity(image: img, judgment: bj, at: pos)
        showConfirmationSheet = false
    }

    func userCancelled() {
        pendingImage = nil; pendingPosition = nil
        phase = .idle; showConfirmationSheet = false
    }

    // MARK: - Entity生成 & 配置
    private func placeEntity(image: UIImage, judgment: VLMJudgment, at pos: SIMD3<Float>) {
        let depth: Float = judgment.isFlat ? 0.004 : 0.025
        let cfg = Mirror3DEntity.Config(
            image: image, judgment: judgment,
            physWidth: 0.28, depth: depth
        )
        let entity = Mirror3DEntity.build(config: cfg)
        entity.position = pos
        entity.name = "Mirror3D_\(judgment.category)"
        rootEntity?.addChild(entity)
        placedEntities.append(entity)

        let mode = judgment.canMirror3D ? "Mirror3D" :
                   (judgment.fallback == .billboard ? "Billboard" : "DominantEdge")
        phase = .placed(category: judgment.category, mode: mode)
    }

    // MARK: - 全削除
    func clearAll() {
        placedEntities.forEach { $0.removeFromParent() }
        placedEntities.removeAll()
        phase = .idle
        judgment = nil
    }
}

// MARK: - Mirror3DStatusView (ImmersiveView上のオーバーレイUI)
struct Mirror3DStatusView: View {
    @ObservedObject var builder: Mirror3DBuilder
    @ObservedObject var vlm:     LocalVLMAnalyzer

    var body: some View {
        Group {
            switch builder.phase {
            case .idle:
                EmptyView()
            case .downloading, .judging, .building:
                activeCard
            case .awaitingConfirmation(let fallback, let msg):
                confirmCard(fallback: fallback, message: msg)
            case .placed(let cat, let mode):
                placedCard(category: cat, mode: mode)
            case .error(let msg):
                errorCard(message: msg)
            }
        }
        .animation(.spring(response: 0.3), value: builder.phase == .idle)
    }

    // ── ローディングカード ─────────────────────────────────────
    private var activeCard: some View {
        HStack(spacing: 14) {
            ProgressView().tint(.white).scaleEffect(0.9)
            VStack(alignment: .leading, spacing: 3) {
                Text(phaseLabel).font(.system(size: 14, weight: .semibold)).foregroundColor(.white)
                if builder.phase == .judging {
                    Text("gemma4:e2b analyzing…")
                        .font(.system(size: 11, design: .monospaced))
                        .foregroundColor(.white.opacity(0.7))
                }
            }
        }
        .padding(.horizontal, 18).padding(.vertical, 12)
        .background(.ultraThinMaterial)
        .clipShape(Capsule())
        .shadow(radius: 8)
    }

    private var phaseLabel: String {
        switch builder.phase {
        case .downloading: return "Downloading image…"
        case .judging:     return "Analyzing symmetry…"
        case .building:    return "Building pseudo-3D…"
        default: return ""
        }
    }

    // ── フォールバック確認カード ───────────────────────────────
    private func confirmCard(fallback: FallbackStrategy, message: String) -> some View {
        VStack(spacing: 14) {
            // 判定結果バッジ
            HStack(spacing: 8) {
                Image(systemName: "exclamationmark.triangle.fill")
                    .foregroundColor(.orange)
                Text("非対称オブジェクト").font(.system(size: 14, weight: .bold)).foregroundColor(.white)
            }

            // VLM判定結果表示
            if let j = builder.judgment {
                HStack(spacing: 12) {
                    judgmentBadge(label: "Category", value: j.category)
                    judgmentBadge(label: "Confidence", value: String(format: "%.0f%%", j.confidence * 100))
                    judgmentBadge(label: "Axis", value: j.axis)
                }
            }

            Text(message)
                .font(.system(size: 12))
                .foregroundColor(.white.opacity(0.85))
                .multilineTextAlignment(.center)
                .frame(maxWidth: 340)

            Divider().background(.white.opacity(0.3))

            // 選択肢ボタン
            HStack(spacing: 10) {
                // 鏡写し (強制)
                actionButton(
                    icon: "arrow.left.arrow.right",
                    label: "鏡写しで3D化",
                    sublabel: "裏面は反転画像",
                    color: Color(red: 99/255, green: 102/255, blue: 241/255)
                ) { builder.userApprovedFlipAnyway() }

                // ビルボード
                actionButton(
                    icon: "rectangle.on.rectangle",
                    label: "2D表示",
                    sublabel: "カメラを向く",
                    color: Color(red: 107/255, green: 114/255, blue: 128/255)
                ) { builder.userChoseBillboard() }

                // キャンセル
                actionButton(
                    icon: "xmark",
                    label: "キャンセル",
                    sublabel: "",
                    color: Color(red: 75/255, green: 85/255, blue: 99/255)
                ) { builder.userCancelled() }
            }
        }
        .padding(20)
        .background(.ultraThinMaterial)
        .clipShape(RoundedRectangle(cornerRadius: 20))
        .overlay(RoundedRectangle(cornerRadius: 20).stroke(.orange.opacity(0.5), lineWidth: 1.5))
        .shadow(color: .orange.opacity(0.2), radius: 20)
        .frame(maxWidth: 420)
    }

    // ── 配置完了カード ─────────────────────────────────────────
    private func placedCard(category: String, mode: String) -> some View {
        HStack(spacing: 12) {
            Image(systemName: modeIcon(mode))
                .font(.system(size: 16))
                .foregroundStyle(modeGrad(mode))
            VStack(alignment: .leading, spacing: 2) {
                Text("\(category.capitalized) placed").font(.system(size: 13, weight: .semibold)).foregroundColor(.white)
                Text(modeDescription(mode)).font(.system(size: 10)).foregroundColor(.white.opacity(0.6))
            }
            Spacer()
            Button { builder.clearAll() } label: {
                Image(systemName: "trash").font(.system(size: 12)).foregroundColor(.white.opacity(0.5))
            }.buttonStyle(.plain)
        }
        .padding(.horizontal, 16).padding(.vertical, 10)
        .background(.ultraThinMaterial)
        .clipShape(Capsule())
        .transition(.scale.combined(with: .opacity))
    }

    // ── エラーカード ───────────────────────────────────────────
    private func errorCard(message: String) -> some View {
        HStack(spacing: 10) {
            Image(systemName: "exclamationmark.triangle.fill").foregroundColor(.red)
            Text(message).font(.system(size: 12)).foregroundColor(.white).lineLimit(2)
            Button { builder.phase = .idle } label: {
                Image(systemName: "xmark").foregroundColor(.white.opacity(0.5))
            }.buttonStyle(.plain)
        }
        .padding(.horizontal, 14).padding(.vertical, 10)
        .background(.ultraThinMaterial)
        .clipShape(Capsule())
    }

    // MARK: - Sub components
    private func judgmentBadge(label: String, value: String) -> some View {
        VStack(spacing: 2) {
            Text(label).font(.system(size: 9, weight: .medium)).foregroundColor(.white.opacity(0.5))
            Text(value).font(.system(size: 12, weight: .bold)).foregroundColor(.white)
        }
        .padding(.horizontal, 10).padding(.vertical, 6)
        .background(.white.opacity(0.08))
        .clipShape(RoundedRectangle(cornerRadius: 8))
    }

    private func actionButton(icon: String, label: String, sublabel: String, color: Color, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            VStack(spacing: 6) {
                Image(systemName: icon).font(.system(size: 18)).foregroundColor(.white)
                Text(label).font(.system(size: 11, weight: .semibold)).foregroundColor(.white)
                if !sublabel.isEmpty {
                    Text(sublabel).font(.system(size: 9)).foregroundColor(.white.opacity(0.6))
                }
            }
            .frame(width: 100, height: 76)
            .background(color.opacity(0.6))
            .clipShape(RoundedRectangle(cornerRadius: 12))
        }.buttonStyle(.plain)
    }

    private func modeIcon(_ mode: String) -> String {
        switch mode {
        case "Mirror3D":      return "cube.fill"
        case "Billboard":     return "rectangle.on.rectangle"
        case "DominantEdge":  return "cube.transparent"
        default:              return "questionmark"
        }
    }

    private func modeGrad(_ mode: String) -> AnyShapeStyle {
        switch mode {
        case "Mirror3D":
            return AnyShapeStyle(LinearGradient(colors: [Color(red: 99/255, green: 102/255, blue: 241/255),
                                                         Color(red: 168/255, green: 85/255, blue: 247/255)],
                                               startPoint: .leading, endPoint: .trailing))
        case "DominantEdge":
            return AnyShapeStyle(LinearGradient(colors: [Color(red: 34/255, green: 197/255, blue: 94/255),
                                                         Color(red: 6/255, green: 182/255, blue: 212/255)],
                                               startPoint: .leading, endPoint: .trailing))
        default:
            return AnyShapeStyle(Color(red: 107/255, green: 114/255, blue: 128/255))
        }
    }

    private func modeDescription(_ mode: String) -> String {
        switch mode {
        case "Mirror3D":     return "表:元画像 / 裏:鏡写し / 側面:主要色"
        case "Billboard":    return "常にカメラ方向を向く2D表示"
        case "DominantEdge": return "表:元画像 / 側面:主要色 / 裏:不透明"
        default:             return mode
        }
    }
}

// MARK: - VLM設定ビュー (AIプロバイダ設定から起動)
struct VLMSettingsView: View {
    @ObservedObject var vlm: LocalVLMAnalyzer
    @State private var endpointText = "http://localhost:11434"

    var body: some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("Local VLM Settings")
                .font(.system(size: 16, weight: .bold))
                .foregroundColor(.white)

            // モデル選択
            VStack(alignment: .leading, spacing: 6) {
                Text("Model").font(.system(size: 11)).foregroundColor(.white.opacity(0.6))
                Picker("", selection: $vlm.model) {
                    Text("gemma4:e2b").tag("gemma4:e2b")
                    Text("llava:7b").tag("llava:7b")
                    Text("llava:13b").tag("llava:13b")
                    Text("moondream").tag("moondream")
                    Text("gemma3:4b").tag("gemma3:4b")
                }
                .pickerStyle(.segmented)
            }

            // エンドポイント
            VStack(alignment: .leading, spacing: 6) {
                Text("Ollama Endpoint").font(.system(size: 11)).foregroundColor(.white.opacity(0.6))
                HStack {
                    TextField("http://localhost:11434", text: $endpointText)
                        .font(.system(size: 12, design: .monospaced))
                        .textFieldStyle(.plain)
                        .foregroundColor(.white)
                        .autocorrectionDisabled()
                    Button("Apply") {
                        vlm.endpoint = URL(string: endpointText) ?? vlm.endpoint
                    }
                    .font(.system(size: 11, weight: .semibold))
                    .foregroundColor(Color(red: 99/255, green: 102/255, blue: 241/255))
                    .buttonStyle(.plain)
                }
                .padding(10)
                .background(.white.opacity(0.08))
                .clipShape(RoundedRectangle(cornerRadius: 8))
            }

            // 使い方ガイド
            VStack(alignment: .leading, spacing: 4) {
                Text("使い方").font(.system(size: 11, weight: .semibold)).foregroundColor(.white.opacity(0.7))
                Text("1. Ollamaをインストール: brew install ollama\n2. モデルを取得: ollama pull gemma4:e2b\n3. ブラウザで画像をタッチ → 疑似3D化")
                    .font(.system(size: 10, design: .monospaced))
                    .foregroundColor(.white.opacity(0.5))
                    .lineSpacing(3)
            }
            .padding(10)
            .background(.white.opacity(0.06))
            .clipShape(RoundedRectangle(cornerRadius: 8))
        }
        .padding(20)
        .background(.ultraThinMaterial)
        .clipShape(RoundedRectangle(cornerRadius: 16))
    }
}
