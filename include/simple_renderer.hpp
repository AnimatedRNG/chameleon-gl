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
#include "input.hpp"
#include "sdl_helpers.hpp"
#include "renderer.hpp"
#include "graphics_context.hpp"

class SDFRenderer : public Renderer {
  public:
    explicit SDFRenderer(InputController& controller);

    virtual void operator()(const int& width,
                            const int& height) override;

    inline virtual float sdf(const glm::vec4& location) const;
  private:
    inline virtual float sphere_sdf(const glm::vec4& location,
                                    const float& radius) const;
    inline virtual float fancy_torus_sdf(const glm::vec4& location,
                                         const glm::vec2& t) const;
    inline float length8(const glm::vec2& q) const {
        return pow(pow(q.x, 8.) + pow(q.y, 8.), 1.0 / 8.0);
    }

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    Program program;
    GLuint vao;
    VBO vbo;
    InputController& ctrl;

    constexpr static float FOVY = 60.0;
    constexpr static float NEAR = 0.1;
    constexpr static float FAR = 100;

    // The fullscreen quad's VBO
    const GLfloat quad_vertex_buffer_data[18] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
    };
};
