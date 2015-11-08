#version 150
#define distortion 0.2

uniform sampler2D source[];
uniform vec4 sourceSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

vec2 radialDistortion(vec2 coord) {
  vec2 cc = coord - vec2(0.5);
  float dist = dot(cc, cc) * distortion;
  return coord + cc * (1.0 - dist) * dist;
}

void main() {
  fragColor = texture(source[0], radialDistortion(texCoord));
}
