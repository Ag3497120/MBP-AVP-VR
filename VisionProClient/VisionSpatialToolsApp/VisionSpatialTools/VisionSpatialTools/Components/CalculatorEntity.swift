import RealityKit
import SwiftUI

class CalculatorEntity: Entity {
    private var currentValue: Double = 0
    private var displayText: String = "0"
    private var operation: Operation?
    private var previousValue: Double = 0

    enum Operation {
        case add, subtract, multiply, divide
    }

    required init() {
        super.init()
        setupCalculator()
    }

    private func setupCalculator() {
        // Create calculator base
        let baseMesh = MeshResource.generateBox(width: 0.25, height: 0.02, depth: 0.35, cornerRadius: 0.008)
        let baseMaterial = SimpleMaterial(color: .systemGray2.withAlphaComponent(0.95), isMetallic: true)
        let baseModel = ModelEntity(mesh: baseMesh, materials: [baseMaterial])
        baseModel.position = [0, 0, 0]
        addChild(baseModel)

        // Create display
        createDisplay()

        // Create number buttons (0-9)
        createNumberButtons()

        // Create operation buttons
        createOperationButtons()

        // Create function buttons
        createFunctionButtons()

        // Enable interaction
        components.set(InputTargetComponent(allowedInputTypes: .indirect))
        components.set(CollisionComponent(shapes: [.generateBox(width: 0.25, height: 0.02, depth: 0.35)]))
    }

    private func createDisplay() {
        let displayMesh = MeshResource.generateBox(width: 0.22, height: 0.008, depth: 0.06, cornerRadius: 0.004)
        let displayMaterial = SimpleMaterial(color: .black.withAlphaComponent(0.8), isMetallic: false)
        let displayModel = ModelEntity(mesh: displayMesh, materials: [displayMaterial])
        displayModel.position = [0, 0.015, 0.135]
        displayModel.name = "Display"
        addChild(displayModel)

        // Display text area (simulated)
        let textMesh = MeshResource.generatePlane(width: 0.2, height: 0.04)
        let textMaterial = SimpleMaterial(color: .green.withAlphaComponent(0.3), isMetallic: false)
        let textModel = ModelEntity(mesh: textMesh, materials: [textMaterial])
        textModel.position = [0, 0.02, 0.135]
        textModel.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])
        addChild(textModel)
    }

    private func createNumberButtons() {
        let buttonSize: Float = 0.048
        let spacing: Float = 0.006

        // Layout: 3x3 grid for 1-9, and 0 at bottom
        let layout = [
            ["7", "8", "9"],
            ["4", "5", "6"],
            ["1", "2", "3"],
            ["0", ".", "="]
        ]

        for (rowIndex, row) in layout.enumerated() {
            for (colIndex, number) in row.enumerated() {
                let xPos: Float = -0.06 + Float(colIndex) * (buttonSize + spacing)
                let zPos: Float = 0.065 - Float(rowIndex) * (buttonSize + spacing)

                let button = createButton(
                    label: number,
                    position: [xPos, 0.015, zPos],
                    size: buttonSize,
                    color: .systemGray6
                )
                button.name = "Number_\(number)"
                addChild(button)
            }
        }
    }

    private func createOperationButtons() {
        let buttonSize: Float = 0.048
        let spacing: Float = 0.006

        let operations = ["+", "-", "×", "÷"]

        for (index, op) in operations.enumerated() {
            let zPos: Float = 0.065 - Float(index) * (buttonSize + spacing)

            let button = createButton(
                label: op,
                position: [0.06, 0.015, zPos],
                size: buttonSize,
                color: .systemOrange
            )
            button.name = "Operation_\(op)"
            addChild(button)
        }
    }

    private func createFunctionButtons() {
        let buttonSize: Float = 0.048
        let spacing: Float = 0.006

        // Clear button
        let clearButton = createButton(
            label: "C",
            position: [-0.06, 0.015, -0.145],
            size: buttonSize,
            color: .systemRed
        )
        clearButton.name = "Clear"
        addChild(clearButton)

        // All clear button
        let acButton = createButton(
            label: "AC",
            position: [0, 0.015, -0.145],
            size: buttonSize,
            color: .systemRed.withAlphaComponent(0.8)
        )
        acButton.name = "AllClear"
        addChild(acButton)

        // Backspace button
        let backButton = createButton(
            label: "⌫",
            position: [0.06, 0.015, -0.145],
            size: buttonSize,
            color: .systemGray
        )
        backButton.name = "Backspace"
        addChild(backButton)
    }

    private func createButton(label: String, position: SIMD3<Float>, size: Float, color: UIColor) -> Entity {
        let button = Entity()

        // Button body
        let buttonMesh = MeshResource.generateBox(
            width: size,
            height: 0.008,
            depth: size,
            cornerRadius: 0.006
        )
        let buttonMaterial = SimpleMaterial(color: color, isMetallic: false)
        let buttonModel = ModelEntity(mesh: buttonMesh, materials: [buttonMaterial])
        button.addChild(buttonModel)

        // Button label (represented as a small plane)
        let labelMesh = MeshResource.generatePlane(width: size * 0.6, height: size * 0.6)
        let labelMaterial = SimpleMaterial(color: .black, isMetallic: false)
        let labelModel = ModelEntity(mesh: labelMesh, materials: [labelMaterial])
        labelModel.position = [0, 0.005, 0]
        labelModel.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])
        button.addChild(labelModel)

        button.position = position
        button.components.set(InputTargetComponent())
        button.components.set(HoverEffectComponent())

        return button
    }

    // Calculator logic
    func inputNumber(_ digit: String) {
        if displayText == "0" {
            displayText = digit
        } else {
            displayText += digit
        }
        currentValue = Double(displayText) ?? 0
    }

    func inputOperation(_ op: Operation) {
        previousValue = currentValue
        operation = op
        displayText = "0"
    }

    func calculate() {
        guard let operation = operation else { return }

        switch operation {
        case .add:
            currentValue = previousValue + currentValue
        case .subtract:
            currentValue = previousValue - currentValue
        case .multiply:
            currentValue = previousValue * currentValue
        case .divide:
            currentValue = previousValue / currentValue
        }

        displayText = String(currentValue)
        self.operation = nil
    }

    func clear() {
        currentValue = 0
        displayText = "0"
    }

    func allClear() {
        currentValue = 0
        previousValue = 0
        displayText = "0"
        operation = nil
    }
}
