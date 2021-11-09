#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 viewPos; // 相机位置

// 材质
struct Material {
  sampler2D diffuse; // 漫反射贴图
  sampler2D specular; // 镜面光贴图
  float shininess; // 高光指数
};
uniform Material material;

// 光源
struct Light {
  vec3 position; // 光源位置

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant; // 常数项
  float linear; // 一次项
  float quadratic; // 二次项
};
uniform Light light;

uniform float factor;

void main() {

  // 计算衰减值
  float distance = length(light.position - outFragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

  vec4 objectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  vec3 diffuseTexture = vec3(texture(material.diffuse, outTexCoord));
  vec3 specularTexture = vec3(texture(material.specular, outTexCoord));

  vec3 ambient = light.ambient * diffuseTexture; // 环境光

  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(light.position - outFragPos);
  // vec3 lightDir = normalize(light.position);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff * diffuseTexture; // 漫反射

  vec3 viewDir = normalize(viewPos - outFragPos);
  vec3 reflectDir = reflect(-lightDir, norm);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = light.specular * spec * specularTexture; // 镜面光

  // 将环境光、漫反射、镜面光分别乘以衰减距离
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  vec3 result = (ambient + diffuse + specular) * vec3(objectColor);

  FragColor = vec4(result, 1.0);
}