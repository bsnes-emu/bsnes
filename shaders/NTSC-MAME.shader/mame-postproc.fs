#version 150

// This is a port of the NTSC encode/decode shader pair in MAME and MESS, modified to use only
// one pass rather than an encode pass and a decode pass. It accurately emulates the sort of
// signal decimation one would see when viewing a composite signal, though it could benefit from a
// pre-pass to re-size the input content to more accurately reflect the actual size that would
// be incoming from a composite signal source.
//
// To encode the composite signal, I convert the RGB value to YIQ, then subsequently evaluate
// the standard NTSC composite equation. Four composite samples per RGB pixel are generated from
// the incoming linearly-interpolated texels.
//
// The decode pass implements a Fixed Impulse Response (FIR) filter designed by MAME/MESS contributor
// "austere" in matlab (if memory serves correctly) to mimic the behavior of a standard television set
// as closely as possible. The filter window is 83 composite samples wide, and there is an additional
// notch filter pass on the luminance (Y) values in order to strip the color signal from the luminance
// signal prior to processing.
//
// Yes, this code could greatly use some cleaning up.

// ported from UltraMoogleMan's "Full MAME/MESS Shader Pipe" shadertoy: https://www.shadertoy.com/view/ldf3Rf
// license: presumably MAME's license at the time, which was noncommercial

#define scanlines 0.0
#define scandark 0.175
#define deconverge 0.0
#define pincushion 0.0
#define hertzroll 0.0

// Useful Constants
const vec4 Zero = vec4(0.0);
const vec4 Half = vec4(0.5);
const vec4 One = vec4(1.0);
const vec4 Two = vec4(2.0);
const vec3 Gray = vec3(0.3, 0.59, 0.11);
const float Pi = 3.1415926535;
const float Pi2 = 6.283185307;

// NTSC Constants
const vec4 A = vec4(0.5);
const vec4 A2 = vec4(1.0);
const vec4 B = vec4(0.5);
const float P = 1.0;
const float CCFrequency = 3.59754545;
const float NotchUpperFrequency = 5.59754545; //3.59754545 + 2.0;
const float NotchLowerFrequency = 1.59754545; //3.59754545 - 2.0;
const float YFrequency = 6.0;
const float IFrequency = 1.2;
const float QFrequency = 0.6;
const float NotchHalfWidth = 2.0;
const float ScanTime = 52.6;
const float Pi2ScanTime = 330.4955471482;// 6.283185307 * 52.6;
const float MaxC = 2.1183;
const vec4 YTransform = vec4(0.299, 0.587, 0.114, 0.0);
const vec4 ITransform = vec4(0.595716, -0.274453, -0.321263, 0.0);
const vec4 QTransform = vec4(0.211456, -0.522591, 0.311135, 0.0);
const vec3 YIQ2R = vec3(1.0, 0.956, 0.621);
const vec3 YIQ2G = vec3(1.0, -0.272, -0.647);
const vec3 YIQ2B = vec3(1.0, -1.106, 1.703);
const vec4 MinC = vec4(-1.1183);
const vec4 CRange = vec4(3.2366);
const vec4 InvCRange = vec4(1.0/3.2366);
const float Pi2Length = Pi2 / 63.0;
const vec4 NotchOffset = vec4(0.0, 1.0, 2.0, 3.0);
vec4 W = vec4(Pi2 * CCFrequency * ScanTime);

// Color Convolution Constants
const vec3 RedMatrix = vec3(1.0, 0.0, 0.0);
const vec3 GrnMatrix = vec3(0.0, 1.0, 0.0);
const vec3 BluMatrix = vec3(0.0, 0.0, 1.0);
const vec3 DCOffset = vec3(0.0, 0.0, 0.0);
const vec3 ColorScale = vec3(0.95, 0.95, 0.95);
const float Saturation = 1.4;

// Deconverge Constants
const vec3 ConvergeX = vec3(-0.4,  0.0, 0.2);
const vec3 ConvergeY = vec3( 0.0, -0.4, 0.2);
const vec3 RadialConvergeX = vec3(1.0, 1.0, 1.0);
const vec3 RadialConvergeY = vec3(1.0, 1.0, 1.0);

// Scanline/Pincushion Constants
const float PincushionAmount = 0.015;
const float CurvatureAmount = 0.015;
//const float ScanlineAmount = 0.175; <- move to parameter
const float ScanlineScale = 1.0;
const float ScanlineHeight = 1.0;
const float ScanlineBrightScale = 1.0;
const float ScanlineBrightOffset = 0.0;
const float ScanlineOffset = 0.0;
const vec3 Floor = vec3(0.05, 0.05, 0.05);

// 60Hz Bar Constants
const float SixtyHertzRate = (60.0 / 59.97 - 1.0); // Difference between NTSC and line frequency
const float SixtyHertzScale = 0.1;

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
uniform int phase;

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

vec4 ColorConvolution(vec2 UV, vec2 InverseRes)
{
	vec3 InPixel = texture(source[0], UV).rgb;
	
	// Color Matrix
	float RedValue = dot(InPixel, RedMatrix);
	float GrnValue = dot(InPixel, GrnMatrix);
	float BluValue = dot(InPixel, BluMatrix);
	vec3 OutColor = vec3(RedValue, GrnValue, BluValue);
	
	// DC Offset & Scale
	OutColor = (OutColor * ColorScale) + DCOffset;
	
	// Saturation
	float Luma = dot(OutColor, Gray);
	vec3 Chroma = OutColor - Luma;
	OutColor = (Chroma * Saturation) + Luma;
	
	return vec4(OutColor, 1.0);
}

vec4 Deconverge(vec2 UV)
{
	vec2 InverseRes = 1.0 / sourceSize[0].xy;
	vec2 InverseSrcRes = 1.0 / sourceSize[0].xy;

	vec3 CoordX = UV.x * RadialConvergeX;
	vec3 CoordY = UV.y * RadialConvergeY;

	CoordX += ConvergeX * InverseRes.x - (RadialConvergeX - 1.0) * 0.5;
	CoordY += ConvergeY * InverseRes.y - (RadialConvergeY - 1.0) * 0.5;

	float RedValue = ColorConvolution(vec2(CoordX.x, CoordY.x), InverseSrcRes).r;
	float GrnValue = ColorConvolution(vec2(CoordX.y, CoordY.y), InverseSrcRes).g;
	float BluValue = ColorConvolution(vec2(CoordX.z, CoordY.z), InverseSrcRes).b;

	if (deconverge > 0.5) return vec4(RedValue, GrnValue, BluValue, 1.0);
	else return vec4(texture(source[0], UV));
}

vec4 ScanlinePincushion(vec2 UV)
{
	vec4 InTexel = Deconverge(UV);
	
	vec2 PinUnitCoord = UV * Two.xy - One.xy;
	float PincushionR2 = pow(length(PinUnitCoord), 2.0);
	vec2 PincushionCurve = PinUnitCoord * PincushionAmount * PincushionR2;
	vec2 BaseCoord = UV;
	vec2 ScanCoord = UV;
	
	BaseCoord *= One.xy - PincushionAmount * 0.2; // Warning: Magic constant
	BaseCoord += PincushionAmount * 0.1;
	BaseCoord += PincushionCurve;
	
	ScanCoord *= One.xy - PincushionAmount * 0.2; // Warning: Magic constant
	ScanCoord += PincushionAmount * 0.1;
	ScanCoord += PincushionCurve;
	
	vec2 CurveClipUnitCoord = UV * Two.xy - One.xy;
	float CurvatureClipR2 = pow(length(CurveClipUnitCoord), 2.0);
	vec2 CurvatureClipCurve = CurveClipUnitCoord * CurvatureAmount * CurvatureClipR2;
	vec2 ScreenClipCoord = UV;
	ScreenClipCoord -= Half.xy;
	ScreenClipCoord *= One.xy - CurvatureAmount * 0.2; // Warning: Magic constant
	ScreenClipCoord += Half.xy;
	ScreenClipCoord += CurvatureClipCurve;

	if (pincushion > 0.5){
		// -- Alpha Clipping --
		if (BaseCoord.x < 0.0) return vec4(0.0, 0.0, 0.0, 1.0);
		if (BaseCoord.y < 0.0) return vec4(0.0, 0.0, 0.0, 1.0);
		if (BaseCoord.x > 1.0) return vec4(0.0, 0.0, 0.0, 1.0);
		if (BaseCoord.y > 1.0) return vec4(0.0, 0.0, 0.0, 1.0);
	}
	
	// -- Scanline Simulation --
	float InnerSine = ScanCoord.y * sourceSize[0].y * ScanlineScale;
	float ScanBrightMod = sin(InnerSine * Pi + ScanlineOffset * sourceSize[0].y);
	float ScanBrightness = mix(1.0, (pow(ScanBrightMod * ScanBrightMod, ScanlineHeight) * ScanlineBrightScale + 1.0) * 0.5, scandark);
	vec3 ScanlineTexel = InTexel.rgb * ScanBrightness;
	
	// -- Color Compression (increasing the floor of the signal without affecting the ceiling) --
	ScanlineTexel = Floor + (One.xyz - Floor) * ScanlineTexel;
	if (scanlines > 0.5)	return vec4(ScanlineTexel, 1.0);
	else return vec4(InTexel);
}

vec4 SixtyHertz(vec2 UV)
{
	vec4 InPixel = ScanlinePincushion(UV);
	float Milliseconds = float(phase) * 15.0;
	float TimeStep = fract(Milliseconds * SixtyHertzRate);
	float BarPosition = 1.0 - fract(-UV.y + TimeStep) * SixtyHertzScale;
	vec4 OutPixel = InPixel * BarPosition;
	if (hertzroll > 0.5) return OutPixel;
	else return InPixel;
}

void main() {
   vec4 OutPixel = SixtyHertz(vTexCoord.xy);
   FragColor = OutPixel;
}