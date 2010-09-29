<?xml version="1.0" encoding="UTF-8"?>
<shader language="GLSL">
  <vertex><![CDATA[
    void main(void) {
      gl_Position = ftransform();
      gl_TexCoord[0] = gl_MultiTexCoord0;
    }
  ]]></vertex>

  <fragment><![CDATA[
    uniform sampler2D rubyTexture;

    void main(void) {   
      vec4 rgb = texture2D(rubyTexture, gl_TexCoord[0].xy);
      vec4 intens = smoothstep(0.2,0.8,rgb) + normalize(vec4(rgb.xyz, 1.0));

      if(fract(gl_FragCoord.y * 0.5) > 0.5) intens = rgb * 0.8;
      gl_FragColor = intens;
    }
  ]]></fragment>
</shader>
