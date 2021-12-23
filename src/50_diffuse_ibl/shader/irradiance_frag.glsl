#version 330 core
layout(location = 0) out vec4 FragColor;

in vec3 worldPos;
uniform samplerCube envMap;

const float PI = 3.14159265359;
void main() {

  vec3 N = normalize(worldPos);

  vec3 irradiance = vec3(0.0);

  // 从原点计算切线空间
  vec3 up = vec3(0.0, 1.0, 0.0);
  vec3 right = normalize(cross(up, N));
  up = normalize(cross(N, right));

  float sampleDelta = 0.025;
  float nrSamples = 0.0;
  for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {

      // 笛卡尔球面坐标 (切线空间中)
      vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
      // 切线空间转换到世界空间
      vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

      irradiance += texture(envMap, sampleVec).rgb * cos(theta) * sin(theta);
      nrSamples++;
    }
  }
  irradiance = PI * irradiance * (1.0 / float(nrSamples));

  FragColor = vec4(irradiance, 1.0);

}