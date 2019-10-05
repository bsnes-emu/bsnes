// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/dithering/mdapt-4p

/*
   Merge Dithering and Pseudo Transparency Shader v1.5 - Pass 1
   by Sp00kyFox, 2013

   Finds specific patterns and tags their central pixel.

*/

#version 150
// Radius of the area where the algorithm looks for other detected pixels.
#define rad 2

// Minimal count of detection pixels in search area to be rated as valid.
#define minimum 3

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex{
	vec2 texCoord;
};

out vec4 fragColor;

float remapTo01(float v, float low, float high)
{
	return clamp((v - low)/(high-low),0.0,1.0);
}

float remapFrom01(float v, float low, float high)
{
    return round(mix(low, high, v));
}

void main(void) {
	vec2 pos = texCoord*sourceSize[0].xy;	// pos = pixel position
	vec2 dir = sign(pos); // dir = pixel direction

	vec2 g1 = dir*vec2(sourceSize[0].z,0.0);
	vec2 g2 = dir*vec2(0.0,sourceSize[0].w);
	
	vec4 CC   = texture(source[0], texCoord).xyzw;
	
	float C = remapFrom01(CC.w, 0, 15) - 2;

	if(C == 0){
		fragColor=CC;
		return;
	}
	
	/*
	tag values:
		0	nothing	
		
			checkerboard pattern
		9	DL
		8	DR
		7	UR
		6	UL
		5	full
		
			horizontal two-line checkerboard
		4	bottom line
		3	upper line

			vertical two-line checkerboard
		2	left line
		1	right line

			one line dither
		-1	horizontal
		-2	vertical
	*/

	int hits = 0;

	if(C > 0)
	{
//		for(int x=-rad; x<=rad; x++)
//		{
//			for(int y=-rad; y<=rad; y++)
//			{
//				hits += ((remapFrom01(texture(source[0], texCoord + (x*g1) + (y*g2)).x, 0, 15) - 2) > 0) ? 1 : 0;
//			}	
//		}
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + (-2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + (-1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + ( 0*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + ( 1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + ( 2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + (-2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + (-1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + ( 0*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + ( 1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + ( 2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + (-2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + (-1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + ( 0*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + ( 1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + ( 2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + (-2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + (-1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + ( 0*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + ( 1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + ( 2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + (-2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + (-1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + ( 0*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + ( 1*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + ( 2*g2)).w, 0, 15) - 2) > 0) ? 1 : 0;
	}
	else
	{
//		for(int x=-rad; x<=rad; x++)
//		{
//			for(int y=-rad; y<=rad; y++)
//			{
//				hits += ((remapFrom01(texture(source[0], texCoord + (x*g1) + (y*g2)).x, 0, 15) - 2) == C) ? 1 : 0;
//			}	
//		}
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + (-2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + (-1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + ( 0*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + ( 1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-2*g1) + ( 2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + (-2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + (-1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + ( 0*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + ( 1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + (-1*g1) + ( 2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + (-2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + (-1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + ( 0*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + ( 1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 0*g1) + ( 2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + (-2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + (-1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + ( 0*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + ( 1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 1*g1) + ( 2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + (-2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + (-1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + ( 0*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + ( 1*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
			hits += ((remapFrom01(texture(source[0], texCoord + ( 2*g1) + ( 2*g2)).w, 0, 15) - 2) == C) ? 1 : 0;
		fragColor=CC;    
		return;
	}
	
	if(hits < minimum)
		fragColor=vec4(CC.xyz, remapTo01(2, 0, 15));
	else
		fragColor=CC;    
}



