#include <windows.h>
#include <d3d11_1.h>
#include <d3d11_4.h>

void test(ID3D11Device* pDevice) {
    ID3D11Multithread* pMt = nullptr;
    pDevice->QueryInterface(__uuidof(ID3D11Multithread), (void**)&pMt);
}
