#version 150
uniform sampler2D image;
uniform sampler2D previousImage;
uniform bool uMixPrevious;

uniform vec2 uResolution;
const vec2 textureDimensions = vec2(160, 144);

out vec4 frag_color;

#line 1
{filter}

void main() {
    if (uMixPrevious) {
        frag_color = mix(scale(image), scale(previousImage), 0.5);
    }
    else {
        frag_color = scale(image);
    }
}