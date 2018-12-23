#version 410

uniform vec3 vColor;  // ambient and diffuse color
uniform vec3 vSColor; // specular color

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 modelView;

uniform vec4 lightPos;

in vec3 vPosition;
in vec2 vTexture;
in vec3 vNormal;

out vec4 color;

void main() {
  vec3 ambient, diffuse, specular;
  vec4 pos = vec4(vPosition, 1.);
  vec3 N = normalize(normalMatrix * vNormal);
  vec3 L = normalize((camera * lightPos).xyz - (modelView * pos).xyz);
  vec3 E = -normalize((modelView * pos).xyz); // from pt to viewer
  vec3 H = normalize(L + E);
  float Kd = max(dot(L, N), 0.0);
  float Ks = pow(max(dot(N, H), 0.0), 80);
  ambient = vColor * 0.2;
  diffuse = vColor * Kd * 0.5;
  specular = vSColor * Ks * 0.3;

  gl_Position = projection * modelView * pos;
  color = vec4((ambient + diffuse + specular).xyz, 1.0);
  // color = vec4(vColor,1.0);
}
