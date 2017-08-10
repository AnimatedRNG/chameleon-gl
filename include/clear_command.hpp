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
#include "abstract_surface.hpp"
#include "opengl_utils.hpp"

class ClearCommand {
  public:
    ClearCommand(AbstractSurfacePtr ptr,
                 const uint8_t& buffers,
                 glm::vec4 clear_color = glm::vec4(0.0),
                 float depth_value = 1.0,
                 GLint stencil_value = 0) :
        _fbo(ptr),
        _buffers(buffers),
        _clear_color(clear_color),
        _depth_value(depth_value),
        _stencil_value(stencil_value),
        _use_framebuffer(ptr->get_width() > 0) {

    }

    void operator()() {
        // Use glClear for non-FBO, and glClearBuffer for FBO
        if (_use_framebuffer) {
            GLbitfield clear_field = 0;

            if (_buffers & CLEAR_COLOR) {
                clear_field |= GL_COLOR_BUFFER_BIT;
            }
            if (_buffers & CLEAR_DEPTH) {
                clear_field |= GL_DEPTH_BUFFER_BIT;
            }
            if (_buffers & CLEAR_STENCIL) {
                clear_field |= GL_STENCIL_BUFFER_BIT;
            }

            // Ideally cache the results of previous clear color/depth/stencil
            // changes so that we don't have to issue all these calls!
            glClearColor(_clear_color[0],
                         _clear_color[1],
                         _clear_color[2],
                         _clear_color[3]);
            glClearDepth(_depth_value);
            glClearStencil(_stencil_value);
            glClear(clear_field);
        } else {
            // Implement as an FBO function called "clear"
        }
    }

    static void exec(ClearCommand& clearCommand) {
        clearCommand();
    }

    constexpr static uint8_t CLEAR_COLOR =    0b001;
    constexpr static uint8_t CLEAR_DEPTH =    0b010;
    constexpr static uint8_t CLEAR_STENCIL =  0b100;

  private:
    AbstractSurfacePtr _fbo;
    uint8_t _buffers;
    glm::vec4 _clear_color;
    float _depth_value;
    GLint _stencil_value;

    bool _use_framebuffer;
};
