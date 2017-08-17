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
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "util.hpp"
#include "opengl_utils.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "mesh.hpp"
#include "draw_command.hpp"
#include "clear_command.hpp"

class MeshRenderer : public Renderer {
  public:
    explicit MeshRenderer(InputController& controller, Mesh& mesh) :
        program(),
        ctrl(controller),
        render_state( {
        GL_MULTISAMPLE, GL_DITHER, GL_DEPTH_TEST, GL_CULL_FACE
    }),
    mesh(mesh) {
        program.compile_shader("shaders/simple_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("shaders/simple_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        render_state.set_param(DepthFunction({GL_LESS}));
        render_state.set_param(CullFace({GL_BACK}));
    }

    virtual CommandList operator()(AbstractSurfacePtr surface) override {
        CommandPtr clear(new ClearCommand(surface,
                                          ClearCommand::CLEAR_COLOR |
                                          ClearCommand::CLEAR_DEPTH,
                                          glm::vec4(0.0)));

        CommandPtr mesh_draw(new DrawCommand(
                                 mesh,
                                 program,
                                 surface,
                                 UniformMap(),
                                 render_state));
        return CommandList({clear, mesh_draw});
    }
  private:
    Program program;
    InputController& ctrl;
    RenderState render_state;
    Mesh& mesh;
};
