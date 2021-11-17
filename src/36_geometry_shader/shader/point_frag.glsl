#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec2 TexCoord;
} vs_in;

in vec2 fColor;

uniform sampler2D uvMap;
uniform sampler2D triMap;

void main() {

  FragColor = vec4(vec3(fColor, 0.5), 1.0);
}