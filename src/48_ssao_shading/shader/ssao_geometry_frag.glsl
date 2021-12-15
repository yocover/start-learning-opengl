#version 330 core
layout(location = 0) out vec3 aPosition;
layout(location = 1) out vec3 aNormal;
layout(location = 2) out vec3 aAlbedo;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

void main() {
  // 将片段位置向量存储在第一个gbuffer纹理中
  aPosition = FragPos;
  // 将每个片段法线存储到gbuffer中
  aNormal = normalize(Normal);
  // 灰度颜色
  aAlbedo.rgb = vec3(1.0);
}