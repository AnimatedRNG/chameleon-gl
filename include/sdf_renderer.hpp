#pragma once

#include <iostream>
#include <cmath>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "util.hpp"
#include "opengl_utils.hpp"
#include "mesh.hpp"
#include "input.hpp"
#include "sdl_helpers.hpp"
#include "renderer.hpp"
#include "graphics_context.hpp"

class SDFRenderer : public Renderer {
  public:
    explicit SDFRenderer(InputController& controller) :
        program(),
        mesh(),
        ctrl(controller) {
        program.compile_shader("shaders/sdf_shader.vs", GL_VERTEX_SHADER);
        program.compile_shader("shaders/sdf_shader.fs", GL_FRAGMENT_SHADER);
        program.link_program();

        mesh = Mesh::construct_fullscreen_quad();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClearColor(0.0, 0.0, 0.0, 1.0);
    }

    virtual void operator()(const int& width,
                            const int& height) override {
        glm::mat4 projection_matrix = ctrl.get_projection();
        glm::mat4 view_matrix = ctrl.get_view();

        glm::vec4 viewport(0, 0, width, height);
        glm::vec3 origin(glm::inverse(view_matrix) * glm::vec4(0, 0, 0, 1));
        glm::mat4 inverse(glm::inverse(projection_matrix * view_matrix));

        program.bind();
        program.set_uniform("origin", origin);
        program.set_uniform("inv", inverse);
        program.set_uniform("viewport", viewport);

        program.set_uniform("NEAR", InputController::NEAR_PLANE);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mesh.draw();
    }

  private:
    Program program;
    GLuint vao;
    Mesh mesh;
    InputController& ctrl;
};
