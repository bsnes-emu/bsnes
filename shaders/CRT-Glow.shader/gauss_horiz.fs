#version 150

#define INV_SQRT_2_PI 0.38 // Doesn't have to be accurate.

#define HORIZ_GAUSS_WIDTH 0.5

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
   vec2 vTexCoord;
   float data_pix_no;
   float data_one;
};

out vec4 FragColor;

void main() {
   float texel      = floor(data_pix_no);
   float phase      = data_pix_no - texel;
   float base_phase = phase - 0.5;
   vec2 tex         = vec2((texel + 0.5) * sourceSize[0].z, vTexCoord.y);

   vec3 col = vec3(0.0);
   for (int i = -2; i <= 2; i++)
   {
      float phase = base_phase - float(i);
      float g = INV_SQRT_2_PI * exp(-0.5 * phase * phase / (HORIZ_GAUSS_WIDTH * HORIZ_GAUSS_WIDTH)) / HORIZ_GAUSS_WIDTH;
      col += texture(source[0], tex + vec2(float(i) * data_one, 0.0)).rgb * g;
   }

   FragColor = vec4(col, 1.0);
}
