import RealityKit
import SwiftUI

class KeyboardEntity: Entity {
    private var keys: [KeyEntity] = []
    private let keyLayout = [
        ["Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P"],
        ["A", "S", "D", "F", "G", "H", "J", "K", "L"],
        ["Z", "X", "C", "V", "B", "N", "M"]
    ]

    required init() {
        super.init()
        setupKeyboard()
    }

    private func setupKeyboard() {
        // Create keyboard base
        let baseMesh = MeshResource.generateBox(width: 0.4, height: 0.01, depth: 0.25)
        let baseMaterial = SimpleMaterial(color: .white.withAlphaComponent(0.9), isMetallic: false)
        let baseModel = ModelEntity(mesh: baseMesh, materials: [baseMaterial])
        baseModel.position = [0, 0, 0]
        addChild(baseModel)

        // Create keys
        let keyWidth: Float = 0.035
        let keyHeight: Float = 0.035
        let keySpacing: Float = 0.004

        for (rowIndex, row) in keyLayout.enumerated() {
            let rowOffset = Float(rowIndex) * (keyHeight + keySpacing)
            let startX = -Float(row.count) * (keyWidth + keySpacing) / 2

            for (colIndex, letter) in row.enumerated() {
                let keyX = startX + Float(colIndex) * (keyWidth + keySpacing)
                let keyZ = -rowOffset + 0.08

                let key = KeyEntity(letter: letter)
                key.position = [keyX, 0.015, keyZ]
                addChild(key)
                keys.append(key)
            }
        }

        // Add space bar
        let spaceBar = KeyEntity(letter: "SPACE", width: 0.25)
        spaceBar.position = [0, 0.015, -0.12]
        addChild(spaceBar)
        keys.append(spaceBar)

        // Add text display area
        let displayMesh = MeshResource.generateBox(width: 0.38, height: 0.05, depth: 0.001)
        let displayMaterial = SimpleMaterial(color: .gray.withAlphaComponent(0.5), isMetallic: false)
        let displayModel = ModelEntity(mesh: displayMesh, materials: [displayMaterial])
        displayModel.position = [0, 0.015, 0.13]
        addChild(displayModel)

        // Enable input handling
        components.set(InputTargetComponent())
        components.set(CollisionComponent(shapes: [.generateBox(width: 0.4, height: 0.05, depth: 0.25)]))
    }
}

class KeyEntity: Entity, HasModel, HasCollision {
    let letter: String

    required init(letter: String, width: Float = 0.035) {
        self.letter = letter
        super.init()

        // Create key model
        let keyMesh = MeshResource.generateBox(width: width, height: 0.01, depth: 0.035, cornerRadius: 0.003)
        let keyMaterial = SimpleMaterial(color: .systemGray6, isMetallic: false)
        let keyModel = ModelEntity(mesh: keyMesh, materials: [keyMaterial])
        addChild(keyModel)

        // Add text label (using a simple plane for now)
        let textMesh = MeshResource.generatePlane(width: width * 0.6, height: 0.025)
        let textMaterial = SimpleMaterial(color: .black, isMetallic: false)
        let textModel = ModelEntity(mesh: textMesh, materials: [textMaterial])
        textModel.position = [0, 0.006, 0]
        addChild(textModel)

        // Enable interaction
        components.set(InputTargetComponent(allowedInputTypes: .indirect))
        components.set(CollisionComponent(shapes: [.generateBox(width: width, height: 0.01, depth: 0.035)]))

        // Add hover effect component
        components.set(HoverEffectComponent())
    }

    required init() {
        fatalError("Use init(letter:)")
    }
}
