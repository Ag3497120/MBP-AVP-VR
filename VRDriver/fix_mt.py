import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Enable SetMultithreadProtected(TRUE) to prevent thread collisions in D3DMetal which causes black screens
content = content.replace(
    'if (pMt) pMt->Enter();',
    'if (pMt) { pMt->SetMultithreadProtected(TRUE); pMt->Enter(); }'
)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Enabled SetMultithreadProtected!")
