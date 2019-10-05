#version 150

////////////////////////////////////////////////////////
//	GTU version 0.40	
//	Author: aliaspider - aliaspider@gmail.com
//	License: GPLv3      
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// SETTINGS 
////////////////////////////////////////////////////////

//#define	CROP_OVERSCAN
#define	TV_COLOR_LEVELS
//#define	COMPOSITE_CONNECTION
//#define	NO_SCANLINES
#define	TV_HORIZONTAL_RESOLUTION	400.0
#define	TV_VERTICAL_RESOLUTION	300.0
#define	SIGNAL_RESOLUTION	280.0
#define	SIGNAL_RESOLUTION_I	83.0
#define	SIGNAL_RESOLUTION_Q	25.0
#define	TV_DISPLAY_GAMMA	2.4
#define	OUTPUT_DISPLAY_GAMMA	2.2

////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////



#define SCANLINE_WIDTH (1.5*sourceSize[0].y/TV_VERTICAL_RESOLUTION)

uniform sampler2D	source[];
uniform sampler2D 	texture[];
uniform vec4		sourceSize[];

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;


#define GAMMAOUT(c0)	(pow(c0, vec3(1.0/OUTPUTG2)))
#define pi			3.14159265358
#define GAUSS(x,w) ((sqrt(2.0) / (w)) * (exp((-2.0 * pi * (x) * (x)) / ((w) * (w)))))
#define Y(j) (offset.y-(j))
#define a(x) abs(x)
#define d(x,b) (pi*b*min(a(x)+0.5,1.0/b))
#define e(x,b) (pi*b*min(max(a(x)-0.5,-1.0/b),1.0/b))
#define STU(x,b) ((d(x,b)+sin(d(x,b))-e(x,b)-sin(e(x,b)))/(2.0*pi))

#define SOURCE(j) vec2(texCoord.x,texCoord.y - Y(j)*sourceSize[0].w)
#define C(j) (texture2D(source[0], SOURCE(j)).xyz)

#ifdef NO_SCANLINES
#define VAL(j) (C(j)*STU(Y(j),(TV_VERTICAL_RESOLUTION*sourceSize[0].w)))
#else
#define VAL(j) (C(j)*GAUSS(Y(j),SCANLINE_WIDTH))
#endif



void main() {
	
	vec2	offset	= fract((texCoord.xy * sourceSize[0].xy) - 0.5);
	vec3	tempColor = vec3(0.0);	

	tempColor+=VAL(-3.0);
	tempColor+=VAL(-2.0);
	tempColor+=VAL(-1.0);
	tempColor+=VAL(0.0);
	tempColor+=VAL(1.0);
	tempColor+=VAL(2.0);
	tempColor+=VAL(3.0);
	tempColor+=VAL(4.0);
	
	fragColor = vec4(pow(tempColor,vec3(1.0/OUTPUT_DISPLAY_GAMMA)), 1.0);
}