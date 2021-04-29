#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec3 outPosition;
out vec2 outTexCoord;

uniform float factor;

/**
  * |  cos0  -sin0   0   0|    |x|    |x * cos0 - y * sin0|
  * |  sin0   cos0   0   0|  * |y| =  |x * sin0 + y * cos0|
  * |  0       0     1   0|    |z|    |         z         |
  * |  0       0     0   1|    |w|    |         1         |
  * 可以用矩阵来旋转坐标系统
  */
mat4 rotate3d(float _angle) {
  return mat4(cos(_angle), -sin(_angle), 0.0f, 0.0f, sin(_angle), cos(_angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

void main() {
  gl_Position = vec4(rotate3d(factor) * vec4(Position, 1.0f));
  gl_PointSize = 10.0f;
  outTexCoord = TexCoords;
}