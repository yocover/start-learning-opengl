#version 330 core
out vec4 FragColor;

// 点光源
struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define NR_POINT_LIGHTS 12

uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D gPosition; // 贴图
uniform sampler2D gNormal; // 贴图
uniform sampler2D gAlbedoSpec; // 贴图

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} fs_in;

uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {

  vec3 FragPos = texture(gPosition, fs_in.TexCoords).rgb;
  vec3 Normal = texture(gNormal, fs_in.TexCoords).rgb;
  vec3 Diffuse = texture(gAlbedoSpec, fs_in.TexCoords).rgb;
  float Specular = texture(gAlbedoSpec, fs_in.TexCoords).a;

  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result = vec3(0.0f);
  // 点光源
  for(int i = 0; i < NR_POINT_LIGHTS; i++) {
    result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);
  }
  FragColor = vec4(result, 1.0);
}

// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
  float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // 衰减
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));    
    // 合并结果
  vec3 ambient = light.ambient;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}