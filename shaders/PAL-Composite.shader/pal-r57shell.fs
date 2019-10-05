#version 150

// NES PAL composite signal simulation for RetroArch
// shader by r57shell
// thanks to feos & HardWareMan & NewRisingSun

// also TV subpixels and scanlines

// LICENSE: PUBLIC DOMAIN

// NOTE: for nice TV subpixels and scanlines I recommend to
// disable this features here and apply CRT-specialized shader.

// Quality considerations

// there are three main options:
// USE_RAW (R), USE_DELAY_LINE (D), USE_COLORIMETRY (C)
// here is table of quality in decreasing order:
// RDC, RD, RC, DC, D, C

// compatibility macros
#define float2 vec2
#define float3 vec3
#define float4 vec4
#define frac(c) fract(c)
#define saturate(c) clamp(c, 0.0, 1.0)
#define fmod(x,y) mod(x,y)
#define mul(x,y) (y*x)
#define float2x2 mat2
#define float3x3 mat3
#define float4x4 mat4
#define bool2 bvec2
#define bool3 bvec3
#define bool4 bvec4
#define static 

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 outputSize;
uniform int phase;

in Vertex {
   vec2 texCoord;
};

out vec4 fragColor;

// TWEAKS start

// use delay line technique
// without delay line technique, color would interleave
// to avoid this, set HueRotation to zero.
#define USE_DELAY_LINE

// use this if you need to swap even/odd V sign.
// sign of V changes each scanline
// so if some scanline is positive, then next is negative
// and if you want to match picture
// to actual running PAL NES on TV
// you may want to have this option, to change signs
// if they don't match
//#define SWAP_VSIGN

// rough simulation of scanlines
// better if you use additional shader instead
// if you still use it, make sure that SizeY
// is at least twice lower than output height
#define USE_SCANLINES // FIXME: scanlines are broken and I'm too lazy to fix it right now

// to change gamma of virtual TV from 2.2 to something else
#define USE_GAMMA

// use sampled version. it's much more slower version of shader.
// because it is computing x4 more values. NOT RECOMMENDED.
//#define USE_SAMPLED

#ifndef PARAMETER_UNIFORM
// NTSC standard gamma = 2.2
// PAL standard gamma = 2.8
// according to many sources, very unlikely gamma of TV is 2.8
// most likely gamma of PAL TV is in range 2.4-2.5
const float Gamma_static = 2.8; // gamma of virtual TV

const float Brightness_static = 0.25;
const float Contrast_static = 1.1;
const float Saturation_static = 1.1;

const int
	Ywidth_static = 12,
	Uwidth_static = 23,
	Vwidth_static = 23;
   
#define Brightness Brightness_static
#define Gamma Gamma_static

#define Ywidth Ywidth_static
#define Uwidth Uwidth_static
#define Vwidth Vwidth_static

#define SCANLINE_MUL (sw.x*dark_scanline+sw.y)

int Mwidth = int(max(float(Ywidth), max(float(Uwidth), float(Vwidth))));

// correct one is -2.5
// works only with USE_RAW
const float HueShift = -2.5;

// rotation of hue due to luma level.
const float HueRotation = 2.;

// touch this only if you know what you doing
const float Phase_Y = 2.; // fmod(341*10,12)
const float Phase_One = 0.; // alternating phases.
const float Phase_Two = 8.;

// screen size, scanlines = y*2; y one field, and y other field.
const int SizeX = 256;
const int SizeY = 240;

// count of pixels of virtual TV.
// value close to 1000 produce small artifacts
const int TV_Pixels = 400;

const float dark_scanline = 0.5; // half
#endif

// this is using following matrixes.
// it provides more scientific approach
// by conversion into linear XYZ space
// and back to sRGB.
// it's using Gamma setting too.
// define USE_GAMMA is not required.
#define USE_COLORIMETRY

const float3x3 RGB_to_XYZ =
mat3(
	0.4306190, 0.3415419, 0.1783091,
	0.2220379, 0.7066384, 0.0713236,
	0.0201853, 0.1295504, 0.9390944
);

const float3x3 XYZ_to_sRGB =
mat3(
	 3.2406, -1.5372, -0.4986,
	-0.9689,  1.8758,  0.0415,
	 0.0557, -0.2040,  1.0570
);

// TWEAKS end

const float YUV_u = 0.492;
const float YUV_v = 0.877;

const mat3 RGB_to_YUV =
mat3(
	float3( 0.299, 0.587, 0.114), //Y
	float3(-0.299,-0.587, 0.886)*YUV_u, //B-Y
	float3( 0.701,-0.587,-0.114)*YUV_v //R-Y
);

#ifdef USE_DELAY_LINE
const float comb_line = 1.;
#else
const float comb_line = 2.;
#endif

static const float Voltage_0 = 0.518;
static const float Voltage_1 = 1.962;
static const float DeltaV = (Voltage_1-Voltage_0);

float RGB_y = Contrast_static/Ywidth_static/DeltaV;
float RGB_u = comb_line*Contrast_static*Saturation_static/YUV_u/Uwidth_static/DeltaV;
float RGB_v = comb_line*Contrast_static*Saturation_static/YUV_v/Vwidth_static/DeltaV;

mat3 YUV_to_RGB =
mat3(
	float3(1., 1., 1.)*RGB_y,
	float3(0., -0.114/0.587, 1.)*RGB_u,
	float3(1., -0.299/0.587, 0.)*RGB_v
);

const float pi = 3.1415926535897932384626433832795;

float sinn(float x)
{
	return sin(/*fmod(x,24)*/x*(pi*2./24.));
}

float coss(float x)
{
	return cos(/*fmod(x,24)*/x*(pi*2./24.));
}

float3 monitor(sampler2D tex, float2 p)
{
   float2 size = sourceSize[0].xy;//float2(SizeX,SizeY);
	// align vertical coord to center of texel
	float2 uv = float2(

		p.x,

		(floor(p.y*sourceSize[0].y)+0.5)/sourceSize[0].y);
#ifdef USE_DELAY_LINE
	float2 sh = (sourceSize[0].xy/sourceSize[0].xy/size)*float2(14./10.,-1.0);
#endif
	float2 pc = uv*sourceSize[0].xy/sourceSize[0].xy*size*float2(10.,1.);
	float alpha = dot(floor(float2(pc.x,pc.y)),float2(2.,Phase_Y*2.));
	alpha += Phase_One*2.;

	// 1/size.x of screen in uv coords = sourceSize[0].x/sourceSize[0].x/size.x;
	// then 1/10*size.x of screen:
	float ustep = sourceSize[0].x/sourceSize[0].x/size.x/10.;

	float border = sourceSize[0].x/sourceSize[0].x;
	float ss = 2.0;
#ifdef SWAP_VSIGN
#define PAL_SWITCH(A) A < 1.
#else
#define PAL_SWITCH(A) A > 1.
#endif
	if (PAL_SWITCH(fmod(uv.y*sourceSize[0].y/sourceSize[0].y*size.y,2.0)))
	{
		// cos(pi-alpha) = -cos(alpha)
		// sin(pi-alpha) = sin(alpha)
		// pi - alpha
		alpha = -alpha+12012.0;
		ss = -2.0;
	}
   
	float ysum = 0., usum = 0., vsum = 0.;
	for (int i=0; i<Mwidth; ++i)
	{
		float4 res = texture(tex, uv);
      float3 yuv = mul(RGB_to_YUV, res.xyz);
		float a1 = alpha+(HueShift+2.5)*2.-yuv.x*ss*HueRotation;
		float sig = yuv.x+dot(yuv.yz,sign(float2(sinn(a1),coss(a1))));
#ifdef USE_DELAY_LINE
		float4 res1 = texture(tex, uv+sh);
		float3 yuv1 = mul(RGB_to_YUV, res1.xyz);
		float a2 = (HueShift+2.5)*2.+12012.0-alpha+yuv.x*ss*HueRotation;
		float sig1 = yuv1.x+dot(yuv1.yz,sign(float2(sinn(a2),coss(a2))));
#endif
		if (i < Ywidth)
			ysum += sig;

#ifdef USE_DELAY_LINE
		if (i < Uwidth)
			usum += (sig+sig1)*sinn(alpha);
		if (i < Vwidth)
			vsum += (sig-sig1)*coss(alpha);
#else
		if (i < Uwidth)
			usum += sig*sinn(alpha);
		if (i < Vwidth)
			vsum += sig*coss(alpha);
#endif
		alpha -= ss;
		uv.x -= ustep;
	}
   
	float3 rgb = mul(float3(ysum+Brightness*Ywidth_static,usum,vsum), YUV_to_RGB);
#if defined(USE_GAMMA) && !defined(USE_COLORIMETRY)
	float3 rgb1 = saturate(rgb);
	rgb = pow(rgb1, Gamma/2.2);
#endif

#ifdef USE_COLORIMETRY
	float3 rgb1 = saturate(rgb);
	rgb = pow(rgb1, float3(Gamma, Gamma, Gamma));
#endif

#if (defined(USE_SCANLINES))
	float2 q = (p*sourceSize[0].xy/sourceSize[0].xy)*float2(TV_Pixels*3.,size.y*2.);
#endif

#ifdef USE_SCANLINES
	float scanlines = size.y * outputSize.z;
	float top = fmod(q.y-0.5*scanlines*2.,2.);
	float bottom = top+frac(scanlines)*2.;
	float2 sw = saturate(min(float2(1.,2.),float2(bottom, bottom))
		-max(float2(0.,1.),float2(top)))
		+saturate(min(float2(3.,4.),float2(bottom, bottom))
		-max(float2(2.,3.),float2(top)))
		+floor(scanlines);

	rgb = rgb*SCANLINE_MUL/(sw.x+sw.y);
   
   /*
	//old stupid method
	float z =
#ifdef ANIMATE_SCANLINE
	fmod(phase,2.0)+
#endif
		0.5;

	if (abs(fmod(q.y+0.5,2)-z)<0.5)
		rgb *= dark_scanline;
*/
#endif

	// size of pixel screen in texture coords:
	//float output_pixel_size = sourceSize[0].x/(outputSize.x*sourceSize[0].x);

	// correctness check
	//if (fmod(p.x*output_pixel_size,2.0) < 1.0)
	//	rgb = float3(0.,0.,0.);

#ifdef USE_COLORIMETRY
	float3 xyz1 = mul(RGB_to_XYZ,rgb);
	float3 srgb = saturate(mul(XYZ_to_sRGB,xyz1));
	float3 a1 = 12.92*srgb;
	float3 a2 = 1.055*pow(srgb,float3(1./2.4))-0.055;
	float3 ssrgb;
   ssrgb.x = (srgb.x<0.0031308?a1.x:a2.x);
   ssrgb.y = (srgb.y<0.0031308?a1.y:a2.y);
   ssrgb.z = (srgb.z<0.0031308?a1.z:a2.z);
	return ssrgb;
#else
	return rgb;
#endif
}

// pos (left corner, sample size)
float4 monitor_sample(sampler2D tex, float2 p, float2 sample_)
{
	// linear interpolation was...
	// now other thing.
	// http://imgur.com/m8Z8trV
	// AT LAST IT WORKS!!!!
	// going to check in retroarch...
	float2 size = sourceSize[0].xy;
	float2 next = float2(.25,1.)/size;
	float2 f = frac(float2(4.,1.)*size*p);
	sample_ *= float2(4.,1.)*size;
	float2 l;
	float2 r;
	if (f.x+sample_.x < 1.)
	{
		l.x = f.x+sample_.x;
		r.x = 0.;
	}
	else
	{
		l.x = 1.-f.x;
		r.x = min(1.,f.x+sample_.x-1.);
	}
	if (f.y+sample_.y < 1.)
	{
		l.y = f.y+sample_.y;
		r.y = 0.;
	}
	else
	{
		l.y = 1.-f.y;
		r.y = min(1.,f.y+sample_.y-1.);
	}
	float3 top = mix(monitor(tex, p), monitor(tex, p+float2(next.x,0.)), r.x/(l.x+r.x));
	float3 bottom = mix(monitor(tex, p+float2(0.,next.y)), monitor(tex, p+next), r.x/(l.x+r.x));
	return float4(mix(top,bottom, r.y/(l.y+r.y)),1.0);
}

void main() {
#ifdef USE_SAMPLED
	fragColor = vec4(monitor_sample(source[0], texCoord, outputSize.zw).rgb, 1.0);
#else
	fragColor = float4(monitor(source[0], texCoord), 1.);
#endif
}