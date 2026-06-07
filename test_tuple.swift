var tuple: (Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float, Float) = (
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
)
print("size:", MemoryLayout.size(ofValue: tuple))
withUnsafePointer(to: &tuple) { ptr in
    let ptrFloat = ptr.withMemoryRebound(to: Float.self, capacity: 16) { $0 }
    for i in 0..<16 {
        print(ptrFloat[i], terminator: " ")
    }
}
print("")
