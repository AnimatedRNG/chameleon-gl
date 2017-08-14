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
#include <climits>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <functional>
#include <exception>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "util.hpp"
#include "opengl_utils.hpp"

typedef struct {
    GLuint buf = UINT_MAX;
    GLenum src_RGB = GL_ONE;
    GLenum dst_RGB = GL_ZERO;
    GLenum src_alpha = GL_ONE;
    GLenum dst_alpha = GL_ZERO;
} BlendFunction;

typedef struct {
    GLuint buf = UINT_MAX;
    GLenum mode_RGB = GL_FUNC_ADD;
    GLenum mode_alpha = GL_FUNC_ADD;
} BlendEquation;

typedef struct {
    GLfloat value = 1.0;
    GLboolean invert = GL_FALSE;
} SampleCoverage;

typedef struct {
    GLfloat factor = 0;
    GLfloat units = 0;
} PolygonOffset;

typedef struct {
    GLenum face = GL_FRONT_AND_BACK; // CHECK
    GLenum mode = GL_FILL;
} PolygonMode;

// Non-default
typedef struct {
    GLuint mask_number = 0;
    GLbitfield mask = 0xFFFFFFFF;
} SampleMask;

typedef struct {
    GLenum func = GL_ALWAYS;
    GLint ref = 0;
    GLuint mask = 0xFFFFFFFF;
} StencilFunction;

typedef struct {
    GLenum sfail = GL_KEEP;
    GLenum dpfail = GL_KEEP;
    GLenum dppass = GL_KEEP;
} StencilOperation;

typedef struct {
    GLenum opcode = GL_COPY;
} LogicalOperation;

typedef struct {
    GLenum mode = GL_BACK;
} CullFace;

typedef struct {
    glm::dvec2 range = glm::dvec2(0.0, 1.0);
} DepthRange;

typedef struct {
    GLenum mode = GL_LESS;
} DepthFunction;

typedef struct {
    GLfloat width = 1.0;
} LineWidth;

typedef struct {
    GLfloat size = 1.0;
} PointSize;

// Non-default
typedef struct {
    GLuint index = 0;
} PrimitiveRestartIndex;

// Non-default
typedef struct {
    GLfloat value = 1.0;
} MinSampleShading;

// Width/height are non-default; be careful
typedef struct {
    glm::ivec4 box = glm::ivec4(0, 0, 0, 0);
} ScissorBox;

class RenderState {
  public:
    explicit RenderState(
        std::unordered_set<GLenum> enabled =
            std::unordered_set<GLenum>({GL_MULTISAMPLE, GL_DITHER}),
        std::unordered_map<std::string, std::function<void()>> params =
            std::unordered_map<std::string, std::function<void()>>()) :
        _enabled(new std::unordered_set<GLenum>(enabled)),
        _params(new std::unordered_map<std::string,
                std::function<void()>>(params)) {

    }

    template <typename T>
    void set_param(T value) {
        throw std::runtime_error("Invalid parameter!");
    }

    void set_param(const BlendFunction& value) {
        (*_params)["blend_function"] = [value]() {
            if (value.buf == UINT_MAX) {
                glBlendFuncSeparate(value.src_RGB,
                                    value.dst_RGB,
                                    value.src_alpha,
                                    value.dst_alpha);
            } else {
                glBlendFuncSeparatei(value.buf,
                                     value.src_RGB,
                                     value.dst_RGB,
                                     value.src_alpha,
                                     value.dst_alpha);
            }
        };
    }

    void set_param(const BlendEquation& value) {
        (*_params)["blend_equation"] = [value]() {
            if (value.buf == UINT_MAX) {
                glBlendEquationSeparate(value.mode_RGB,
                                        value.mode_alpha);
            } else {
                glBlendEquationSeparatei(value.buf,
                                         value.mode_RGB,
                                         value.mode_alpha);
            }
        };
    }

    void set_param(const LogicalOperation& value) {
        (*_params)["logical_operation"] = [value]() {
            glLogicOp(value.opcode);
        };
    }

    void set_param(const CullFace& value) {
        (*_params)["cull_face"] = [value]() {
            glCullFace(value.mode);
        };
    }

    void set_param(const DepthRange& value) {
        (*_params)["depth_range"] = [value]() {
            glDepthRange(value.range[0], value.range[1]);
        };
    }

    void set_param(const DepthFunction& value) {
        (*_params)["depth_function"] = [value]() {
            glDepthFunc(value.mode);
        };
    }

    void set_param(const LineWidth& value) {
        (*_params)["line_width"] = [value]() {
            glLineWidth(value.width);
        };
    }

    void set_param(const PointSize& value) {
        (*_params)["point_size"] = [value]() {
            glPointSize(value.size);
        };
    }

    void set_param(const SampleCoverage& value) {
        (*_params)["sample_coverage"] = [value]() {
            glSampleCoverage(value.value, value.invert);
        };
    }

    void set_param(const PolygonOffset& value) {
        (*_params)["polygon_offset"] = [value]() {
            glPolygonOffset(value.factor, value.units);
        };
    }

    void set_param(const PolygonMode& value) {
        (*_params)["polygon_mode"] = [value]() {
            glPolygonMode(value.face, value.mode);
        };
    }

    void set_param(const PrimitiveRestartIndex& value) {
        (*_params)["primitive_restart_index"] = [value]() {
            glPrimitiveRestartIndex(value.index);
        };
    }

    void set_param(const MinSampleShading& value) {
        (*_params)["min_sample_shading"] = [value]() {
            glMinSampleShading(value.value);
        };
    }

    void set_param(const SampleMask& value) {
        (*_params)["sample_mask"] = [value]() {
            glSampleMaski(value.mask_number, value.mask);
        };
    }

    void set_param(const ScissorBox& value) {
        (*_params)["scissor_box"] = [value]() {
            glScissor(value.box[0], value.box[1], value.box[2], value.box[3]);
        };
    }

    void set_param(const StencilFunction& value) {
        (*_params)["stencil_function"] = [value]() {
            glStencilFunc(value.func, value.ref, value.mask);
        };
    }

    void set_param(const StencilOperation& value) {
        (*_params)["stencil_operation"] = [value]() {
            glStencilOp(value.sfail, value.dpfail, value.dppass);
        };
    }

  private:
    std::shared_ptr<std::unordered_set<GLenum>> _enabled;
    std::shared_ptr < std::unordered_map <
    std::string, std::function<void()>>> _params;

    // Add support for glDebugMessageCallback -- perhaps through std::function?
};
