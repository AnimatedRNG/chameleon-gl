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

#include "chameleon_gl.hpp"
#include "input.hpp"
#include "graphics_context.hpp"
#include "mesh_renderer.hpp"
#include "framebuffer_renderer.hpp"
#include "mesh.hpp"

STATIC_INIT()

int main(int argc, char** args) {
    InputController input;
    GraphicsContext context(input);
    Mesh mesh;
    mesh.load("assets/sculpt.obj");

    MeshRenderer mesh_renderer(input, mesh);
    FramebufferRenderer framebuffer_renderer(input, mesh_renderer);
    context.start(framebuffer_renderer);
}
