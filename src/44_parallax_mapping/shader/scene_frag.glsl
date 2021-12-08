#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec2 TexCoords;
  vec3 TangentLightPos;
  vec3 TangentViewPos;
  vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap; // 漫反射贴图
uniform sampler2D normalMap;  // 法线贴图
uniform sampler2D depthMap;   // 高度贴图

uniform float strength;
uniform bool parallax;
uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
  // number of depth layers
  const float minLayers = 10;
  const float maxLayers = 20;
  float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
  float layerDepth = 1.0 / numLayers;
    // depth of current layer
  float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
  vec2 P = viewDir.xy / viewDir.z * height_scale;
  vec2 deltaTexCoords = P / numLayers;

    // get initial values
  vec2 currentTexCoords = texCoords;
  float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

  while(currentLayerDepth < currentDepthMapValue) {
        // shift texture coordinates along direction of P
    currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
    currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // get depth of next layer
    currentLayerDepth += layerDepth;
  }

    // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
  vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
  float afterDepth = currentDepthMapValue - currentLayerDepth;
  float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
  float weight = afterDepth / (afterDepth - beforeDepth);
  vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

  return finalTexCoords;
}

void main() {

  vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

  vec2 texCoords = fs_in.TexCoords;

  if(parallax) {
    texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
      discard;
  }

  float gamma = 2.2;
  vec3 color = pow(texture(diffuseMap, texCoords).rgb, vec3(gamma));

  vec3 ambient = strength * color; // 环境光

  // vec3 normal = normalize(oNormal);
  // 从法线贴图获取[0,1]范围的法线值
  vec3 normal = texture(normalMap, texCoords).rgb;
  // 将法线向量转换到[-1，1]范围
  normal = normalize(normal * 2.0 - 1.0);

  vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color; // 漫反射

  // vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);

  vec3 specular = vec3(0.3) * spec; // 镜面光
  vec3 result = (ambient + diffuse + specular);

  FragColor = vec4(result, 1.0);
  FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));

}