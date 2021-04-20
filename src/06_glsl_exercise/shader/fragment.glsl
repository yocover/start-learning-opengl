#version 330 core
out vec4 FracColor;
in vec3 ourColor;
in vec3 ourPos;

void main(){
  FracColor = vec4(ourPos, 1.0f);
}