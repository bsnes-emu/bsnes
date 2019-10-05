#version 150

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

void main() {
      vec2 texelSize = 1.0 * sourceSize[0].zw;

      vec2 range = vec2(abs(dFdx(texCoord.x)), abs(dFdy(texCoord.y)));
      range = range / 2.0 * 0.999;

      float left   = texCoord.x - range.x;
      float top    = texCoord.y + range.y;
      float right  = texCoord.x + range.x;
      float bottom = texCoord.y - range.y;

      vec4 topLeftColor     = pow(texture(source[0], vec2(left, top)), vec4(2.2));
      vec4 bottomRightColor = pow(texture(source[0], vec2(right, bottom)), vec4(2.2));
      vec4 bottomLeftColor  = pow(texture(source[0], vec2(left, bottom)), vec4(2.2));
      vec4 topRightColor    = pow(texture(source[0], vec2(right, top)), vec4(2.2));

      vec2 border = clamp(round(texCoord / texelSize) * texelSize, vec2(left, bottom), vec2(right, top));

      float totalArea = 4.0 * range.x * range.y;

      vec4 averageColor;
      averageColor  = ((border.x - left)  * (top - border.y)    / totalArea) * topLeftColor;
      averageColor += ((right - border.x) * (border.y - bottom) / totalArea) * bottomRightColor;
      averageColor += ((border.x - left)  * (border.y - bottom) / totalArea) * bottomLeftColor;
      averageColor += ((right - border.x) * (top - border.y)    / totalArea) * topRightColor;

      fragColor = pow(averageColor, vec4(1.0/2.2));
}
