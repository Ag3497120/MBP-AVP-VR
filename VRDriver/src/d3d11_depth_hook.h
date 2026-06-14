#pragma once
#include <d3d11.h>
#include <stdio.h>
#include "minhook/include/MinHook.h"

typedef void (STDMETHODCALLTYPE *ClearDepthStencilView_t)(
    ID3D11DeviceContext* This,
    ID3D11DepthStencilView *pDepthStencilView,
    UINT ClearFlags,
    FLOAT Depth,
    UINT8 Stencil);

extern ClearDepthStencilView_t oClearDepthStencilView;
extern ID3D11DepthStencilView* g_lastEyeDSV;
extern ID3D11Resource* g_lastEyeDepthResource;

void hookD3D11Context(ID3D11DeviceContext* pContext);
