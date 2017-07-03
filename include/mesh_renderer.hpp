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

class MeshRenderer : public Renderer {
  public:
    explicit MeshRenderer(InputController& controller, Mesh& mesh) :
        program(),
        ctrl(controller),
        mesh(mesh) {
        program.compile_shader("shaders/simple_shader.vs", GL_VERTEX_SHADER);
        program.compile_shader("shaders/simple_shader.fs", GL_FRAGMENT_SHADER);
        program.link_program();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClearColor(0.0, 0.0, 0.0, 0.0);
    }

    virtual void operator()(const int& width,
                            const int& height) override {
        glm::mat4 projection_matrix = ctrl.get_projection();
        glm::mat4 view_matrix = ctrl.get_view();

        program.set_uniform("model", glm::mat4());
        program.set_uniform("view", view_matrix);
        program.set_uniform("projection", projection_matrix);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        program.bind();
        mesh.draw();
    }
  private:
    Program program;
    InputController& ctrl;
    Mesh& mesh;
};
