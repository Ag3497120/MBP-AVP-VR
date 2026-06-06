import RealityKit
import SwiftUI

class DrawingCanvasEntity: Entity {
    private var strokePoints: [[SIMD3<Float>]] = []
    private var currentStroke: [SIMD3<Float>] = []
    private var isDrawing = false

    private var selectedColor: UIColor = .black
    private var brushSize: Float = 0.003

    required init() {
        super.init()
        setupCanvas()
    }

    private func setupCanvas() {
        // Create canvas frame
        let frameMesh = MeshResource.generateBox(width: 0.45, height: 0.02, depth: 0.35, cornerRadius: 0.005)
        let frameMaterial = SimpleMaterial(color: .systemGray.withAlphaComponent(0.9), isMetallic: true)
        let frameModel = ModelEntity(mesh: frameMesh, materials: [frameMaterial])
        frameModel.position = [0, 0, 0]
        addChild(frameModel)

        // Create white drawing surface
        let canvasMesh = MeshResource.generatePlane(width: 0.4, height: 0.3, cornerRadius: 0.003)
        let canvasMaterial = SimpleMaterial(color: .white, isMetallic: false)
        let canvasModel = ModelEntity(mesh: canvasMesh, materials: [canvasMaterial])
        canvasModel.position = [0, 0.011, 0]
        canvasModel.orientation = simd_quatf(angle: .pi / 2, axis: [1, 0, 0])
        canvasModel.name = "DrawingSurface"
        addChild(canvasModel)

        // Create color palette
        createColorPalette()

        // Create tool buttons
        createToolbar()

        // Enable interaction
        components.set(InputTargetComponent(allowedInputTypes: [.indirect, .direct]))
        components.set(CollisionComponent(shapes: [.generateBox(width: 0.45, height: 0.02, depth: 0.35)]))
    }

    private func createColorPalette() {
        let colors: [UIColor] = [
            .black, .red, .blue, .green, .yellow, .orange, .purple, .brown
        ]

        let paletteStartX: Float = -0.18
        let colorSize: Float = 0.025
        let colorSpacing: Float = 0.005

        for (index, color) in colors.enumerated() {
            let colorButton = Entity()
            let buttonMesh = MeshResource.generateSphere(radius: colorSize / 2)
            let buttonMaterial = SimpleMaterial(color: color, isMetallic: false)
            let buttonModel = ModelEntity(mesh: buttonMesh, materials: [buttonMaterial])
            colorButton.addChild(buttonModel)

            let xPos = paletteStartX + Float(index) * (colorSize + colorSpacing)
            colorButton.position = [xPos, 0.015, -0.19]

            colorButton.components.set(InputTargetComponent())
            colorButton.components.set(HoverEffectComponent())
            colorButton.name = "Color_\(index)"

            addChild(colorButton)
        }
    }

    private func createToolbar() {
        // Clear canvas button
        let clearButton = createToolButton(
            icon: "trash.fill",
            position: [0.18, 0.015, -0.19],
            color: .systemRed
        )
        clearButton.name = "ClearButton"
        addChild(clearButton)

        // Eraser button
        let eraserButton = createToolButton(
            icon: "eraser.fill",
            position: [0.18, 0.015, -0.14],
            color: .systemGray
        )
        eraserButton.name = "EraserButton"
        addChild(eraserButton)

        // Save drawing button
        let saveButton = createToolButton(
            icon: "square.and.arrow.down.fill",
            position: [0.18, 0.015, -0.09],
            color: .systemBlue
        )
        saveButton.name = "SaveButton"
        addChild(saveButton)

        // Brush size controls
        createBrushSizeControl()
    }

    private func createToolButton(icon: String, position: SIMD3<Float>, color: UIColor) -> Entity {
        let button = Entity()
        let buttonMesh = MeshResource.generateBox(width: 0.03, height: 0.01, depth: 0.03, cornerRadius: 0.005)
        let buttonMaterial = SimpleMaterial(color: color.withAlphaComponent(0.8), isMetallic: false)
        let buttonModel = ModelEntity(mesh: buttonMesh, materials: [buttonMaterial])
        button.addChild(buttonModel)
        button.position = position

        button.components.set(InputTargetComponent())
        button.components.set(HoverEffectComponent())

        return button
    }

    private func createBrushSizeControl() {
        let sizes: [Float] = [0.002, 0.003, 0.005, 0.008]

        for (index, size) in sizes.enumerated() {
            let sizeButton = Entity()
            let buttonMesh = MeshResource.generateSphere(radius: size * 3)
            let buttonMaterial = SimpleMaterial(color: .systemGray6, isMetallic: false)
            let buttonModel = ModelEntity(mesh: buttonMesh, materials: [buttonMaterial])
            sizeButton.addChild(buttonModel)

            let zPos: Float = 0.05 + Float(index) * 0.04
            sizeButton.position = [0.18, 0.015, zPos]

            sizeButton.components.set(InputTargetComponent())
            sizeButton.components.set(HoverEffectComponent())
            sizeButton.name = "BrushSize_\(index)"

            addChild(sizeButton)
        }
    }

    // Drawing functions
    func startDrawing(at point: SIMD3<Float>) {
        isDrawing = true
        currentStroke = [point]
    }

    func continueDrawing(at point: SIMD3<Float>) {
        guard isDrawing else { return }
        currentStroke.append(point)
        drawLine(from: currentStroke[currentStroke.count - 2], to: point)
    }

    func endDrawing() {
        if isDrawing && !currentStroke.isEmpty {
            strokePoints.append(currentStroke)
            currentStroke = []
        }
        isDrawing = false
    }

    private func drawLine(from start: SIMD3<Float>, to end: SIMD3<Float>) {
        let direction = end - start
        let distance = length(direction)

        let lineMesh = MeshResource.generateBox(width: brushSize, height: distance, depth: brushSize)
        let lineMaterial = SimpleMaterial(color: selectedColor, isMetallic: false)
        let lineModel = ModelEntity(mesh: lineMesh, materials: [lineMaterial])

        let midPoint = (start + end) / 2
        lineModel.position = midPoint

        // Rotate to align with direction
        let angle = acos(dot(direction / distance, [0, 1, 0]))
        let axis = cross(direction / distance, [0, 1, 0])
        if length(axis) > 0.001 {
            lineModel.orientation = simd_quatf(angle: angle, axis: normalize(axis))
        }

        addChild(lineModel)
    }

    func clearCanvas() {
        strokePoints.removeAll()
        currentStroke = []

        // Remove all drawn lines
        children.forEach { child in
            if child.name.starts(with: "Line_") {
                child.removeFromParent()
            }
        }
    }

    func setColor(_ color: UIColor) {
        selectedColor = color
    }

    func setBrushSize(_ size: Float) {
        brushSize = size
    }
}
