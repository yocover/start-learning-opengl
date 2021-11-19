#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;
uniform sampler2D diffuseTexture;

void main() {
  FragColor = vec4(texture(diffuseTexture, oTexCoord).rgb, 1.0);
}