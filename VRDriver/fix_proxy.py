import re
with open('src/d3d11_proxy_exports.cpp', 'r') as f:
    content = f.read()

content = content.replace('pAdapter = NULL;', '// pAdapter = NULL;')
content = content.replace('DriverType = D3D_DRIVER_TYPE_HARDWARE;', '// DriverType = D3D_DRIVER_TYPE_HARDWARE;')

with open('src/d3d11_proxy_exports.cpp', 'w') as f:
    f.write(content)
print("Fixed Proxy!")
