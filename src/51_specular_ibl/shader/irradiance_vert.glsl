#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec3 worldPos;

uniform mat4 view;
uniform mat4 projection;

void main() {

  worldPos = Position;
  gl_Position = projection * view * vec4(worldPos, 1.0f);
}