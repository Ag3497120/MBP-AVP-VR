#include <metal_stdlib>
using namespace metal;

struct VertexOut {
    float4 position [[position]];
    float2 texCoord;
    uint renderTargetArrayIndex [[render_target_array_index]];
};

vertex VertexOut vrVertexShader(uint vertexID [[vertex_id]], uint amp_id [[amplification_id]]) {
    VertexOut out;
    float2 positions[4] = {
        float2(-1.0, -1.0),
        float2( 1.0, -1.0),
        float2(-1.0,  1.0),
        float2( 1.0,  1.0)
    };
    out.position = float4(positions[vertexID], 0.0, 1.0);
    out.texCoord = float2((positions[vertexID].x + 1.0) * 0.5, 1.0 - (positions[vertexID].y + 1.0) * 0.5);
    
    out.renderTargetArrayIndex = amp_id;
    
    // Split screen for left and right eyes
    if (amp_id == 0) {
        // Left eye gets left half of texture
        out.texCoord.x *= 0.5;
    } else {
        // Right eye gets right half of texture
        out.texCoord.x = out.texCoord.x * 0.5 + 0.5;
    }
    
    return out;
}

fragment float4 vrFragmentShader(VertexOut in [[stage_in]],
                               texture2d<float, access::sample> colorTexture [[texture(0)]],
                               sampler textureSampler [[sampler(0)]]) {
    float4 color = colorTexture.sample(textureSampler, in.texCoord);
    return float4(color.rgb, 1.0);
}
