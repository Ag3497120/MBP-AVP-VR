#include <d3d11.h>
#include <windows.h>
#include <stdio.h>

#include "minhook/include/MinHook.h"

typedef void (STDMETHODCALLTYPE *ClearDepthStencilView_t)(
    ID3D11DeviceContext* This,
    ID3D11DepthStencilView *pDepthStencilView,
    UINT ClearFlags,
    FLOAT Depth,
    UINT8 Stencil);

ClearDepthStencilView_t oClearDepthStencilView = nullptr;
ID3D11DepthStencilView* g_lastDSV = nullptr;

void STDMETHODCALLTYPE hkClearDepthStencilView(
    ID3D11DeviceContext* This,
    ID3D11DepthStencilView *pDepthStencilView,
    UINT ClearFlags,
    FLOAT Depth,
    UINT8 Stencil)
{
    g_lastDSV = pDepthStencilView;
    oClearDepthStencilView(This, pDepthStencilView, ClearFlags, Depth, Stencil);
}

void hookContext(ID3D11DeviceContext* pContext) {
    static bool hooked = false;
    if (hooked) return;
    hooked = true;
    
    MH_Initialize();
    void** vtable = *(void***)pContext;
    MH_CreateHook(vtable[53], (LPVOID)hkClearDepthStencilView, (LPVOID*)&oClearDepthStencilView);
    MH_EnableHook(vtable[53]);
}
