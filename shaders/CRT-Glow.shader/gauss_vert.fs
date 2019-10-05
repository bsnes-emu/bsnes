#version 150

#define BOOST 1.0
#define CRT_GEOM_BEAM 1

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
   vec2 vTexCoord;
   vec2 data_pix_no;
   float data_one;
};

out vec4 FragColor;

vec3 beam(vec3 color, float dist)
{
#if CRT_GEOM_BEAM
   vec3 wid     = 2.0 + 2.0 * pow(color, vec3(4.0));
   vec3 weights = vec3(abs(dist) * 3.333333333);

   return 2.0 * color * exp(-pow(weights * inversesqrt(0.5 * wid), wid)) / (0.6 + 0.2 * wid);
#else
   float reciprocal_width = 4.0;
   vec3 x = dist * reciprocal_width;

   return 2.0 * color * exp(-0.5 * x * x) * reciprocal_width;
#endif
}

void main() {
   vec2  texel = floor(data_pix_no);
   float phase = data_pix_no.y - texel.y;
   vec2  tex   = vec2(texel + 0.5) * sourceSize[0].zw;

   vec3 top    = texture(source[0], tex + vec2(0.0, 0 * data_one)).rgb;
   vec3 bottom = texture(source[0], tex + vec2(0.0, 1 * data_one)).rgb;

   float dist0 = phase;
   float dist1 = 1.0 - phase;

   vec3 scanline = vec3(0.0);

   scanline += beam(top,    dist0);
   scanline += beam(bottom, dist1);

   FragColor = vec4(BOOST * scanline * 0.869565217391304, 1.0);
}