#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
out vec3 ourPos;

uniform float xOffset;
// vec3(0.0, 0.0, 0.0)

void main(){
  gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0f);
  ourColor = aColor;
  ourPos = aPos;
}