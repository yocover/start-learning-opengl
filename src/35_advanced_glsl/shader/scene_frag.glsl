#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;

uniform vec3 objectColor;

uniform sampler2D uvMap;
uniform sampler2D triMap;

void main() {

  vec3 result = vec3(1.0);
  if(gl_FrontFacing) {
    result = texture(uvMap, oTexCoord).rgb;
  } else {
    result = texture(triMap, oTexCoord).rgb;
  }
  FragColor = vec4(result, 1.0);

}