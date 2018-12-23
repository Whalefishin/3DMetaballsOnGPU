#version 410

uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;

uniform vec4 lightPos;

in vec3 vPosition;
in vec2 vTexture;
in vec3 vNormal;

out vec3 N;
out vec3 L;
out vec3 E;

void main() {
  vec4 pos = vec4(vPosition, 1.);
  N = normalMatrix * vNormal;
  L = (camera * lightPos).xyz - (modelView * pos).xyz;
  E = -(modelView * pos).xyz; // from pt to viewer

  gl_Position = projection * modelView * pos;
}
