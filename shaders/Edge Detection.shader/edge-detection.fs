#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

vec3 grayscale(vec3 color) {
  return vec3(dot(color, vec3(0.3, 0.59, 0.11)));
}

void main() {
  vec2 offset = fract(texCoord * sourceSize[0].xy) - 0.5;
  offset /= sourceSize[0].xy;

  vec3 cx = texture2D(source[0], texCoord - offset).xyz;
  vec3 cy = texture2D(source[0], texCoord).xyz;
  vec3 cz = vec3(5.0 * grayscale(abs(cx - cy)));

  fragColor = vec4(clamp(cz, 0.0, 1.0), 1.0);
}
