# MBP-AVP-VR: The Ultimate Ultra-Low Latency VR Streaming Architecture

> **A native stereoscopic VR engine for Mac and Apple Vision Pro. Bypasses D3DMetal IPC constraints to stream x86 AAA PCVR titles (e.g., Source 2) at 120Hz/150Mbps via low-level C++ `openvr_api.dll` VTable hooking, direct memory mapping, and visionOS hardware compositing—complete with controller-free full hand tracking.**

## Table of Contents
1. [Introduction & The "Mirage" Concept](#1-introduction--the-mirage-concept)
2. [System Architecture & Data Flow (Deep Technical Dive)](#2-system-architecture--data-flow-deep-technical-dive)
    - [Game Porting Toolkit (D3DMetal)](#game-porting-toolkit-d3dmetal)
    - [The OpenVR Mocking Engine (`openvr_api.dll`)](#the-openvr-mocking-engine-openvr_apidll)
    - [Shared Memory IPC (`vr_shared_frame.dat`)](#shared-memory-ipc-vr_shared_framedat)
    - [HardwareEncoder (Swift)](#hardwareencoder-swift)
3. [The Ultimate Full Hand Tracking Engine](#3-the-ultimate-full-hand-tracking-engine)
    - [Skeletal Joint Calculations](#skeletal-joint-calculations)
    - [Gesture Recognition Logic](#gesture-recognition-logic)
    - [The Virtual Joystick (Locomotion)](#the-virtual-joystick-locomotion)
4. [Custom UDP Networking Protocol (`VRAN`)](#4-custom-udp-networking-protocol-vran)
    - [AWDL / Bonjour Discovery](#awdl--bonjour-discovery)
    - [Packet Fragmentation & I-Frame Recovery](#packet-fragmentation--i-frame-recovery)
5. [Setup & Installation Guide](#5-setup--installation-guide)
    - [Requirements](#requirements)
    - [Cloning and Compilation](#cloning-and-compilation)
    - [Boot Sequence](#boot-sequence)
6. [Troubleshooting](#6-troubleshooting)

---

## 1. Introduction & The "Mirage" Concept

Conventional VR streaming solutions (such as Virtual Desktop or Steam Link) rely heavily on the SteamVR compositor. They capture frames *after* they have been processed by SteamVR, incurring substantial IPC overhead, process switching delays, and unpredictable frame pacing.

**MBP-AVP-VR** eliminates the middleman. We engineered a complete C++ proxy DLL (`openvr_api.dll`) that forcibly injects itself into the game process. By mimicking the VTable structure of SteamVR's COM interfaces, we trick the game engine (like Half-Life: Alyx's Source 2 engine) into directly handing us raw DirectX 11 textures. We instantly dump these textures into shared memory and compress them using Apple's hardware media engine (VideoToolbox). The result is a "Mirage"—an illusion so perfectly orchestrated that the game believes it's rendering to a tethered PCVR headset, while delivering a wireless 120Hz retinal experience to the Apple Vision Pro.

---

## 2. System Architecture & Data Flow (Deep Technical Dive)

### Game Porting Toolkit (D3DMetal)
We rely on Apple's **Game Porting Toolkit (GPTK)**, specifically the **D3DMetal** translation layer. When we launch `hlvr.exe` via Wine with the `-dx11` flag, D3DMetal intercepts all DirectX 11 API calls and translates them directly into native Apple Metal API calls. **Vulkan (and DXVK) are completely bypassed**, eliminating an entire translation step and unlocking native-level Metal performance on Apple Silicon.

### The OpenVR Mocking Engine (`openvr_api.dll`)
Located in `VRDriver/src/openvr_emulator.cpp`, this is the heart of the hijacking system. OpenVR is built on C++ COM interfaces with virtual function tables (VTables). We reverse-engineered the memory layout of `IVRSystem`, `IVRCompositor`, and `IVRSettings`.
To prevent segmentation faults caused by missing vtable entries, our `UniversalMock` class injects precisely 100 dummy padding functions (`Dummy0()` to `Dummy99()`). This guarantees memory alignment.
When the game calls `IVRCompositor::Submit()`, instead of sending the frame to SteamVR, our emulator extracts the `ID3D11Texture2D` object directly.

### Shared Memory IPC (`vr_shared_frame.dat`)
Inter-Process Communication (IPC) is the enemy of low latency. We use an ultra-fast memory-mapped file `C:\vr_shared_frame.dat` mapped via `CreateFileMappingA`.
- **Texture Staging**: A CPU-readable `ID3D11Texture2D` staging buffer is created. The raw 4K NV12/BGRA pixel data is copied from VRAM to RAM.
- **The Payload**: The memory map consists of a 16-byte header, the raw pixel buffer, and a `SharedHands` struct (236 bytes) containing full `simd_float4x4` transform matrices, button states, and stick states from the Vision Pro. This bidirectional memory pool allows the Mac encoder and the Windows game process to communicate synchronously at 120Hz.

### HardwareEncoder (Swift)
Located in `MacEncoder/HardwareEncoder.swift`, this standalone macOS daemon continuously polls the shared memory.
- **Color Correction**: D3DMetal outputs BGRA, but Apple's VideoToolbox requires ARGB or NV12 formats, resulting in blue-skinned characters if encoded natively. We utilize Apple's `Accelerate` framework (`vImagePermuteChannels_ARGB8888`) to execute SIMD hardware-accelerated Red/Blue channel swapping in less than 1ms for a 4K frame.
- **VideoToolbox Extremes**: We drive the hardware HEVC encoder at extreme parameters:
  - `kVTCompressionPropertyKey_AllowFrameReordering = false` (Disables B-Frames).
  - `kVTCompressionPropertyKey_ExpectedFrameRate = 120` (Paced for 120fps).
  - `kVTCompressionPropertyKey_AverageBitRate = 150_000_000` (150 Mbps for zero artifacting).
  - `kVTCompressionPropertyKey_MaxKeyFrameInterval = 60` (I-Frame every 0.5s for immediate packet loss recovery).

---

## 3. The Ultimate Full Hand Tracking Engine

We completely eradicated the need for physical controllers (e.g., Joy-Cons). We leverage VisionOS's `HandTrackingProvider` to turn your bare hands into precision VR input devices.

### Skeletal Joint Calculations
In `VisionProClient/VisionSpatialToolsApp/VisionSpatialTools/VisionSpatialTools/Models/AppModel.swift`, we track 25 skeletal joints per hand. Using `simd_distance`, we calculate the precise physical distance between spatial points (e.g., `.thumbTip` and `.indexFingerTip`).

### Gesture Recognition Logic
We map organic human movements to discrete OpenVR `VRControllerState_t` axis and button inputs:
- **Trigger (Fire / Select)**: Distance between Thumb Tip and Index Tip `< 0.025m (2.5cm)`. This sets the OpenVR trigger axis (`rAxis[1].x`) to `1.0`.
- **Grip (Grab / Gravity Gloves)**: Distance between Middle, Ring, and Pinky Tips to the Palm Center `< 0.06m (6.0cm)`. Sets the OpenVR grip axis (`rAxis[2].x`).
- **A Button**: Thumb Tip to Ring Tip `< 2.5cm`.
- **B Button**: Thumb Tip to Pinky Tip `< 2.5cm`.

### The Virtual Joystick (Locomotion)
To simulate an analog thumbstick for walking and turning, we created a revolutionary "Virtual Joystick" mechanic.
1. When the user pinches their Thumb and Middle finger together, we capture the hand's current rotational matrix as the **Neutral Anchor**.
2. As the user tilts their wrist (Pitch) while holding the pinch, we calculate `matrix_multiply(simd_inverse(neutral), currentRot)`.
3. The extracted Pitch translates to `Stick Y` (Forward/Backward), and Roll translates to `Stick X` (Strafe/Turn).
Releasing the pinch instantly snaps the analog stick back to `0.0, 0.0`.

---

## 4. Custom UDP Networking Protocol (`VRAN`)

Standard TCP is catastrophic for VR streaming due to Head-of-Line blocking and retransmission delays. We built a pure, fragmented UDP pipeline.

### AWDL / Bonjour Discovery
The Mac encoder and the Vision Pro discover each other instantly using Apple Wireless Direct Link (AWDL) and `_verantyxvr._udp` Bonjour broadcasts. By connecting directly to the Mac's Internet Sharing Wi-Fi AP, we completely bypass router latency.

### Packet Fragmentation & I-Frame Recovery
A 4K HEVC encoded frame vastly exceeds the standard 1500-byte UDP MTU.
1. The Swift encoder splits the `CMSampleBuffer` into `1400-byte` fragments.
2. Each fragment is prepended with a custom 16-byte `VRAN` header (`Sequence Number`, `Chunk Index`, `Total Chunks`).
3. `VisionClientCompositor.swift` receives these chunks, drops incomplete sequences to prevent decoding corruption, and assembles full NAL units.
4. The NAL units are fed into `AVSampleBufferDisplayLayer` for zero-latency hardware decoding. If a packet is lost, the encoder's 0.5s I-Frame interval guarantees immediate visual recovery.

---

## 5. Setup & Installation Guide

### Requirements
- Apple Silicon Mac (M1/M2/M3)
- Apple Vision Pro
- macOS Sonoma or newer
- Xcode 15 or newer
- Game Porting Toolkit (GPTK / D3DMetal)
- Windows Steam + Half-Life: Alyx (installed inside the GPTK Wine prefix)
- MinGW (`x86_64-w64-mingw32-g++`), Python 3, Swift compiler

### Cloning and Compilation

This repository consolidates the OpenVR emulator, the Mac Hardware Encoder, and the Vision Pro Client.

```bash
# Clone the unified repository
git clone https://github.com/Ag3497120/MBP-AVP-VR.git
cd MBP-AVP-VR

# 1. Compile the OpenVR Emulator DLL (Cross-compile to Windows DLL)
cd VRDriver/src
x86_64-w64-mingw32-g++ -shared -static-libgcc -static-libstdc++ -I../include openvr_emulator.cpp ../openvr_api.def -o openvr_api.dll

# Replace the game's actual openvr_api.dll with our mock DLL
cp openvr_api.dll "$HOME/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/Program Files (x86)/Steam/steamapps/common/Half-Life Alyx/game/bin/win64/openvr_api.dll"

# 2. Compile the Mac Hardware Encoder
cd ../../MacEncoder
swiftc HardwareEncoder.swift -o HardwareEncoder -O
```

### 3. Deploying the Vision Pro Client
Open `VisionProClient/VisionSpatialToolsApp/VisionSpatialToolsApp.xcodeproj` in Xcode. Select your Apple Vision Pro as the target, and press `Cmd + R` to build and run the immersive app.

### Boot Sequence
1. Enable **Internet Sharing** on your Mac (System Settings > General > Sharing) and connect your Vision Pro to this direct Wi-Fi network.
2. Launch the server script:
   ```bash
   cd ~/MBP-AVP-VR/scripts
   ./launch_vision_pro_vr.sh
   ```
   This script spins up the `HardwareEncoder` daemon and then boots `hlvr.exe` via Wine/D3DMetal.
3. Put on your Vision Pro. The client will automatically discover the Mac via Bonjour and stream the 120Hz VR matrix straight into your eyes.

---

## 6. Troubleshooting

- **Symptom: The screen is blue (Smurf characters).**
  - **Fix**: The BGRA to ARGB `vImage` permutation in `HardwareEncoder.swift` failed. Ensure you are compiling with `-O` to enable Accelerate optimizations.
- **Symptom: Game crashes immediately upon opening.**
  - **Fix**: The VTable signature for `IVRSystem` may have changed in a recent SteamVR update. You must manually verify `openvr.h` and update the `Dummy()` padding counts in `openvr_emulator.cpp` to prevent segfaults.
- **Symptom: Severe visual stuttering.**
  - **Fix**: You are likely routed through a physical router. Connect the Vision Pro **directly** to the Mac's Internet Sharing Access Point to avoid local network congestion.

---
*Architected and developed by the Verantyx Advanced AI Team.*
