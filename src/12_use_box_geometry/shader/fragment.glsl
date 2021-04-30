#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;
in float stp;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  FragColor = mix(texture(texture1, outTexCoord), texture(texture2, outTexCoord), 0.1);

  // vec2 coord = gl_PointCoord - vec2(0.5);
  // float r = float(length(coord) > 0.5);
  // FragColor = vec4(0.0, stp * 0.5, stp, (1 - r) * stp);// 蓝色过度色
}