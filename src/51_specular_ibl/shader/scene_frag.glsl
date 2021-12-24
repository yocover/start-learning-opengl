#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
  return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------
void main() {
  vec3 N = Normal;
  vec3 V = normalize(camPos - WorldPos);
  vec3 R = reflect(-V, N); 

  // 计算法向入射的反射率；如果介电（如塑料）使用 F0 0.04，如果它是金属，使用反照率颜色作为 F0（金属工作流程）  
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);

  // 反射方程
  vec3 Lo = vec3(0.0);
  for(int i = 0; i < 4; ++i) {
    // 计算每个光源的辐射
    vec3 L = normalize(lightPositions[i] - WorldPos);
    vec3 H = normalize(V + L);
    float distance = length(lightPositions[i] - WorldPos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColors[i] * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS 等于菲涅尔
    vec3 kS = F;

    // 为了保持能量守恒，漫反射和镜面反射不能高于1.0（除非自发光表面）
    // 所以KD（漫反射分量）等于1.0 - Ks
    vec3 kD = vec3(1.0) - kS;

    // 这样只有非金属才有漫反射，或者部分与金属的混合材质，纯金属没有漫反射
    kD *= 1.0 - metallic;	                

    // 通过NdotL缩放
    float NdotL = max(dot(N, L), 0.0);

    Lo += (kD * albedo / PI + specular) * radiance * NdotL; // 因为已经将BRDF乘以菲涅尔KS，因此不需要再乘以KS
  }   

  // IBL diffuse部分
  vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

  vec3 kS = F;
  vec3 kD = 1.0 - kS;
  kD *= 1.0 - metallic;

  vec3 irradiance = texture(irradianceMap, N).rgb;
  vec3 diffuse = irradiance * albedo;

  // 对预过滤贴图和BRDF进行采样，并根据Split-Sum近似将他们组合一起 获取IBL镜面反射部分
  const float MAX_REFLECTION_LOD = 4.0;
  vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
  vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
  vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

  vec3 ambient = (kD * diffuse + specular) * ao;

  vec3 color = ambient + Lo;

  // HDR 色调映射
  color = color / (color + vec3(1.0));
  // gamma 校正
  color = pow(color, vec3(1.0 / 2.2));

  FragColor = vec4(color, 1.0);
}