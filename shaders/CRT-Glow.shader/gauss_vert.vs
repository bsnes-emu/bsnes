#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 vTexCoord;
   vec2 data_pix_no;
   float data_one;
};

uniform vec4 sourceSize[];

void main() {
   gl_Position = position;
   vTexCoord = texCoord;
   
   data_pix_no = vTexCoord.xy * sourceSize[0].xy - vec2(0.0, 0.5);
   data_one = sourceSize[0].w;
}