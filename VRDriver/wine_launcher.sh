#!/bin/bash
export WINEPREFIX="$HOME/Verantyx_VR_Drive/SteamVR_Prefix"
export PATH="/tmp/gptk_wine/bin:$PATH"
export DYLD_FALLBACK_LIBRARY_PATH="/tmp/gptk_wine/lib:/usr/lib"
export DYLD_LIBRARY_PATH="/tmp/gptk_wine/lib/external:$HOME/Verantyx_VR_Drive/moltenvk_env:$DYLD_LIBRARY_PATH"
export WINEESYNC=1
export WINEDLLOVERRIDES="dxgi,d3d9,d3d10core,d3d11=n,b;${WINEDLLOVERRIDES}"
export D3D11_ENABLE=1
export D3DMETAL_ENABLE=1
export D3DMetal=1
~/Verantyx_VR_Drive/wine_wrapper "$@"
