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

#version 430 core
out vec3 color;

uniform vec3 chml_origin;
uniform mat4 chml_inverse_view_projection;
uniform vec4 chml_viewport;

uniform float chml_near;
//uniform float FAR;

float sdfSphere(in vec3 pos, in float radius) {
    return length(pos) - radius;
}

float sdfTriPrism(in vec3 p, in vec2 h) {
    vec3 q = abs(p);
    return max(q.z - h.y, max(q.x * 0.866025 + p.y * 0.5, -p.y) - h.x * 0.5);
}

float sdTorus(in vec3 p, in vec2 t) {
    vec2 q = vec2(length(p.xz) - t.x, p.y);
    return length(q) - t.y;
}

vec3 opCheapBend(vec3 p) {
    float c = cos(0.1 * p.y);
    float s = sin(0.1 * p.y);
    mat2 m = mat2(c, -s, s, c);
    return vec3(m * p.xy, p.z);
}

void main() {
    float i = gl_FragCoord.x;
    float j = gl_FragCoord.y;
    vec4 world_space_vec = vec4((i - chml_viewport[0]) / chml_viewport[2],
                                ((chml_viewport[3] - j) - chml_viewport[1]) / chml_viewport[3],
                                1,
                                1);
    world_space_vec = world_space_vec * 2.0 - 1.0;

    vec4 tmp = chml_inverse_view_projection * world_space_vec;
    tmp /= tmp.w;
    tmp -= vec4(chml_origin, 0);
    vec3 ray_vec = vec3(tmp);
    ray_vec = normalize(ray_vec);

    vec3 current_point = chml_origin + ray_vec * chml_near;
    float radius = 0;
    for (int k = 0; k < 20; k++) {
        radius = sdTorus(current_point, vec2(3, 2));
        current_point += ray_vec * radius;
    }

    float distance = length(current_point - chml_origin);

    if (radius < 0.1)
        color = vec3(dot(normalize(current_point), normalize(vec3(0, -5, 0))));
    else
        color = vec3(0);
}
