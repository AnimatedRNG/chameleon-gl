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

#include "util.hpp"
#include "opengl_utils.hpp"
#include "mesh.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "command.hpp"
#include "draw_command.hpp"
#include "clear_command.hpp"
#include "abstract_surface.hpp"

class SDFRenderer : public Renderer {
  public:
    explicit SDFRenderer(InputController& controller) :
        program(),
        mesh(),
        ctrl(controller),
        render_state( {
        GL_MULTISAMPLE, GL_DITHER, GL_DEPTH_TEST
    }) {
        program.compile_shader("examples/shaders/sdf_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("examples/shaders/sdf_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        mesh = Mesh::construct_fullscreen_quad();

        render_state.set_param(DepthFunction({GL_LESS}));
    }

    virtual CommandList operator()(AbstractSurfacePtr surface) override {
        CommandPtr clear(new ClearCommand(surface,
                                          ClearCommand::CLEAR_COLOR |
                                          ClearCommand::CLEAR_DEPTH,
                                          glm::vec4(0.0)));

        CommandPtr sdf_draw(new DrawCommand(mesh,
                                            program,
                                            surface,
                                            UniformMap(),
                                            render_state));
        return CommandList({clear, sdf_draw});
    }

  private:
    Program program;
    Mesh mesh;
    InputController& ctrl;
    RenderState render_state;
};
