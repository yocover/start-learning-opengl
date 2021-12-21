#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 worldPos;
uniform samplerCube envMap;

void main() {

  vec3 envColor = texture(envMap, worldPos).rgb;

  // HDR色调映射和校正
  envColor = envColor / (envColor + vec3(1.0));
  envColor = pow(envColor, vec3(1.0 / 2.2));

  FragColor = vec4(envColor, 1.0);
}