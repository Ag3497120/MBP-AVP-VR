import Foundation
let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let fd = open(filePath, O_RDONLY)
if fd < 0 { print("FILE NOT FOUND"); exit(1) }
let headerSize = 16
let pixelSize = 4096 * 4096 * 4
let mapSize = headerSize + pixelSize
let mapPtr = mmap(nil, mapSize, PROT_READ, MAP_SHARED, fd, 0)
if mapPtr == MAP_FAILED { print("mmap failed"); exit(1) }

// Read header
let seq = mapPtr!.load(fromByteOffset: 0, as: UInt32.self)
let width = mapPtr!.load(fromByteOffset: 4, as: UInt32.self)
let height = mapPtr!.load(fromByteOffset: 8, as: UInt32.self)
let format = mapPtr!.load(fromByteOffset: 12, as: UInt32.self)
print("Header: seq=\(seq) width=\(width) height=\(height) format=\(format)")

let pixelPtr = mapPtr! + headerSize
// Sample multiple pixels
var nonBlackCount = 0
for i in 0..<100 {
    let offset = Int(width * UInt32(height/2)) * 4 + i * Int(width/100) * 4
    let b = pixelPtr.load(fromByteOffset: offset + 0, as: UInt8.self)
    let g = pixelPtr.load(fromByteOffset: offset + 1, as: UInt8.self)
    let r = pixelPtr.load(fromByteOffset: offset + 2, as: UInt8.self)
    if r != 0 || g != 0 || b != 0 { nonBlackCount += 1 }
}
print("Non-black pixels in sample: \(nonBlackCount)/100")
if nonBlackCount == 0 { print("VERDICT: SHARED MEMORY IS COMPLETELY BLACK") }
else { print("VERDICT: HAS CONTENT") }
