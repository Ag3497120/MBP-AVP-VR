// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "VisionSpatialTools",
    platforms: [
        .visionOS(.v1)
    ],
    products: [
        .library(
            name: "VisionSpatialTools",
            targets: ["VisionSpatialTools"]),
    ],
    dependencies: [],
    targets: [
        .target(
            name: "VisionSpatialTools",
            dependencies: []),
        .testTarget(
            name: "VisionSpatialToolsTests",
            dependencies: ["VisionSpatialTools"]),
    ]
)
