#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 ourPos;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

/**
  * |  cos0  -sin0   0   0|    |x|    |x * cos0 - y * sin0|
  * |  sin0   cos0   0   0|  * |y| =  |x * sin0 + y * cos0|
  * |  0       0     1   0|    |z|    |         z         |
  * |  0       0     0   1|    |w|    |         1         |
  * 可以用矩阵来旋转坐标系统
  */
mat4 rotate3d(float _angle) {
  return mat4(cos(_angle), -sin(_angle), 0, 0 sin(_angle), cos(_angle), 0, 0 0, 0, 1, 0, 0, 0, 0, 1);
}

uniform float factor;

void main() {
  // FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

  float xy = length(ourPos.xy);
  FragColor = rotate3d(45.0) * mix(texture(texture1, TexCoord), vec4(ourColor, 1.0 - xy * 2.0) * texture(texture2, TexCoord), 0.1);
}