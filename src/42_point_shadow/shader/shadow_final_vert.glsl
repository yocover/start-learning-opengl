#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
  vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform float uvScale;

void main() {
  vs_out.FragPos = vec3(model * vec4(Position, 1.0));
  vs_out.Normal = transpose(inverse(mat3(model))) * Normal;
  vs_out.TexCoords = TexCoords * uvScale;
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
  gl_Position = projection * view * model * vec4(Position, 1.0);
}