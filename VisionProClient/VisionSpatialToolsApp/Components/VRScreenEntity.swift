import RealityKit
import SwiftUI
import CoreMedia
import VideoToolbox
import CoreImage
import Metal
import MetalFX

class VRScreenEntity: Entity, HasModel {
    
    private var compositor: VisionClientCompositor
    private weak var appModel: AppModel?
    
    // MetalFX & DrawableQueue Upscaling components
    private var drawableQueue: TextureResource.DrawableQueue?
    
    // MetalFX Upscaling components
    private var mtlDevice: MTLDevice?
    private var commandQueue: MTLCommandQueue?
    private var textureCache: CVMetalTextureCache?
    private var spatialScaler: MTLFXSpatialScaler?
    private var outputTexture: MTLTexture?
    private var isMetalFXInitialized = false
    
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
        self.compositor.onFrameDecoded = { [weak self] (pixelBuffer, renderPose) in
            self?.updateTexture(with: pixelBuffer, renderPose: renderPose)
            
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
    
    private func setupMetalFX(inputWidth: Int, inputHeight: Int) {
        guard let device = MTLCreateSystemDefaultDevice(),
              let queue = device.makeCommandQueue() else {
            print("[VRScreenEntity] Metal not supported")
            return
        }
        self.mtlDevice = device
        self.commandQueue = queue
        
        var cache: CVMetalTextureCache?
        CVMetalTextureCacheCreate(kCFAllocatorDefault, nil, device, nil, &cache)
        self.textureCache = cache
        
        let descriptor = MTLFXSpatialScalerDescriptor()
        descriptor.colorTextureFormat = .bgra8Unorm
        descriptor.outputTextureFormat = .bgra8Unorm
        descriptor.inputWidth = inputWidth
        descriptor.inputHeight = inputHeight
        // Scale 2x for native 4K feeling
        descriptor.outputWidth = inputWidth * 2
        descriptor.outputHeight = inputHeight * 2
        
        guard let scaler = descriptor.makeSpatialScaler(device: device) else {
            print("[VRScreenEntity] Failed to create MTLFXSpatialScaler")
            return
        }
        self.spatialScaler = scaler
        
        do {
            let queueDesc = TextureResource.DrawableQueue.Descriptor(
                pixelFormat: .bgra8Unorm,
                width: inputWidth * 2,
                height: inputHeight * 2,
                usage: [.renderTarget, .shaderRead],
                mipmapsMode: .none
            )
            let queue = try TextureResource.DrawableQueue(queueDesc)
            self.drawableQueue = queue
            
            let texture = try TextureResource.generate(from: queue)
            DispatchQueue.main.async { [weak self] in
                var material = UnlitMaterial()
                material.color = .init(tint: .white, texture: .init(texture))
                self?.model?.materials = [material]
            }
            print("[VRScreenEntity] DrawableQueue created successfully.")
        } catch {
            print("[VRScreenEntity] Failed to create DrawableQueue: \(error)")
        }
        
        self.isMetalFXInitialized = true
        print("[VRScreenEntity] MetalFX Spatial Scaler Initialized (Input: \(inputWidth)x\(inputHeight) -> Output: \(inputWidth*2)x\(inputHeight*2))")
    }
    
    private func updateTexture(with pixelBuffer: CVPixelBuffer, renderPose: simd_float4x4) {
        let inputWidth = CVPixelBufferGetWidth(pixelBuffer)
        let inputHeight = CVPixelBufferGetHeight(pixelBuffer)
        
        if !isMetalFXInitialized {
            setupMetalFX(inputWidth: inputWidth, inputHeight: inputHeight)
        }
        
        // Apply MetalFX Spatial Upscaling directly into DrawableQueue
        if let cache = textureCache, let scaler = spatialScaler, let queue = commandQueue,
           let drawableQueue = self.drawableQueue,
           let commandBuffer = queue.makeCommandBuffer() {
            
            var cvMetalTexture: CVMetalTexture?
            CVMetalTextureCacheCreateTextureFromImage(kCFAllocatorDefault, cache, pixelBuffer, nil, .bgra8Unorm, inputWidth, inputHeight, 0, &cvMetalTexture)
            
            if let cvMetalTexture = cvMetalTexture, let inTex = CVMetalTextureGetTexture(cvMetalTexture) {
                do {
                    let drawable = try drawableQueue.nextDrawable()
                    scaler.colorTexture = inTex
                    scaler.outputTexture = drawable.texture
                    scaler.encode(commandBuffer: commandBuffer)
                    commandBuffer.commit()
                    commandBuffer.waitUntilCompleted()
                    
                    // Zero-copy presentation directly to VisionOS Compositor
                    drawable.present()
                } catch {
                    print("[VRScreenEntity] Failed to get next drawable: \(error)")
                }
            }
        }
        
        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            var newTransform = Transform(matrix: renderPose)
            newTransform.scale = [-1, 1, 1]
            self.transform = newTransform
        }
    }
}
