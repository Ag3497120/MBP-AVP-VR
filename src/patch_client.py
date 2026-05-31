import sys

dll_path = sys.argv[1]

with open(dll_path, "rb") as f:
    data = bytearray(f.read())

offset1 = 0x254AF0
offset2 = 0x254B04

if data[offset1] == 0x75 and data[offset1+1] == 0x31:
    print("Found first JNZ, patching to JMP...")
    data[offset1] = 0xEB
else:
    print(f"First JNZ not found at 0x{offset1:X}: {data[offset1]:02X} {data[offset1+1]:02X}")

if data[offset2] == 0x75 and data[offset2+1] == 0x1D:
    print("Found second JNZ, patching to JMP...")
    data[offset2] = 0xEB
else:
    print(f"Second JNZ not found at 0x{offset2:X}: {data[offset2]:02X} {data[offset2+1]:02X}")

with open(dll_path, "wb") as f:
    f.write(data)

print("Patching complete!")
