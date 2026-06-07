#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <d3d11.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#include <stdint.h>

#pragma comment(lib, "d3d11.lib")

int main() {
    printf("Starting D3DMetal Texture Analysis...\n");
    
    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    D3D_FEATURE_LEVEL featureLevel;
    
    HRESULT hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &device, &featureLevel, &context
    );
    
    if (FAILED(hr)) {
        printf("Failed to create D3D11 device: %x\n", hr);
        return 1;
    }
    printf("D3D11 Device created: %p\n", device);
    
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = 1024;
    desc.Height = 1024;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
    
    ID3D11Texture2D* texture = nullptr;
    hr = device->CreateTexture2D(&desc, nullptr, &texture);
    if (FAILED(hr)) {
        printf("Failed to create texture: %x\n", hr);
        return 1;
    }
    
    // Create Staging Texture
    D3D11_TEXTURE2D_DESC stagingDesc = desc;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.MiscFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    
    ID3D11Texture2D* stagingTexture = nullptr;
    hr = device->CreateTexture2D(&stagingDesc, nullptr, &stagingTexture);
    if (FAILED(hr)) {
        printf("Failed to create staging texture: %x\n", hr);
        return 1;
    }
    
    // Time the Copy and Map
    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);
    
    double total_ms = 0.0;
    
    for (int i = 0; i < 100; i++) {
        QueryPerformanceCounter(&start);
        
        context->CopyResource(stagingTexture, texture);
        
        D3D11_MAPPED_SUBRESOURCE mapped;
        hr = context->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);
        if (SUCCEEDED(hr)) {
            // Read first pixel to force evaluation
            volatile uint8_t* pixels = (uint8_t*)mapped.pData;
            volatile uint8_t p = pixels[0];
            context->Unmap(stagingTexture, 0);
        } else {
            printf("Map failed: %x\n", hr);
        }
        
        QueryPerformanceCounter(&end);
        double ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
        
        if (i >= 10) {
            total_ms += ms;
        }
    }
    
    printf("Average CopyResource + Map took: %.3f ms\n", total_ms / 90.0);
    
    // Cleanup
    if (stagingTexture) stagingTexture->Release();
    if (texture) texture->Release();
    if (device) device->Release();
    context->Release();
    
    return 0;
}
