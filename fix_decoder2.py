with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/HEVCVideoDecoder.swift', 'r') as f:
    content = f.read()

if "var decodeTimestampQueue" not in content:
    content = content.replace('var formatDesc: CMVideoFormatDescription?', 'var formatDesc: CMVideoFormatDescription?\n    var decodeTimestampQueue: [UInt32: Double] = [:]')

if "decodeTimestampQueue[frameSequence]" not in content:
    content = content.replace('decodeFrame(data: payload, frameSequence: frameSequence)', 'self.decodeTimestampQueue[frameSequence] = header.timestamp\n                    decodeFrame(data: payload, frameSequence: frameSequence)')

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/HEVCVideoDecoder.swift', 'w') as f:
    f.write(content)
