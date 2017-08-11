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
#include <unordered_set>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "util.hpp"
#include "opengl_utils.hpp"

typedef struct {
    GLuint buf = -1;
    GLenum sfactor;
    GLenum dfactor;
} BlendFunction;

typedef struct {
    GLfloat value;
    GLboolean invert;
} SampleCoverage;

typedef struct {
    GLfloat factor;
    GLfloat units;
} PolygonOffset;

typedef struct {
    GLenum face;
    GLenum mode;
} PolygonMode;

typedef struct {
    GLuint mask_number;
    GLbitfield mask;
} SampleMask;

typedef struct {
    GLenum func;
    GLint ref;
    GLuint mask;
} StencilFunction;

typedef struct {
    GLenum sfail;
    GLenum dpfail;
    GLenum dppass;
} StencilOperation;

class RenderState {
  public:
    explicit RenderState(
        std::unordered_set<GLenum> enabled =
            std::unordered_set<GLenum>({GL_MULTISAMPLE, GL_DITHER})) :
        _enabled(new std::unordered_set<GLenum>(enabled)) {

    }

  private:
    std::shared_ptr<std::unordered_set<GLenum>> _enabled;
    std::shared_ptr<BlendFunction> _blend_function;
    std::shared_ptr<GLenum> _logic_op;
    std::shared_ptr<GLenum> _cull_face;
    std::shared_ptr<glm::dvec2> _depth_range;
    std::shared_ptr<GLenum> _depth_func;
    std::shared_ptr<GLfloat> _line_width;
    std::shared_ptr<GLfloat> _point_size;
    std::shared_ptr<SampleCoverage> _sample_coverage;
    std::shared_ptr<PolygonOffset> _polygon_offset;
    std::shared_ptr<PolygonMode> _polygon_mode;
    std::shared_ptr<GLuint> _primitive_restart_index;
    std::shared_ptr<GLfloat> _min_sample_shading;
    std::shared_ptr<SampleMask> _sample_mask;
    std::shared_ptr<glm::ivec4> _scissor_box;
    std::shared_ptr<StencilFunction> _stencil_function;
    std::shared_ptr<StencilOperation> _stencil_operation;
    // Add support for glDebugMessageCallback -- perhaps through std::function?
};
