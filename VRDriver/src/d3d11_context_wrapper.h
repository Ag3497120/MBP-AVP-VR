
ID3D11DepthStencilView* g_lastEyeDSV = nullptr;
ID3D11Resource* g_lastEyeDepthResource = nullptr;

extern "C" __declspec(dllexport) ID3D11DepthStencilView* VerantyxGetDepthStencilView() {
    return g_lastEyeDSV;
}

extern "C" __declspec(dllexport) ID3D11Resource* VerantyxGetDepthResource() {
    return g_lastEyeDepthResource;
}

class ProxyID3D11DeviceContext : public ID3D11DeviceContext {
public:
    ID3D11DeviceContext* pOriginal;
    ProxyID3D11DeviceContext(ID3D11DeviceContext* orig) : pOriginal(orig) {}

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject) {
        if (riid == __uuidof(ID3D11DeviceContext)) {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        return pOriginal->QueryInterface(riid, ppvObject);
    }
    virtual ULONG STDMETHODCALLTYPE AddRef(void) { return pOriginal->AddRef(); }
    virtual ULONG STDMETHODCALLTYPE Release(void) {
        ULONG count = pOriginal->Release();
        if (count == 0) delete this;
        return count;
    }
    
    virtual void STDMETHODCALLTYPE GetDevice(ID3D11Device** ppDevice) {
        pOriginal->GetDevice(ppDevice);
    }
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(REFGUID guid, UINT* pDataSize, void* pData) {
        return pOriginal->GetPrivateData(guid, pDataSize, pData);
    }
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(REFGUID guid, UINT DataSize, const void* pData) {
        return pOriginal->SetPrivateData(guid, DataSize, pData);
    }
    virtual HRESULT STDMETHODCALLTYPE SetPrivateDataInterface(REFGUID guid, const IUnknown* pData) {
        return pOriginal->SetPrivateDataInterface(guid, pData);
    }

    virtual void STDMETHODCALLTYPE VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) { pOriginal->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE PSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews) { pOriginal->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE PSSetShader(ID3D11PixelShader *pPixelShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) { pOriginal->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances); }
    virtual void STDMETHODCALLTYPE PSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers) { pOriginal->PSSetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE VSSetShader(ID3D11VertexShader *pVertexShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) { pOriginal->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances); }
    virtual void STDMETHODCALLTYPE DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) { pOriginal->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation); }
    virtual void STDMETHODCALLTYPE Draw(UINT VertexCount, UINT StartVertexLocation) { pOriginal->Draw(VertexCount, StartVertexLocation); }
    virtual HRESULT STDMETHODCALLTYPE Map(ID3D11Resource *pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE *pMappedResource) { return pOriginal->Map(pResource, Subresource, MapType, MapFlags, pMappedResource); }
    virtual void STDMETHODCALLTYPE Unmap(ID3D11Resource *pResource, UINT Subresource) { pOriginal->Unmap(pResource, Subresource); }
    virtual void STDMETHODCALLTYPE PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) { pOriginal->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE IASetInputLayout(ID3D11InputLayout *pInputLayout) { pOriginal->IASetInputLayout(pInputLayout); }
    virtual void STDMETHODCALLTYPE IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets) { pOriginal->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    virtual void STDMETHODCALLTYPE IASetIndexBuffer(ID3D11Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset) { pOriginal->IASetIndexBuffer(pIndexBuffer, Format, Offset); }
    virtual void STDMETHODCALLTYPE DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) { pOriginal->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation); }
    virtual void STDMETHODCALLTYPE DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) { pOriginal->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation); }
    virtual void STDMETHODCALLTYPE GSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) { pOriginal->GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE GSSetShader(ID3D11GeometryShader *pShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) { pOriginal->GSSetShader(pShader, ppClassInstances, NumClassInstances); }
    virtual void STDMETHODCALLTYPE IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology) { pOriginal->IASetPrimitiveTopology(Topology); }
    virtual void STDMETHODCALLTYPE VSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews) { pOriginal->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE VSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers) { pOriginal->VSSetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE Begin(ID3D11Asynchronous *pAsync) { pOriginal->Begin(pAsync); }
    virtual void STDMETHODCALLTYPE End(ID3D11Asynchronous *pAsync) { pOriginal->End(pAsync); }
    virtual HRESULT STDMETHODCALLTYPE GetData(ID3D11Asynchronous *pAsync, void *pData, UINT DataSize, UINT GetDataFlags) { return pOriginal->GetData(pAsync, pData, DataSize, GetDataFlags); }
    virtual void STDMETHODCALLTYPE SetPredication(ID3D11Predicate *pPredicate, BOOL PredicateValue) { pOriginal->SetPredication(pPredicate, PredicateValue); }
    virtual void STDMETHODCALLTYPE GSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews) { pOriginal->GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE GSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers) { pOriginal->GSSetSamplers(StartSlot, NumSamplers, ppSamplers); }

    virtual void STDMETHODCALLTYPE OMSetRenderTargets(UINT NumViews, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView) {
        if (pDepthStencilView) {
            ID3D11Resource* res = nullptr;
            pDepthStencilView->GetResource(&res);
            if (res) {
                ID3D11Texture2D* tex = nullptr;
                if (SUCCEEDED(res->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&tex))) {
                    D3D11_TEXTURE2D_DESC desc;
                    tex->GetDesc(&desc);
                    if (desc.Width > 1500 && desc.Height > 1500 && desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM && desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB && desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM && desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM_SRGB) {
                        g_lastEyeDSV = pDepthStencilView;
                        g_lastEyeDepthResource = res;
                        
                        FILE* f = fopen("C:\\\\VR_Depth_Steal_Log.txt", "a");
                        if (f) {
                            fprintf(f, "CAUGHT DSV in proxy! %dx%d, Format=%d\n", desc.Width, desc.Height, desc.Format);
                            fclose(f);
                        }
                    }
                    tex->Release();
                }
                res->Release();
            }
        }
        return pOriginal->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
    }

    virtual void STDMETHODCALLTYPE OMSetRenderTargetsAndUnorderedAccessViews(UINT NumRTVs, ID3D11RenderTargetView *const *ppRenderTargetViews, ID3D11DepthStencilView *pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts) { pOriginal->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    virtual void STDMETHODCALLTYPE OMSetBlendState(ID3D11BlendState *pBlendState, const FLOAT BlendFactor[ 4 ], UINT SampleMask) { pOriginal->OMSetBlendState(pBlendState, BlendFactor, SampleMask); }
    virtual void STDMETHODCALLTYPE OMSetDepthStencilState(ID3D11DepthStencilState *pDepthStencilState, UINT StencilRef) { pOriginal->OMSetDepthStencilState(pDepthStencilState, StencilRef); }
    virtual void STDMETHODCALLTYPE SOSetTargets(UINT NumBuffers, ID3D11Buffer *const *ppSOTargets, const UINT *pOffsets) { pOriginal->SOSetTargets(NumBuffers, ppSOTargets, pOffsets); }
    virtual void STDMETHODCALLTYPE DrawAuto(void) { pOriginal->DrawAuto(); }
    virtual void STDMETHODCALLTYPE DrawIndexedInstancedIndirect(ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs) { pOriginal->DrawIndexedInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs); }
    virtual void STDMETHODCALLTYPE DrawInstancedIndirect(ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs) { pOriginal->DrawInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs); }
    virtual void STDMETHODCALLTYPE Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ) { pOriginal->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ); }
    virtual void STDMETHODCALLTYPE DispatchIndirect(ID3D11Buffer *pBufferForArgs, UINT AlignedByteOffsetForArgs) { pOriginal->DispatchIndirect(pBufferForArgs, AlignedByteOffsetForArgs); }
    virtual void STDMETHODCALLTYPE RSSetState(ID3D11RasterizerState *pRasterizerState) { pOriginal->RSSetState(pRasterizerState); }
    virtual void STDMETHODCALLTYPE RSSetViewports(UINT NumViewports, const D3D11_VIEWPORT *pViewports) { pOriginal->RSSetViewports(NumViewports, pViewports); }
    virtual void STDMETHODCALLTYPE RSSetScissorRects(UINT NumRects, const D3D11_RECT *pRects) { pOriginal->RSSetScissorRects(NumRects, pRects); }
    virtual void STDMETHODCALLTYPE CopySubresourceRegion(ID3D11Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource *pSrcResource, UINT SrcSubresource, const D3D11_BOX *pSrcBox) { pOriginal->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox); }
    virtual void STDMETHODCALLTYPE CopyResource(ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource) { pOriginal->CopyResource(pDstResource, pSrcResource); }
    virtual void STDMETHODCALLTYPE UpdateSubresource(ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch) { pOriginal->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch); }
    virtual void STDMETHODCALLTYPE CopyStructureCount(ID3D11Buffer *pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView *pSrcView) { pOriginal->CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView); }
    virtual void STDMETHODCALLTYPE ClearRenderTargetView(ID3D11RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[ 4 ]) { pOriginal->ClearRenderTargetView(pRenderTargetView, ColorRGBA); }
    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView *pUnorderedAccessView, const UINT Values[ 4 ]) { pOriginal->ClearUnorderedAccessViewUint(pUnorderedAccessView, Values); }
    virtual void STDMETHODCALLTYPE ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView *pUnorderedAccessView, const FLOAT Values[ 4 ]) { pOriginal->ClearUnorderedAccessViewFloat(pUnorderedAccessView, Values); }
    virtual void STDMETHODCALLTYPE ClearDepthStencilView(ID3D11DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil) { pOriginal->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil); }
    virtual void STDMETHODCALLTYPE GenerateMips(ID3D11ShaderResourceView *pShaderResourceView) { pOriginal->GenerateMips(pShaderResourceView); }
    virtual void STDMETHODCALLTYPE ResolveSubresource(ID3D11Resource *pDstResource, UINT DstSubresource, ID3D11Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format) { pOriginal->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format); }
    virtual void STDMETHODCALLTYPE ExecuteCommandList(ID3D11CommandList *pCommandList, BOOL RestoreContextState) { pOriginal->ExecuteCommandList(pCommandList, RestoreContextState); }
    virtual void STDMETHODCALLTYPE HSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews) { pOriginal->HSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE HSSetShader(ID3D11HullShader *pHullShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) { pOriginal->HSSetShader(pHullShader, ppClassInstances, NumClassInstances); }
    virtual void STDMETHODCALLTYPE HSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers) { pOriginal->HSSetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE HSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) { pOriginal->HSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE DSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews) { pOriginal->DSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE DSSetShader(ID3D11DomainShader *pDomainShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) { pOriginal->DSSetShader(pDomainShader, ppClassInstances, NumClassInstances); }
    virtual void STDMETHODCALLTYPE DSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers) { pOriginal->DSSetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE DSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) { pOriginal->DSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE CSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView *const *ppShaderResourceViews) { pOriginal->CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE CSSetUnorderedAccessViews(UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView *const *ppUnorderedAccessViews, const UINT *pUAVInitialCounts) { pOriginal->CSSetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts); }
    virtual void STDMETHODCALLTYPE CSSetShader(ID3D11ComputeShader *pComputeShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances) { pOriginal->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances); }
    virtual void STDMETHODCALLTYPE CSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState *const *ppSamplers) { pOriginal->CSSetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE CSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) { pOriginal->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers) { pOriginal->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE PSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews) { pOriginal->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE PSGetShader(ID3D11PixelShader **ppPixelShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances) { pOriginal->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances); }
    virtual void STDMETHODCALLTYPE PSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers) { pOriginal->PSGetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE VSGetShader(ID3D11VertexShader **ppVertexShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances) { pOriginal->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances); }
    virtual void STDMETHODCALLTYPE PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers) { pOriginal->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE IAGetInputLayout(ID3D11InputLayout **ppInputLayout) { pOriginal->IAGetInputLayout(ppInputLayout); }
    virtual void STDMETHODCALLTYPE IAGetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets) { pOriginal->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets); }
    virtual void STDMETHODCALLTYPE IAGetIndexBuffer(ID3D11Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset) { pOriginal->IAGetIndexBuffer(pIndexBuffer, Format, Offset); }
    virtual void STDMETHODCALLTYPE GSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers) { pOriginal->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE GSGetShader(ID3D11GeometryShader **ppGeometryShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances) { pOriginal->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances); }
    virtual void STDMETHODCALLTYPE IAGetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY *pTopology) { pOriginal->IAGetPrimitiveTopology(pTopology); }
    virtual void STDMETHODCALLTYPE VSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews) { pOriginal->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE VSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers) { pOriginal->VSGetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE GetPredication(ID3D11Predicate **ppPredicate, BOOL *pPredicateValue) { pOriginal->GetPredication(ppPredicate, pPredicateValue); }
    virtual void STDMETHODCALLTYPE GSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews) { pOriginal->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE GSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers) { pOriginal->GSGetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE OMGetRenderTargets(UINT NumViews, ID3D11RenderTargetView **ppRenderTargetViews, ID3D11DepthStencilView **ppDepthStencilView) { pOriginal->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView); }
    virtual void STDMETHODCALLTYPE OMGetRenderTargetsAndUnorderedAccessViews(UINT NumRTVs, ID3D11RenderTargetView **ppRenderTargetViews, ID3D11DepthStencilView **ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView **ppUnorderedAccessViews) { pOriginal->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews); }
    virtual void STDMETHODCALLTYPE OMGetBlendState(ID3D11BlendState **ppBlendState, FLOAT BlendFactor[ 4 ], UINT *pSampleMask) { pOriginal->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask); }
    virtual void STDMETHODCALLTYPE OMGetDepthStencilState(ID3D11DepthStencilState **ppDepthStencilState, UINT *pStencilRef) { pOriginal->OMGetDepthStencilState(ppDepthStencilState, pStencilRef); }
    virtual void STDMETHODCALLTYPE SOGetTargets(UINT NumBuffers, ID3D11Buffer **ppSOTargets) { pOriginal->SOGetTargets(NumBuffers, ppSOTargets); }
    virtual void STDMETHODCALLTYPE RSGetState(ID3D11RasterizerState **ppRasterizerState) { pOriginal->RSGetState(ppRasterizerState); }
    virtual void STDMETHODCALLTYPE RSGetViewports(UINT *pNumViewports, D3D11_VIEWPORT *pViewports) { pOriginal->RSGetViewports(pNumViewports, pViewports); }
    virtual void STDMETHODCALLTYPE RSGetScissorRects(UINT *pNumRects, D3D11_RECT *pRects) { pOriginal->RSGetScissorRects(pNumRects, pRects); }
    virtual void STDMETHODCALLTYPE HSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews) { pOriginal->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE HSGetShader(ID3D11HullShader **ppHullShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances) { pOriginal->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances); }
    virtual void STDMETHODCALLTYPE HSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers) { pOriginal->HSGetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE HSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers) { pOriginal->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE DSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews) { pOriginal->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE DSGetShader(ID3D11DomainShader **ppDomainShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances) { pOriginal->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances); }
    virtual void STDMETHODCALLTYPE DSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers) { pOriginal->DSGetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE DSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers) { pOriginal->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE CSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView **ppShaderResourceViews) { pOriginal->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews); }
    virtual void STDMETHODCALLTYPE CSGetUnorderedAccessViews(UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView **ppUnorderedAccessViews) { pOriginal->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews); }
    virtual void STDMETHODCALLTYPE CSGetShader(ID3D11ComputeShader **ppComputeShader, ID3D11ClassInstance **ppClassInstances, UINT *pNumClassInstances) { pOriginal->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances); }
    virtual void STDMETHODCALLTYPE CSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState **ppSamplers) { pOriginal->CSGetSamplers(StartSlot, NumSamplers, ppSamplers); }
    virtual void STDMETHODCALLTYPE CSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers) { pOriginal->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers); }
    virtual void STDMETHODCALLTYPE ClearState(void) { pOriginal->ClearState(); }
    virtual void STDMETHODCALLTYPE Flush(void) { pOriginal->Flush(); }
    virtual D3D11_DEVICE_CONTEXT_TYPE STDMETHODCALLTYPE GetType(void) { return pOriginal->GetType(); }
    virtual UINT STDMETHODCALLTYPE GetContextFlags(void) { return pOriginal->GetContextFlags(); }
    virtual HRESULT STDMETHODCALLTYPE FinishCommandList(BOOL RestoreDeferredContextState, ID3D11CommandList **ppCommandList) { return pOriginal->FinishCommandList(RestoreDeferredContextState, ppCommandList); }
    virtual void STDMETHODCALLTYPE SetResourceMinLOD(ID3D11Resource *pResource, FLOAT MinLOD) { pOriginal->SetResourceMinLOD(pResource, MinLOD); }
    virtual FLOAT STDMETHODCALLTYPE GetResourceMinLOD(ID3D11Resource *pResource) { return pOriginal->GetResourceMinLOD(pResource); }
};