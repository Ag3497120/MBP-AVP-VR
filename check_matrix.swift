import Foundation

let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let fd = open(filePath, O_RDONLY)
let mapSize = 16 + 4096 * 4096 * 4 + 1024
let mapPtr = mmap(nil, mapSize, PROT_READ, MAP_SHARED, fd, 0)
let handsMapPtr = mapPtr! + 16 + 4096 * 4096 * 4

let headPtr = (handsMapPtr + 16).bindMemory(to: Float.self, capacity: 16)
print("Head Matrix:")
for r in 0..<4 {
    print(String(format: "%f %f %f %f", headPtr[0*4+r], headPtr[1*4+r], headPtr[2*4+r], headPtr[3*4+r]))
}
