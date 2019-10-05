// Gaussian Scanlines
//
// adapted from NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 08/05/2013

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

#define DISPLAY_GAMMA 2.1
#define CRT_GAMMA 2.5
#define one (sourceSize[0]) //this is set to 1.0 / sourceSize[0] in the original version, but I think this looks better
#define pix_no texCoord.y * sourceSize[0]
#define TEX(off) pow(texture(source[0], texCoord - vec2(0.0, (off) * one.y)).rgb, vec3(CRT_GAMMA))

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {

vec3 frame0 = TEX(-2.0);
vec3 frame1 = TEX(-1.0);
vec3 frame2 = TEX(0.0);
vec3 frame3 = TEX(1.0);
vec3 frame4 = TEX(2.0);

float offset_dist = fract(pix_no.y) - 0.5;
float dist0 =  2.0 + offset_dist;
float dist1 =  1.0 + offset_dist;
float dist2 =  0.0 + offset_dist;
float dist3 = -1.0 + offset_dist;
float dist4 = -2.0 + offset_dist;

vec3 scanline = frame0 * exp(-5.0 * dist0 * dist0);
	scanline += frame1 * exp(-5.0 * dist1 * dist1);
	scanline += frame2 * exp(-5.0 * dist2 * dist2);
	scanline += frame3 * exp(-5.0 * dist3 * dist3);
	scanline += frame4 * exp(-5.0 * dist4 * dist4);

fragColor = vec4(pow(1.15 * scanline, vec3(1.0 / DISPLAY_GAMMA)), 1.0);
}