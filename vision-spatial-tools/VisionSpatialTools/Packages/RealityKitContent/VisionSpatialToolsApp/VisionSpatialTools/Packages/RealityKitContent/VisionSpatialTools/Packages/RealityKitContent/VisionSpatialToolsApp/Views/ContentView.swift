import SwiftUI
import RealityKit

struct ContentView: View {
    @EnvironmentObject var appModel: AppModel
    @Environment(\.openImmersiveSpace) var openImmersiveSpace
    @Environment(\.dismissImmersiveSpace) var dismissImmersiveSpace

    var body: some View {
        VStack(spacing: 30) {
            Text("Spatial Tools")
                .font(.system(size: 48, weight: .bold))
                .padding(.top, 40)

            Text("任意のオブジェクトにツールを配置")
                .font(.title2)
                .foregroundStyle(.secondary)

            Divider()
                .padding(.vertical)

            // Tool selection
            VStack(alignment: .leading, spacing: 20) {
                Text("ツール選択")
                    .font(.title3)
                    .fontWeight(.semibold)

                HStack(spacing: 15) {
                    ToolButton(icon: "keyboard", title: "キーボード", toolType: .keyboard)
                    ToolButton(icon: "note.text", title: "メモ", toolType: .memo)
                    ToolButton(icon: "pencil.tip.crop.circle", title: "描画", toolType: .drawing)
                    ToolButton(icon: "function", title: "計算機", toolType: .calculator)
                }

                HStack(spacing: 15) {
                    ToolButton(icon: "hand.draw", title: "トラックパッド", toolType: .trackpad)
                }
            }
            .padding()
            .background(.ultraThinMaterial)
            .cornerRadius(20)

            // Advanced features
            VStack(alignment: .leading, spacing: 15) {
                Text("高度な機能")
                    .font(.title3)
                    .fontWeight(.semibold)

                Toggle(isOn: $appModel.magneticSnapEnabled) {
                    HStack {
                        Image(systemName: "magnet")
                        Text("磁気吸着")
                    }
                }
                .toggleStyle(.switch)

                Toggle(isOn: $appModel.isScanningMode) {
                    HStack {
                        Image(systemName: "cube.transparent.fill")
                        Text("オブジェクトスキャン")
                    }
                }
                .toggleStyle(.switch)

                if appModel.isScanningMode {
                    Text("オブジェクトに近づけてスキャン")
                        .font(.caption)
                        .foregroundStyle(.blue)
                }

                if appModel.magneticSnapEnabled {
                    Text("ツールがオブジェクトに自動吸着")
                        .font(.caption)
                        .foregroundStyle(.green)
                }
            }
            .padding()
            .background(.ultraThinMaterial)
            .cornerRadius(20)

            Spacer()

            // Immersive space toggle
            Toggle(isOn: Binding(
                get: { appModel.isImmersiveSpaceActive },
                set: { newValue in
                    Task {
                        if newValue {
                            await openImmersiveSpace(id: "SpatialTools")
                            await appModel.startTracking()
                        } else {
                            await dismissImmersiveSpace()
                        }
                        appModel.isImmersiveSpaceActive = newValue
                    }
                }
            )) {
                Text("空間モードを開始")
                    .font(.title2)
            }
            .toggleStyle(.button)
            .padding()
            .background(appModel.isImmersiveSpaceActive ? Color.green.opacity(0.3) : Color.blue.opacity(0.3))
            .cornerRadius(15)

            if appModel.isImmersiveSpaceActive {
                Text("オブジェクトをタップしてツールを配置")
                    .font(.caption)
                    .foregroundStyle(.secondary)
            }
        }
        .padding(40)
        .frame(maxWidth: 600)
    }
}

struct ToolButton: View {
    @EnvironmentObject var appModel: AppModel
    let icon: String
    let title: String
    let toolType: ToolType

    var body: some View {
        Button(action: {
            appModel.selectedToolType = toolType
        }) {
            VStack(spacing: 8) {
                Image(systemName: icon)
                    .font(.system(size: 32))
                Text(title)
                    .font(.caption)
            }
            .frame(width: 100, height: 100)
            .background(appModel.selectedToolType == toolType ? Color.blue.opacity(0.3) : Color.secondary.opacity(0.1))
            .cornerRadius(12)
        }
        .buttonStyle(.plain)
    }
}
