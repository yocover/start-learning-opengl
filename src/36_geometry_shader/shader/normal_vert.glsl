#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;

uniform mat4 model;
uniform mat4 view;

out VS_OUT {
  vec3 normal;
} vs_out;

void main() {
  mat3 normalMatrix = mat3(transpose(inverse(view * model)));
  vs_out.normal = normalize(vec3(vec4(normalMatrix * Normal, 0.0)));
  gl_Position = view * model * vec4(Position, 1.0f);
}