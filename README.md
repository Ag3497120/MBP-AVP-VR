# Verantyx Mirage

A Native VR Hooking and Proxy System designed for Half-Life: Alyx on macOS.

## The D3DMetal HEVC VR Bridge Architecture (Latest)

This project has successfully implemented a hyper-low latency PCVR bridge directly from Mac to Apple Vision Pro using Apple's Game Porting Toolkit (D3DMetal).

**Architecture**:
1. **DirectX 11 Interception**: The `openvr_emulator.cpp` hooks into `hlvr.exe` (Half-Life: Alyx) and intercepts the rendered DirectX 11 stereoscopic textures.
2. **Hardware Encoding (HEVC)**: `HardwareEncoder.swift` runs natively on macOS, reading the shared memory frames and instantly encoding them using VideoToolbox HEVC (H.265) hardware acceleration.
3. **UDP Streaming**: The encoded chunks are streamed via UDP directly to the Vision Pro (port 9999).
4. **Input Mapping**: Joy-Con or standard gamepads connected to the Mac are mapped into VR Controller inputs, feeding directly back into the OpenVR emulator.

## Setup Instructions

To achieve maximum performance with hyper-low latency, this project requires Apple's **Game Porting Toolkit (D3DMetal)**.
Due to Apple's licensing restrictions, D3DMetal cannot be redistributed with this repository. You must download and install it manually.

1. **Install the Base Wine Engine**:
   Run the setup script to download Gcenx Wine-Staging 11.9 and initialize your `SteamVR_Prefix`.
   ```bash
   bash scripts/setup_beer_engine.sh
   ```

2. **Download Apple's Game Porting Toolkit (GPTK)**:
   - Go to the [Apple Developer Downloads](https://developer.apple.com/download/all/) page (requires an Apple ID).
   - Search for **Game Porting Toolkit** and download the `.dmg` file.
   - Mount the `.dmg` by double-clicking it.

3. **Install D3DMetal into the Prefix**:
   Copy the D3DMetal libraries from the mounted Game Porting Toolkit into your Wine Prefix:
   ```bash
   # Replace /Volumes/Game Porting Toolkit-1.0 with your mounted path if different
   ditto /Volumes/Game\ Porting\ Toolkit-1.0/lib/ `brew --prefix`/lib/
   
   # Copy the D3DMetal DLLs into your SteamVR Prefix
   cp /Volumes/Game\ Porting\ Toolkit-1.0/lib/external/d3dmetal/x86_64/* ~/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/windows/system32/
   ```

4. **Launch the Game**:
   The launch script automatically handles SteamVR emulation, tracking receivers, and boots `hlvr.exe` using the installed D3DMetal translation layer.
   ```bash
   bash scripts/launch_beer_vr.sh
   ```

## Components
- `scripts/setup_beer_engine.sh`: Automated Wine-Staging engine downloader and prefix creator.
- `scripts/launch_beer_vr.sh`: The master launch script that activates Native Vulkan / D3DMetal.
- `src/verantyx_hook.cpp`: MinHook-based DLL to intercept `VR_GetGenericInterface`.
- `src/openvr_api_proxy.cpp`: Proof-of-concept Proxy DLL for hooking without external injectors.
