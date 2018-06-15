vec4 scale2x(sampler2D image, vec2 position)
{
    // o = offset, the width of a pixel
    vec2 o = 1.0 / input_resolution;
    // texel arrangement
    // A B C
    // D E F
    // G H I
    vec4 A = texture(image, position + vec2( -o.x,  o.y));
    vec4 B = texture(image, position + vec2(    0,  o.y));
    vec4 C = texture(image, position + vec2(  o.x,  o.y));
    vec4 D = texture(image, position + vec2( -o.x,    0));
    vec4 E = texture(image, position + vec2(    0,    0));
    vec4 F = texture(image, position + vec2(  o.x,    0));
    vec4 G = texture(image, position + vec2( -o.x, -o.y));
    vec4 H = texture(image, position + vec2(    0, -o.y));
    vec4 I = texture(image, position + vec2(  o.x, -o.y));
    vec2 p = position * input_resolution;
    // p = the position within a pixel [0...1]
    vec4 R;
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

vec4 scale(sampler2D image, vec2 position)
{
    // o = offset, the width of a pixel
    vec2 o = 1.0 / (input_resolution * 2.);
    
    // texel arrangement
    // A B C
    // D E F
    // G H I
    vec4 A = scale2x(image, position + vec2( -o.x,  o.y));
    vec4 B = scale2x(image, position + vec2(    0,  o.y));
    vec4 C = scale2x(image, position + vec2(  o.x,  o.y));
    vec4 D = scale2x(image, position + vec2( -o.x,    0));
    vec4 E = scale2x(image, position + vec2(    0,    0));
    vec4 F = scale2x(image, position + vec2(  o.x,    0));
    vec4 G = scale2x(image, position + vec2( -o.x, -o.y));
    vec4 H = scale2x(image, position + vec2(    0, -o.y));
    vec4 I = scale2x(image, position + vec2(  o.x, -o.y));
    vec2 p = position * input_resolution * 2.;
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
