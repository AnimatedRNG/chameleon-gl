#version 300 es

layout(location = 0) in vec4 vertex_pos;
layout(location = 1) in vec4 vertex_normal;
layout(location = 2) in vec2 vertex_uv;
  
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 normal_EC;
out vec2 uv;

void main() {
    mat4 mv = view * model;
    mat4 mvp = projection * mv;
    gl_Position = mvp * vec4(vertex_pos.xyz, 1);
    normal_EC = mv * vertex_normal;
    uv = vertex_uv;
}
