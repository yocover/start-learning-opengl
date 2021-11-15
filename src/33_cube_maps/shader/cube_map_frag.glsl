#version 330 core
out vec4 FragColor;
in vec3 outTexCoord;

uniform samplerCube skyboxTexture;

void main() {
  FragColor = texture(skyboxTexture, outTexCoord);
}