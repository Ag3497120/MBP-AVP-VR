import RealityKit
import SwiftUI
import CoreMedia
import VideoToolbox
import CoreImage

class VRScreenEntity: Entity, HasModel {
    
    private var compositor: VisionClientCompositor
    private var videoTexture: TextureResource?
    private weak var appModel: AppModel?
    private let ciContext = CIContext(options: nil)
    
    @MainActor required init() {
        self.compositor = VisionClientCompositor()
        super.init()
    }
    
    @MainActor init(appModel: AppModel) {
        self.compositor = VisionClientCompositor()
        self.appModel = appModel
        super.init()
        // 巨大な球体を生成してユーザーを包み込む形に変更します（没入感のある全天球VR）
        let mesh = MeshResource.generateSphere(radius: 10)
        
        // 初期のマテリアル（黒で初期化）
        var initialMaterial = UnlitMaterial()
        initialMaterial.color = .init(tint: .black)
        self.model = ModelComponent(mesh: mesh, materials: [initialMaterial])
        
        self.components.set(InputTargetComponent())
        
        // スケールを反転して球体の内側にテクスチャを貼り、中心に配置
        self.scale = [-1, 1, 1]
        self.position = [0, 0, 0]
        
        // コンポジターからのデコード完了コールバック
        self.compositor.onFrameDecoded = { [weak self] pixelBuffer in
            self?.updateTexture(with: pixelBuffer)
            
            DispatchQueue.main.async {
                self?.appModel?.framesReceived += 1
            }
        }
        
        self.compositor.getTrackingData = { [weak self] in
            guard let self = self, let model = self.appModel else { return nil }
            
            let head = model.worldTrackingProvider.queryDeviceAnchor(atTimestamp: CACurrentMediaTime())?.originFromAnchorTransform ?? matrix_identity_float4x4
            
            var left = matrix_identity_float4x4
            var right = matrix_identity_float4x4
            var lPinch = false
            var rPinch = false
            
            // Note: In a real app we would use model.handTrackingProvider.latestAnchors
            // and checking chirality.
            
            return (head, left, right, lPinch, rPinch)
        }
        
        print("[VRScreenEntity] Initialized. Waiting for Mac UDP Stream...")
    }
    
    private func updateTexture(with pixelBuffer: CVPixelBuffer) {
        // CVPixelBuffer -> CGImage
        let ciImage = CIImage(cvPixelBuffer: pixelBuffer)
        guard let cgImg = self.ciContext.createCGImage(ciImage, from: ciImage.extent) else {
            print("[VRScreenEntity] Failed to create CGImage from CIImage")
            return
        }
        
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            
            if self.videoTexture == nil {
                do {
                    // 初回テクスチャ生成
                    let texture = try TextureResource.generate(from: cgImg, options: .init(semantic: .color))
                    self.videoTexture = texture
                    
                    var material = UnlitMaterial()
                    material.color = .init(texture: .init(texture))
                    self.model?.materials = [material]
                } catch {
                    print("[VRScreenEntity] Failed to generate initial texture: \(error)")
                }
            } else {
                // 以降は既存テクスチャを高速置換
                do {
                    try self.videoTexture?.replace(with: cgImg)
                } catch {
                    print("[VRScreenEntity] Failed to replace texture: \(error)")
                }
            }
        }
    }
}
