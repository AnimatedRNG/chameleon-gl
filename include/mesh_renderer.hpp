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

        glClearColor(0.0, 0.0, 0.0, 1.0);
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
