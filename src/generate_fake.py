import sys
import re

with open("/Users/motonishikoudai/Verantyx-Mirage/src/official_openvr.h", "r") as f:
    lines = f.readlines()

out = open("/Users/motonishikoudai/Verantyx-Mirage/src/fake_openvr_generated.h", "w")
out.write("#pragma once\n")
out.write("#include \"official_openvr.h\"\n")
out.write("#include <string.h>\n")
out.write("using namespace vr;\n\n")

out.write("""
static HmdMatrix34_t Identity34_Gen() {
    HmdMatrix34_t m;
    memset(&m, 0, sizeof(m));
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f;
    return m;
}
static HmdMatrix44_t Identity44_Gen() {
    HmdMatrix44_t m;
    memset(&m, 0, sizeof(m));
    m.m[0][0] = 1.0f; m.m[1][1] = 1.0f; m.m[2][2] = 1.0f; m.m[3][3] = 1.0f;
    return m;
}
""")

class_name = ""
in_class = False

# Regex to capture return type, method name, and args
method_re = re.compile(r"^\s*virtual\s+(.*?)([A-Za-z0-9_]+)\s*\((.*)\)\s*=\s*0;")

for line in lines:
    line = line.strip()
    if line.startswith("class IVR") and "{" not in line and ";" not in line:
        class_name = line.split()[1]
        out.write(f"class Fake{class_name} : public vr::{class_name} {{\npublic:\n")
        in_class = True
        continue
    
    if in_class and line == "};":
        out.write("};\n\n")
        in_class = False
        continue

    m = method_re.search(line)
    if m and in_class:
        ret_type = m.group(1).strip()
        name = m.group(2).strip()
        args = m.group(3).strip()
        
        clean_args = []
        for arg in args.split(','):
            arg = arg.split('=')[0].strip()
            clean_args.append(arg)
        clean_args_str = ", ".join(clean_args)
        
        out.write(f"    virtual {ret_type} {name}({clean_args_str}) override {{\n")
        if ret_type != "void":
            if "HmdMatrix34_t" in ret_type:
                out.write("        return Identity34_Gen();\n")
            elif "HmdMatrix44_t" in ret_type:
                out.write("        return Identity44_Gen();\n")
            elif "*" in ret_type:
                out.write("        return nullptr;\n")
            else:
                out.write("        return {};\n")
        out.write("    }\n")

out.close()
print("Generated fake_openvr_generated.h")
