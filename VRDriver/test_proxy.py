import re
with open('src/d3d11_proxy_exports.cpp', 'r') as f:
    content = f.read()

# Add logging to CreateTexture2D unconditionally
content = content.replace('if (desc.MiscFlags & D3D11_RESOURCE_MISC_SHARED) {', 'FILE* _ftest = fopen("C:\\\\d3d11_proxy_log.txt", "a"); if(_ftest){fprintf(_ftest, "CreateTexture2D Called!\\n"); fclose(_ftest);}\n        if (desc.MiscFlags & D3D11_RESOURCE_MISC_SHARED) {')

with open('src/d3d11_proxy_exports.cpp', 'w') as f:
    f.write(content)
