#version 150
uniform sampler2D image;
uniform sampler2D previousImage;
uniform bool uMixPrevious;

uniform vec2 uResolution;
uniform vec2 uOrigin;
const vec2 textureDimensions = vec2(160, 144);

out vec4 frag_color;

vec4 modified_frag_cord;
#define gl_FragCoord modified_frag_cord
#line 1
{filter}
#undef gl_FragCoord

void main() {
    modified_frag_cord = gl_FragCoord - vec4(uOrigin.x, uOrigin.y, 0, 0);
    
    if (uMixPrevious) {
        frag_color = mix(scale(image), scale(previousImage), 0.5);
    }
    else {
        frag_color = scale(image);
    }
}