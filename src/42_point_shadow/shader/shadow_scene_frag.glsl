#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[] (vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

float ShadowCalculation(vec3 fragPos) {
  // 获取片段指向灯光位置的向量
  vec3 fragToLight = fragPos - lightPos;
  // 从深度贴图中取样  
  float closestDepth = texture(depthMap, fragToLight).r;
  // 将其转换至0到far_plane范围内
  closestDepth *= far_plane;
  // 获取当前片元和光源之间的深度值
  float currentDepth = length(fragToLight);
  // 避免阴影失真
  // float bias = 0.05; 
  // float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

  // PCF 算法1 采样过多
  // float shadow = 0.0;
  // float bias = 0.05;
  // float samples = 4.0;
  // float offset = 0.1;
  // for(float x = -offset; x < offset; x += offset / (samples * 0.5)) {
  //   for(float y = -offset; y < offset; y += offset / (samples * 0.5)) {
  //     for(float z = -offset; z < offset; z += offset / (samples * 0.5)) {
  //       float closestDepth = texture(depthMap, fragToLight + vec3(x, y, z)).r;
  //       closestDepth *= far_plane;   // Undo mapping [0;1]
  //       if(currentDepth - bias > closestDepth)
  //         shadow += 1.0;
  //     }
  //   }
  // }
  // shadow /= (samples * samples * samples);

  // PCF 算法2
  float shadow = 0.0;
  float bias = 0.15;
  int samples = 20;
  float viewDistance = length(viewPos - fragPos);
  float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
  for(int i = 0; i < samples; ++i) {
    float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
    closestDepth *= far_plane;   // Undo mapping [0;1]
    if(currentDepth - bias > closestDepth)
      shadow += 1.0;
  }
  shadow /= float(samples);

  return shadow;
}

void main() {
  vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
  vec3 normal = normalize(fs_in.Normal);
  vec3 lightColor = vec3(0.3);
  // Ambient
  vec3 ambient = 0.4 * color;
  // Diffuse
  vec3 lightDir = normalize(lightPos - fs_in.FragPos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * lightColor;
  // Specular
  vec3 viewDir = normalize(viewPos - fs_in.FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = 0.0;
  vec3 halfwayDir = normalize(lightDir + viewDir);
  spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
  vec3 specular = spec * lightColor;    
  // Calculate shadow
  float shadow = ShadowCalculation(fs_in.FragPos);
  vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

  FragColor = vec4(lighting, 1.0f);
}