#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offsets[100];

out vec2 oTexCoord;

void main() {
  oTexCoord = TexCoords;
  vec3 offset = offsets[gl_InstanceID];
  gl_Position = projection * view * model * vec4(Position + offset, 1.0f);
}