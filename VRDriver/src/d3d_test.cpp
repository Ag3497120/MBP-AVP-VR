#include <windows.h>
#include <d3d11.h>
#include <stdio.h>

int main() {
    ID3D11Device* pDevice = NULL;
    ID3D11DeviceContext* pContext = NULL;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
    D3D_FEATURE_LEVEL featureLevel;
    
    HRESULT hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &pDevice, &featureLevel, &pContext);
    if (SUCCEEDED(hr)) {
        printf("D3D11CreateDevice SUCCEEDED: Feature Level %x\n", featureLevel);
        pDevice->Release();
        pContext->Release();
    } else {
        printf("D3D11CreateDevice FAILED: hr = 0x%08X\n", hr);
    }
    return 0;
}
