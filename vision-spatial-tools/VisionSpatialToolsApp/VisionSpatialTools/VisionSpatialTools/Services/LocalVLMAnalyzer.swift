import Combine
import Foundation
import UIKit

// MARK: - LocalVLMAnalyzer
// Ollama経由でローカルVLM (gemma4:e2b / llava / moondream) を実行し、
// 画像に対して「対称性・薄さ・カテゴリ」を判定する。
// AIには生成をさせず、分類・判定（True/False + カテゴリ文字列）のみを行わせる。

struct VLMJudgment: Codable {
    let isSymmetrical: Bool     // 左右/前後対称か
    let isFlat:        Bool     // 薄い物体か（刀・コイン・板）
    let isOpaque:      Bool     // 不透明か（透明な物体への特殊処理用）
    let category:      String   // "sword", "coin", "mug", "sign", "bottle", etc.
    let axis:          String   // "horizontal", "vertical", "both", "none"
    let confidence:    Double   // 0.0 - 1.0
    let fallbackHint:  String   // 非対称時のヒント: "use_dominant_color", "flip_anyway", "billboard_only"

    // 導出プロパティ
    var canMirror3D: Bool { isSymmetrical && confidence > 0.6 }
    var fallback: FallbackStrategy { FallbackStrategy.from(hint: fallbackHint, isFlat: isFlat) }
}

// MARK: - フォールバック戦略
enum FallbackStrategy: Equatable {
    /// 非対称だが薄い → 断面を支配色で塗るだけで十分 (看板、スマホ)
    case dominantColorEdge
    /// 非対称で厚い → ビルボード化(常にカメラ方向を向く2D)
    case billboard
    /// 対称ではないが「裏返しでも違和感が許容される」→ そのまま鏡写し
    case flipAnyway
    /// 自信不足 → ユーザーに確認ダイアログ
    case askUser(message: String)

    static func from(hint: String, isFlat: Bool) -> FallbackStrategy {
        switch hint {
        case "use_dominant_color": return .dominantColorEdge
        case "billboard_only":     return .billboard
        case "flip_anyway":        return .flipAnyway
        default:                   return isFlat ? .dominantColorEdge : .billboard
        }
    }

    var displayMessage: String {
        switch self {
        case .dominantColorEdge:
            return "この物体は非対称です。側面に主要色を使って疑似3D化します。"
        case .billboard:
            return "この物体は立体化が難しいため、常にあなたの方を向く2Dとして表示します。"
        case .flipAnyway:
            return "完全な対称ではありませんが、鏡写しで3D化します。"
        case .askUser(let msg):
            return msg
        }
    }

    var icon: String {
        switch self {
        case .dominantColorEdge: return "cube.fill"
        case .billboard:         return "rectangle.on.rectangle"
        case .flipAnyway:        return "arrow.left.arrow.right"
        case .askUser:           return "questionmark.circle"
        }
    }
}

// MARK: - LocalVLMAnalyzer
@MainActor
class LocalVLMAnalyzer: ObservableObject {

    @Published var isAnalyzing = false
    @Published var lastJudgment: VLMJudgment?
    @Published var errorMessage: String?

    // Ollama エンドポイント (デフォルト: localhost)
    var endpoint: URL = URL(string: "http://localhost:11434")!

    // 使用モデル: gemma4:e2b が最軽量。llava, moondream も対応
    var model: String = "gemma4:e2b"

    // MARK: - 判定実行
    func judge(image: UIImage) async -> VLMJudgment? {
        isAnalyzing = true
        errorMessage = nil
        defer { isAnalyzing = false }

        guard let b64 = image.jpegData(compressionQuality: 0.80)?.base64EncodedString() else {
            errorMessage = "Image encoding failed"; return nil
        }

        let prompt = buildPrompt()

        do {
            let raw = try await callOllama(model: model, prompt: prompt, image: b64)
            let judgment = try parseJudgment(from: raw)
            lastJudgment = judgment
            return judgment
        } catch {
            // Ollamaが起動していない場合はシンプルなフォールバック
            errorMessage = "Local AI unavailable: \(error.localizedDescription)"
            return heuristicFallback(image: image)
        }
    }

    // MARK: - Ollamaプロンプト (日英両対応)
    private func buildPrompt() -> String {
        return """
        Analyze this image and respond with ONLY a JSON object (no markdown, no explanation):
        {
          "isSymmetrical": true,
          "isFlat": false,
          "isOpaque": true,
          "category": "sword",
          "axis": "horizontal",
          "confidence": 0.92,
          "fallbackHint": "flip_anyway"
        }

        Rules:
        - isSymmetrical: true if the object looks the same mirrored left-right or front-back
        - isFlat: true if the object is thin (blade, coin, card, phone, book)
        - axis: "horizontal" (left-right), "vertical" (top-bottom), "both", or "none"
        - confidence: your confidence 0.0-1.0
        - fallbackHint must be one of: "use_dominant_color", "billboard_only", "flip_anyway"
          - use_dominant_color: flat but asymmetric (sign, phone screen)
          - billboard_only: thick and asymmetric (mug, teapot)
          - flip_anyway: close enough to symmetric
        - category: single English word (sword, coin, bottle, mug, book, phone, shield, key, etc.)
        """
    }

    // MARK: - Ollama API 呼び出し
    private func callOllama(model: String, prompt: String, image: String) async throws -> String {
        let url = endpoint.appendingPathComponent("/api/generate")
        var req = URLRequest(url: url, timeoutInterval: 30)
        req.httpMethod = "POST"
        req.setValue("application/json", forHTTPHeaderField: "Content-Type")

        let body: [String: Any] = [
            "model":  model,
            "prompt": prompt,
            "images": [image],
            "stream": false,
            "options": ["temperature": 0.1, "top_p": 0.9] // 判定タスクは低温度で確定的に
        ]
        req.httpBody = try JSONSerialization.data(withJSONObject: body)

        let (data, resp) = try await URLSession.shared.data(for: req)
        guard let http = resp as? HTTPURLResponse, http.statusCode == 200 else {
            throw VLMError.serverError
        }
        guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
              let response = json["response"] as? String else {
            throw VLMError.parseError
        }
        return response
    }

    // MARK: - JSON パース
    private func parseJudgment(from text: String) throws -> VLMJudgment {
        // JSON部分を抽出
        let jsonStr: String
        if let s = text.firstIndex(of: "{"), let e = text.lastIndex(of: "}") {
            jsonStr = String(text[s...e])
        } else {
            throw VLMError.parseError
        }
        guard let data = jsonStr.data(using: .utf8) else { throw VLMError.parseError }
        let decoder = JSONDecoder()
        decoder.keyDecodingStrategy = .convertFromSnakeCase
        return try decoder.decode(VLMJudgment.self, from: data)
    }

    // MARK: - ヒューリスティックフォールバック (Ollama未起動時)
    // 画像のアスペクト比と主要色の単純分析だけで暫定判定
    private func heuristicFallback(image: UIImage) -> VLMJudgment {
        let ratio = image.size.width / image.size.height
        let isFlat = ratio > 3.0 || ratio < 0.33 // 極端に細長い = flat
        return VLMJudgment(
            isSymmetrical: false,
            isFlat: isFlat,
            isOpaque: true,
            category: "unknown",
            axis: "none",
            confidence: 0.3,
            fallbackHint: isFlat ? "use_dominant_color" : "billboard_only"
        )
    }

    enum VLMError: Error {
        case serverError, parseError
    }
}

// MARK: - UIImage 主要色抽出 (Mirror3DEntityで使用)
extension UIImage {
    func dominantColor() -> UIColor {
        guard let cgImage = self.cgImage else { return .gray }
        let width = 8; let height = 8
        guard let ctx = CGContext(
            data: nil, width: width, height: height,
            bitsPerComponent: 8, bytesPerRow: width * 4,
            space: CGColorSpaceCreateDeviceRGB(),
            bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue
        ) else { return .gray }
        ctx.draw(cgImage, in: CGRect(x: 0, y: 0, width: width, height: height))
        guard let data = ctx.data else { return .gray }
        let ptr = data.bindMemory(to: UInt8.self, capacity: width * height * 4)
        var r = 0.0, g = 0.0, b = 0.0
        let count = Double(width * height)
        for i in 0..<(width * height) {
            r += Double(ptr[i*4+0]); g += Double(ptr[i*4+1]); b += Double(ptr[i*4+2])
        }
        return UIColor(red: r/count/255, green: g/count/255, blue: b/count/255, alpha: 1)
    }
}
