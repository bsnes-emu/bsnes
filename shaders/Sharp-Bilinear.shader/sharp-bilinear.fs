#version 150

// Sharp Bilinear
// Author: Themaister
// License: Public Domain

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
float prescale = floor(targetSize.y / sourceSize[0].y);
   vec2 texel = texCoord.xy * sourceSize[0].xy;
   vec2 texel_floored = floor(texel);
   vec2 s = fract(texel);
   float region_range = 0.5 - 0.5 / prescale;

   // Figure out where in the texel to sample to get correct pre-scaled bilinear.
   // Uses the hardware bilinear interpolator to avoid having to sample 4 times manually.

   vec2 center_dist = s - 0.5;
   vec2 f = (center_dist - clamp(center_dist, -region_range, region_range)) * prescale + 0.5;

   vec2 mod_texel = texel_floored + f;
    
   fragColor = texture(source[0], mod_texel / sourceSize[0].xy);;
}