// Retro v2 Shader
// Written by Hyllian / Jararaca

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

// This value must be between 0.0 (totally black) and 1.0 (nearest neighbor)
#define PIXEL_SIZE  0.64

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;
out vec3 E;
out vec2 fp;
out vec2 ps;
out vec2 f;
out vec3 res;

void main() {
    // Reading the texel
    vec3 E = pow(texture(source[0], texCoord).xyz, vec3(2.4));

    vec2 fp = fract(texCoord.xy * sourceSize[0].xy);
    vec2 ps = sourceSize[0].xy / targetSize.xy;

    vec2 f = clamp(clamp(fp + 0.5*ps, 0.0, 1.0) - PIXEL_SIZE, vec2(0.0, 0.0), ps)/ps;

    float max_coord = max(f.x, f.y);

    vec3 res = mix(E*(1.04+fp.x*fp.y), E*0.36, max_coord);

fragColor = vec4(clamp( pow(res, vec3(1.0 / 2.2)), 0.0, 1.0 ), 1.0);
}