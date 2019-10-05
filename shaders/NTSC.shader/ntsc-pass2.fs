// NTSC Shader - written by Hans-Kristian Arntzen
// License: GPLv3
// pulled from git://github.com/libretro/common-shaders.git on 01/30/2014

#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

#define THREE_PHASE //options here include THREE_PHASE, TWO_PHASE or OLD_THREE_PHASE
#define GAMMA_CORRECTION //comment to disable gamma correction, usually because higan's gamma correction is enabled or you have another shader already doing it
#define CRT_GAMMA 2.5
#define DISPLAY_GAMMA 2.1

#if defined OLD_THREE_PHASE
#define TAPS 24
const float luma_filter[TAPS + 1] = float[TAPS + 1](
   -0.000071070,
   -0.000032816,
   0.000128784,
   0.000134711,
   -0.000226705,
   -0.000777988,
   -0.000997809,
   -0.000522802,
   0.000344691,
   0.000768930,
   0.000275591,
   -0.000373434,
   0.000522796,
   0.003813817,
   0.007502825,
   0.006786001,
   -0.002636726,
   -0.019461182,
   -0.033792479,
   -0.029921972,
   0.005032552,
   0.071226466,
   0.151755921,
   0.218166470,
   0.243902439);

const float chroma_filter[TAPS + 1] = float[TAPS + 1](
   0.001845562,
   0.002381606,
   0.003040177,
   0.003838976,
   0.004795341,
   0.005925312,
   0.007242534,
   0.008757043,
   0.010473987,
   0.012392365,
   0.014503872,
   0.016791957,
   0.019231195,
   0.021787070,
   0.024416251,
   0.027067414,
   0.029682613,
   0.032199202,
   0.034552198,
   0.036677005,
   0.038512317,
   0.040003044,
   0.041103048,
   0.041777517,
   0.042004791);
#endif

#if defined(THREE_PHASE)
// #include "ntsc-decode-filter-3phase.inc" //
#define TAPS 24
const float luma_filter[TAPS + 1] = float[TAPS + 1](
   -0.000012020,
   -0.000022146,
   -0.000013155,
   -0.000012020,
   -0.000049979,
   -0.000113940,
   -0.000122150,
   -0.000005612,
   0.000170516,
   0.000237199,
   0.000169640,
   0.000285688,
   0.000984574,
   0.002018683,
   0.002002275,
   -0.000909882,
   -0.007049081,
   -0.013222860,
   -0.012606931,
   0.002460860,
   0.035868225,
   0.084016453,
   0.135563500,
   0.175261268,
   0.190176552);

const float chroma_filter[TAPS + 1] = float[TAPS + 1](
   -0.000118847,
   -0.000271306,
   -0.000502642,
   -0.000930833,
   -0.001451013,
   -0.002064744,
   -0.002700432,
   -0.003241276,
   -0.003524948,
   -0.003350284,
   -0.002491729,
   -0.000721149,
   0.002164659,
   0.006313635,
   0.011789103,
   0.018545660,
   0.026414396,
   0.035100710,
   0.044196567,
   0.053207202,
   0.061590275,
   0.068803602,
   0.074356193,
   0.077856564,
   0.079052396);
// END "ntsc-decode-filter-3phase.inc" //

#elif defined(TWO_PHASE)
// #include "ntsc-decode-filter-3phase.inc" //
#define TAPS 24
const float luma_filter[TAPS + 1] = float[TAPS + 1](
   -0.000012020,
   -0.000022146,
   -0.000013155,
   -0.000012020,
   -0.000049979,
   -0.000113940,
   -0.000122150,
   -0.000005612,
   0.000170516,
   0.000237199,
   0.000169640,
   0.000285688,
   0.000984574,
   0.002018683,
   0.002002275,
   -0.000909882,
   -0.007049081,
   -0.013222860,
   -0.012606931,
   0.002460860,
   0.035868225,
   0.084016453,
   0.135563500,
   0.175261268,
   0.190176552);

const float chroma_filter[TAPS + 1] = float[TAPS + 1](
   -0.000118847,
   -0.000271306,
   -0.000502642,
   -0.000930833,
   -0.001451013,
   -0.002064744,
   -0.002700432,
   -0.003241276,
   -0.003524948,
   -0.003350284,
   -0.002491729,
   -0.000721149,
   0.002164659,
   0.006313635,
   0.011789103,
   0.018545660,
   0.026414396,
   0.035100710,
   0.044196567,
   0.053207202,
   0.061590275,
   0.068803602,
   0.074356193,
   0.077856564,
   0.079052396);
// END "ntsc-decode-filter-3phase.inc" //
#endif

// #include ntsc-rgbyuv.inc //
mat3 yiq2rgb_mat = mat3(
   1.0, 1.0, 1.0,
   0.956, -0.2720, -1.1060,
   0.6210, -0.6474, 1.7046
);

vec3 yiq2rgb(vec3 yiq)
{
   return (yiq * yiq2rgb_mat);
}

mat3 yiq_mat = mat3(
      0.2989, 0.5959, 0.2115,
      0.5870, -0.2744, -0.5229,
      0.1140, -0.3216, 0.3114
);

vec3 rgb2yiq(vec3 col)
{
   return (col * yiq_mat);
}
// END ntsc-rgbyuv.inc //

// fixCoord moved from vertex
#define fixCoord (texCoord - vec2(0.5 / sourceSize[0].x, 0.0)) // Compensate for decimate-by-2.

#define fetch_offset(offset, one_x) \
texture(source[0], fixCoord + vec2((offset) * (one_x), 0.0)).xyz
   
in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
// #include "ntsc-pass2-decode.inc" //
float one_x = 1.0 / sourceSize[0].x;
vec3 signal = vec3(0.0);

for (int i = 0; i < TAPS; i++)
{
   float offset = float(i);
   vec3 sums = fetch_offset(offset - float(TAPS), one_x) +
		fetch_offset(float(TAPS) - offset, one_x);
	
	signal += sums * vec3(luma_filter[i], chroma_filter[i], chroma_filter[i]);
}

signal += texture(source[0], fixCoord).xyz * vec3(luma_filter[TAPS], chroma_filter[TAPS], chroma_filter[TAPS]);

// END "ntsc-pass2-decode.inc" //

   vec3 rgb = yiq2rgb(signal);
   
#ifdef GAMMA_CORRECTION
   vec3 gamma = vec3(CRT_GAMMA / DISPLAY_GAMMA);
   rgb = pow(rgb, gamma.rgb);
#endif

fragColor = vec4(rgb, 1.0);
}