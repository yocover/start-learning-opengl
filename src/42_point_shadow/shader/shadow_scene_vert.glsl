#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

out vec2 TexCoords;

out VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float uvScale;
uniform int reverse_normal; // 反转法线, 让盒子内部计算光照

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vs_out.FragPos = vec3(model * vec4(position, 1.0));
  vs_out.Normal = transpose(inverse(mat3(model))) * (reverse_normal * normal);
  vs_out.TexCoords = texCoords * uvScale;
}