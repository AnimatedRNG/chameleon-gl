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

precision highp float;

in vec4 normal_EC;
in vec2 uv;

out vec4 color;

void main() {
    float zFar = 100.0;
    float zNear = 0.1;
    //color = vec4(vec3(1.0 - (2.0 * zNear) /
                      //(zFar + zNear - gl_FragCoord.z * (zFar - zNear))), 1.0);
    //color = vec4(vec3(dot(normalize(normal_EC), normalize(vec4(5)))), 1.0);
    color = vec4(uv, 1.0, 1.0);
}
