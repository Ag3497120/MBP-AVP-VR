import Foundation
import RealityKit
import ARKit

@MainActor
class PersistenceManager: ObservableObject {
    @Published var savedAnchors: [SavedAnchor] = []

    private let userDefaults = UserDefaults.standard
    private let anchorsKey = "SavedSpatialAnchors"

    init() {
        loadAnchors()
    }

    func saveAnchor(tool: AttachedTool, worldTransform: simd_float4x4) {
        let savedAnchor = SavedAnchor(
            id: tool.id,
            toolType: tool.type,
            worldTransform: worldTransform,
            timestamp: Date()
        )

        savedAnchors.append(savedAnchor)
        persistAnchors()
    }

    func removeAnchor(id: UUID) {
        savedAnchors.removeAll { $0.id == id }
        persistAnchors()
    }

    func updateAnchor(id: UUID, worldTransform: simd_float4x4) {
        if let index = savedAnchors.firstIndex(where: { $0.id == id }) {
            savedAnchors[index].worldTransform = worldTransform
            savedAnchors[index].timestamp = Date()
            persistAnchors()
        }
    }

    private func persistAnchors() {
        do {
            let data = try JSONEncoder().encode(savedAnchors)
            userDefaults.set(data, forKey: anchorsKey)
        } catch {
            print("Failed to save anchors: \(error)")
        }
    }

    private func loadAnchors() {
        guard let data = userDefaults.data(forKey: anchorsKey) else { return }

        do {
            savedAnchors = try JSONDecoder().decode([SavedAnchor].self, from: data)
        } catch {
            print("Failed to load anchors: \(error)")
        }
    }

    func clearAll() {
        savedAnchors.removeAll()
        userDefaults.removeObject(forKey: anchorsKey)
    }
}

struct SavedAnchor: Codable, Identifiable {
    let id: UUID
    let toolType: ToolType
    var worldTransform: simd_float4x4
    var timestamp: Date

    enum CodingKeys: String, CodingKey {
        case id, toolType, worldTransform, timestamp
    }

    init(id: UUID, toolType: ToolType, worldTransform: simd_float4x4, timestamp: Date) {
        self.id = id
        self.toolType = toolType
        self.worldTransform = worldTransform
        self.timestamp = timestamp
    }

    init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        id = try container.decode(UUID.self, forKey: .id)
        toolType = try container.decode(ToolType.self, forKey: .toolType)
        timestamp = try container.decode(Date.self, forKey: .timestamp)

        // Decode transform matrix
        let transformData = try container.decode([Float].self, forKey: .worldTransform)
        worldTransform = simd_float4x4(
            SIMD4<Float>(transformData[0...3]),
            SIMD4<Float>(transformData[4...7]),
            SIMD4<Float>(transformData[8...11]),
            SIMD4<Float>(transformData[12...15])
        )
    }

    func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(id, forKey: .id)
        try container.encode(toolType, forKey: .toolType)
        try container.encode(timestamp, forKey: .timestamp)

        // Encode transform matrix as flat array
        let transformData: [Float] = [
            worldTransform.columns.0.x, worldTransform.columns.0.y, worldTransform.columns.0.z, worldTransform.columns.0.w,
            worldTransform.columns.1.x, worldTransform.columns.1.y, worldTransform.columns.1.z, worldTransform.columns.1.w,
            worldTransform.columns.2.x, worldTransform.columns.2.y, worldTransform.columns.2.z, worldTransform.columns.2.w,
            worldTransform.columns.3.x, worldTransform.columns.3.y, worldTransform.columns.3.z, worldTransform.columns.3.w
        ]
        try container.encode(transformData, forKey: .worldTransform)
    }
}

// Make ToolType Codable
extension ToolType: Codable {
    enum CodingKeys: String, CodingKey {
        case keyboard, memo, drawing, calculator
    }

    init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        let value = try container.decode(String.self)

        switch value {
        case "keyboard": self = .keyboard
        case "memo": self = .memo
        case "drawing": self = .drawing
        case "calculator": self = .calculator
        default: throw DecodingError.dataCorrupted(
            DecodingError.Context(codingPath: decoder.codingPath, debugDescription: "Unknown tool type")
        )
        }
    }

    func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        switch self {
        case .keyboard: try container.encode("keyboard")
        case .memo: try container.encode("memo")
        case .drawing: try container.encode("drawing")
        case .calculator: try container.encode("calculator")
        }
    }
}
