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

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include <unordered_set>
#include <assert.h>

#include "image_data.hpp"

#define GL_STATIC_INIT() \
    std::unordered_set<int> GLContext::texture_image_units; \
    GLint GLContext::max_texture_image_units;               \
    ImagePool GLContext::image_pool; \
    const GLfloat GLContext::quad_vertex_buffer_data[18] = { \
        -1.0f, -1.0f, 0.0f, \
        1.0f, -1.0f, 0.0f, \
        -1.0f,  1.0f, 0.0f, \
        -1.0f,  1.0f, 0.0f, \
        1.0f, -1.0f, 0.0f, \
        1.0f,  1.0f, 0.0f}; \
    std::unordered_set<GLuint> GLContext::bound_buffers;

class GLContext {
  public:
    static GLenum get_texturing_unit() {
        for (int i = 0; i < GLContext::max_texture_image_units; i++) {
            if (texture_image_units.find(i) == texture_image_units.end()) {
                texture_image_units.insert(i);
                return i;
            }
        }
        return -1;
    }

    static void clear_texturing_unit() {
        GLContext::texture_image_units.clear();
    }

    static int load_image(std::string filename) {
        return image_pool.add(filename);
    }

    static ImageData get_image(const int& index) {
        return image_pool.get(index);
    }

    static void erase_image(const int& index) {
        image_pool.remove(index);
    }

    static bool add_bound_buffer(const GLuint& buffer) {
        if (bound_buffers.count(buffer) != 0) {
            return false;
        } else {
            bound_buffers.insert(buffer);
            return true;
        }
    }

    static bool is_buffer_bound(const GLuint& buffer) {
        return bound_buffers.count(buffer) == 1;
    }

    static bool remove_bound_buffer(const GLuint& buffer) {
        if (bound_buffers.count(buffer) == 0) {
            return false;
        } else {
            bound_buffers.erase(buffer);
            return true;
        }
    }

    static void gl_init() {
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                      &max_texture_image_units);
        assert(GLEW_ARB_direct_state_access);
    }

    static void gl_refresh() {
        clear_texturing_unit();
    }

    static std::unordered_set<int> texture_image_units;
    static GLint max_texture_image_units;
    static ImagePool image_pool;
    static std::unordered_set<GLuint> bound_buffers;


    const static GLfloat quad_vertex_buffer_data[18];

  private:
    static GLenum get_imaging_unit_from_offset(const int& offset) {
        switch (offset) {
            case 0:
                return GL_TEXTURE0;
            case 1:
                return GL_TEXTURE1;
            case 2:
                return GL_TEXTURE2;
            case 3:
                return GL_TEXTURE3;
            case 4:
                return GL_TEXTURE4;
            case 5:
                return GL_TEXTURE5;
            case 6:
                return GL_TEXTURE6;
            case 7:
                return GL_TEXTURE7;
            case 8:
                return GL_TEXTURE8;
            case 9:
                return GL_TEXTURE9;
            case 10:
                return GL_TEXTURE10;
            case 11:
                return GL_TEXTURE11;
            case 12:
                return GL_TEXTURE12;
            case 13:
                return GL_TEXTURE13;
            case 14:
                return GL_TEXTURE14;
            case 15:
                return GL_TEXTURE15;
            case 16:
                return GL_TEXTURE16;
            case 17:
                return GL_TEXTURE17;
            case 18:
                return GL_TEXTURE18;
            case 19:
                return GL_TEXTURE19;
            case 20:
                return GL_TEXTURE20;
            case 21:
                return GL_TEXTURE21;
            case 22:
                return GL_TEXTURE22;
            case 23:
                return GL_TEXTURE23;
            case 24:
                return GL_TEXTURE24;
            case 25:
                return GL_TEXTURE25;
            case 26:
                return GL_TEXTURE26;
            case 27:
                return GL_TEXTURE27;
            case 28:
                return GL_TEXTURE28;
            case 29:
                return GL_TEXTURE29;
            case 30:
                return GL_TEXTURE30;
            case 31:
                return GL_TEXTURE31;
            default:
                return -1;
        }
    }
};
