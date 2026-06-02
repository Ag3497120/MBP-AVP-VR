#!/bin/bash
set -e

# Project Beer: Automated Launch Script for Native Vulkan VR
# Ensure setup_beer_engine.sh has been run before executing this!

export WINEPREFIX="$HOME/Verantyx_VR_Drive/SteamVR_Prefix"
export PATH="$HOME/Verantyx_VR_Drive/Beer/wine/WineStaging.app/Contents/Resources/wine/bin:$PATH"

export WINEESYNC=1
export WINEFSYNC=1

# Native Vulkan forces DX11/DX12 to be disabled, so no DLL overrides are needed.
export WINEDLLOVERRIDES=""

STEAM_DIR="$WINEPREFIX/drive_c/Program Files (x86)/Steam"
STEAMVR_DIR="$STEAM_DIR/steamapps/common/SteamVR"
HLA_DIR="$STEAM_DIR/steamapps/common/Half-Life Alyx/game/bin/win64"

# Clear old pose data
rm -f "$HOME/Verantyx_VR_Drive/verantyx_pose.txt"

# Kill lingering processes
pkill -9 -f wine || true
killall -9 wineserver || true
sleep 1

# Launch the Tracker Streamer
cd "$HLA_DIR"
wine tb_streamer.exe 127.0.0.1 11000 > "$HOME/Verantyx_VR_Drive/tb_streamer_vulkan.log" 2>&1 &
TB_PID=$!
sleep 2

# Launch OpenVR / SteamVR Emulation
cd "$STEAMVR_DIR/bin/win64"
wine vrserver.exe > "$HOME/Verantyx_VR_Drive/vrserver_out_vulkan.log" 2>&1 &
VR_PID=$!
sleep 3

wine vrcompositor.exe > "$HOME/Verantyx_VR_Drive/vrcompositor_out_vulkan.log" 2>&1 &
COMP_PID=$!
sleep 5

# Launch Python Tracking/Texture Receivers
python3 "$HOME/Verantyx_VR_Drive/test_tracking.py" > "$HOME/Verantyx_VR_Drive/test_tracking_vulkan.log" 2>&1 &
PY_PID=$!

python3 "$HOME/Verantyx_VR_Drive/test_texture_recv.py" > "$HOME/Verantyx_VR_Drive/test_texture_vulkan.log" 2>&1 &
RECV_PID=$!

# Launch Half-Life: Alyx
# -vulkan is the crucial flag here. It bypasses DX11 entirely.
cd "$HLA_DIR"
echo "[*] Launching Half-Life: Alyx with Native Vulkan backend..."
wine hlvr.exe -vulkan -vr -windowed -w 1280 -h 720 -noassert -novid -console > "$HOME/Verantyx_VR_Drive/hla_out_vulkan.log" 2>&1

# Cleanup on exit
kill -9 $TB_PID $VR_PID $COMP_PID $PY_PID $RECV_PID
