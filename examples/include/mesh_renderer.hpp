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

#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "util.hpp"
#include "opengl_utils.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "graphics_context.hpp"
#include "mesh.hpp"
#include "draw_command.hpp"
#include "clear_command.hpp"

class MeshRenderer : public Renderer {
  public:
    explicit MeshRenderer(InputController& controller, Mesh& mesh) :
        program(),
        ctrl(controller),
        mesh(mesh) {
        program.compile_shader("shaders/simple_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("shaders/simple_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    virtual void operator()(AbstractSurfacePtr surface) override {
        ClearCommand clear(surface,
                           ClearCommand::CLEAR_COLOR |
                           ClearCommand::CLEAR_DEPTH,
                           glm::vec4(0.0));
        ClearCommand::exec(clear);

        DrawCommand mesh_draw(mesh, program, surface);
        DrawCommand::exec(mesh_draw);
    }
  private:
    Program program;
    InputController& ctrl;
    Mesh& mesh;
};
