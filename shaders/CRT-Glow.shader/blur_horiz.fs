#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
   vec2 vTexCoord;
};

out vec4 FragColor;

// Higher value, more centered glow.
// Lower values might need more taps.
#define GLOW_FALLOFF 0.35
#define TAPS 4

#define kernel(x) exp(-GLOW_FALLOFF * (x) * (x))

void main() {
   vec3 col = vec3(0.0);
   float dx = 4.0 * sourceSize[0].z; // Mipmapped

   float k_total = 0.0;
   for (int i = -TAPS; i <= TAPS; i++)
   {
      float k  = kernel(i);
      k_total += k;
      col     += k * texture(source[0], vTexCoord + vec2(float(i) * dx, 0.0)).rgb;
   }

   FragColor = vec4(col / k_total, 1.0);
}