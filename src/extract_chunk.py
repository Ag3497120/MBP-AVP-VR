import struct
import sys
import subprocess

dll_path = sys.argv[1]
with open(dll_path, "rb") as f:
    data = f.read()

start = 0x254A86
end = 0x254B46
chunk = data[start:end]

with open("/Users/motonishikoudai/Verantyx-Mirage/src/chunk.bin", "wb") as f:
    f.write(chunk)
