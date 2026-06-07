#include <windows.h>
#include <stdio.h>
#include <d3d11.h>
#include <dxgi.h>

typedef HRESULT (WINAPI *D3D11CreateDeviceAndSwapChain_t)(
    IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT,
    const D3D_FEATURE_LEVEL*, UINT, UINT,
    const DXGI_SWAP_CHAIN_DESC*, IDXGISwapChain**,
    ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

int main() {
    HMODULE hD3D11 = LoadLibraryA("d3d11.dll");
    D3D11CreateDeviceAndSwapChain_t pD3D11CreateDeviceAndSwapChain = (D3D11CreateDeviceAndSwapChain_t)GetProcAddress(hD3D11, "D3D11CreateDeviceAndSwapChain");

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = GetDesktopWindow();
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;

    HRESULT hr = pD3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
        featureLevels, 1, D3D11_SDK_VERSION,
        &scd, &pSwapChain, &pDevice, NULL, &pContext
    );

    if (SUCCEEDED(hr)) {
        void** pDeviceVTable = *(void***)pDevice;
        unsigned char* pCreateBuffer = (unsigned char*)pDeviceVTable[3];
        printf("CreateBuffer Address: %p\n", pCreateBuffer);
        printf("Bytes: ");
        for (int i=0; i<16; i++) {
            printf("%02X ", pCreateBuffer[i]);
        }
        printf("\n");
    }
    return 0;
}
