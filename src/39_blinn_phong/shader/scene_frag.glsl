#version 330 core
out vec4 FragColor;

uniform sampler2D woodMap; // 贴图

in vec2 outTexCoord;
in vec3 outNormal;
in vec3 outFragPos;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float strength;

uniform bool blinn;

void main() {

  vec3 color = texture(woodMap, outTexCoord).rgb;

  vec3 ambient = strength * color; // 环境光

  vec3 normal = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - outFragPos);

  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diff * color; // 漫反射

  vec3 viewDir = normalize(viewPos - outFragPos);

  float spec = 0.0;
  if(blinn) {
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);

  } else {
    vec3 reflectDir = reflect(-lightDir, normal);
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
  }

  vec3 specular = vec3(0.3) * spec; // 镜面光
  vec3 result = (ambient + diffuse + specular);

  FragColor = vec4(result, 1.0);
}