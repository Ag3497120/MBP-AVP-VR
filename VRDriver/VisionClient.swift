import Foundation
import Metal
import Cocoa

let shmPath = "/tmp/verantyx_shm.bin"
let width = 1024 // Adjust based on VR game resolution
let height = 1024

// 1. Open the shared memory file
guard let fileHandle = FileHandle(forReadingAtPath: shmPath) else {
    print("Failed to open \(shmPath). Is the VR game running?")
    exit(1)
}

// 2. Map the file into memory
let fd = fileHandle.fileDescriptor
let fileSize = width * height * 4 * 2
let mappedPtr = mmap(nil, fileSize, PROT_READ, MAP_SHARED, fd, 0)

if mappedPtr == MAP_FAILED {
    print("Failed to mmap the shared memory file!")
    exit(1)
}

print("Successfully mapped shared memory at \(mappedPtr!)")

// 3. Create Metal Device and Buffer (Zero-copy)
guard let device = MTLCreateSystemDefaultDevice() else {
    print("Failed to get Metal device")
    exit(1)
}

// Create a Metal buffer that directly wraps the mmap'd pointer WITHOUT copying
guard let buffer = device.makeBuffer(bytesNoCopy: mappedPtr!, length: fileSize, options: .storageModeShared, deallocator: nil) else {
    print("Failed to create zero-copy MTLBuffer")
    exit(1)
}

print("Successfully created zero-copy MTLBuffer!")

// 4. Wrap the buffer in an MTLTexture
let desc = MTLTextureDescriptor()
desc.pixelFormat = .bgra8Unorm
desc.width = width
desc.height = height
desc.depth = 1
desc.textureType = .type2D
desc.usage = [.shaderRead]

guard let texture = buffer.makeTexture(descriptor: desc, offset: 0, bytesPerRow: width * 4) else {
    print("Failed to create MTLTexture from buffer")
    exit(1)
}

print("Successfully created MTLTexture! It is now ready to be sent to Vision Pro.")

// Unmap
munmap(mappedPtr, fileSize)
fileHandle.closeFile()
