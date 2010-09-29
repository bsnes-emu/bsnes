<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">
  <fragment><![CDATA[
    uniform sampler2D rubyTexture;

    #define distortion 0.2

    vec2 barrelDistortion(vec2 coord) {
      vec2 cc = coord - 0.5;
      float dist = dot(cc, cc);
      return coord + cc * (dist + distortion * dist * dist) * distortion;
    }

    void main(void) {
      gl_FragColor = texture2D(rubyTexture, barrelDistortion(gl_TexCoord[0].xy));
    }
  ]]></fragment>
</shader>
