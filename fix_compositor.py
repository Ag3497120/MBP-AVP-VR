with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/VisionClientCompositor.swift', 'r') as f:
    content = f.read()

content = content.replace('public var onFrameDecoded: ((CVPixelBuffer) -> Void)?', 'public var onFrameDecoded: ((CVPixelBuffer, Double) -> Void)?')

# Add videoTimestamp property to CompositorRenderer
old_renderer = """class CompositorRenderer {
    let layerRenderer: LayerRenderer
    let appModel: AppModel
    
    var latestPixelBuffer: CVPixelBuffer?
    private var renderFrameCount: Int = 0"""

new_renderer = """class CompositorRenderer {
    let layerRenderer: LayerRenderer
    let appModel: AppModel
    
    var latestPixelBuffer: CVPixelBuffer?
    var latestTimestamp: Double = 0.0
    private var renderFrameCount: Int = 0"""
content = content.replace(old_renderer, new_renderer)

# Update the closure
old_closure = """            let renderer = CompositorRenderer(layerRenderer, appModel: appModel)
            renderer.startRenderLoop()
            
            // ClientCompositorから映像を受け取る
            let oldClosure = appModel.compositor.onFrameDecoded
            appModel.compositor.onFrameDecoded = { pb in
                oldClosure?(pb)
                renderer.updatePixelBuffer(pb)
            }"""
new_closure = """            let renderer = CompositorRenderer(layerRenderer, appModel: appModel)
            renderer.startRenderLoop()
            
            // ClientCompositorから映像を受け取る
            let oldClosure = appModel.compositor.onFrameDecoded
            appModel.compositor.onFrameDecoded = { pb, ts in
                oldClosure?(pb, ts)
                renderer.updatePixelBuffer(pb, timestamp: ts)
            }"""
content = content.replace(old_closure, new_closure)

# Update updatePixelBuffer func
content = content.replace('func updatePixelBuffer(_ pb: CVPixelBuffer) {', 'func updatePixelBuffer(_ pb: CVPixelBuffer, timestamp: Double) {\n        self.latestTimestamp = timestamp')

# Update device anchor query
old_query = """            let duration = LayerRenderer.Clock.Instant.epoch.duration(to: timing.presentationTime)
            let exactSeconds = Double(duration.components.seconds) + Double(duration.components.attoseconds) / 1e18
            
            // 完璧な未来予測時刻で頭の位置を取得
            let predictedAnchor = worldTrackingProvider.state == .running
                ? worldTrackingProvider.queryDeviceAnchor(atTimestamp: exactSeconds)
                : nil"""

new_query = """            // We now use the exact past timestamp when the frame was rendered by SteamVR
            let ts = self.latestTimestamp > 0 ? self.latestTimestamp : CACurrentMediaTime()
            
            // 過去のタイムスタンプを使ってATWの相殺を行う
            let predictedAnchor = worldTrackingProvider.state == .running
                ? worldTrackingProvider.queryDeviceAnchor(atTimestamp: ts)
                : nil"""
content = content.replace(old_query, new_query)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/VisionClientCompositor.swift', 'w') as f:
    f.write(content)
