import SwiftUI
import CompositorServices

@main
struct VisionSpatialToolsApp: App {
    @StateObject private var appModel = AppModel()

    struct VRCompositorConfiguration: CompositorLayerConfiguration {
        func makeConfiguration(capabilities: LayerRenderer.Capabilities, configuration: inout LayerRenderer.Configuration) {
            configuration.layout = .layered
            configuration.colorFormat = .bgra8Unorm_srgb
            configuration.depthFormat = .depth32Float
        }
    }

    var body: some Scene {
        WindowGroup(id: "mainWindow") {
            ContentView()
                .environmentObject(appModel)
        }
        .windowStyle(.automatic)

        ImmersiveSpace(id: "SpatialTools") {
            ImmersiveView()
                .environmentObject(appModel)
        }
        .immersionStyle(selection: .constant(.mixed), in: .mixed)
        .upperLimbVisibility(.visible)
        
        ImmersiveSpace(id: "PassthroughStandbySpace") {
            PassthroughStandbyView()
                .environmentObject(appModel)
        }
        .immersionStyle(selection: .constant(.mixed), in: .mixed)
        
        ImmersiveSpace(id: "SteamVRFullSpace") {
            CompositorLayer(configuration: VRCompositorConfiguration()) { layerRenderer in
                let renderer = CompositorRenderer(layerRenderer, appModel: appModel)
                renderer.startRenderLoop()
            }
        }
        .immersionStyle(selection: .constant(.full), in: .full)
        .upperLimbVisibility(.hidden)
        
        WindowGroup(id: "SteamVRStreamer") {
            SteamVRStreamerView()
        }
        .windowStyle(.plain)
        .defaultSize(width: 1.5, height: 1.0, depth: 0.1, in: .meters)
    }
}
