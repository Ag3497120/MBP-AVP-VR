import Foundation
let filePath = NSHomeDirectory() + "/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/vr_shared_frame.dat"
let fd = open(filePath, O_RDONLY)
if fd < 0 { print("FILE NOT FOUND"); exit(1) }
let mapPtr = mmap(nil, 16, PROT_READ, MAP_SHARED, fd, 0)!
// Read seq twice with a delay to see if it's changing
let seq1 = mapPtr.load(fromByteOffset: 0, as: UInt32.self)
print("seq now: \(seq1)")
Thread.sleep(forTimeInterval: 1.0)
// Re-read
let fd2 = open(filePath, O_RDONLY)
let mapPtr2 = mmap(nil, 16, PROT_READ, MAP_SHARED, fd2, 0)!
let seq2 = mapPtr2.load(fromByteOffset: 0, as: UInt32.self)
print("seq +1s: \(seq2)")
print("frames/sec: \(seq2 - seq1)")
