#include <d3d11.h>
// Example code just to test compilation of D3D11 OMGetRenderTargets
void steal(ID3D11DeviceContext* pContext) {
    ID3D11RenderTargetView* rtv = nullptr;
    ID3D11DepthStencilView* dsv = nullptr;
    pContext->OMGetRenderTargets(1, &rtv, &dsv);
    if (dsv) {
        ID3D11Resource* res = nullptr;
        dsv->GetResource(&res);
        ID3D11Texture2D* depthTex = nullptr;
        res->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&depthTex);
        // We have the depth texture!
    }
}
