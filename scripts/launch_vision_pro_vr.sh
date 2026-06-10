#!/bin/bash
WINE_LAUNCHER="$HOME/verantyx-cli/cli/VRDriver/wine_launcher.sh"
STEAMVR_DIR="$HOME/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/Program Files (x86)/Steam/steamapps/common/SteamVR/bin/win64"
HLA_DIR="$HOME/Verantyx_VR_Drive/SteamVR_Prefix/drive_c/Program Files (x86)/Steam/steamapps/common/Half-Life Alyx/game/bin/win64"

echo "Bypassing SteamVR and using custom OpenVR API wrapper..."

echo "Starting MacEncoderServer (HardwareEncoder)..."
echo "Starting MacEncoderServer (HardwareEncoder)..."
cd "$HOME/Verantyx-Mirage/MacEncoder"
./HardwareEncoder > "$HOME/Verantyx_VR_Drive/hardware_encoder.log" 2>&1 &
ENC_PID=$!
echo "Joy-Cons disabled (Full Hand Tracking active)"
sleep 1


echo "Starting Half-Life: Alyx..."
cd "$HLA_DIR"
WINEDLLOVERRIDES="nvapi64=;nvapi=;d3d11=n,b;openvr_api=n,b" $WINE_LAUNCHER hlvr.exe -vr -novid -console -vconsole -dx11 +vr_fidelity_level_auto 0 +vr_fidelity_level 3 +vr_chaperone_enable 0 +vr_enable_chaperone 0 > "$HOME/Verantyx_VR_Drive/hla_out.log" 2>&1
HLA_PID=$!

# Wait for the game to exit
wait $HLA_PID

echo "Game exited. Cleaning up background processes..."
kill -9 $ENC_PID
killall -9 vrserver.exe vrcompositor.exe wineserver wine64-preloader 2>/dev/null
