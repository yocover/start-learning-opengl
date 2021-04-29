#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  // FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  // FragColor = vec4(1.0, 0.6, 0.1, 0.3);
  FragColor = vec4(0.32, 0.67, 0.9, 0.3);
}