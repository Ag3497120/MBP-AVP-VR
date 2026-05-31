import re

with open("/Users/motonishikoudai/openvr.h", "r") as f:
    content = f.read()

output = []
output.append("#pragma once")
output.append("#include <openvr.h>")
output.append("#include <stdio.h>")
output.append("namespace vr {")

# Remove block comments first to avoid parsing 'virtual' inside comments!
content = re.sub(r'/\*[\s\S]*?\*/', '', content)
# Remove line comments
content = re.sub(r'//.*', '', content)

class_matches = re.finditer(r'class (IVR[A-Za-z0-9_]+)\s*\{([\s\S]*?)\};', content)

for match in class_matches:
    class_name = match.group(1)
    class_body = match.group(2)
    
    output.append(f"\nclass Fake{class_name} : public {class_name} {{")
    output.append("public:")
    
    # Find all pure virtual functions in this class
    func_matches = re.finditer(r'virtual\s+(.*?)\s*\=\s*0;', class_body, re.MULTILINE | re.DOTALL)
    for fmatch in func_matches:
        func_sig = fmatch.group(1).strip()
        func_sig_oneline = re.sub(r'\s+', ' ', func_sig)
        
        # Get everything before the first '(' as the return type and name
        ret_and_name = func_sig_oneline.split('(')[0].strip()
        tokens = ret_and_name.split()
        
        func_name = tokens[-1]
        ret_type = " ".join(tokens[:-1])
        
        impl = " { return 0; }"
        if "char *" in ret_type or "char*" in ret_type or "char" in ret_type:
            impl = ' { return ""; }'
        elif "bool" in ret_type:
            impl = ' { return false; }'
        elif "void" in ret_type and not "*" in ret_type:
            impl = ' {}'
        elif "EVR" in ret_type or "ETracked" in ret_type or "VROverlay" in ret_type or "EChaperone" in ret_type or "EIOBuffer" in ret_type:
            enum_type = tokens[-2].replace("vr::", "")
            impl = f' {{ return ({enum_type})0; }}'
        elif "HmdMatrix" in ret_type or "Distortion" in ret_type or "RenderModel" in ret_type:
            impl = ' { return {}; }'
        elif "float" in ret_type or "double" in ret_type:
            impl = ' { return 0.0f; }'
        elif "*" in ret_type:
            impl = ' { return nullptr; }'
            
        output.append(f"    virtual {func_sig_oneline} override{impl}")
        
    output.append("};")

output.append("} // namespace vr")

with open("/Users/motonishikoudai/Verantyx-Mirage/src/fake_openvr.h", "w") as f:
    f.write("\n".join(output))

print("Generated fake_openvr.h with perfect multi-line and comment handling!")
