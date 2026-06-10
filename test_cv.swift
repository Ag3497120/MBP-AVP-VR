import Foundation
import CoreVideo

func testFormat(_ format: OSType) {
    var pixelBuffer: CVPixelBuffer?
    let status = CVPixelBufferCreate(kCFAllocatorDefault, 3840, 2160, format, nil, &pixelBuffer)
    print("Status for \(format): \(status)")
}
testFormat(kCVPixelFormatType_32BGRA)
testFormat(kCVPixelFormatType_32ARGB)
testFormat(kCVPixelFormatType_32ABGR)
