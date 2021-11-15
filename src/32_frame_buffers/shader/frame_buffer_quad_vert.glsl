#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
out vec2 outTexCoord;

void main() {
  gl_Position = vec4(Position.x, Position.y, 0.0f, 1.0f);
  outTexCoord = TexCoords;
}