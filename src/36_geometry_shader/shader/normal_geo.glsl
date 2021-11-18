#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT {
  vec3 normal;
} gs_in[];

const float LENGTH = 0.1;
uniform mat4 projection;

void GenerateLine(int index) {
  gl_Position = projection * gl_in[index].gl_Position;
  EmitVertex();
  gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * LENGTH);
  EmitVertex();
  EndPrimitive();
}

void main() {
  GenerateLine(0); // 第一个顶点法线
  GenerateLine(1); // 第二个顶点法线
  GenerateLine(2); // 第三个顶点法线
}
