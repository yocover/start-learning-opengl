#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec3 outPosition;
out vec2 outTexCoord;

uniform float factor;

/**
  * |  1   0      0     0|    |x|    |         x         |
  * |  0   cos0  -sin0  0|  * |y| =  |cos0 * y - sin0 * z|
  * |  0   sin0  cos0   0|    |z|    |sin0 * y + cos0 * z|
  * |  0   0      0     1|    |w|    |         1         |
  */
mat4 rotateX(float _angle) {
  return mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos(_angle), -sin(_angle), 0.0f, 0.0f, sin(_angle), cos(_angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

/**
  * |  cos0  0  sin0   0|    |x|    |x * cos0 + z * sin0|
  * |  0     0     1   0|  * |y| =  |         y         |
  * | -sin0  0  cos0   0|    |z|    |-sinθ *x + cosθ * z|
  * |  0     0     0   1|    |w|    |         1         |
  */
mat4 rotateY(float _angle) {
  return mat4(cos(_angle), 0.0f, sin(_angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin(_angle), 0.0f, cos(_angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

/**
  * |  cos0  -sin0   0   0|    |x|    |x * cos0 - y * sin0|
  * |  sin0   cos0   0   0|  * |y| =  |x * sin0 + y * cos0|
  * |  0       0     1   0|    |z|    |         z         |
  * |  0       0     0   1|    |w|    |         1         |
  */
mat4 rotateZ(float _angle) {
  return mat4(cos(_angle), -sin(_angle), 0.0f, 0.0f, sin(_angle), cos(_angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

mat4 rotateXYZ(float angle) {
  return rotateZ(angle) * rotateY(angle) * rotateX(angle);
}

void main() {

  // mat4 rotate = rotateZ(factor) * rotateY(factor) * rotateX(factor);

  gl_Position = rotateXYZ(factor) * vec4(Position, 1.0f);

  // gl_Position = vec4(Position, 1.0f);
  gl_PointSize = 10.0f;
  outTexCoord = TexCoords;
}