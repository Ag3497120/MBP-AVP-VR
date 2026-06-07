import Foundation
let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let fd = open(filePath, O_RDWR)
if fd < 0 { print("Failed to open"); exit(1) }
let mapPtr = mmap(nil, 16 + 4096 * 4096 * 4, PROT_READ, MAP_SHARED, fd, 0)
if mapPtr == MAP_FAILED { print("mmap failed"); exit(1) }

let pixelPtr = mapPtr! + 16
let width = 3840
let height = 1080

let center_x = width / 2
let center_y = height / 2

let offset = (center_y * width + center_x) * 4
let r = pixelPtr.load(fromByteOffset: offset + 2, as: UInt8.self)
let g = pixelPtr.load(fromByteOffset: offset + 1, as: UInt8.self)
let b = pixelPtr.load(fromByteOffset: offset + 0, as: UInt8.self)
let a = pixelPtr.load(fromByteOffset: offset + 3, as: UInt8.self)
print("Center Pixel: R=\(r) G=\(g) B=\(b) A=\(a)")
