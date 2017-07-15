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
#include "renderer.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "graphics_context.hpp"
#include "draw_command.hpp"

class TextureRenderer : public Renderer {
  public:
    explicit TextureRenderer(InputController& controller, Texture& tex) :
        quad(),
        texture(tex) {
        program.compile_shader("shaders/texture_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("shaders/texture_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        quad = Mesh::construct_fullscreen_quad();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClearColor(0.0, 0.0, 0.0, 0.0);
    }

    virtual void operator()(const int& width,
                            const int& height) override {
        glm::vec4 viewport(0, 0, width, height);

        program.bind();
        texture.bind();
        program.set_uniform("tex", texture);
        program.set_uniform("viewport", viewport);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawCommand tex_draw(quad, program);
        DrawCommand::exec(tex_draw);
    }

  private:
    Program program;
    GLuint vao;
    Mesh quad;
    Texture& texture;
};
