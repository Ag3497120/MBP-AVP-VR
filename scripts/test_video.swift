import Foundation

struct SharedFrame {
    var sequenceNumber: UInt32
    var width: UInt32
    var height: UInt32
    var format: UInt32
}

let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let width = 640
let height = 480
let mapSize = 16 + 1920 * 1080 * 4 + 130 // Max size

let fd = open(filePath, O_RDWR | O_CREAT, 0o666)
ftruncate(fd, off_t(mapSize))

let mapPtr = mmap(nil, mapSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)
if mapPtr == MAP_FAILED {
    print("mmap failed")
    exit(1)
}

let headerPtr = mapPtr!.bindMemory(to: SharedFrame.self, capacity: 1)
let pixelPtr = mapPtr! + 16

headerPtr.pointee.width = UInt32(width)
headerPtr.pointee.height = UInt32(height)
headerPtr.pointee.format = 0

var seq: UInt32 = 1
var x = 0
var y = 0

print("Generating dummy frames at 640x480 (~60fps)...")

while true {
    // Background: Blue
    for i in 0..<(width * height) {
        pixelPtr.advanced(by: i * 4).storeBytes(of: 0xFF, as: UInt8.self) // B
        pixelPtr.advanced(by: i * 4 + 1).storeBytes(of: 0x00, as: UInt8.self) // G
        pixelPtr.advanced(by: i * 4 + 2).storeBytes(of: 0x00, as: UInt8.self) // R
        pixelPtr.advanced(by: i * 4 + 3).storeBytes(of: 0xFF, as: UInt8.self) // A
    }

    // Square: Green
    for dy in 0..<100 {
        for dx in 0..<100 {
            let px = (x + dx) % width
            let py = (y + dy) % height
            let idx = py * width + px
            pixelPtr.advanced(by: idx * 4).storeBytes(of: 0x00, as: UInt8.self) // B
            pixelPtr.advanced(by: idx * 4 + 1).storeBytes(of: 0xFF, as: UInt8.self) // G
            pixelPtr.advanced(by: idx * 4 + 2).storeBytes(of: 0x00, as: UInt8.self) // R
            pixelPtr.advanced(by: idx * 4 + 3).storeBytes(of: 0xFF, as: UInt8.self) // A
        }
    }

    headerPtr.pointee.sequenceNumber = seq
    seq += 1
    
    x = (x + 5) % width
    y = (y + 5) % height

    usleep(16000) // ~60fps
}
