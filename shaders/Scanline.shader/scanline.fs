#version 150

uniform sampler2D source[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
  vec4 rgba = texture(source[0], texCoord);
  vec4 intensity;
  if(fract(gl_FragCoord.y * (0.5 * 4.0 / 3.0)) > 0.5) {
    intensity = vec4(0);
  } else {
    intensity = smoothstep(0.2, 0.8, rgba) + normalize(rgba);
  }
  fragColor = intensity * -0.25 + rgba * 1.1;
}
