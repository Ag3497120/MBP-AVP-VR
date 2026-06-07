import Foundation

let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let fd = open(filePath, O_RDONLY)
if fd < 0 { print("Failed"); exit(1) }
let mapSize = 16 + 4096 * 4096 * 4 + 1024
let mapPtr = mmap(nil, mapSize, PROT_READ, MAP_SHARED, fd, 0)
if mapPtr == MAP_FAILED { print("Failed map"); exit(1) }

let handsMapPtr = mapPtr! + 16 + 4096 * 4096 * 4

let tsPtr = handsMapPtr.bindMemory(to: Double.self, capacity: 1)
print("poseTimestamp:", tsPtr.pointee)

let rtsPtr = (handsMapPtr + 8).bindMemory(to: Double.self, capacity: 1)
print("renderedTimestamp:", rtsPtr.pointee)

let headPtr = (handsMapPtr + 16).bindMemory(to: Float.self, capacity: 16)
print("headTransform translation:")
print(headPtr[12], headPtr[13], headPtr[14])

let leftPtr = (handsMapPtr + 80).bindMemory(to: Float.self, capacity: 16)
print("leftTransform translation:")
print(leftPtr[12], leftPtr[13], leftPtr[14])
