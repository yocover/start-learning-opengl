#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
uniform vec3 lightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  // FragColor = vec4(vec3(1.0f, 0.5f, 0.31f) * lightColor, 1.0);
}