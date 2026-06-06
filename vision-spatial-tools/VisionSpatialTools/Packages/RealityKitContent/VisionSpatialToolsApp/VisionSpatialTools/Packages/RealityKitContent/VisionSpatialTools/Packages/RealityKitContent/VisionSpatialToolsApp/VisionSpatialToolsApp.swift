import SwiftUI

@main
struct VisionSpatialToolsApp: App {
    @StateObject private var appModel = AppModel()

    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(appModel)
        }
        .windowStyle(.volumetric)
        .defaultSize(width: 1.0, height: 1.0, depth: 1.0, in: .meters)

        ImmersiveSpace(id: "SpatialTools") {
            ImmersiveView()
                .environmentObject(appModel)
        }
        .immersionStyle(selection: .constant(.mixed), in: .mixed)
    }
}
