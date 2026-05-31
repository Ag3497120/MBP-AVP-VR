# Verantyx Mirage

A VR Hooking and Proxy System designed for Half-Life: Alyx on macOS via Game Porting Toolkit (Wine).

## Overview
This repository contains experimental tools to inject and intercept `openvr_api.dll` calls in order to extract rendered framebuffers directly from the game engine before they are sent to the SteamVR Compositor. This circumvents the limitations of D3DMetal background process initialization.

## Components
- `src/verantyx_hook.cpp`: MinHook-based DLL to intercept `VR_GetGenericInterface` and hook `IVRCompositor::Submit` to dump frames.
- `src/verantyx_injector.cpp`: DLL Injector using `CreateProcessA` suspended and `CreateRemoteThread` (currently causes IPC initialization issues).
- `src/openvr_api_proxy.cpp`: Proof-of-concept Proxy DLL for hooking without external injectors.
- `scripts/run_hla.sh`: Shell script to boot the environment.
