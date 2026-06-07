import re

with open('openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Find all occurrences of DummyPadding and replace them
def repl(match):
    num = match.group(1)
    return f'virtual void* DummyPadding{num}() {{ FILE* _f = fopen("vr_emulator_log.txt", "a"); if(_f) {{ fprintf(_f, "Called: DummyPadding{num}\\n"); fclose(_f); }} return nullptr; }}'

content = re.sub(r'virtual void\*\s+DummyPadding(\d+)\(\)\s*\{\s*return nullptr;\s*\}', repl, content)

with open('openvr_emulator.cpp', 'w') as f:
    f.write(content)

print("Updated openvr_emulator.cpp with logging in DummyPadding functions.")
