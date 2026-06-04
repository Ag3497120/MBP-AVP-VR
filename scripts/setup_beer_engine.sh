#!/bin/bash
set -e

# Project Beer: Automated Setup for Native Vulkan VR Pipeline
# This completely replaces Whisky and Apple GPTK with Gcenx Wine-Staging.

WINE_VERSION="11.9"
DOWNLOAD_URL="https://github.com/Gcenx/macOS_Wine_builds/releases/download/${WINE_VERSION}/wine-staging-${WINE_VERSION}-osx64.tar.xz"
INSTALL_DIR="$HOME/Verantyx_VR_Drive/Beer/wine"
APP_NAME="WineStaging.app"
PREFIX_DIR="$HOME/Verantyx_VR_Drive/SteamVR_Prefix"

echo "=== Verantyx-Mirage: Project Beer Setup ==="

# 1. Download and extract Gcenx Wine-Staging
if [ ! -d "$INSTALL_DIR/$APP_NAME" ]; then
    echo "[*] Downloading Gcenx Wine-Staging v$WINE_VERSION..."
    mkdir -p "$INSTALL_DIR"
    curl -L "$DOWNLOAD_URL" | tar -xJ -C "$INSTALL_DIR"
    
    # Rename app to remove spaces (which break Wine paths)
    echo "[*] Renaming Wine Staging.app to $APP_NAME..."
    mv "$INSTALL_DIR/Wine Staging.app" "$INSTALL_DIR/$APP_NAME"
else
    echo "[+] Gcenx Wine-Staging already installed at $INSTALL_DIR/$APP_NAME"
fi

# 2. Configure Prefix
echo "[*] Initializing SteamVR Wine Prefix at $PREFIX_DIR..."
export WINEPREFIX="$PREFIX_DIR"
export PATH="$INSTALL_DIR/$APP_NAME/Contents/Resources/wine/bin:$PATH"

wineboot -u

# 3. Clean up conflicting translation layers (DXVK/D3DMetal)
echo "[*] Cleaning up old DX11 translation DLLs to force native Vulkan..."
rm -f "$PREFIX_DIR/drive_c/windows/system32/dxvk"*.dll
rm -f "$PREFIX_DIR/drive_c/windows/system32/d3d11.dll"
rm -f "$PREFIX_DIR/drive_c/windows/system32/dxgi.dll"

echo "=== Setup Complete! You can now use launch_beer_vr.sh ==="
