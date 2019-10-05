#version 150

uniform sampler2D source[];
uniform sampler2D history[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  vec4 color = pow(texture(history[7], texCoord).rgba, vec4(2.2));
  color = (color + pow(texture(history[6], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(history[5], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(history[4], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(history[3], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(history[2], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(history[1], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(history[0], texCoord).rgba, vec4(2.2))) / 2.0;
  color = (color + pow(texture(source[0], texCoord).rgba, vec4(2.2))) / 2.0;
  fragColor = pow(color, vec4(1.0 / 2.2));
}