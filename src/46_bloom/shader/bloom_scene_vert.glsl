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

uniform float uvScale;

void main() {

  vs_out.FragPos = vec3(model * vec4(Position, 1.0));
  vs_out.TexCoords = TexCoords * uvScale;

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vs_out.Normal = normalize(normalMatrix * Normal);

  gl_Position = projection * view * model * vec4(Position, 1.0);

}