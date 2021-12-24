#version 330 core
layout(location = 0) out vec4 FragColor;

in vec2 outTexCoord;

uniform sampler2D brdfTexture;

void main() {

  vec3 brdf = texture(brdfTexture, outTexCoord).rgb;
  FragColor = vec4(vec3(brdf.rgb), 1.0);
}