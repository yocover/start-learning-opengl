#version 330 core
out vec4 FragColor;

uniform vec3 lightColor; // 光照颜色
uniform vec3 lightPosition; // 光源位置
uniform vec3 viewPosition; // 视线位置

uniform float ambientStrength; // 环境光强度
uniform float specularStrength = 0.5; // 镜面光强度

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float iTime;

in vec2 outTexCoord; // 纹理坐标
in vec3 outNormal; // 法向量
out vec3 fragPosition;

void main() {
  // FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);

  vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);

  vec3 ambient = ambientStrength * lightColor;

  vec3 lightPos = vec3(sin(iTime) * lightPosition.x, lightPosition.y, lightPosition.z);

  vec3 normal = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - fragPosition);

  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 viewDir = normalize(viewPosition - fragPosition);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;

  FragColor = vec4(result, 1.0);
}