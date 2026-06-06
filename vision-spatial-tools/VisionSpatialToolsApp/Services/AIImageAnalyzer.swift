import Foundation
import UIKit
import Combine

// MARK: - AI分析メタデータ型
// AIプロバイダが返すJSON構造を定義

struct ImageAnalysisResult: Codable {
    let objectClass:  String           // "piano", "drum_kit", "sf_console"
    let description:  String
    let regions:      [InteractiveRegion]
}

struct InteractiveRegion: Codable, Identifiable {
    let id:        String              // "keyboard", "snare", "slider_1"
    let feature:   String             // ユーザーフレンドリーな名前
    let type:      TemplateType       // テンプレート種別
    let rect:      NormalizedRect     // 画像内の正規化座標 (0-1)
    let soundSet:  String             // "grand_piano", "snare_drum", "sci_fi_beep"
    let octave:    Int?               // キーボード系のみ
    let keyCount:  Int?               // キー数 (鍵盤系)
}

struct NormalizedRect: Codable {
    let x: Double; let y: Double
    let w: Double; let h: Double

    func toCGRect(in size: CGSize) -> CGRect {
        CGRect(x: x * size.width, y: y * size.height,
               width: w * size.width, height: h * size.height)
    }
}

enum TemplateType: String, Codable {
    case keyboard   = "keyboard_strip"  // 鍵盤列 - ピアノ、シンセ
    case button     = "single_surface"  // 単一打面 - ドラム、ボタン
    case slider     = "slider"          // スライダー - フェーダー
    case string     = "string"          // 弦 - ギター、ハープ
}

// MARK: - AI Image Analyzer
// ユーザー設定の外部AIプロバイダ (OpenAI / Anthropic / Gemini / ローカル) に
// 画像を送信し、インタラクティブ領域のメタデータを取得する。

@MainActor
class AIImageAnalyzer: ObservableObject {

    @Published var isAnalyzing = false
    @Published var lastResult:  ImageAnalysisResult?
    @Published var errorMessage: String?

    // ユーザー設定 (AppModel経由で注入)
    var provider: AIProvider = .openAI
    var apiKey:   String     = ""
    var localEndpoint: URL?  = nil

    enum AIProvider: String, CaseIterable {
        case openAI     = "OpenAI GPT-4o"
        case anthropic  = "Anthropic Claude"
        case gemini     = "Google Gemini"
        case local      = "Local (Ollama)"
    }

    // MARK: - 分析実行
    func analyze(image: UIImage) async -> ImageAnalysisResult? {
        isAnalyzing = true
        errorMessage = nil
        defer { isAnalyzing = false }

        guard let b64 = image.jpegData(compressionQuality: 0.85)?.base64EncodedString() else {
            errorMessage = "Image encoding failed"; return nil
        }

        // Prompt: AIに対して構造分析を要求
        let systemPrompt = """
        You are an XR interaction analyzer. Given an image, identify interactive regions.
        Return ONLY valid JSON matching this schema exactly:
        {
          "objectClass": "piano",
          "description": "Grand piano keyboard view",
          "regions": [{
            "id": "keyboard",
            "feature": "Piano keyboard",
            "type": "keyboard_strip",
            "rect": {"x":0.0,"y":0.5,"w":1.0,"h":0.5},
            "soundSet": "grand_piano",
            "octave": 4,
            "keyCount": 12
          }]
        }
        Valid types: keyboard_strip, single_surface, slider, string.
        Valid soundSets: grand_piano, electric_piano, organ, drum_kit, snare_drum, hi_hat, bass_drum, guitar, bass_guitar, sci_fi_beep, sci_fi_buzz, wood_block.
        Be precise about rect coordinates (0.0 to 1.0 relative to image).
        """

        do {
            let result: ImageAnalysisResult
            switch provider {
            case .openAI:    result = try await analyzeOpenAI(b64: b64, systemPrompt: systemPrompt)
            case .anthropic: result = try await analyzeAnthropic(b64: b64, systemPrompt: systemPrompt)
            case .gemini:    result = try await analyzeGemini(b64: b64, systemPrompt: systemPrompt)
            case .local:     result = try await analyzeLocal(b64: b64, systemPrompt: systemPrompt)
            }
            lastResult = result
            return result
        } catch {
            errorMessage = error.localizedDescription
            return nil
        }
    }

    // MARK: - OpenAI GPT-4o Vision
    private func analyzeOpenAI(b64: String, systemPrompt: String) async throws -> ImageAnalysisResult {
        let url = URL(string: "https://api.openai.com/v1/chat/completions")!
        var req = URLRequest(url: url)
        req.httpMethod = "POST"
        req.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
        req.setValue("application/json", forHTTPHeaderField: "Content-Type")

        let body: [String: Any] = [
            "model": "gpt-4o",
            "max_tokens": 800,
            "messages": [[
                "role": "user",
                "content": [
                    ["type": "text", "text": systemPrompt],
                    ["type": "image_url", "image_url": ["url": "data:image/jpeg;base64,\(b64)"]]
                ]
            ]]
        ]
        req.httpBody = try JSONSerialization.data(withJSONObject: body)
        let (data, _) = try await URLSession.shared.data(for: req)
        return try parseAIResponse(data: data, keyPath: ["choices", 0, "message", "content"])
    }

    // MARK: - Anthropic Claude
    private func analyzeAnthropic(b64: String, systemPrompt: String) async throws -> ImageAnalysisResult {
        let url = URL(string: "https://api.anthropic.com/v1/messages")!
        var req = URLRequest(url: url)
        req.httpMethod = "POST"
        req.setValue(apiKey, forHTTPHeaderField: "x-api-key")
        req.setValue("2023-06-01", forHTTPHeaderField: "anthropic-version")
        req.setValue("application/json", forHTTPHeaderField: "Content-Type")

        let body: [String: Any] = [
            "model": "claude-3-5-sonnet-20241022",
            "max_tokens": 800,
            "system": systemPrompt,
            "messages": [[
                "role": "user",
                "content": [
                    ["type": "image", "source": ["type": "base64", "media_type": "image/jpeg", "data": b64]],
                    ["type": "text", "text": "Analyze this image and return the JSON."]
                ]
            ]]
        ]
        req.httpBody = try JSONSerialization.data(withJSONObject: body)
        let (data, _) = try await URLSession.shared.data(for: req)
        return try parseAIResponse(data: data, keyPath: ["content", 0, "text"])
    }

    // MARK: - Google Gemini
    private func analyzeGemini(b64: String, systemPrompt: String) async throws -> ImageAnalysisResult {
        let url = URL(string: "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=\(apiKey)")!
        var req = URLRequest(url: url)
        req.httpMethod = "POST"
        req.setValue("application/json", forHTTPHeaderField: "Content-Type")

        let body: [String: Any] = [
            "contents": [[
                "parts": [
                    ["text": systemPrompt + "\nAnalyze this image:"],
                    ["inline_data": ["mime_type": "image/jpeg", "data": b64]]
                ]
            ]]
        ]
        req.httpBody = try JSONSerialization.data(withJSONObject: body)
        let (data, _) = try await URLSession.shared.data(for: req)
        return try parseAIResponse(data: data, keyPath: ["candidates", 0, "content", "parts", 0, "text"])
    }

    // MARK: - Local Ollama
    private func analyzeLocal(b64: String, systemPrompt: String) async throws -> ImageAnalysisResult {
        let base = localEndpoint ?? URL(string: "http://localhost:11434")!
        let url = base.appendingPathComponent("/api/generate")
        var req = URLRequest(url: url)
        req.httpMethod = "POST"
        req.setValue("application/json", forHTTPHeaderField: "Content-Type")

        let body: [String: Any] = [
            "model": "llava",
            "prompt": systemPrompt,
            "images": [b64],
            "stream": false
        ]
        req.httpBody = try JSONSerialization.data(withJSONObject: body)
        let (data, _) = try await URLSession.shared.data(for: req)
        return try parseAIResponse(data: data, keyPath: ["response"])
    }

    // MARK: - JSONパーサー (共通)
    private func parseAIResponse(data: Data, keyPath: [Any]) throws -> ImageAnalysisResult {
        guard var json = try JSONSerialization.jsonObject(with: data) as? [String: Any] else {
            throw AnalyzerError.parseError("Invalid JSON root")
        }
        // keyPathを辿って文字列を取得
        var current: Any = json
        for key in keyPath {
            if let k = key as? String, let d = current as? [String: Any] { current = d[k] ?? "" }
            else if let i = key as? Int, let a = current as? [Any] { current = a[i] }
        }
        guard let text = current as? String else { throw AnalyzerError.parseError("No text content") }

        // JSON部分を抽出
        let jsonText = extractJSON(from: text)
        guard let jsonData = jsonText.data(using: .utf8) else { throw AnalyzerError.parseError("UTF8 fail") }
        return try JSONDecoder().decode(ImageAnalysisResult.self, from: jsonData)
    }

    private func extractJSON(from text: String) -> String {
        // ```json ... ``` ブロックを抽出、なければそのまま
        if let start = text.range(of: "```json\n"), let end = text.range(of: "\n```", range: start.upperBound..<text.endIndex) {
            return String(text[start.upperBound..<end.lowerBound])
        }
        if let start = text.firstIndex(of: "{"), let end = text.lastIndex(of: "}") {
            return String(text[start...end])
        }
        return text
    }

    enum AnalyzerError: LocalizedError {
        case parseError(String)
        var errorDescription: String? { if case .parseError(let m) = self { return m }; return nil }
    }
}
