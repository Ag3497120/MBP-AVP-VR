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

We have fully automated the installation and configuration of the Wine-Staging environment.

1. **Install the Engine**:
   Run the setup script to download Gcenx Wine-Staging 11.9 and configure your `SteamVR_Prefix`.
   ```bash
   bash scripts/setup_beer_engine.sh
   ```

2. **Launch the Game**:
   The launch script automatically handles SteamVR emulation, tracking receivers, and boots `hlvr.exe` using the `-vulkan` backend.
   ```bash
   bash scripts/launch_beer_vr.sh
   ```

## Components
- `scripts/setup_beer_engine.sh`: Automated Wine-Staging engine downloader and prefix creator.
- `scripts/launch_beer_vr.sh`: The master launch script that activates Native Vulkan.
- `src/verantyx_hook.cpp`: MinHook-based DLL to intercept `VR_GetGenericInterface`.
- `src/openvr_api_proxy.cpp`: Proof-of-concept Proxy DLL for hooking without external injectors.
