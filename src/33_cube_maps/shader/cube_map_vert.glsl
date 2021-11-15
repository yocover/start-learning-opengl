#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
out vec3 outTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  vec4 pos = projection * view * vec4(Position, 1.0f);
  outTexCoord = Position;
  gl_Position = pos;
}