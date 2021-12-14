#version 330 core
layout(location = 0) out vec4 gPositionDepth;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
  vec3 FragPos;
  vec3 Normal;
  vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

const float NEAR = 0.1; // 投影矩阵的近平面
const float FAR = 100.0f; // 投影矩阵的远平面
float LinearizeDepth(float depth) {
  float z = depth * 2.0 - 1.0; // 回到NDC
  return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main() {
  gPositionDepth.xyz = fs_in.FragPos;
  // 存储线性深度
  gPositionDepth.a = LinearizeDepth(gl_FragCoord.z);

  gNormal = normalize(fs_in.Normal);
  gAlbedoSpec.rgb = texture(texture_diffuse1, fs_in.TexCoords).rgb;
  gAlbedoSpec.a = texture(texture_specular1, fs_in.TexCoords).r;
}