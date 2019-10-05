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

in vec4 position;
in vec2 texCoord;

out Vertex{
	vec2 texCoord;
}vertexOut;

void main(void){
	
#ifdef CROP_OVERSCAN
	gl_Position=position;
	gl_Position.x/=(224.0/240.0);
#else
	gl_Position=position;
#endif
	
	vertexOut.texCoord=texCoord;	
}
