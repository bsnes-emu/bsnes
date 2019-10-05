// This is a port of the original CG shader to the quark format
// the original shader can be found here :
// https://github.com/libretro/common-shaders/tree/master/dithering/mdapt-4p

/*
   Merge Dithering and Pseudo Transparency Shader v1.5 - Pass 1
   by Sp00kyFox, 2013

   Finds specific patterns and tags their central pixel.

*/
#version 150
//#define HORI
//#define VERT
#define dtt				vec3(65536,255,1)
#define eq_threshold	5.0


uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;
in Vertex{
	vec2 texCoord;
};

out vec4 fragColor;

float reduce(vec3 color)
{
	return dot(color, dtt);
}

float df(float A, float B)
{
  return abs(A-B);
}

bool eq(float A, float B)
{
  return (A == B);
}

float remapTo01(float v, float low, float high)
{
	return clamp((v - low)/(high-low),0.0,1.0);
}


void main(void) {
	vec2 pos = texCoord*sourceSize[0].xy;	// pos = pixel position
	vec2 dir = sign(pos);	// dir = pixel direction

	vec2 g1 = dir*vec2(sourceSize[0].z,0.0);
	vec2 g2 = dir*vec2(0.0,sourceSize[0].w);;
	
	/*
				U3
		            UUL U2 UUR
		        ULL UL  U1 UR  URR
		    L3  L2  L1  C  R1  R2  R3
		        DLL DL  D1 DR  DRR	
			    DDL D2 DDR
				D3
	*/

	vec3 c = texture(source[0], texCoord).xyz;
	vec3 l1 = texture(source[0], texCoord - g1).xyz;
	vec3 l2 = texture(source[0], texCoord - 2*g1).xyz;
	vec3 r1 = texture(source[0], texCoord + g1).xyz;
	vec3 r2 = texture(source[0], texCoord + 2*g1).xyz;
	vec3 u1 = texture(source[0], texCoord - g2).xyz;
	vec3 u2 = texture(source[0], texCoord - 2*g2).xyz;
	vec3 d1 = texture(source[0], texCoord + g2).xyz;
	vec3 d2 = texture(source[0], texCoord + 2*g2).xyz;
	vec3 ul = texture(source[0], texCoord - g1 - g2).xyz;
	vec3 ur = texture(source[0], texCoord + g1 - g2).xyz;
	vec3 dl = texture(source[0], texCoord - g1 + g2).xyz;
	vec3 dr = texture(source[0], texCoord + g1 + g2).xyz;

	vec3 ull = texture(source[0], texCoord - 2*g1 - g2).xyz;
	vec3 uul = texture(source[0], texCoord - g1 - 2*g2).xyz;
	vec3 uur = texture(source[0], texCoord + g1 - 2*g2).xyz;
	vec3 urr = texture(source[0], texCoord + 2*g1 - g2).xyz;
	vec3 drr = texture(source[0], texCoord + 2*g1 + g2).xyz;
	vec3 ddr = texture(source[0], texCoord + g1 + 2*g2).xyz;
	vec3 ddl = texture(source[0], texCoord - g1 + 2*g2).xyz;
	vec3 dll = texture(source[0], texCoord - 2*g1 + g2).xyz;

	vec3 l3 = texture(source[0], texCoord - 3*g1).xyz;
	vec3 r3 = texture(source[0], texCoord + 3*g1).xyz;
	vec3 u3 = texture(source[0], texCoord - 3*g2).xyz;
	vec3 d3 = texture(source[0], texCoord + 3*g2).xyz;

	float C   = reduce(   c );

	float L1  = reduce(  l1 ); float U1  = reduce(  u1 );
	float L2  = reduce(  l2 ); float U2  = reduce(  u2 );
	float R1  = reduce(  r1 ); float D1  = reduce(  d1 );
	float R2  = reduce(  r2 ); float D2  = reduce(  d2 );

	float UL  = reduce(  ul ); float L3  = reduce(  l3 );
	float UR  = reduce(  ur ); float R3  = reduce(  r3 );
	float DL  = reduce(  dl ); float U3  = reduce(  u3 );
	float DR  = reduce(  dr ); float D3  = reduce(  d3 );

	float ULL = reduce( ull ); float DRR = reduce( drr );
	float UUL = reduce( uul ); float DDR = reduce( ddr );
	float UUR = reduce( uur ); float DDL = reduce( ddl );
	float URR = reduce( urr ); float DLL = reduce( dll );
	
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

	float type=0;

	// checkerboard pattern
	if(!eq(C,D1) && !eq(C,U1) && !eq(C,L1) && !eq(C,R1))
	{
		if(eq(C,UL))
		{
			if(eq(C,UR))
			{
				if(eq(C,DR))
				{
					if(eq(C,DL))
						type = 5;
					else if(!eq(D1,L1) || eq(D1,DL))
						type = 9;							
				}
				else
				{
					if(eq(C,DL) && (!eq(D1,R1) || eq(D1,DR)))
						type = 8;
				}	
			}
			else
			{
				if(eq(C,DR) && eq(C,DL) && (!eq(U1,R1) || eq(U1,UR)))
					type = 7;							
			}
		}
		else if(eq(C,UR) && eq(C,DR) && eq(C,DL) && (!eq(U1,L1) || eq(U1,UL)))
			type = 6;	
	}
	// horizontal two-line checkerboard
	else if(eq(C,L2) && eq(C,R2) && eq(C,UL) && eq(C,UR) && !eq(C,L1) && !eq(C,R1) && !eq(C,ULL) && !eq(C,U1) && !eq(C,URR))
	{
		type = 4;
	}
	else if(eq(C,L2) && eq(C,R2) && eq(C,DL) && eq(C,DR) && !eq(C,L1) && !eq(C,R1) && !eq(C,DLL) && !eq(C,D1) && !eq(C,DRR))
	{
		type = 3;
	}
	// vertical two-line checkerboard
	else if(eq(C,U2) && eq(C,D2) && eq(C,UR) && eq(C,DR) && !eq(C,U1) && !eq(C,D1) && !eq(C,R1) && !eq(C,UUR) && !eq(C,DDR))				
	{
		type = 2;
	}
	else if(eq(C,U2) && eq(C,D2) && eq(C,UL) && eq(C,DL) && !eq(C,U1) && !eq(C,D1) && !eq(C,L1) && !eq(C,UUL) && !eq(C,DDL))				
	{
		type = 1;
	}	
#ifdef HORI
	// horizontal one line dither
	else if(eq(C,L2) && eq(C,R2) && eq(L1,R1) && !eq(C,L1) && !eq(C,L3) && !eq(C,R3))
		type = -1;
#endif
#ifdef VERT
	// vertical one line dither
	else if(eq(C,U2) && eq(C,D2) && eq(U1,D1) && !eq(C,U1) && !eq(C,U3) && !eq(C,D3))
		type = -2;
#endif	

	fragColor=vec4(c, remapTo01(type+2, 0, 15));
    
}



