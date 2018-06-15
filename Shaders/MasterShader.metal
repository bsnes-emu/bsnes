#include <metal_stdlib>
#include <simd/simd.h>
#include <metal_math>

using namespace metal;

/* For GLSL compatibility */
typedef float2 vec2;
typedef float3 vec3;
typedef float4 vec4;

typedef struct {
    float4 position [[position]];
    float2 texcoords;
} rasterizer_data;

// Vertex Function
vertex rasterizer_data vertex_shader(uint index [[ vertex_id ]],
                                     constant vector_float2 *vertices [[ buffer(0) ]])

{
    rasterizer_data out;

    out.position.xy = vertices[index].xy;
    out.position.z = 0.0;
    out.position.w = 1.0;
    out.texcoords = (vertices[index].xy + float2(1, 1)) / 2.0;

    return out;
}


static inline float4 texture(texture2d<half> texture, float2 pos)
{
    constexpr sampler texture_sampler;
    return float4(texture.sample(texture_sampler, pos));
}

fragment float4 fragment_shader(rasterizer_data in [[stage_in]],
                                texture2d<half> image [[ texture(0) ]])
{
    in.texcoords.y = 1 - in.texcoords.y;
    return texture(image, in.texcoords);
}

