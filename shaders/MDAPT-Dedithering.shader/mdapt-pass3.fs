// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/dithering/mdapt-4p

/*
   Merge Dithering and Pseudo Transparency Shader v1.5 - Pass 3
   by Sp00kyFox, 2013

   Tags the rest of the detected pattern from pass 1.

*/

#version 150
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
		/*
			U2
		   UL	U1 UR
		L2 L1   xC  R1 R2
    		   DL   D1 DR	
			D2
	*/

	vec4 xC = texture(source[0], texCoord).xyzw;
	vec4 xL1 = texture(source[0], texCoord - g1).xyzw;
	vec4 xL2 = texture(source[0], texCoord - 2*g1).xyzw;
	vec4 xR1 = texture(source[0], texCoord + g1).xyzw;
	vec4 xR2 = texture(source[0], texCoord + 2*g1).xyzw;
	vec4 xU1 = texture(source[0], texCoord - g2).xyzw;
	vec4 xU2 = texture(source[0], texCoord - 2*g2).xyzw;
	vec4 xD1 = texture(source[0], texCoord + g2).xyzw;
	vec4 xD2 = texture(source[0], texCoord + 2*g2).xyzw;
	vec4 xUL = texture(source[0], texCoord - g1 - g2).xyzw;
	vec4 xUR = texture(source[0], texCoord + g1 - g2).xyzw;
	vec4 xDL = texture(source[0], texCoord - g1 + g2).xyzw;
	vec4 xDR = texture(source[0], texCoord + g1 + g2).xyzw;
	
	float  C = remapFrom01(xC.w, 0, 15) - 2;
	float L1 = remapFrom01(xL1.w, 0, 15) - 2;
	float L2 = remapFrom01(xL2.w, 0, 15) - 2;
	float R1 = remapFrom01(xR1.w, 0, 15) - 2;
	float R2 = remapFrom01(xR2.w, 0, 15) - 2;
	float U1 = remapFrom01(xU1.w, 0, 15) - 2;
	float U2 = remapFrom01(xU2.w, 0, 15) - 2;
	float D1 = remapFrom01(xD1.w, 0, 15) - 2;
	float D2 = remapFrom01(xD2.w, 0, 15) - 2;
	float UL = remapFrom01(xUL.w, 0, 15) - 2;
	float UR = remapFrom01(xUR.w, 0, 15) - 2;
	float DL = remapFrom01(xDL.w, 0, 15) - 2;
	float DR = remapFrom01(xDR.w, 0, 15) - 2;

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
	
	// checkerboard pattern
	if(U1 == 5 || D1 == 5 || L1 == 5 || R1 == 5 || UL == 5 || UR == 5 || DR == 5 || DL == 5)
	{
		fragColor=vec4(xC.xyz, remapTo01(5+2, 0, 15));	
		return ;
	}
	if(U1 == 6 || L1 == 6 || UL == 6 || UR == 6 || DL == 6) // UL
	{
		fragColor=vec4(xC.xyz, remapTo01(6+2, 0, 15));	
		return ;
	}

	if(U1 == 7 || R1 == 7 || UL == 7 || UR == 7 || DR == 7) // UR
	{
		fragColor=vec4(xC.xyz, remapTo01(7+2, 0, 15));	
		return ;
	}

	if(D1 == 8 || R1 == 8 || UR == 8 || DR == 8 || DL == 8) // DR
	{
		fragColor=vec4(xC.xyz, remapTo01(8+2, 0, 15));	
		return ;
	}

	if(D1 == 9 || L1 == 9 || UL == 9 || DR == 9 || DL == 9) // DL
	{
		fragColor=vec4(xC.xyz, remapTo01(9+2, 0, 15));	
		return ;
	}

	// horizontal two-line checkerboard
	if (L2 == 4 || L1 == 4 || R1 == 4 || R2 == 4 || DL == 4 || D1 == 4 || DR == 4)		
	{
		fragColor=vec4(xC.xyz, remapTo01(4+2, 0, 15));
		return ;
	}
	if (L2 == 3 || L1 == 3 || R1 == 3 || R2 == 3 || UL == 3 || U1 == 3 || UR == 3)		
	{
		fragColor=vec4(xC.xyz, remapTo01(3+2, 0, 15));	
		return ;
	}
	
	// vertical two-line checkerboard
	if (U2 == 2 || U1 == 2 || D1 == 2 || D2 == 2 || UL == 2 || L1 == 2 || DL == 2)			
	{
		fragColor=vec4(xC.xyz, remapTo01(2+2, 0, 15));	
		return ;
	}
	if (U2 == 1 || U1 == 1 || D1 == 1 || D2 == 1 || UR == 1 || R1 == 1 || DR == 1)			
	{
		fragColor=vec4(xC.xyz, remapTo01(1+2, 0, 15));	
		return ;
	}
	
	if(C > 0){
		fragColor=xC;
		return ;
	}

	// horizontal one line dither
	if (L2 == -1 || L1 == -1 || R1 == -1 || R2 == -1)
	{
		fragColor=vec4(xC.xyz, remapTo01(-1+2, 0, 15));	
		return ;
	}

	// vertical one line dither
	if (U2 == -2 || U1 == -2 || D1 == -2 || D2 == -2)
	{
		fragColor=vec4(xC.xyz, remapTo01(-2+2, 0, 15));
		return;
	}

	fragColor=xC;
}



