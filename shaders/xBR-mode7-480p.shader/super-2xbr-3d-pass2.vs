#version 150
 
in vec4 position;
in vec2 texCoord;
in vec4 sourceSize[];
in vec4 targetSize;
 
out Vertex {
   vec2 texCoord;
//   vec4 t1;
//   vec4 t2;
//   vec4 t3;
//   vec4 t4;
} vertexOut;

#define SourceSize sourceSize[0]

void main()
{
   vertexOut.texCoord = texCoord.xy * 1.0001;
   gl_Position = position;

   float dx = SourceSize.z;
   float dy = SourceSize.w;
//   vertexOut.t1 = texCoord.xyxy + vec4(-2.0*dx, -2.0*dy, dx, dy);
//   vertexOut.t2 = texCoord.xyxy + vec4(    -dx, -2.0*dy,  0, dy);
//   vertexOut.t3 = texCoord.xyxy + vec4(-2.0*dx,     -dy, dx,  0);
//   vertexOut.t4 = texCoord.xyxy + vec4(    -dx,     -dy,  0,  0); // dunno why this doesn't want to carry over, but whatever
}