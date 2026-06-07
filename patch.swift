                if data.count == MemoryLayout<VRPosePacket>.size {
                    data.withUnsafeBytes { rawBuffer in
                        let posePkt = rawBuffer.load(as: VRPosePacket.self)
                        // "POSE" is little-endian 0x45534F50
                        if posePkt.magic == UInt32(0x45534F50) {
                            // offset 0 (8 bytes): poseTimestamp
