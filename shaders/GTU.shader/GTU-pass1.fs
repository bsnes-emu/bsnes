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


#define RGB_to_YIQ 	mat3x3( 0.299 , 0.595716 , 0.211456 ,	0.587    , -0.274453 , -0.522591 ,		0.114    , -0.321263 , 0.311135 )
#define YIQ_to_RGB 	mat3x3( 1.0   , 1.0      , 1.0      ,	0.9563   , -0.2721   , -1.1070   ,		0.6210   , -0.6474   , 1.7046   )
#define LEVELS(C) 		clamp((C -16/ 255.0)*255.0/(235.0-16.0),0.0,1.0)

uniform sampler2D	source[];
uniform vec4		sourceSize[];

in Vertex {
	vec2 texCoord;
};
out vec4 fragColor;


void main() {
	vec3 c=texture2D(source[0], texCoord.xy).xyz;
#ifdef TV_COLOR_LEVELS
	c=LEVELS(c);
#endif 	
#ifdef COMPOSITE_CONNECTION
	c=RGB_to_YIQ*c;
#endif
	fragColor = vec4(c, 1.0);
}
