// Gamma Adjustment Shader
// Written by Themaister - based on byuu's bsnes gamma ramp code

#version 150

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

const float saturation = 1.0;
const float gamma = 1.0;
const float luminance = 1.2;

vec3 grayscale(vec3 col)
{
   // Non-conventional way to do grayscale,
   // but bSNES uses this as grayscale value.
   return vec3(dot(col, vec3(0.3333)));
}

void main() {
   vec3 res = texture(source[0], texCoord).xyz;
   res = mix(grayscale(res), res, saturation); // Apply saturation
   res = pow(res, vec3(gamma)); // Apply gamma

  fragColor = vec4(clamp(res * luminance, 0.0, 1.0), 1.0);
}
