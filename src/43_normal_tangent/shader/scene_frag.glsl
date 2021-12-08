#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec2 TexCoords;
  vec3 TangentLightPos;
  vec3 TangentViewPos;
  vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap; // 贴图
uniform sampler2D normalMap; // 贴图
uniform float strength;

void main() {

  float gamma = 2.2;
  vec3 color = pow(texture(diffuseMap, fs_in.TexCoords).rgb, vec3(gamma));

  vec3 ambient = strength * color; // 环境光

  // vec3 normal = normalize(oNormal);
  // 从法线贴图获取[0,1]范围的法线值
  vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
  // 将法线向量转换到[-1，1]范围
  normal = normalize(normal * 2.0 - 1.0);

  vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color; // 漫反射

  vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

  // vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

  vec3 specular = vec3(0.3) * spec; // 镜面光
  vec3 result = (ambient + diffuse + specular);

  FragColor = vec4(result, 1.0);
  FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));

}