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

class TextureRenderer : public Renderer {
  public:
    explicit TextureRenderer(InputController& controller, Texture& tex) :
        quad(),
        texture(tex) {
        program.compile_shader("shaders/texture_shader.vs", GL_VERTEX_SHADER);
        program.compile_shader("shaders/texture_shader.fs", GL_FRAGMENT_SHADER);
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

        quad.draw();
    }

  private:
    Program program;
    GLuint vao;
    Mesh quad;
    Texture& texture;
};
