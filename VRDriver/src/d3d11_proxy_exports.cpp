#include <windows.h>
#include <d3d11.h>
#include <stdio.h>
#include <stdlib.h>

HMODULE g_hOriginalD3D11 = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        FILE* f = fopen("C:\\d3d11_proxy_log.txt", "a");
        if(f) { fprintf(f, "[Verantyx] d3d11.dll proxy loaded!\n"); fclose(f); }
    }
    return TRUE;
}

void EnsureD3D11Loaded() {
    if (!g_hOriginalD3D11) {
        char path[MAX_PATH];
        GetSystemDirectoryA(path, MAX_PATH);
        strcat_s(path, "\\d3d11.dll");
        g_hOriginalD3D11 = LoadLibraryA(path);
    }
}

class ProxyID3D11Device : public ID3D11Device {
private:
    ID3D11Device* pOriginal;
public:
    ProxyID3D11Device(ID3D11Device* orig) : pOriginal(orig) {}

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) { return pOriginal->QueryInterface(riid, ppvObject); }
    virtual ULONG STDMETHODCALLTYPE AddRef(void) { return pOriginal->AddRef(); }
    virtual ULONG STDMETHODCALLTYPE Release(void) {
        ULONG count = pOriginal->Release();
        if (count == 0) delete this;
        return count;
    }

    virtual HRESULT STDMETHODCALLTYPE CreateBuffer(const D3D11_BUFFER_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer) {
        D3D11_BUFFER_DESC desc = *pDesc;
        if (desc.MiscFlags & D3D11_RESOURCE_MISC_SHARED) {
            FILE* f = fopen("C:\\d3d11_proxy_log.txt", "a");
            if(f) { fprintf(f, "Stripped D3D11_RESOURCE_MISC_SHARED from Buffer %u\n", desc.ByteWidth); fclose(f); }
            desc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED;
        }
        return pOriginal->CreateBuffer(&desc, pInitialData, ppBuffer);
    }
    
    virtual HRESULT STDMETHODCALLTYPE CreateTexture1D(const D3D11_TEXTURE1D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture1D **ppTexture1D) { return pOriginal->CreateTexture1D(pDesc, pInitialData, ppTexture1D); }
    
    virtual HRESULT STDMETHODCALLTYPE CreateTexture2D(const D3D11_TEXTURE2D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture2D **ppTexture2D) {
        D3D11_TEXTURE2D_DESC desc = *pDesc;
        if (desc.MiscFlags & D3D11_RESOURCE_MISC_SHARED) {
            FILE* f = fopen("C:\\d3d11_proxy_log.txt", "a");
            if(f) { fprintf(f, "Stripped D3D11_RESOURCE_MISC_SHARED from Texture2D\n"); fclose(f); }
            desc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED;
            desc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
            desc.MiscFlags &= ~D3D11_RESOURCE_MISC_SHARED_NTHANDLE;
        }
        return pOriginal->CreateTexture2D(&desc, pInitialData, ppTexture2D);
    }
    
    virtual HRESULT STDMETHODCALLTYPE CreateTexture3D(const D3D11_TEXTURE3D_DESC *pDesc, const D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Texture3D **ppTexture3D) { return pOriginal->CreateTexture3D(pDesc, pInitialData, ppTexture3D); }
    virtual HRESULT STDMETHODCALLTYPE CreateShaderResourceView(ID3D11Resource *pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D11ShaderResourceView **ppSRView) { return pOriginal->CreateShaderResourceView(pResource, pDesc, ppSRView); }
    virtual HRESULT STDMETHODCALLTYPE CreateUnorderedAccessView(ID3D11Resource *pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC *pDesc, ID3D11UnorderedAccessView **ppUAView) { return pOriginal->CreateUnorderedAccessView(pResource, pDesc, ppUAView); }
    virtual HRESULT STDMETHODCALLTYPE CreateRenderTargetView(ID3D11Resource *pResource, const D3D11_RENDER_TARGET_VIEW_DESC *pDesc, ID3D11RenderTargetView **ppRTView) { return pOriginal->CreateRenderTargetView(pResource, pDesc, ppRTView); }
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilView(ID3D11Resource *pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D11DepthStencilView **ppDepthStencilView) { return pOriginal->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView); }
    virtual HRESULT STDMETHODCALLTYPE CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout **ppInputLayout) { return pOriginal->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout); }
    virtual HRESULT STDMETHODCALLTYPE CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader) { return pOriginal->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader); }
    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader) { return pOriginal->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader); }
    virtual HRESULT STDMETHODCALLTYPE CreateGeometryShaderWithStreamOutput(const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, const UINT *pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage *pClassLinkage, ID3D11GeometryShader **ppGeometryShader) { return pOriginal->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader); }
    virtual HRESULT STDMETHODCALLTYPE CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader) { return pOriginal->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader); }
    virtual HRESULT STDMETHODCALLTYPE CreateHullShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11HullShader **ppHullShader) { return pOriginal->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader); }
    virtual HRESULT STDMETHODCALLTYPE CreateDomainShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11DomainShader **ppDomainShader) { return pOriginal->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader); }
    virtual HRESULT STDMETHODCALLTYPE CreateComputeShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11ComputeShader **ppComputeShader) { return pOriginal->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader); }
    virtual HRESULT STDMETHODCALLTYPE CreateClassLinkage(ID3D11ClassLinkage **ppLinkage) { return pOriginal->CreateClassLinkage(ppLinkage); }
    virtual HRESULT STDMETHODCALLTYPE CreateBlendState(const D3D11_BLEND_DESC *pBlendStateDesc, ID3D11BlendState **ppBlendState) { return pOriginal->CreateBlendState(pBlendStateDesc, ppBlendState); }
    virtual HRESULT STDMETHODCALLTYPE CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D11DepthStencilState **ppDepthStencilState) { return pOriginal->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState); }
    virtual HRESULT STDMETHODCALLTYPE CreateRasterizerState(const D3D11_RASTERIZER_DESC *pRasterizerDesc, ID3D11RasterizerState **ppRasterizerState) { return pOriginal->CreateRasterizerState(pRasterizerDesc, ppRasterizerState); }
    virtual HRESULT STDMETHODCALLTYPE CreateSamplerState(const D3D11_SAMPLER_DESC *pSamplerDesc, ID3D11SamplerState **ppSamplerState) { return pOriginal->CreateSamplerState(pSamplerDesc, ppSamplerState); }
    virtual HRESULT STDMETHODCALLTYPE CreateQuery(const D3D11_QUERY_DESC *pQueryDesc, ID3D11Query **ppQuery) { return pOriginal->CreateQuery(pQueryDesc, ppQuery); }
    virtual HRESULT STDMETHODCALLTYPE CreatePredicate(const D3D11_QUERY_DESC *pPredicateDesc, ID3D11Predicate **ppPredicate) { return pOriginal->CreatePredicate(pPredicateDesc, ppPredicate); }
    virtual HRESULT STDMETHODCALLTYPE CreateCounter(const D3D11_COUNTER_DESC *pCounterDesc, ID3D11Counter **ppCounter) { return pOriginal->CreateCounter(pCounterDesc, ppCounter); }
    virtual HRESULT STDMETHODCALLTYPE CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext **ppDeferredContext) { return pOriginal->CreateDeferredContext(ContextFlags, ppDeferredContext); }
    virtual HRESULT STDMETHODCALLTYPE OpenSharedResource(HANDLE hResource, REFIID ReturnedInterface, void **ppResource) { return pOriginal->OpenSharedResource(hResource, ReturnedInterface, ppResource); }
    virtual HRESULT STDMETHODCALLTYPE CheckFormatSupport(DXGI_FORMAT Format, UINT *pFormatSupport) { return pOriginal->CheckFormatSupport(Format, pFormatSupport); }
    virtual HRESULT STDMETHODCALLTYPE CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels) { return pOriginal->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels); }
    virtual void STDMETHODCALLTYPE CheckCounterInfo(D3D11_COUNTER_INFO *pCounterInfo) { pOriginal->CheckCounterInfo(pCounterInfo); }
    virtual HRESULT STDMETHODCALLTYPE CheckCounter(const D3D11_COUNTER_DESC *pDesc, D3D11_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength) { return pOriginal->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength); }
    virtual HRESULT STDMETHODCALLTYPE CheckFeatureSupport(D3D11_FEATURE Feature, void *pFeatureSupportData, UINT FeatureSupportDataSize) { return pOriginal->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize); }
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT *pDataSize, void *pData) { return pOriginal->GetPrivateData(guid, pDataSize, pData); }
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void *pData) { return pOriginal->SetPrivateData(guid, DataSize, pData); }
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown *pData) { return pOriginal->SetPrivateDataInterface(guid, pData); }
    virtual D3D_FEATURE_LEVEL STDMETHODCALLTYPE GetFeatureLevel(void) { return pOriginal->GetFeatureLevel(); }
    virtual UINT STDMETHODCALLTYPE GetCreationFlags(void) { return pOriginal->GetCreationFlags(); }
    virtual HRESULT STDMETHODCALLTYPE GetDeviceRemovedReason(void) { return pOriginal->GetDeviceRemovedReason(); }
    virtual void STDMETHODCALLTYPE GetImmediateContext(ID3D11DeviceContext **ppImmediateContext) { pOriginal->GetImmediateContext(ppImmediateContext); }
    virtual HRESULT STDMETHODCALLTYPE SetExceptionMode(UINT RaiseFlags) { return pOriginal->SetExceptionMode(RaiseFlags); }
    virtual UINT STDMETHODCALLTYPE GetExceptionMode(void) { return pOriginal->GetExceptionMode(); }
};

typedef HRESULT (WINAPI *D3D11CreateDevice_t)(
    IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT,
    const D3D_FEATURE_LEVEL*, UINT, UINT,
    ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

extern "C" __declspec(dllexport) HRESULT WINAPI D3D11CreateDevice(
    IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
    const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
    ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    
    FILE* f = fopen("C:\\d3d11_proxy_log.txt", "a");
    if(f) { fprintf(f, "[Verantyx] D3D11CreateDevice PROXY CALLED!\n"); fclose(f); }
    
    EnsureD3D11Loaded();
    D3D11CreateDevice_t pOrig = (D3D11CreateDevice_t)GetProcAddress(g_hOriginalD3D11, "D3D11CreateDevice");
    if (!pOrig) {
        if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] ERROR: pOrig is NULL! g_hOriginalD3D11=%p\n", g_hOriginalD3D11); fclose(f);} }
        return E_FAIL;
    }

    if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] D3D11CreateDevice overriding adapter to NULL and DriverType to HARDWARE\n"); fclose(f);} }
    pAdapter = NULL;
    DriverType = D3D_DRIVER_TYPE_HARDWARE;
    Flags &= ~D3D11_CREATE_DEVICE_DEBUG;
    Flags &= ~D3D11_CREATE_DEVICE_SINGLETHREADED;
    
    if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){
        fprintf(f, "[Verantyx] D3D11CreateDevice requested %d feature levels. SDKVersion=%u\n", FeatureLevels, SDKVersion);
        for(UINT i=0; i<FeatureLevels; i++) {
            fprintf(f, "  FeatureLevel[%d] = %04X\n", i, pFeatureLevels[i]);
        }
        fclose(f);
    } }
    
    ID3D11Device* pOrigDevice = NULL;
    HRESULT hr = pOrig(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, &pOrigDevice, pFeatureLevel, ppImmediateContext);
    
    if (FAILED(hr)) {
        if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] D3D11CreateDevice still failed with hr=%08X\n", hr); fclose(f);} }
    }
    if (SUCCEEDED(hr) && pOrigDevice && ppDevice) {
        *ppDevice = new ProxyID3D11Device(pOrigDevice);
        if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] Successfully wrapped ID3D11Device!\n"); fclose(f);} }
    } else if (ppDevice) {
        *ppDevice = pOrigDevice;
    }
    return hr;
}

typedef HRESULT (WINAPI *D3D11CreateDeviceAndSwapChain_t)(
    IDXGIAdapter*, D3D_DRIVER_TYPE, HMODULE, UINT,
    const D3D_FEATURE_LEVEL*, UINT, UINT, const DXGI_SWAP_CHAIN_DESC*,
    IDXGISwapChain**, ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext**);

extern "C" __declspec(dllexport) HRESULT WINAPI D3D11CreateDeviceAndSwapChain(
    IDXGIAdapter* pAdapter, D3D_DRIVER_TYPE DriverType, HMODULE Software, UINT Flags,
    const D3D_FEATURE_LEVEL* pFeatureLevels, UINT FeatureLevels, UINT SDKVersion,
    const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc, IDXGISwapChain** ppSwapChain,
    ID3D11Device** ppDevice, D3D_FEATURE_LEVEL* pFeatureLevel, ID3D11DeviceContext** ppImmediateContext) {
    
    FILE* f = fopen("C:\\d3d11_proxy_log.txt", "a");
    if(f) { fprintf(f, "[Verantyx] D3D11CreateDeviceAndSwapChain PROXY CALLED!\n"); fclose(f); }

    EnsureD3D11Loaded();
    D3D11CreateDeviceAndSwapChain_t pOrig = (D3D11CreateDeviceAndSwapChain_t)GetProcAddress(g_hOriginalD3D11, "D3D11CreateDeviceAndSwapChain");
    if (!pOrig) return E_FAIL;

    if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] D3D11CreateDeviceAndSwapChain overriding adapter to NULL and DriverType to HARDWARE\n"); fclose(f);} }
    pAdapter = NULL;
    DriverType = D3D_DRIVER_TYPE_HARDWARE;
    Flags &= ~D3D11_CREATE_DEVICE_DEBUG;
    Flags &= ~D3D11_CREATE_DEVICE_SINGLETHREADED;

    if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){
        fprintf(f, "[Verantyx] SwapChain requested %d feature levels. SDKVersion=%u\n", FeatureLevels, SDKVersion);
        for(UINT i=0; i<FeatureLevels; i++) {
            fprintf(f, "  FeatureLevel[%d] = %04X\n", i, pFeatureLevels[i]);
        }
        fclose(f);
    } }

    ID3D11Device* pOrigDevice = NULL;
    HRESULT hr = pOrig(pAdapter, DriverType, Software, Flags, pFeatureLevels, FeatureLevels, SDKVersion, pSwapChainDesc, ppSwapChain, &pOrigDevice, pFeatureLevel, ppImmediateContext);
    
    if (FAILED(hr)) {
        if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] SwapChain device creation still failed with hr=%08X\n", hr); fclose(f);} }
    }
    if (SUCCEEDED(hr) && pOrigDevice && ppDevice) {
        *ppDevice = new ProxyID3D11Device(pOrigDevice);
        if(f) { f = fopen("C:\\d3d11_proxy_log.txt", "a"); if(f){fprintf(f, "[Verantyx] Successfully wrapped ID3D11Device in SwapChain call!\n"); fclose(f);} }
    } else if (ppDevice) {
        *ppDevice = pOrigDevice;
    }
    return hr;
}
