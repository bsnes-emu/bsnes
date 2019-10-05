#version 150

#define BLOOM_STRENGTH 0.25
#define OUTPUT_GAMMA 2.2

uniform sampler2D source[];

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

// For debugging
#define BLOOM_ONLY 0

#define CRT_PASS source[3]

void main() {
#if BLOOM_ONLY
   vec3 source = BLOOM_STRENGTH * texture(source[0], vTexCoord).rgb;
#else
   vec3 source_ = 1.15 * texture(CRT_PASS, vTexCoord).rgb;
   vec3 bloom  = texture(source[0], vTexCoord).rgb;
   source_     += BLOOM_STRENGTH * bloom;
#endif
   FragColor = vec4(pow(clamp(source_, 0.0, 1.0), vec3(1.0 / OUTPUT_GAMMA)), 1.0);
}