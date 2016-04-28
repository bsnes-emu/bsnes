vec4 filter(sampler2D image)
{
    vec2 texCoord = vec2(gl_FragCoord.x, uResolution.y - gl_FragCoord.y) / uResolution;

    vec2 pixel = texCoord * textureDimensions;

    vec4 q11 = texture2D(image, vec2(floor(pixel.x) / textureDimensions.x, floor(pixel.y) / textureDimensions.y));
    vec4 q12 = texture2D(image, vec2(floor(pixel.x) / textureDimensions.x, ceil(pixel.y) / textureDimensions.y));
    vec4 q21 = texture2D(image, vec2(ceil(pixel.x) / textureDimensions.x, floor(pixel.y) / textureDimensions.y));
    vec4 q22 = texture2D(image, vec2(ceil(pixel.x) / textureDimensions.x, ceil(pixel.y) / textureDimensions.y));

    vec4 r1 = mix(q11, q21, fract(pixel.x));
    vec4 r2 = mix(q12, q22, fract(pixel.x));

    return mix (r1, r2, fract(pixel.y));
}