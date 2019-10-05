// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/dithering/mdapt-4p

/*
   Merge Dithering and Pseudo Transparency Shader v1.5 - Pass 4
   by Sp00kyFox, 2013

   Blends tagged pixels with tagged neighbors.

*/

#version 150
uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex{
	vec2 texCoord;
};

out vec4 fragColor;

bool eq(vec4 A, vec4 B)
{
	return (A.x == B.x && A.y == B.y && A.z == B.z);
}

float remapFrom01(float v, float low, float high)
{
    return round(mix(low, high, v));
}


vec3 merge3(vec4 C, vec4 A, vec4 B, float tag)
{
	if(A.w == tag)
	{
		if(B.w == tag)
			return 0.5*C.xyz + 0.25*(A.xyz + B.xyz);
		else
			return 0.5*(C.xyz + A.xyz);
	}
	else if(B.w == tag)
		return 0.5*(C.xyz + B.xyz);
	else
		return C.xyz;	
}

vec3 merge9(vec4 C, vec4 UL, vec4 UR, vec4 DL, vec4 DR, vec4 U1, vec4 D1, vec4 L1, vec4 R1)
{
	//return 0.25*C.xyz + 0.0625*(UL.xyz + UR.xyz + DR.xyz + DL.xyz) + 0.125*(L1.xyz + R1.xyz + D1.xyz + U1.xyz);
	
	vec3 B = vec3(0.0);
	vec3 W = vec3(0.0);
	float cntB = 0;
	float cntW = 0;	

	if(UL.w > 0 || eq(UL,C) || (D1.w > 0 && eq(UL,D1)) || (R1.w > 0 && eq(UL,R1)) )
	{
		B = B+UL.xyz;
		cntB++;
	}
	if(UR.w > 0 || eq(UR,C) || (D1.w > 0 && eq(UR,D1)) || (L1.w > 0 && eq(UR,L1)) )
	{
		B = B+UR.xyz;
		cntB++;
	}
	if(DL.w > 0 || eq(DL,C) || (U1.w > 0 && eq(DL,U1)) || (R1.w > 0 && eq(DL,R1)) )
	{
		B = B+DL.xyz;
		cntB++;
	}
	if(DR.w > 0 || eq(DR,C) || (U1.w > 0 && eq(DR,U1)) || (L1.w > 0 && eq(DR,L1)) )
	{
		B = B+DR.xyz;
		cntB++;
	}
	if(U1.w > 0 || eq(U1,C) || (D1.w > 0 && eq(U1,D1)) )
	{
		W = W+U1.xyz;
		cntW++;
	}
	if(D1.w > 0 || eq(D1,C) || (U1.w > 0 && eq(D1,U1)) )
	{
		W = W+D1.xyz;
		cntW++;
	}
	if(L1.w > 0 || eq(L1,C) || (R1.w > 0 && eq(L1,R1)) )
	{
		W = W+L1.xyz;
		cntW++;
	}
	if(R1.w > 0 || eq(R1,C) || (L1.w > 0 && eq(R1,L1)) )
	{
		W = W+R1.xyz;
		cntW++;
	}
		
	if(cntB == 0)
	{
		if(cntW == 0)
			return C.xyz;
		else
			return 0.5*C.xyz + (1/(cntW*2))*W;
	}
	if(cntW == 0)
		return 0.5*C.xyz + (1/(cntB*2))*B;
	else
		return 0.25*C.xyz + (1/(cntB*4))*B + (1/(cntW*2))*W;	
}
void main(void) {
	vec2 pos = texCoord*sourceSize[0].xy;	// pos = pixel position
	vec2 dir = sign(pos); // dir = pixel direction

	vec2 g1 = dir*vec2(sourceSize[0].z,0.0);
	vec2 g2 = dir*vec2(0.0,sourceSize[0].w);
	
	/*
		UL U1 UR
		L1 C  R1
		DL D1 DR	
	*/

	vec4 C   = texture(source[0], texCoord).xyzw;
	vec4 L1  = texture(source[0], texCoord - g1).xyzw;	
	vec4 R1  = texture(source[0], texCoord + g1).xyzw;
	vec4 U1  = texture(source[0], texCoord - g2).xyzw;
	vec4 D1  = texture(source[0], texCoord + g2).xyzw;
	vec4 UL  = texture(source[0], texCoord - g1 - g2).xyzw;
	vec4 UR  = texture(source[0], texCoord + g1 - g2).xyzw;
	vec4 DL  = texture(source[0], texCoord - g1 + g2).xyzw;
	vec4 DR  = texture(source[0], texCoord + g1 + g2).xyzw;


	C.w   = remapFrom01(C.w, 0, 15) - 2;
	L1.w  = remapFrom01(L1.w, 0, 15) - 2;	
	R1.w  = remapFrom01(R1.w, 0, 15) - 2;
	U1.w  = remapFrom01(U1.w, 0, 15) - 2;
	D1.w  = remapFrom01(D1.w, 0, 15) - 2;
	UL.w  = remapFrom01(UL.w, 0, 15) - 2;
	UR.w  = remapFrom01(UR.w, 0, 15) - 2;
	DL.w  = remapFrom01(DL.w, 0, 15) - 2;
	DR.w  = remapFrom01(DR.w, 0, 15) - 2;
	
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
	if(C.w > 0){
		fragColor=vec4(merge9(C,UL,UR,DL,DR,U1,D1,L1,R1),0);
		return;
	}
	// horizontal one line dither
	if(C.w == -1){
		fragColor=vec4(merge3(C,L1,R1,-1),0);
		return;
	}

	// vertical one line dither
	if(C.w == -2){
		fragColor=vec4(merge3(C,U1,D1,-2),0);
		return;
	}
	
	fragColor=vec4(C.xyz, 1.0);
    
}



