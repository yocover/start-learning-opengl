#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

void main() {

  FragColor = vec4(vec3(0.2, 0.5, 0.5), 1.0);
}