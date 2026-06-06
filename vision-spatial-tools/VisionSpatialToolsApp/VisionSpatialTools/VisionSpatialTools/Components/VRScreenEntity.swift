import ARKit
import RealityKit
import SwiftUI
import CoreMedia
import VideoToolbox
import CoreImage

class VRScreenEntity: Entity, HasModel {
    
    private var compositor: VisionClientCompositor?
    private var videoTexture: TextureResource?
    private weak var appModel: AppModel?
    private let ciContext = CIContext(options: nil)
    
    @MainActor required init() {
        self.compositor = nil
        super.init()
    }
    
    @MainActor init(appModel: AppModel) {
        self.compositor = appModel.compositor
        self.appModel = appModel
        super.init()
        // 巨大な球体を生成してユーザーを包み込む形に変更します（没入感のある全天球VR）
        let mesh = MeshResource.generateSphere(radius: 10)
        
        // 初期のマテリアル（黒で初期化）
        var initialMaterial = UnlitMaterial()
        initialMaterial.color = .init(tint: .black)
        self.model = ModelComponent(mesh: mesh, materials: [initialMaterial])
        
        // 巨大な球体がすべてのタップ（ピンチ）を吸収することで、
        // visionOSのシステムメニューが誤爆して開くのを完全に防ぎます。
        self.components.set(InputTargetComponent())
        self.components.set(CollisionComponent(shapes: [.generateSphere(radius: 10)]))
        
        // スケールを反転して球体の内側にテクスチャを貼り、中心に配置
        self.scale = [-1, 1, 1]
        self.position = [0, 0, 0]
        
        // コンポジターからのデコード完了コールバック
        self.compositor?.onFrameDecoded = { [weak self] pixelBuffer in
            self?.updateTexture(with: pixelBuffer)
        }
        
        // 削除: Tracking data is now pushed directly from AppModel to VisionClientCompositor via sendPose()
        
        print("[VRScreenEntity] Initialized. Waiting for Mac UDP Stream...")
    }
    
    private func updateTexture(with pixelBuffer: CVPixelBuffer) {
        let ciImage = CIImage(cvPixelBuffer: pixelBuffer)
        
        guard let cgImg = self.ciContext.createCGImage(ciImage, from: ciImage.extent) else {
            print("[VRScreenEntity] Failed to create CGImage from CIImage")
            return
        }
        
        Task.detached { [weak self] in
            guard let self = self else { return }
            do {
                let newTexture = try await TextureResource.generate(from: cgImg, options: .init(semantic: .color))
                
                await MainActor.run {
                    self.appModel?.framesReceived += 1
                    var material = UnlitMaterial()
                    material.color = .init(texture: .init(newTexture))
                    self.model?.materials = [material]
                }
            } catch {
                print("[VRScreenEntity] Failed to generate texture: \(error)")
            }
        }
    }
}
