with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/HEVCVideoDecoder.swift', 'r') as f:
    content = f.read()

old_udp_header = """struct UDPHeader {
    var magic: UInt32 // 0x5652414E ("VRAN")
    var frameSequence: UInt32
    var chunkIndex: UInt32
    var totalChunks: UInt32
    var chunkOffset: UInt32
    var payloadSize: UInt32
}"""

new_udp_header = """struct UDPHeader {
    var magic: UInt32 // 0x5652414E ("VRAN")
    var frameSequence: UInt32
    var chunkIndex: UInt32
    var totalChunks: UInt32
    var chunkOffset: UInt32
    var payloadSize: UInt32
    var timestamp: Double
}"""

content = content.replace(old_udp_header, new_udp_header)

old_protocol = """protocol HEVCVideoDecoderDelegate: AnyObject {
    var onFrameDecoded: ((CVPixelBuffer) -> Void)? { get set }
}"""

new_protocol = """protocol HEVCVideoDecoderDelegate: AnyObject {
    var onFrameDecoded: ((CVPixelBuffer, Double) -> Void)? { get set }
}"""

content = content.replace(old_protocol, new_protocol)

old_cb = """        if let pixelBuffer = pixelBuffer {
            self.delegate?.onFrameDecoded?(pixelBuffer)
        }"""
new_cb = """        if let pixelBuffer = pixelBuffer {
            // Retrieve timestamp from our decode queue using the output presentation time
            let seq = UInt32(presentationTimeStamp.value)
            let ts = self.decodeTimestampQueue[seq] ?? 0.0
            self.decodeTimestampQueue.removeValue(forKey: seq)
            self.delegate?.onFrameDecoded?(pixelBuffer, ts)
        }"""
content = content.replace(old_cb, new_cb)

with open('/Users/motonishikoudai/vision-spatial-tools/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Services/HEVCVideoDecoder.swift', 'w') as f:
    f.write(content)
