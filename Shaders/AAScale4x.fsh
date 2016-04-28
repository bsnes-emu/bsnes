vec4 scale2x(sampler2D image, vec2 texCoord)
{
    // o = offset, the width of a pixel
    vec2 o = 1.0 / textureDimensions;
    // texel arrangement
    // A B C
    // D E F
    // G H I
    vec4 A = texture2D(image, texCoord + vec2( -o.x,  o.y));
    vec4 B = texture2D(image, texCoord + vec2(    0,  o.y));
    vec4 C = texture2D(image, texCoord + vec2(  o.x,  o.y));
    vec4 D = texture2D(image, texCoord + vec2( -o.x,    0));
    vec4 E = texture2D(image, texCoord + vec2(    0,    0));
    vec4 F = texture2D(image, texCoord + vec2(  o.x,    0));
    vec4 G = texture2D(image, texCoord + vec2( -o.x, -o.y));
    vec4 H = texture2D(image, texCoord + vec2(    0, -o.y));
    vec4 I = texture2D(image, texCoord + vec2(  o.x, -o.y));
    vec2 p = texCoord * textureDimensions;
    // p = the position within a pixel [0...1]
    p = fract(p);
    if (p.x > .5) {
        if (p.y > .5) {
            // Top Right
            return B == F && B != D && F != H ? F : E;
        } else {
            // Bottom Right
            return H == F && D != H && B != F ? F : E;
        }
    } else {
        if (p.y > .5) {
            // Top Left
            return D == B && B != F && D != H ? D : E;
        } else {
            // Bottom Left
            return D == H && D != B && H != F ? D : E;
        }
    }
}

vec4 aaScale2x(sampler2D image, vec2 texCoord)
{
    return mix(texture2D(image, texCoord), scale2x(image, texCoord), 0.5);
}

vec4 filter(sampler2D image)
{
    // o = offset, the width of a pixel
    vec2 o = 1.0 / (textureDimensions * 2.);
    vec2 texCoord = vec2(gl_FragCoord.x, uResolution.y - gl_FragCoord.y) / uResolution;

    // texel arrangement
    // A B C
    // D E F
    // G H I
    vec4 A = aaScale2x(image, texCoord + vec2( -o.x,  o.y));
    vec4 B = aaScale2x(image, texCoord + vec2(    0,  o.y));
    vec4 C = aaScale2x(image, texCoord + vec2(  o.x,  o.y));
    vec4 D = aaScale2x(image, texCoord + vec2( -o.x,    0));
    vec4 E = aaScale2x(image, texCoord + vec2(    0,    0));
    vec4 F = aaScale2x(image, texCoord + vec2(  o.x,    0));
    vec4 G = aaScale2x(image, texCoord + vec2( -o.x, -o.y));
    vec4 H = aaScale2x(image, texCoord + vec2(    0, -o.y));
    vec4 I = aaScale2x(image, texCoord + vec2(  o.x, -o.y));
    vec4 R;
    vec2 p = texCoord * textureDimensions * 2.;
    // p = the position within a pixel [0...1]
    p = fract(p);
    if (p.x > .5) {
        if (p.y > .5) {
            // Top Right
            R = B == F && B != D && F != H ? F : E;
        } else {
            // Bottom Right
            R = H == F && D != H && B != F ? F : E;
        }
    } else {
        if (p.y > .5) {
            // Top Left
            R = D == B && B != F && D != H ? D : E;
        } else {
            // Bottom Left
            R = D == H && D != B && H != F ? D : E;
        }
    }

    return mix(R, E, 0.5);
}