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
#include <utility>
#include <vector>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "util.hpp"
#include "opengl_utils.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "command.hpp"
#include "draw_command.hpp"
#include "clear_command.hpp"
#include "uniform_map.hpp"

class ConwayLifeRenderer : public Renderer {
  public:
    explicit ConwayLifeRenderer() :
        program(),
        texture_program(),
        quad(),
        fbo(std::make_pair(new Framebuffer(), new Framebuffer())),
        render_state( {
        GL_MULTISAMPLE, GL_DITHER, GL_DEPTH_TEST, GL_CULL_FACE
    }),
    frame_count(0) {
        srand((int) time(0));

        program.compile_shader("examples/shaders/texture_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("examples/shaders/conway_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        texture_program.compile_shader("examples/shaders/texture_shader.vs", GL_VERTEX_SHADER,
                                       true, true);
        texture_program.compile_shader("examples/shaders/texture_shader.fs", GL_FRAGMENT_SHADER,
                                       true, true);
        texture_program.link_program();

        quad = Mesh::construct_fullscreen_quad();

        render_state.set_param(DepthFunction({GL_LESS}));
    }

    virtual CommandList operator()(AbstractSurfacePtr surface) override {
        int width = surface->get_width();
        int height = surface->get_height();
        if (fbo.first->get_width() < 0 ||
                fbo.second->get_width() < 0) {
            auto random = random_data(width * height);

            Texture texture_1 = Texture(GL_TEXTURE_2D, width, height);
            texture_1.init({WRAP_ST_REPEAT, FILTER_MIN_MAG_NEAREST},
                         0,
                         GL_RGBA32F,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         NULL,
                         false);
            Texture texture_2 = Texture(GL_TEXTURE_2D, width, height);
            texture_2.init({WRAP_ST_REPEAT, FILTER_MIN_MAG_NEAREST},
                         0,
                         GL_RGBA32F,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         &(random[0]),
                         false);

            fbo.first->on_resize(width, height);
            fbo.first->bind_textures(std::unordered_map<std::string, Texture> {
                {"color", texture_1}
            });

            fbo.second->on_resize(width, height);
            fbo.second->bind_textures(std::unordered_map<std::string, Texture> {
                {"color", texture_2}
            });
        }

        bool ping_pong = (frame_count++ % 4 < 2);

        FramebufferPtr update_surface = (ping_pong) ? fbo.first : fbo.second;
        FramebufferPtr other_surface = (ping_pong) ? fbo.second : fbo.first;

        UniformMap map;
        map.set("prior", other_surface->get_texture("color"));

        CommandPtr clear_screen(new ClearCommand(surface,
                                ClearCommand::CLEAR_COLOR |
                                ClearCommand::CLEAR_DEPTH,
                                glm::vec4(0.0)));
        CommandPtr clear_fbo(new ClearCommand(update_surface,
                                              ClearCommand::CLEAR_COLOR |
                                              ClearCommand::CLEAR_DEPTH,
                                              glm::vec4(0.0)));

        CommandPtr update(new DrawCommand(quad,
                                          program,
                                          update_surface,
                                          map,
                                          render_state));

        UniformMap render_map;
        render_map.set("tex", update_surface->get_texture("color"));
        CommandPtr render(new DrawCommand(quad,
                                          texture_program,
                                          surface,
                                          render_map,
                                          render_state));

        return CommandList({clear_screen, clear_fbo, update, render});
    }

  private:
    std::vector<uint8_t> random_data(const int& num_elements) {
        int size = num_elements * 4;
        std::vector<uint8_t> data;
        data.reserve(size);
        for (int i = 0; i < size; i += 4) {
            if (rand() % 5 == 1) {
                data[i] = 255;
                data[i + 1] = 255;
                data[i + 2] = 255;
                data[i + 3] = 255;
            } else {
                data[i] = 0;
                data[i + 1] = 0;
                data[i + 2] = 0;
                data[i + 3] = 0;
            }
        }
        return data;
    }

    Program program;
    Program texture_program;
    Mesh quad;
    std::pair<FramebufferPtr, FramebufferPtr> fbo;
    RenderState render_state;
    int frame_count;
};
