import ARKit
import CompositorServices

@MainActor
func test(provider: WorldTrackingProvider, drawable: LayerRenderer.Drawable) {
    let time = drawable.frameTiming.presentationTime
    let anchor = provider.queryDeviceAnchor(atTimestamp: time)
}
