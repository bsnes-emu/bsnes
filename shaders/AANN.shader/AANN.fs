#version 150

// AntiAliased Nearest Neighbor
// by jimbo1qaz and wareya
// Licensed MIT

precision highp float;

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize[];

in Vertex {
  vec2 texCoord;
};

out vec4 fragColor;

#define NOT(fl) (1-fl)
#define YES(fl) fl

vec4 vpow(vec4 n, float e)
{
    return vec4(pow(n.x, e), pow(n.y, e), pow(n.z, e), pow(n.w, e));
}

vec4 getLQV(vec3 mine) {
    return vec4
    ( mine.r
    , mine.g
    , mine.b
    , mine.r*0.2989 + mine.g*0.5870 + mine.b*0.1140);
}
vec3 fromLQV(vec4 mine) {
    float f = mine.w/(mine.r*0.2989 + mine.g*0.5870 + mine.b*0.1140);
    return vec3(mine.rgb)*f;
}

vec3 percent(float ssize, float tsize, float coord) {
    float minfull = (coord*tsize - 0.5) /tsize*ssize;
    float maxfull = (coord*tsize + 0.5) /tsize*ssize;

    float realfull = floor(maxfull);

    if (minfull > realfull) {
        return vec3(1, (realfull+0.5)/ssize, (realfull+0.5)/ssize);
    }

    return vec3(
            (maxfull - realfull) / (maxfull - minfull),
            (realfull-0.5) / ssize,
            (realfull+0.5) / ssize
        );
}


void main() {
    float cheapsrgb = 2.1;
    float gamma = 3.0;
    vec3 xstuff = percent(sourceSize[0].x, targetSize[0].x, texCoord.x);
    vec3 ystuff = percent(sourceSize[0].y, targetSize[0].y, texCoord.y);

    float xkeep = xstuff[0];
    float ykeep = ystuff[0];
    
    // get points to interpolate across, in linear rgb
    vec4 a = getLQV(vpow(texture(source[0],vec2(xstuff[1],ystuff[1])), cheapsrgb).rgb);
    vec4 b = getLQV(vpow(texture(source[0],vec2(xstuff[2],ystuff[1])), cheapsrgb).rgb);
    vec4 c = getLQV(vpow(texture(source[0],vec2(xstuff[1],ystuff[2])), cheapsrgb).rgb);
    vec4 d = getLQV(vpow(texture(source[0],vec2(xstuff[2],ystuff[2])), cheapsrgb).rgb);
    
    // use perceptual gamma for luminance component
    a.w = pow(a.w, 1/gamma);
    b.w = pow(b.w, 1/gamma);
    c.w = pow(c.w, 1/gamma);
    d.w = pow(d.w, 1/gamma);
    
    // interpolate
    vec4 gammaLQVresult =
        NOT(xkeep)*NOT(ykeep)*a +
        YES(xkeep)*NOT(ykeep)*b +
        NOT(xkeep)*YES(ykeep)*c +
        YES(xkeep)*YES(ykeep)*d;
    
    // change luminance gamma back to linear
    vec4 LQVresult = gammaLQVresult;
    LQVresult.w = pow(gammaLQVresult.w, gamma);
    
    // convert back to srgb; lqv -> lrgb -> srgb
    vec4 c1 = vpow(vec4(fromLQV(LQVresult), 1), 1/cheapsrgb);
    
    fragColor = c1;
}
