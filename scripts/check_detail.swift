import Foundation
let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let fd = open(filePath, O_RDONLY)
if fd < 0 { print("FILE NOT FOUND"); exit(1) }
let headerSize = 16
let pixelSize = 4096 * 4096 * 4
let mapSize = headerSize + pixelSize
let mapPtr = mmap(nil, mapSize, PROT_READ, MAP_SHARED, fd, 0)!

let seq = mapPtr.load(fromByteOffset: 0, as: UInt32.self)
let width = mapPtr.load(fromByteOffset: 4, as: UInt32.self)
let height = mapPtr.load(fromByteOffset: 8, as: UInt32.self)
let format = mapPtr.load(fromByteOffset: 12, as: UInt32.self)
print("seq=\(seq) width=\(width) height=\(height) format=\(format)")

let pixelPtr = mapPtr + headerSize
// Scan ALL pixels to find max brightness
var maxR = UInt8(0), maxG = UInt8(0), maxB = UInt8(0)
var totalPixels = Int(width * height)
let stride = max(1, totalPixels / 1000)  // sample 1000 pixels
for i in stride(from: 0, to: totalPixels, by: stride) {
    let b = pixelPtr.load(fromByteOffset: i*4 + 0, as: UInt8.self)
    let g = pixelPtr.load(fromByteOffset: i*4 + 1, as: UInt8.self)
    let r = pixelPtr.load(fromByteOffset: i*4 + 2, as: UInt8.self)
    if r > maxR { maxR = r }
    if g > maxG { maxG = g }
    if b > maxB { maxB = b }
}
print("Max RGB in frame: R=\(maxR) G=\(maxG) B=\(maxB)")
if maxR == 0 && maxG == 0 && maxB == 0 {
    // Also check channel 0,1,2 as channels (maybe it's BGRA not RGBA)
    var maxA = UInt8(0)
    for i in stride(from: 0, to: totalPixels, by: stride) {
        let a = pixelPtr.load(fromByteOffset: i*4 + 3, as: UInt8.self)
        if a > maxA { maxA = a }
    }
    print("Max alpha: \(maxA)")
}
// Print first 10 non-alpha pixels
print("First 10 pixels (BGRA):")
for i in 0..<10 {
    let b = pixelPtr.load(fromByteOffset: i*4 + 0, as: UInt8.self)
    let g = pixelPtr.load(fromByteOffset: i*4 + 1, as: UInt8.self)
    let r = pixelPtr.load(fromByteOffset: i*4 + 2, as: UInt8.self)
    let a = pixelPtr.load(fromByteOffset: i*4 + 3, as: UInt8.self)
    print("  px[\(i)]: B=\(b) G=\(g) R=\(r) A=\(a)")
}
