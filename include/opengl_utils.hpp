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
#include <cmath>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cstring>
#include <unordered_set>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "util.hpp"
#include "gl_context.hpp"

typedef struct {
    GLuint index;
    GLint vector_size;
    GLsizei stride;
    GLvoid* offset;
} VertexAttribute;

inline std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    out << "("
        << vec.x << ", " << vec.y << ", " << vec.z
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec4& vec) {
    out << "("
        << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const VertexAttribute& va) {
    out << "(index: "
        << va.index
        << ", vector_size: "
        << va.vector_size
        << ", stride: "
        << va.stride
        << ", offset: "
        << va.offset
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec2& vec) {
    out << "("
        << vec.x << ", " << vec.y
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::mat4& mat4) {
    out << "["
        << mat4[0][0] << ", " << mat4[0][1] << ", " << mat4[0][2] << ", " << mat4[0][3]
        << "\n "
        << mat4[1][0] << ", " << mat4[1][1] << ", " << mat4[1][2] << ", " << mat4[1][3]
        << "\n "
        << mat4[2][0] << ", " << mat4[2][1] << ", " << mat4[2][2] << ", " << mat4[2][3]
        << "\n "
        << mat4[3][0] << ", " << mat4[3][1] << ", " << mat4[3][2] << ", " << mat4[3][3]
        << "]";

    return out;
}

class VBO {
  public:
    VBO() {

    }

    VBO(const std::vector<GLfloat*>& vertex_data,
        const std::vector<VertexAttribute>& vertex_attributes,
        GLuint num_vertices,
        GLenum primitive_type) :
        _vertex_attributes(vertex_attributes),
        _primitive_type(primitive_type),
        _num_vertices(num_vertices) {
        this->_vertex_buffer = new GLuint[vertex_data.size()];
        glGenBuffers(vertex_data.size(), _vertex_buffer);
        for (size_t i = 0; i < vertex_data.size(); i++) {
            glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer[i]);
            auto va = vertex_attributes[i];
            glBufferData(GL_ARRAY_BUFFER,
                         num_vertices * va.vector_size * sizeof(GLfloat),
                         &(vertex_data[i][0]),
                         GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        assert(_vertex_attributes.size() == vertex_data.size());
    }

    VBO(const VBO& other) {
        this->_num_vertices = other._num_vertices;
        this->_vertex_buffer = new GLuint[other._vertex_attributes.size()];
        this->_primitive_type = other._primitive_type;
        this->_vertex_attributes = other._vertex_attributes;
    }

    ~VBO() {
        //delete this->_vertex_buffer;
    }

    bool draw() {
        std::unordered_set<GLuint> indices;
        for (size_t i = 0; i < _vertex_attributes.size(); i++) {
            const VertexAttribute va = _vertex_attributes[i];

            if (indices.count(va.index)) {
                ERROR("Vertex attribute " << va <<
                      " has an index conflict with another vertex attribute!");
                return false;
            }

            glEnableVertexAttribArray(va.index);
            glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer[i]);
            glVertexAttribPointer(
                i,
                va.vector_size,
                GL_FLOAT,
                GL_FALSE,
                va.stride,
                va.offset
            );
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            indices.insert(va.index);
        }

        glDrawArrays(_primitive_type, 0, _num_vertices);

        for (size_t i = 0; i < _vertex_attributes.size(); i++) {
            const VertexAttribute va = _vertex_attributes[i];

            glDisableVertexAttribArray(va.index);
        }

        return true;
    }
  private:
    GLuint* _vertex_buffer;
    std::vector<VertexAttribute> _vertex_attributes;
    GLenum _primitive_type;
    GLuint _num_vertices;
};

enum TextureParameter {
    WRAP_ST_CLAMP_TO_BORDER,
    WRAP_ST_CLAMP_TO_EDGE,
    WRAP_ST_REPEAT,
    WRAP_ST_MIRRORED_REPEAT,

    FILTER_MIN_MAG_NEAREST,
    FILTER_MIN_MAG_LINEAR
};
typedef std::unordered_set<TextureParameter> TextureParameterSet;

class Texture {
  public:
    Texture(const GLenum& texture_enum,
            const int& w,
            const int& h,
            const int& d = 1) :
        width(w),
        height(h),
        depth(d),
        _texture_enum(texture_enum) {
        glGenTextures(1, &id);
    }

    Texture(const std::string& filename,
            const GLint& internalFormat,
            const TextureParameterSet parameters,
            const GLint& level = 0,
            const bool& mip_map = true,
            void (*tex_parameter_callback)(void) = NULL) {
        _texture_enum = GL_TEXTURE_2D;
        load_from_file(filename,
                       internalFormat,
                       parameters,
                       level,
                       mip_map,
                       tex_parameter_callback);
    }

    ~Texture() {
        // Maybe be a good custodian and unbind the names here?
    }

    void load_from_file(const std::string& filename,
                        const GLint& internalFormat,
                        const TextureParameterSet parameters,
                        const GLint& level = 0,
                        const bool& mip_map = true,
                        void (*tex_parameter_callback)(void) = NULL) {
        ImageData data =
            GLContext::get_image(GLContext::load_image(filename));

        this->width = data.width;
        this->height = data.height;
        this->depth = 1;
        init(parameters,
             level,
             internalFormat,
             data.format,
             GL_UNSIGNED_BYTE,
             data.data,
             mip_map,
             tex_parameter_callback);
    }

    void init(const TextureParameterSet parameters,
              const GLint& level,
              const GLint& internalFormat,
              const GLenum& format,
              const GLenum& type,
              const GLvoid* data,
              const bool& mip_map = true,
              void (*tex_parameter_callback)(void) = NULL) {
        glBindTexture(_texture_enum, id);
        assert(parameters.size() == 0 ^ tex_parameter_callback == NULL);
        if (tex_parameter_callback != NULL) {
            tex_parameter_callback();
        } else {
            for (auto parameter : parameters) {
                apply_parameter(parameter);
            }
        }
        glTexImage2D(_texture_enum, level, internalFormat,
                     (GLsizei) width, (GLsizei) height, 0,
                     format, type, data);
        if (mip_map) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        _internalFormat = internalFormat;
        _format = format;
        _type = type;

        glBindTexture(_texture_enum, 0);
    }

    void clear(const GLint& level,
               const GLvoid* data,
               glm::ivec4 region = glm::ivec4(-1),
               glm::ivec2 region_z = glm::ivec2(-1)) {
        if (GLEW_ARB_clear_texture) {
            glm::ivec4 adjusted_region;
            glm::ivec2 adjusted_region_z;
            if (region == glm::ivec4(-1)) {
                adjusted_region = glm::ivec4(0, 0, width, height);
            }
            if (region_z == glm::ivec2(-1)) {
                adjusted_region_z = glm::ivec2(0, depth);
            }

            glClearTexSubImage(
                id, level,
                region.x, region.y, region_z.x,
                region.z, region.w, region_z.y,
                _format, _type,
                data);
        }
    }

    void bind() {
        this->texture_image_unit = GLContext::get_texturing_unit();

        glActiveTexture(texture_image_unit);
        glBindTexture(_texture_enum, id);
    }

    GLuint id;
    GLint texture_image_unit;
    int width, height, depth;
  private:

    void apply_parameter(const TextureParameter& parameter) {
        switch (parameter) {
            case WRAP_ST_CLAMP_TO_BORDER:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                break;
            case WRAP_ST_CLAMP_TO_EDGE:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            case WRAP_ST_REPEAT:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            case WRAP_ST_MIRRORED_REPEAT:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
            case FILTER_MIN_MAG_NEAREST:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
            case FILTER_MIN_MAG_LINEAR:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
        }
    }

    GLenum _texture_enum;

    GLenum _format, _type;
    GLint _internalFormat;
};

class Program {
  public:
    Program() {
        id = glCreateProgram();
    }

    bool compile_shader(const std::string& filename,
                        GLenum shader_type) {
        GLuint shader = glCreateShader(shader_type);

        std::ifstream t(filename);
        std::string source_str((std::istreambuf_iterator<char>(t)),
                               std::istreambuf_iterator<char>());

        const GLchar* source = (const GLchar*) source_str.c_str();
        glShaderSource(shader, 1, &source, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> info_log(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &info_log[0]);

            glDeleteShader(shader);

            ERROR("Failed to compile shader " << filename << "! Info log:\n\n");
            for (auto& a : info_log)
                std::cout << a;

            return false;
        }

        this->shader_ids.push_back(shader);

        glAttachShader(id, shader);

        return true;
    }

    bool link_program() {
        glLinkProgram(id);

        GLint isLinked = 0;
        glGetProgramiv(id, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> info_log(maxLength);
            glGetProgramInfoLog(id, maxLength, &maxLength, &info_log[0]);

            ERROR("Failed to link shader! Info log:\n\n");
            for (auto& a : info_log)
                std::cout << a;

            return false;
        }

        for (auto& shader : shader_ids)
            glDetachShader(id, shader);

        return true;
    }

    void bind() {
        glUseProgram(this->id);
    }

    template <typename T>
    void set_uniform(const std::string& name,
                     T value) {
        GLint location;
        if (uniform_cache.count(name))
            location = uniform_cache[name];
        else {
            location = glGetUniformLocation(id, name.c_str());
            uniform_cache[name] = location;
        }
        if (location == -1) {
            ERROR("Could not find uniform " << name << "!" <<
                  " Error: " << glGetError());
            exit(1);
        }
        _set_uniform(location, value);
    }

    ~Program() {
        glDeleteProgram(id);
        for (auto& shader : shader_ids)
            glDeleteShader(shader);
    }

  private:
    template <typename T>
    void _set_uniform(const GLint& location, T value) {
        ERROR("Don't know what to do with a uniform like " <<
              value << " bound at " << location);
    }

    void _set_uniform(const GLint& location, GLfloat value) {
        glUniform1f(location, value);
    }

    void _set_uniform(const GLint& location, glm::vec2 value) {
        glUniform2f(location, value.x, value.y);
    }

    void _set_uniform(const GLint& location, glm::vec3 value) {
        glUniform3f(location, value.x, value.y, value.z);
    }

    void _set_uniform(const GLint& location, glm::vec4 value) {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void _set_uniform(const GLint& location, GLint value) {
        glUniform1i(location, value);
    }

    void _set_uniform(const GLint& location, glm::ivec2 value) {
        glUniform2i(location, value.x, value.y);
    }

    void _set_uniform(const GLint& location, glm::ivec3 value) {
        glUniform3i(location, value.x, value.y, value.z);
    }

    void _set_uniform(const GLint& location, glm::ivec4 value) {
        glUniform4i(location, value.x, value.y, value.z, value.w);
    }

    void _set_uniform(const GLint& location, glm::mat2 value) {
        glUniformMatrix2fv(location, 1, GL_FALSE, &(value[0][0]));
    }

    void _set_uniform(const GLint& location, glm::mat3 value) {
        glUniformMatrix3fv(location, 1, GL_FALSE, &(value[0][0]));
    }

    void _set_uniform(const GLint& location, glm::mat4 value) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0]));
    }

    void _set_uniform(const GLint& location, Texture& value) {
        glUniform1i(location, value.texture_image_unit);
    }

    GLuint id;
    std::vector<GLint> shader_ids;
    std::unordered_map<std::string, GLint> uniform_cache;
};
