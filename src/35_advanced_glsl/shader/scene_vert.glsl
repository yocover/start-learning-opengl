#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

layout(std140) uniform Matrices {
  mat4 projection;
  mat4 view;
};

uniform mat4 model;

out VS_OUT {
  vec2 TexCoord;
} vs_out;

void main() {
  vs_out.TexCoord = TexCoords;
  gl_Position = projection * view * model * vec4(Position, 1.0f);
}