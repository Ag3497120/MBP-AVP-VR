import struct
import sys

dll_path = sys.argv[1]

with open(dll_path, "rb") as f:
    data = f.read()

target_str = b"Timed out waiting for response from Mongoose."
str_offset = data.find(target_str)

if str_offset == -1:
    print("String not found!")
    sys.exit(1)

print(f"String found at offset: 0x{str_offset:X}")

# In x64 PE files, string references are usually RIP-relative LEA or MOV.
# We will search for the string offset in the entire file as an absolute pointer (unlikely for strings in code)
# OR we can search for a 32-bit relative offset if we assume RIP-relative addressing.
# Let's search the .text section for a 32-bit relative offset.

# Find PE header to locate .text section
pe_header_offset = struct.unpack("<I", data[0x3C:0x40])[0]
num_sections = struct.unpack("<H", data[pe_header_offset + 6:pe_header_offset + 8])[0]
optional_header_size = struct.unpack("<H", data[pe_header_offset + 20:pe_header_offset + 22])[0]
section_table_offset = pe_header_offset + 24 + optional_header_size

text_section = None
for i in range(num_sections):
    sec_offset = section_table_offset + i * 40
    name = data[sec_offset:sec_offset+8].rstrip(b'\x00')
    virtual_size = struct.unpack("<I", data[sec_offset+8:sec_offset+12])[0]
    virtual_addr = struct.unpack("<I", data[sec_offset+12:sec_offset+16])[0]
    raw_size = struct.unpack("<I", data[sec_offset+16:sec_offset+20])[0]
    raw_addr = struct.unpack("<I", data[sec_offset+20:sec_offset+24])[0]
    
    if name == b".text":
        text_section = (name, virtual_addr, virtual_size, raw_addr, raw_size)
    if virtual_addr <= str_offset < virtual_addr + virtual_size:
        str_rva = virtual_addr + (str_offset - raw_addr)
        print(f"String RVA: 0x{str_rva:X} in section {name}")

if not text_section:
    print("No .text section found.")
    sys.exit(1)

_, text_vaddr, text_vsize, text_raw, text_raw_size = text_section
print(f".text section: raw=0x{text_raw:X}, size=0x{text_raw_size:X}, vaddr=0x{text_vaddr:X}")

# Search for RIP-relative references (lea rcx, [rip + offset])
# Instruction format usually: 48 8d 0d [32-bit rel] or 48 8d 15 [32-bit rel]
refs = []
for i in range(text_raw, text_raw + text_raw_size - 7):
    # LEA RCX, [rip + displacement] is 48 8D 0D xx xx xx xx
    if data[i] == 0x48 and data[i+1] == 0x8D and (data[i+2] == 0x0D or data[i+2] == 0x15):
        disp = struct.unpack("<i", data[i+3:i+7])[0]
        # RIP is i + 7 (RVA = text_vaddr + (i - text_raw) + 7)
        rip_rva = text_vaddr + (i - text_raw) + 7
        target_rva = rip_rva + disp
        if target_rva == str_rva:
            print(f"Found LEA reference at raw offset 0x{i:X} (RVA 0x{rip_rva-7:X})")
            refs.append(i)

if not refs:
    print("No references found.")
    sys.exit(1)

# Dump 64 bytes before and after the first reference to see the control flow
ref = refs[0]
dump_start = max(text_raw, ref - 128)
dump_end = min(text_raw + text_raw_size, ref + 64)
print(f"\nHex dump around reference (0x{dump_start:X} - 0x{dump_end:X}):")
for i in range(dump_start, dump_end, 16):
    chunk = data[i:i+16]
    hex_str = " ".join(f"{b:02X}" for b in chunk)
    print(f"0x{i:X}: {hex_str}")

