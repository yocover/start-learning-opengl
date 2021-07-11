#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
out vec2 outTexCoord;

uniform float factor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outNormal;
out vec3 fragPosition;

void main() {

  gl_Position = projection * view * model * vec4(Position, 1.0f);
  fragPosition = vec3(model * vec4(Position, 1.0f));
  outTexCoord = TexCoords;
  outNormal = mat3(transpose(inverse(model))) * Normal;
}