#!/bin/bash
export WINEPREFIX="$HOME/Verantyx_VR_Drive/SteamVR_Prefix"
export STEAM_COMPAT_DATA_PATH="$WINEPREFIX"
export STEAM_COMPAT_CLIENT_INSTALL_PATH="C:/Program Files (x86)/Steam"
export WINEDLLOVERRIDES="d3d11=n,b;d3d10=n,b;dxgi=n,b"

killall -9 wine64-preloader wine64 wineserver >/dev/null 2>&1
sleep 2

cd "$WINEPREFIX/drive_c/Program Files (x86)/Steam/steamapps/common/SteamVR/bin/win64"
wine64 vrserver.exe > "$HOME/Verantyx_VR_Drive/hla_log.txt" 2>&1 &
sleep 5

cd "$WINEPREFIX/drive_c/Program Files (x86)/Steam/steamapps/common/Half-Life Alyx/game/bin/win64"
wine64 hlvr.exe -vr -nowindow -novid -condebug -dx11 >> "$HOME/Verantyx_VR_Drive/hla_log.txt" 2>&1 &
