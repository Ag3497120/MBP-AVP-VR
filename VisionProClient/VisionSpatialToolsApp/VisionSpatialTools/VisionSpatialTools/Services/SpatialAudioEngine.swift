import Combine
import AVFoundation
import Foundation

// MARK: - SpatialAudioEngine
// AVAudioEngine ベースの音源管理。
// 事前定義されたsoundSetに対してサンプラーを起動し、
// ノート番号 (MIDI) または UV座標から音程を計算して鳴らす。

@MainActor
class SpatialAudioEngine: ObservableObject {

    static let shared = SpatialAudioEngine()

    private let engine   = AVAudioEngine()
    private let sampler  = AVAudioUnitSampler()
    private var reverb   = AVAudioUnitReverb()
    private var isReady  = false

    // 現在ロード中のsoundSet
    private(set) var currentSoundSet: String = ""

    // soundSet → General MIDI program番号 / バンク
    private static let soundSetMap: [String: (bank: UInt8, program: UInt8)] = [
        "grand_piano":    (0,  0),
        "electric_piano": (0,  4),
        "organ":          (0, 16),
        "guitar":         (0, 24),
        "bass_guitar":    (0, 33),
        "drum_kit":       (128, 0),
        "snare_drum":     (128, 0),
        "hi_hat":         (128, 0),
        "bass_drum":      (128, 0),
        "sci_fi_beep":    (0, 98),
        "sci_fi_buzz":    (0, 89),
        "wood_block":     (0, 115),
    ]

    // soundSet → 打楽器MIDI note (GM percussion map)
    private static let percussionMap: [String: UInt8] = [
        "bass_drum":  36,
        "snare_drum": 38,
        "hi_hat":     42,
        "wood_block": 76,
    ]

    init() { setup() }

    private func setup() {
        engine.attach(sampler)
        engine.attach(reverb)
        reverb.loadFactoryPreset(.smallRoom)
        reverb.wetDryMix = 20

        engine.connect(sampler, to: reverb, format: nil)
        engine.connect(reverb, to: engine.mainMixerNode, format: nil)

        do {
            try engine.start()
            isReady = true
        } catch {
            print("[SpatialAudio] Engine start failed: \(error)")
        }
    }

    // MARK: - soundSetのロード
    func load(soundSet: String) {
        guard soundSet != currentSoundSet else { return }
        currentSoundSet = soundSet
        let info = Self.soundSetMap[soundSet] ?? (0, 0)
        let isPercussion = info.bank == 128
        sampler.loadGeneralMidiInstrument(info.program, bank: isPercussion ? 128 : 0)
    }

    // MARK: - 鍵盤テンプレート用: UV座標 (0-1) からMIDIノートを計算して演奏
    // uvX: 0.0(左端=低音) 〜 1.0(右端=高音)
    // keyCount: 表示鍵盤数, octave: 基準オクターブ
    func playKeyboard(uvX: Double, keyCount: Int = 12, octave: Int = 4) {
        let noteOffset = Int(uvX * Double(keyCount))
        let midiNote   = UInt8(clamping: 12 * octave + noteOffset)
        play(note: midiNote)
    }

    // MARK: - ボタンテンプレート用: soundSetに応じた固定音
    func playButton(soundSet: String) {
        if let percNote = Self.percussionMap[soundSet] {
            play(note: percNote, channel: 9) // GM percussion channel
        } else {
            play(note: 60) // middle C
        }
    }

    // MARK: - スライダーテンプレート用: UV値で音程/音量を変化
    func playSlider(uvX: Double, soundSet: String) {
        let note = UInt8(clamping: 48 + Int(uvX * 24)) // C3-C5
        play(note: note)
    }

    // MARK: - 弦テンプレート用: 弾く位置で音程変化
    func playString(uvX: Double, keyCount: Int = 6, octave: Int = 3) {
        let note = UInt8(clamping: 12 * octave + Int(uvX * Double(keyCount * 5)))
        play(note: note)
    }

    // MARK: - Raw MIDI note演奏
    func play(note: UInt8, velocity: UInt8 = 100, channel: UInt8 = 0) {
        guard isReady else { return }
        sampler.startNote(note, withVelocity: velocity, onChannel: channel)
        // 自動ノートオフ (300ms後)
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.3) { [weak self] in
            self?.sampler.stopNote(note, onChannel: channel)
        }
    }
}

// MARK: - AVAudioUnitSampler Extension (GM instrument loader)
extension AVAudioUnitSampler {
    func loadGeneralMidiInstrument(_ program: UInt8, bank: UInt8 = 0) {
        // iOS/visionOSのデフォルトDLS/SF2をロード
        let urls = [
            URL(fileURLWithPath: "/System/Library/Components/CoreAudio.component/Contents/Resources/gs_instruments.dls"),
            URL(fileURLWithPath: "/Library/Audio/Sounds/Banks/General MIDI.sf2"),
        ]
        for url in urls where FileManager.default.fileExists(atPath: url.path) {
            try? self.loadSoundBankInstrument(
                at: url,
                program: program,
                bankMSB: bank == 128 ? UInt8(kAUSampler_DefaultPercussionBankMSB) : UInt8(kAUSampler_DefaultMelodicBankMSB),
                bankLSB: 0
            )
            return
        }
        // フォールバック: プリセット直接ロード
        try? self.loadInstrument(at: URL(fileURLWithPath: "/System/Library/Audio/Plugins/HAL/AppleVAIO.plugin"))
    }
}

private extension UInt8 {
    init(clamping value: Int) {
        self = UInt8(Swift.max(0, Swift.min(127, value)))
    }
}
