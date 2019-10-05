#version 150

#define dt vec3(1.0, 1.0, 1.0)

uniform sampler2D source[];

in vec3 c00;
in vec3 c10;
in vec3 c20;
in vec3 c01;
in vec3 c11;
in vec3 c21;
in vec3 c02;
in vec3 c12;
in vec3 c22;

in float d1;
in float d2;
in float hl;
in float vl;

in float k1;
in float k2;

in Vertex {
  vec2 texCoord;
  vec4 t1;
  vec4 t2;
  vec4 t3;
  vec4 t4;
};

out vec4 fragColor;

void main(void) {
   vec3 c00 = texture(source[0], t1.zw).xyz; 
   vec3 c10 = texture(source[0], t3.xy).xyz;
   vec3 c20 = texture(source[0], t3.zw).xyz;
   vec3 c01 = texture(source[0], t1.xy).xyz;
   vec3 c11 = texture(source[0], texCoord).xyz;
   vec3 c21 = texture(source[0], t2.xy).xyz;
   vec3 c02 = texture(source[0], t2.zw).xyz;
   vec3 c12 = texture(source[0], t4.xy).xyz;
   vec3 c22 = texture(source[0], t4.zw).xyz;

   float d1=dot(abs(c00 - c22), dt) + 0.0001;
   float d2=dot(abs(c20 - c02), dt) + 0.0001;
   float hl=dot(abs(c01 - c21), dt) + 0.0001;
   float vl=dot(abs(c10 - c12), dt) + 0.0001;

   float k1 = 0.5 * (hl + vl);
   float k2 = 0.5 * (d1 + d2);

   vec3 t1 = (hl * (c10 + c12) + vl * (c01 + c21) + k1 * c11) / (2.5 * (hl + vl));
   vec3 t2 = (d1 * (c20 + c02) + d2 * (c00 + c22) + k2 * c11) / (2.5 * (d1 + d2));

   k1 = dot(abs(t1 - c11), dt) + 0.0001;
   k2 = dot(abs(t2 - c11), dt) + 0.0001;
   
  fragColor = vec4((k1 * t2 + k2 * t1) / (k1 + k2), 1.0);
}
