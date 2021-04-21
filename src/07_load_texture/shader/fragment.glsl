#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float iTime;

void main() {
  FragColor = mix(texture(ourTexture1, TexCoord * 2.0f - 0.5f), texture(ourTexture2, TexCoord), abs(sin(iTime * 0.1)));
}