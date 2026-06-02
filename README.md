# Verantyx Mirage

A Native VR Hooking and Proxy System designed for Half-Life: Alyx on macOS.

## The Native Vulkan Architecture (Project Beer)

Originally, this project attempted to utilize Apple's Game Porting Toolkit (D3DMetal) to translate DirectX 11. However, due to stripped Vulkan initialization within Apple's `wine64` and Rosetta 2 memory alignment access violations within DXVK, DX11 translations proved fatally unstable.

**Solution**: Half-Life: Alyx (Source 2) supports a native Vulkan renderer (`-vulkan`). By utilizing **Gcenx Wine-Staging** (which contains full MoltenVK integration), we bypass all DirectX translation layers entirely. The engine natively binds to Vulkan, which MoltenVK seamlessly translates to Apple Metal, providing a hyper-stable VR pipeline on Apple Silicon (M1/M2/M3).

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
