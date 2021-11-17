#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 5) out;

in VS_OUT {
  vec2 TexCoord;
} gs_in[];

out vec2 fColor;

void build_house(vec4 position) {

  fColor = gs_in[0].TexCoord;

  gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:左下
  EmitVertex();
  gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);    // 2:右下
  EmitVertex();
  gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);    // 3:左上
  EmitVertex();
  gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);    // 4:右上
  EmitVertex();
  gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);    // 5:顶部

  fColor = vec2(1.0, 0.5); // 最后一个点的颜色

  EmitVertex();
  EndPrimitive();
}

void main() {
  build_house(gl_in[0].gl_Position);
}