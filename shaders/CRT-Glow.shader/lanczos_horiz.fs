#version 150

uniform sampler2D source[];

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

void main() {
   FragColor = texture(source[0], vTexCoord);
}