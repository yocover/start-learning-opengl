#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; // 相机位置
uniform float ambientStrength;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {

  vec4 objectColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);
  //vec4 objColor = vec4(1.0f, 0.5f, 0.31f, 1.0f);

  vec3 ambient = ambientStrength * lightColor; // 环境光

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor; // 漫反射

  float specularStrength = 0.9;
  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
  vec3 specular = specularStrength * spec * lightColor; // 镜面光

  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}