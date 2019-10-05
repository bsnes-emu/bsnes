#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 vTexCoord;
   float data_pix_no;
   float data_one;
};

uniform vec4 sourceSize[];

void main() {
   gl_Position = position;
   vTexCoord = texCoord;
   
   data_pix_no = vTexCoord.x * sourceSize[0].x;
   data_one = sourceSize[0].z;
}