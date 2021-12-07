#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec2 oTexCoord;
out vec3 oNormal;
out vec3 oFragPos;

uniform float factor;

uniform float uvScale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

  gl_Position = projection * view * model * vec4(Position, 1.0f);

  oFragPos = vec3(model * vec4(Position, 1.0));

  oTexCoord = TexCoords * uvScale;

  // 解决不等比缩放，对法向量产生的影响
  oNormal = mat3(transpose(inverse(model))) * Normal;
}