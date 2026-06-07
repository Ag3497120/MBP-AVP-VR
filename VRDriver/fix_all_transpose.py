import re

with open('src/openvr_emulator.cpp', 'r') as f:
    content = f.read()

# Revert ALL instances of the broken r*4+c back to c*4+r
# Note: I'll use a regex to ensure I catch any spacing variations, though the previous script was exact.
content = content.replace('r*4 + c', 'c*4 + r')
# Also handle the case where it might be r*4+c without spaces
content = content.replace('r*4+c', 'c*4+r')

with open('src/openvr_emulator.cpp', 'w') as f:
    f.write(content)
print("Reverted all transposes!")
