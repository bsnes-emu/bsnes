#version 150

in vec4 position;
in vec2 texCoord;

out Vertex {
   vec2 vTexCoord;
};

uniform vec4 sourceSize[];

void main() {
   gl_Position = position;
   vTexCoord = texCoord;
}