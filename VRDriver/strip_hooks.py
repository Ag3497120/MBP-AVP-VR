import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Remove the COM hook definitions and the remaining MinHook lines
content = re.sub(r'typedef HRESULT \(WINAPI \*CreateTexture2D_t\).*?void ApplyCOMHooks\(\) \{ /\* Removed unsafe COM hooks \*/ \}', 'void ApplyCOMHooks() { }', content, flags=re.DOTALL)

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Stripped all Hooked_ functions!")
