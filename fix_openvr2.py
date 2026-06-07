import re

with open('/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

content = re.sub(r'return pRet;', r'return;', content)

with open('/Users/motonishikoudai/Verantyx-Mirage/src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
