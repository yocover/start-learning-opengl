#version 330 core
out vec4 FragColor;

in VS_OUT {
  vec2 TexCoord;
} vs_in;

uniform vec3 objectColor;
uniform sampler2D uvMap;
uniform sampler2D triMap;

void main() {
  vec3 result = vec3(1.0);
  if(gl_FrontFacing) {
    result = texture(uvMap, vs_in.TexCoord).rgb;
  } else {
    result = texture(triMap, vs_in.TexCoord).rgb;
  }
  FragColor = vec4(result * vec3(1.0, 0.0, 1.0), 1.0);

}