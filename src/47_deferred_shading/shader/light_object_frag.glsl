#version 330 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} fs_in;

uniform vec3 lightColor;

void main() {
  FragColor = vec4(lightColor, 1.0);
}