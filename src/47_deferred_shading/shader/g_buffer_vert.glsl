#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

  gl_Position = projection * view * model * vec4(Position, 1.0f);

  vs_out.FragPos = vec3(model * vec4(Position, 1.0));

  vs_out.TexCoords = TexCoords;
  // 解决不等比缩放，对法向量产生的影响
  vs_out.Normal = mat3(transpose(inverse(model))) * Normal;
}