#version 150

// Bilinear (Precise)
//
// Resolution-independent smoothstep bilinear interpolation.
// Float32 linear-light color blending with exact piecewise
// sRGB transfer functions and RGBA8 output.

uniform sampler2D source[];
uniform vec4 sourceSize[];
uniform vec4 targetSize;

in Vertex {
    vec2 texCoord;
};

out vec4 fragColor;

// -----------------------------------------------------------------------------
// Sharpness configuration
// -----------------------------------------------------------------------------

// Gain > 1.0 = sharper.
const float gain = 1.25;

// -----------------------------------------------------------------------------
// Interpolation helper
// -----------------------------------------------------------------------------

// Cubic smoothstep, equivalent to the C++ implementation.
vec2 smoothstep01(vec2 x)
{
    x = clamp(x, 0.0, 1.0);
    return x * x * (3.0 - 2.0 * x);
}

// -----------------------------------------------------------------------------
// sRGB transfer functions
// -----------------------------------------------------------------------------

// Decode normalized sRGB into Float32 linear light.
vec3 srgb_to_linear(vec3 encoded)
{
    vec3 low = encoded / 12.92;

    vec3 high = pow(
        (encoded + 0.055) / 1.055,
        vec3(2.4)
    );

    return mix(
        high,
        low,
        lessThanEqual(encoded, vec3(0.04045))
    );
}

// Encode Float32 linear light into normalized sRGB.
vec3 linear_to_srgb(vec3 linear)
{
    linear = clamp(linear, 0.0, 1.0);

    vec3 low = 12.92 * linear;

    vec3 high = 1.055 * pow(
        linear,
        vec3(1.0 / 2.4)
    ) - 0.055;

    return mix(
        high,
        low,
        lessThanEqual(linear, vec3(0.0031308))
    );
}

// -----------------------------------------------------------------------------
// Main resolution-independent filter
// -----------------------------------------------------------------------------

void main()
{
    // Original source image dimensions.
    vec2 size = sourceSize[0].xy;

    // Centered pixel-coordinate mapping.
    // Works with arbitrary horizontal and vertical scaling ratios.
    vec2 position = texCoord * size - vec2(0.5);

    // Integer source coordinates and fractional positions.
    vec2 base = floor(position);
    vec2 fraction = fract(position);

    // Apply the same gain-adjusted interpolation curve
    // as in the original C++ filter.
    vec2 adjusted = clamp(
        vec2(0.5) + (fraction - vec2(0.5)) * gain,
        0.0,
        1.0
    );

    // Float32 smoothstep interpolation weights.
    vec2 weight = smoothstep01(adjusted);

    // -------------------------------------------------------------------------
    // Source pixel coordinates
    // -------------------------------------------------------------------------

    ivec2 origin = ivec2(base);
    ivec2 minimum = ivec2(0);
    ivec2 maximum = ivec2(size) - ivec2(1);

    // Clamp each neighbor independently.
    // This extends the outermost source pixels.
    ivec2 p00 = clamp(
        origin + ivec2(0, 0), minimum, maximum
    );

    ivec2 p10 = clamp(
        origin + ivec2(1, 0), minimum, maximum
    );

    ivec2 p01 = clamp(
        origin + ivec2(0, 1), minimum, maximum
    );

    ivec2 p11 = clamp(
        origin + ivec2(1, 1), minimum, maximum
    );

    // -------------------------------------------------------------------------
    // Fetch four original source pixels and decode into linear light
    // -------------------------------------------------------------------------

    vec3 c00 = srgb_to_linear(
        texelFetch(source[0], p00, 0).rgb
    );

    vec3 c10 = srgb_to_linear(
        texelFetch(source[0], p10, 0).rgb
    );

    vec3 c01 = srgb_to_linear(
        texelFetch(source[0], p01, 0).rgb
    );

    vec3 c11 = srgb_to_linear(
        texelFetch(source[0], p11, 0).rgb
    );

    // -------------------------------------------------------------------------
    // Float32 linear-light interpolation
    // -------------------------------------------------------------------------

    // Horizontal blends.
    vec3 top = mix(c00, c10, weight.x);
    vec3 bottom = mix(c01, c11, weight.x);

    // Vertical blend.
    vec3 linear = mix(top, bottom, weight.y);

    // -------------------------------------------------------------------------
    // Direct sRGB encoding and RGBA output
    // -------------------------------------------------------------------------

    vec3 encoded = linear_to_srgb(linear);

    fragColor = vec4(encoded, 1.0);
}