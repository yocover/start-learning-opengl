#version 330 core
out vec4 FracColor;
in vec3 ourColor;

void main(){
  FracColor = vec4(ourColor, 1.0f);
}