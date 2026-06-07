import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Remove the COM hook definitions and MinHook include
content = re.sub(r'#include "MinHook\.h".*?// ---------------------------------------------------------', '// ---------------------------------------------------------', content, flags=re.DOTALL)

# Also find where ApplyCOMHooks is defined and remove it entirely
content = re.sub(r'void ApplyCOMHooks\(\) \{.*?\}', 'void ApplyCOMHooks() { /* Removed unsafe COM hooks */ }', content, flags=re.DOTALL)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Removed COM hooks!")
