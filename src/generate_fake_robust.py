import sys
import re

with open("/Users/motonishikoudai/Verantyx-Mirage/src/official_openvr.h", "r") as f:
    content = f.read()

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

class_re = re.compile(r"class\s+(IVR[A-Za-z0-9_]+)\s*\{([^}]*)\};", re.DOTALL)
method_re = re.compile(r"virtual\s+(.*?)([A-Za-z0-9_]+)\s*\((.*?)\)\s*=\s*0;", re.DOTALL)

for m_class in class_re.finditer(content):
    class_name = m_class.group(1)
    class_body = m_class.group(2)
    
    out.write(f"class Fake{class_name[3:]} : public vr::{class_name} {{\npublic:\n")
    
    for m_method in method_re.finditer(class_body):
        ret_type = m_method.group(1).strip()
        # Clean up return type newlines
        ret_type = " ".join(ret_type.split())
        
        name = m_method.group(2).strip()
        args = m_method.group(3).strip()
        
        clean_args = []
        for arg in args.split(','):
            arg = arg.split('=')[0].strip() # remove default arguments
            arg = " ".join(arg.split()) # normalize whitespace
            if arg:
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
        
    out.write("};\n\n")

out.close()
print("Generated fake_openvr_generated.h robustly")
