with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/VisionSpatialToolsApp.swift', 'r') as f:
    content = f.read()

# Fix oldClosure wrapper
old_code = """                let oldClosure = appModel.compositor.onFrameDecoded
                appModel.compositor.onFrameDecoded = { pb in
                    oldClosure?(pb)
                    renderer.updatePixelBuffer(pb)
                }"""
new_code = """                let oldClosure = appModel.compositor.onFrameDecoded
                appModel.compositor.onFrameDecoded = { pb, ts in
                    oldClosure?(pb, ts)
                    renderer.updatePixelBuffer(pb, timestamp: ts)
                }"""
content = content.replace(old_code, new_code)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/VisionSpatialToolsApp.swift', 'w') as f:
    f.write(content)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Models/AppModel.swift', 'r') as f:
    app_model = f.read()

app_model = app_model.replace('self.compositor.onFrameDecoded = { [weak self] pixelBuffer in', 'self.compositor.onFrameDecoded = { [weak self] pixelBuffer, timestamp in')
with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Models/AppModel.swift', 'w') as f:
    f.write(app_model)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Components/VRScreenEntity.swift', 'r') as f:
    vr_screen = f.read()

vr_screen = vr_screen.replace('self.compositor?.onFrameDecoded = { [weak self] pixelBuffer in', 'self.compositor?.onFrameDecoded = { [weak self] pixelBuffer, timestamp in')
with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Components/VRScreenEntity.swift', 'w') as f:
    f.write(vr_screen)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/VisionClientCompositor.swift', 'r') as f:
    vc = f.read()

vc = vc.replace('public func didDecodeFrame(_ pixelBuffer: CVPixelBuffer) {', 'public func didDecodeFrame(_ pixelBuffer: CVPixelBuffer, timestamp: Double) {')
vc = vc.replace('onFrameDecoded?(pixelBuffer)', 'onFrameDecoded?(pixelBuffer, timestamp)')

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/VisionClientCompositor.swift', 'w') as f:
    f.write(vc)

