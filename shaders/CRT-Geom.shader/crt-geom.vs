#version 150
 
in vec4 position;
in vec2 texCoord;
in vec4 sourceSize[];
in vec4 targetSize;
 
out Vertex {
   vec2 texCoord;
} vertexOut;
 
float CRTgamma;
float monitorgamma;
vec2 overscan;
vec2 aspect;
float d;
float R;
float cornersize;
float cornersmooth;

vec3 stretch;
vec2 sinangle;
vec2 cosangle;

vec2 one;
float mod_factor;
vec2 ilfac;

#define FIX(c) max(abs(c), 1e-5);

float intersect(vec2 xy)
{
  float A = dot(xy,xy)+d*d;
  float B = 2.0*(R*(dot(xy,sinangle)-d*cosangle.x*cosangle.y)-d*d);
  float C = d*d + 2.0*R*d*cosangle.x*cosangle.y;
  return (-B-sqrt(B*B-4.0*A*C))/(2.0*A);
}

vec2 bkwtrans(vec2 xy)
{
  float c = intersect(xy);
  vec2 point = vec2(c)*xy;
  point -= vec2(-R)*sinangle;
  point /= vec2(R);
  vec2 tang = sinangle/cosangle;
  vec2 poc = point/cosangle;
  float A = dot(tang,tang)+1.0;
  float B = -2.0*dot(poc,tang);
  float C = dot(poc,poc)-1.0;
  float a = (-B+sqrt(B*B-4.0*A*C))/(2.0*A);
  vec2 uv = (point-a*sinangle)/cosangle;
  float r = R*acos(a);
  return uv*r/sin(r/R);
}

vec2 fwtrans(vec2 uv)
{
  float r = FIX(sqrt(dot(uv,uv)));
  uv *= sin(r/R)/r;
  float x = 1.0-cos(r/R);
  float D = d/R + x*cosangle.x*cosangle.y+dot(uv,sinangle);
  return d*(uv*cosangle-x*sinangle)/D;
}

vec3 maxscale()
{
  vec2 c = bkwtrans(-R * sinangle / (1.0 + R/d*cosangle.x*cosangle.y));
  vec2 a = vec2(0.5,0.5)*aspect;
  vec2 lo = vec2(fwtrans(vec2(-a.x,c.y)).x,
		 fwtrans(vec2(c.x,-a.y)).y)/aspect;
  vec2 hi = vec2(fwtrans(vec2(+a.x,c.y)).x,
		 fwtrans(vec2(c.x,+a.y)).y)/aspect;
  return vec3((hi+lo)*aspect*0.5,max(hi.x-lo.x,hi.y-lo.y));
}

void main()
{

  // START of parameters

  // gamma of simulated CRT
  CRTgamma = 2.4;
  // gamma of display monitor (typically 2.2 is correct)
  monitorgamma = 2.2;
  // overscan (e.g. 1.02 for 2% overscan)
  overscan = vec2(0.99,0.99);
  // aspect ratio
  aspect = vec2(1.0, 0.75);
  // lengths are measured in units of (approximately) the width of the monitor
  // simulated distance from viewer to monitor
  d = 2.0;
  // radius of curvature
  R = 2.0;
  // tilt angle in radians
  // (behavior might be a bit wrong if both components are nonzero)
  const vec2 angle = vec2(0.0,-0.0);
  // size of curved corners
  cornersize = 0.03;
  // border smoothness parameter
  // decrease if borders are too aliased
  cornersmooth = 80.0;

  // END of parameters
  
vertexOut.texCoord = texCoord.xy;
gl_Position = position;

  // Precalculate a bunch of useful values we'll need in the fragment
  // shader.
  sinangle = sin(angle);
  cosangle = cos(angle);
  stretch = maxscale();

  ilfac = vec2(1.0,floor(sourceSize[0].y/200.0));

  // The size of one texel, in texture-coordinates.
  one = ilfac / sourceSize[0].xy;

  // Resulting X pixel-coordinate of the pixel we're drawing.
  mod_factor = texCoord.x * targetSize.x;			
}