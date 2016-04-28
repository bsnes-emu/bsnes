uniform sampler2D image;
uniform sampler2D previousImage;
uniform bool uMixPrevious;

uniform vec2 uResolution;
const vec2 textureDimensions = vec2(160, 144);

{filter}

void main() {
    if (uMixPrevious) {
        gl_FragColor = mix(filter(image), filter(previousImage), 0.5);
    }
    else {
        gl_FragColor = filter(image);
    }
}