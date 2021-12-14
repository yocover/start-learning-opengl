#version 330 core
out float FragColor;

in vec2 oTexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

int kernelSize = 64;
float radius = 1.0;

const vec2 noiseScale = vec2(800.0f / 4.0f, 600.0f / 4.0f);

uniform mat4 projection;

void main() {
  vec3 fragPos = texture(gPositionDepth, oTexCoords).xyz;
  vec3 normal = texture(gNormal, oTexCoords).rgb;
  vec3 randomVec = texture(texNoise, oTexCoords * noiseScale).xyz;

  // 创建TBN改变基矩阵，从切线空间到视图空间
  vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);

  // 迭代样本，并计算遮挡因子
  float occlusion = 0.0;
  for(int i = 0; i < kernelSize; ++i) {

    vec3 sample = TBN * samples[i]; 
    sample = fragPos + sample * radius; 

    vec4 offset = vec4(sample, 1.0);
    offset = projection * offset;
    offset.xyz /= offset.w;
    offset.xyz = offset.xyz * 0.5 + 0.5;

    float sampleDepth = -texture(gPositionDepth, offset.xy).w; 

    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
    occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;
  }
  occlusion = 1.0 - (occlusion / kernelSize);

  FragColor = occlusion;
}