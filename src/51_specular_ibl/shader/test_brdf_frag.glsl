#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 outTexCoord;

uniform vec3 lightColor;
uniform sampler2D brdfTexture;

void main() {

  vec2 brdf = texture(brdfTexture, outTexCoord).rg;
  FragColor = vec4(vec3(brdf.r, 0.0, 0.0), 1.0);
}