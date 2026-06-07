with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/ContentView.swift', 'r') as f:
    content = f.read()

old_code = """                            if newValue {
                                await openImmersiveSpace(id: "PassthroughStandbySpace")
                                await appModel.startTracking()
                            } else {"""

new_code = """                            if newValue {
                                dismissWindow(id: "mainWindow")
                                await openImmersiveSpace(id: "SteamVRFullSpace")
                                await appModel.startTracking()
                            } else {"""

content = content.replace(old_code, new_code)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/ContentView.swift', 'w') as f:
    f.write(content)
