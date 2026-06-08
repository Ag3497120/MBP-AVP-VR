import Foundation
import UIKit
import RealityKit
import Combine

// MARK: - SpatialObjectBuilder
// 「ブラウザ画像Pinch → AI分析 → テンプレート適用 → 空間配置」の
// 全フローを管理するオーケストレーター。

@MainActor
class SpatialObjectBuilder: ObservableObject {

    // MARK: - 状態
    enum BuildState: Equatable {
        case idle
        case extracting(url: String)
        case analyzing
        case building
        case placed(count: Int)
        case error(String)
    }

    @Published var state:              BuildState = .idle
    @Published var generatingProgress: Double     = 0
    @Published var analysisResult:     ImageAnalysisResult?
    @Published var placedEntities:     [InteractiveObjectEntity] = []

    // 依存サービス
    let analyzer = AIImageAnalyzer()
    weak var rootEntity: Entity?

    // MARK: - エントリーポイント
    // WKWebViewから抽出した画像URL文字列を受け取って処理開始
    func buildFromImageURL(_ urlString: String, placementPosition: SIMD3<Float>) async {
        state = .extracting(url: urlString)
        generatingProgress = 0.1

        // 1. 画像ダウンロード
        guard let url = URL(string: urlString),
              let image = await downloadImage(url: url) else {
            state = .error("Image download failed: \(urlString)")
            return
        }
        generatingProgress = 0.3

        // 2. AI分析
        state = .analyzing
        guard let result = await analyzer.analyze(image: image) else {
            state = .error(analyzer.errorMessage ?? "AI analysis failed")
            return
        }
        analysisResult = result
        generatingProgress = 0.6

        // 3. テンプレート適用 & 配置
        state = .building
        await placeObjects(result: result, sourceImage: image, basePosition: placementPosition)
        generatingProgress = 1.0
        state = .placed(count: result.regions.count)
    }

    // MARK: - 画像ダウンロード
    private func downloadImage(url: URL) async -> UIImage? {
        do {
            let (data, _) = try await URLSession.shared.data(from: url)
            return UIImage(data: data)
        } catch {
            return nil
        }
    }

    // MARK: - オブジェクト配置
    private func placeObjects(
        result: ImageAnalysisResult,
        sourceImage: UIImage,
        basePosition: SIMD3<Float>
    ) async {
        guard let root = rootEntity else { return }

        let totalRegions = result.regions.count
        for (idx, region) in result.regions.enumerated() {

            // 領域画像をクロップ
            let cropRect = region.rect.toCGRect(in: sourceImage.size)
            guard let croppedImage = cropImage(sourceImage, rect: cropRect) else { continue }

            // InteractiveObjectEntity を構築
            let entity = InteractiveObjectEntity.build(
                region: region,
                image: croppedImage,
                scaleM: 0.35
            )

            // 複数regionは横に並べる
            let offsetX = Float(idx - totalRegions / 2) * 0.45
            entity.position = basePosition + SIMD3<Float>(offsetX, 0.05, 0)
            entity.name = "Interactive_\(region.id)"

            root.addChild(entity)
            placedEntities.append(entity)

            // ビルド演出: 少し間を置きながら出現
            try? await Task.sleep(for: .milliseconds(200))
        }
    }

    // MARK: - 画像クロップ
    private func cropImage(_ image: UIImage, rect: CGRect) -> UIImage? {
        guard let cg = image.cgImage else { return nil }
        let scale  = image.scale
        let scaled = CGRect(
            x: rect.origin.x * scale,
            y: rect.origin.y * scale,
            width: rect.size.width * scale,
            height: rect.size.height * scale
        )
        guard let cropped = cg.cropping(to: scaled) else { return nil }
        return UIImage(cgImage: cropped, scale: scale, orientation: image.imageOrientation)
    }

    // MARK: - 配置済みエンティティを全削除
    func clearAll() {
        for e in placedEntities { e.removeFromParent() }
        placedEntities.removeAll()
        analysisResult = nil
        state = .idle
        generatingProgress = 0
    }
}

// MARK: - BuildStateの説明テキスト (UI表示用)
extension SpatialObjectBuilder.BuildState {
    var displayText: String {
        switch self {
        case .idle:                  return ""
        case .extracting(let url):  return "Extracting image…\n\(url)"
        case .analyzing:             return "Analyzing with AI…"
        case .building:              return "Building interactive objects…"
        case .placed(let count):     return "✓ \(count) interactive zone\(count > 1 ? "s" : "") placed"
        case .error(let msg):        return "⚠ \(msg)"
        }
    }

    var isActive: Bool {
        switch self {
        case .idle, .placed, .error: return false
        default: return true
        }
    }
}
