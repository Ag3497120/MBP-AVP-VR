#include <d3d11.h>
#include <stdio.h>
void logDesc(ID3D11Texture2D* tex) {
    D3D11_TEXTURE2D_DESC desc;
    tex->GetDesc(&desc);
    FILE* f = fopen("C:\\\\VR_Depth_Desc.txt", "a");
    if (f) {
        fprintf(f, "Depth Format: %d, BindFlags: %d\\n", desc.Format, desc.BindFlags);
        fclose(f);
    }
}
