with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/HEVCVideoDecoder.swift', 'r') as f:
    content = f.read()

content = content.replace('func didDecodeFrame(_ pixelBuffer: CVPixelBuffer)', 'func didDecodeFrame(_ pixelBuffer: CVPixelBuffer, timestamp: Double)')

old_call = """        if let pixelBuffer = pixelBuffer {
            // Retrieve timestamp from our decode queue using the output presentation time
            let seq = UInt32(presentationTimeStamp.value)
            let ts = self.decodeTimestampQueue[seq] ?? 0.0
            self.decodeTimestampQueue.removeValue(forKey: seq)
            self.delegate?.onFrameDecoded?(pixelBuffer, ts)
        }"""
new_call = """        if let pixelBuffer = pixelBuffer {
            // Retrieve timestamp from our decode queue using the output presentation time
            let seq = UInt32(presentationTimeStamp.value)
            let ts = self.decodeTimestampQueue[seq] ?? 0.0
            self.decodeTimestampQueue.removeValue(forKey: seq)
            self.delegate?.didDecodeFrame(pixelBuffer, timestamp: ts)
        }"""
content = content.replace(old_call, new_call)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/HEVCVideoDecoder.swift', 'w') as f:
    f.write(content)
