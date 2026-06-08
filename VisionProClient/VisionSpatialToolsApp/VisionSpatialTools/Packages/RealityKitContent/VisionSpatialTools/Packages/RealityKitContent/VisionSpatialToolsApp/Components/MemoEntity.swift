import RealityKit
import SwiftUI

class MemoEntity: Entity {
    private var textLines: [String] = []
    private let maxLines = 10
    private var currentText = ""

    required init() {
        super.init()
        setupMemo()
    }

    private func setupMemo() {
        // Create memo base (notepad style)
        let baseMesh = MeshResource.generateBox(width: 0.3, height: 0.02, depth: 0.4, cornerRadius: 0.005)
        let baseMaterial = SimpleMaterial(color: .systemYellow.withAlphaComponent(0.9), isMetallic: false)
        let baseModel = ModelEntity(mesh: baseMesh, materials: [baseMaterial])
        baseModel.position = [0, 0, 0]
        addChild(baseModel)

        // Create writing surface
        let surfaceMesh = MeshResource.generatePlane(width: 0.28, height: 0.38, cornerRadius: 0.003)
        let surfaceMaterial = SimpleMaterial(color: .white.withAlphaComponent(0.95), isMetallic: false)
        let surfaceModel = ModelEntity(mesh: surfaceMesh, materials: [surfaceMaterial])
        surfaceModel.position = [0, 0.011, 0]
        surfaceModel.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])
        addChild(surfaceModel)

        // Add horizontal lines (like ruled paper)
        createRuledLines()

        // Create header area
        let headerMesh = MeshResource.generateBox(width: 0.28, height: 0.001, depth: 0.05)
        let headerMaterial = SimpleMaterial(color: .systemBlue.withAlphaComponent(0.3), isMetallic: false)
        let headerModel = ModelEntity(mesh: headerMesh, materials: [headerMaterial])
        headerModel.position = [0, 0.012, 0.165]
        addChild(headerModel)

        // Add toolbar buttons
        createToolbar()

        // Enable interaction
        components.set(InputTargetComponent(allowedInputTypes: .indirect))
        components.set(CollisionComponent(shapes: [.generateBox(width: 0.3, height: 0.02, depth: 0.4)]))
    }

    private func createRuledLines() {
        let lineSpacing: Float = 0.035
        let lineCount = 10

        for i in 0..<lineCount {
            let yOffset = 0.15 - Float(i) * lineSpacing
            let lineMesh = MeshResource.generateBox(width: 0.26, height: 0.0005, depth: 0.001)
            let lineMaterial = SimpleMaterial(color: .systemGray4.withAlphaComponent(0.5), isMetallic: false)
            let lineModel = ModelEntity(mesh: lineMesh, materials: [lineMaterial])
            lineModel.position = [0, 0.013, yOffset]
            addChild(lineModel)
        }
    }

    private func createToolbar() {
        // Clear button
        let clearButton = createButton(icon: "trash", position: [-0.1, 0.012, 0.165])
        addChild(clearButton)

        // Save button
        let saveButton = createButton(icon: "square.and.arrow.down", position: [0.1, 0.012, 0.165])
        addChild(saveButton)
    }

    private func createButton(icon: String, position: SIMD3<Float>) -> Entity {
        let button = Entity()
        let buttonMesh = MeshResource.generateBox(width: 0.04, height: 0.005, depth: 0.04, cornerRadius: 0.005)
        let buttonMaterial = SimpleMaterial(color: .systemGray5, isMetallic: false)
        let buttonModel = ModelEntity(mesh: buttonMesh, materials: [buttonMaterial])
        button.addChild(buttonModel)
        button.position = position
        button.components.set(InputTargetComponent())
        button.components.set(HoverEffectComponent())
        return button
    }

    func addText(_ text: String) {
        currentText += text
        if text == "\n" || currentText.count > 30 {
            if textLines.count < maxLines {
                textLines.append(currentText)
            }
            currentText = ""
        }
    }

    func clearMemo() {
        textLines.removeAll()
        currentText = ""
    }
}
