#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

layout(location = 3) in mat4 instanceMatrix;

uniform mat4 view;
uniform mat4 projection;

out vec2 oTexCoord;

void main() {
  oTexCoord = TexCoords;
  gl_Position = projection * view * instanceMatrix * vec4(Position, 1.0f);
}