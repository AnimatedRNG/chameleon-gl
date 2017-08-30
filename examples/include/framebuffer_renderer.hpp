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

#include "util.hpp"
#include "opengl_utils.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "mesh.hpp"
#include "command.hpp"
#include "draw_command.hpp"
#include "clear_command.hpp"

class FramebufferRenderer : public Renderer {
  public:
    explicit FramebufferRenderer(InputController& controller, Renderer& renderer) :
        program(),
        ctrl(controller),
        fbo(new Framebuffer()),
        renderer(renderer),
        quad(),
        render_state( {
        GL_MULTISAMPLE, GL_DITHER, GL_DEPTH_TEST
    }) {
        // Compile a shader that renders a fullscreen quad
        program.compile_shader("examples/shaders/texture_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("examples/shaders/texture_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        // Create a fullscreen quad mesh
        quad = Mesh::construct_fullscreen_quad();

        // Set this render state's depth function to GL_LESS
        render_state.set_param(DepthFunction({GL_LESS}));
    }

    virtual CommandList operator()(AbstractSurfacePtr surface) override {
        // Some ugly boilerplate rquired to request (and update) a framebuffer
        int width = surface->get_width();
        int height = surface->get_height();
        if (fbo->get_width() < 0) {
            fbo->on_resize(width, height);
            fbo->typical_fbo();
        }

        CommandList commands;

        // Adds command to clear screen
        commands.push_back(CommandPtr(new ClearCommand({
            surface,
            ClearCommand::CLEAR_COLOR |
            ClearCommand::CLEAR_DEPTH,
            glm::vec4(0.0)
        })));

        // Append the commands of the inner rendering operation to the list
        auto nested_commands = renderer(fbo);
        commands.insert(commands.end(),
                        nested_commands.begin(),
                        nested_commands.end());

        // Binds the framebuffer's color attachment backing texture to a uniform
        // map
        UniformMap map;
        map.set("tex", fbo->get_texture("color"));

        // Draw to fullscreen quad
        commands.push_back(CommandPtr(new DrawCommand({
            quad,
            program,
            surface,
            map,
            render_state
        })));

        return commands;
    }
  private:
    Program program;
    InputController& ctrl;
    FramebufferPtr fbo;
    Renderer& renderer;
    Mesh quad;
    RenderState render_state;
};
