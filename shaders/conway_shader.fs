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

uniform sampler2D prior;
uniform vec4 chml_viewport;

#define SCREEN_NORM(a) (a) / chml_viewport.zw
#define FETCH(pos) texture(prior, SCREEN_NORM(vec2(pos) + gl_FragCoord.xy)).r
#define LIVE() color = vec3(1.0)
#define DIE() color = vec3(0.0)

void main() {
    bool alive = int(FETCH(ivec2(0, 0))) == 1.0;

    float num_neighbors = 0;
    num_neighbors += FETCH(ivec2(-1, -1));
    num_neighbors += FETCH(ivec2(-1, 0));
    num_neighbors += FETCH(ivec2(-1, 1));
    num_neighbors += FETCH(ivec2(0, -1));
    num_neighbors += FETCH(ivec2(0, 1));
    num_neighbors += FETCH(ivec2(1, -1));
    num_neighbors += FETCH(ivec2(1, 0));
    num_neighbors += FETCH(ivec2(1, 1));
    num_neighbors = float(int(round(num_neighbors)));

    if (alive) {
        if (num_neighbors < 2 || num_neighbors > 3) {
            DIE();
        } else {
            LIVE();
        }
    } else {
        if (num_neighbors == 3) {
            LIVE();
        } else {
            DIE();
        }
    }
}
