#version 150

#define GLOW_WHITEPOINT 1.0
#define GLOW_ROLLOFF 3.0

uniform sampler2D source[];

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

void main() {
   vec3 color  = 1.15 * texture(source[0], vTexCoord).rgb;
   vec3 factor = clamp(color / GLOW_WHITEPOINT, 0.0, 1.0);

   FragColor = vec4(pow(factor, vec3(GLOW_ROLLOFF)), 1.0);
}