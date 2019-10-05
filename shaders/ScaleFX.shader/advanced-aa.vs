#version 150

in vec4 position;
in vec2 texCoord;

uniform vec4 sourceSize[];

in vec2 ps;
in float dx;
in float dy;

out Vertex {
  vec2 texCoord;
  vec4 t1;
  vec4 t2;
  vec4 t3;
  vec4 t4;
} vertexOut;

void main() {

	vec2 ps = vec2(1.0/sourceSize[0].x, 1.0/sourceSize[0].y);
	float dx = ps.x * 0.5;
	float dy = ps.y * 0.5;
	
    gl_Position = position;
    vertexOut.texCoord = texCoord;
  	vertexOut.t1.xy = texCoord + vec2(-dx, 0);
	vertexOut.t2.xy = texCoord + vec2( dx, 0);
	vertexOut.t3.xy = texCoord + vec2( 0, -dy);
	vertexOut.t4.xy = texCoord + vec2( 0, dy);
	vertexOut.t1.zw = texCoord + vec2(-dx, -dy);
	vertexOut.t2.zw = texCoord + vec2(-dx, dy);
	vertexOut.t3.zw = texCoord + vec2( dx, -dy);
	vertexOut.t4.zw = texCoord + vec2( dx, dy);
}
