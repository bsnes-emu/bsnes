#version 150

#define INPUT_GAMMA 2.2

uniform sampler2D source[];

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

void main() {
   FragColor = pow(texture(source[0], vTexCoord), vec4(INPUT_GAMMA));
}