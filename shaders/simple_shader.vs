//
// ChameleonGL - A small framework for OpenGL.
// Copyright (C) 2012-2017 Srinivas Kaza
//
// This file is part of ChameleonGL.
//
// ChameleonGL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ChameleonGL is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ChameleonGL.  If not, see <http://www.gnu.org/licenses/>.
//

#version 300 es

layout(location = 0) in vec4 vertex_pos;
layout(location = 1) in vec4 vertex_normal;
layout(location = 2) in vec2 vertex_uv;
  
uniform mat4 chml_model;
uniform mat4 chml_view;
uniform mat4 chml_projection;

out vec4 normal_EC;
out vec2 uv;

void main() {
    mat4 mv = chml_view * chml_model;
    mat4 mvp = chml_projection * mv;
    gl_Position = mvp * vec4(vertex_pos.xyz, 1);
    normal_EC = mv * vertex_normal;
    uv = vertex_uv;
}
