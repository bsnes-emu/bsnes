#version 150

#in red
#in green
#in blue
#in gain
#in gamma
#in blacklevel
#in ambient
#in BGR

#define outgamma 2.2

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define fetch_offset(coord,offset) (pow(vec3(gain) * texelFetchOffset(source[0], (coord), 0, (offset)).rgb + vec3(blacklevel), vec3(gamma)) + vec3(ambient))

// integral of (1 - x^2 - x^4 + x^6)^2
const float coeffs_x[] = float[](1.0, -2.0/3.0, -1.0/5.0, 4.0/7.0, -1.0/9.0, -2.0/11.0, 1.0/13.0);
// integral of (1 - 2x^4 + x^6)^2
const float coeffs_y[] = float[](1.0,      0.0, -4.0/5.0, 2.0/7.0,  4.0/9.0, -4.0/11.0, 1.0/13.0); 
float intsmear_func(float z, float coeffs[7])
{
  float z2 = z*z;
  float zn = z;
  float ret = 0.0;
  for (int i = 0; i < 7; i++) {
    ret += zn*coeffs[i];
    zn *= z2;
  }
  return ret;
}
float intsmear(float x, float dx, float d, float coeffs[7])
{
  float zl = clamp((x-dx*0.5)/d,-1.0,1.0);
  float zh = clamp((x+dx*0.5)/d,-1.0,1.0);
  return d * ( intsmear_func(zh,coeffs) - intsmear_func(zl,coeffs) )/dx;
}

void main()
{
  vec2 texelSize = 1.0 / sourceSize[0].xy;
  vec2 range = sourceSize[0].xy / (targetSize.xy * sourceSize[0].xy);

  vec3 cred   = pow(red,   vec3(outgamma));
  vec3 cgreen = pow(green, vec3(outgamma));
  vec3 cblue  = pow(blue,  vec3(outgamma));

  ivec2 tli = ivec2(floor(texCoord/texelSize-vec2(0.4999)));

  vec3 lcol, rcol;
  float subpix = (texCoord.x/texelSize.x - 0.4999 - float(tli.x))*3.0;
  float rsubpix = range.x/texelSize.x * 3.0;
  lcol = vec3(intsmear(subpix+1.0,rsubpix, 1.5, coeffs_x),
              intsmear(subpix    ,rsubpix, 1.5, coeffs_x),
              intsmear(subpix-1.0,rsubpix, 1.5, coeffs_x));
  rcol = vec3(intsmear(subpix-2.0,rsubpix, 1.5, coeffs_x),
              intsmear(subpix-3.0,rsubpix, 1.5, coeffs_x),
              intsmear(subpix-4.0,rsubpix, 1.5, coeffs_x));
#ifdef BGR
  lcol.rgb = lcol.bgr;
  rcol.rgb = rcol.bgr;
#endif
  float tcol, bcol;
  subpix = texCoord.y/texelSize.y - 0.4999 - float(tli.y);
  rsubpix = range.y/texelSize.y;
  tcol = intsmear(subpix    ,rsubpix, 0.63, coeffs_y);
  bcol = intsmear(subpix-1.0,rsubpix, 0.63, coeffs_y);

  vec3 topLeftColor     = fetch_offset(tli, ivec2(0,0)) * lcol * vec3(tcol);
  vec3 bottomRightColor = fetch_offset(tli, ivec2(1,1)) * rcol * vec3(bcol);
  vec3 bottomLeftColor  = fetch_offset(tli, ivec2(0,1)) * lcol * vec3(bcol);
  vec3 topRightColor    = fetch_offset(tli, ivec2(1,0)) * rcol * vec3(tcol);

  vec3 averageColor = topLeftColor + bottomRightColor + bottomLeftColor + topRightColor;

  averageColor = mat3x3(cred, cgreen, cblue) * averageColor;

  fragColor = vec4(pow(averageColor,vec3(1.0/outgamma)),0.0);
}
