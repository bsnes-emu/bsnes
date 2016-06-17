vec4 scale(sampler2D image)
{
    vec2 texCoord = vec2(gl_FragCoord.x, uResolution.y - gl_FragCoord.y) / uResolution;

    vec2 pixel = texCoord * textureDimensions - vec2(0.5, 0.5);

    vec4 q11 = texture(image, vec2(floor(pixel.x) / textureDimensions.x, floor(pixel.y) / textureDimensions.y));
    vec4 q12 = texture(image, vec2(floor(pixel.x) / textureDimensions.x, ceil(pixel.y) / textureDimensions.y));
    vec4 q21 = texture(image, vec2(ceil(pixel.x) / textureDimensions.x, floor(pixel.y) / textureDimensions.y));
    vec4 q22 = texture(image, vec2(ceil(pixel.x) / textureDimensions.x, ceil(pixel.y) / textureDimensions.y));

    vec2 s = smoothstep(0., 1., fract(pixel));

    vec4 r1 = mix(q11, q21, fract(s.x));
    vec4 r2 = mix(q12, q22, fract(s.x));

    return mix (r1, r2, fract(s.y));
}