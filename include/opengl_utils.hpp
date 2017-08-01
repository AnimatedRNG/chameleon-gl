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

// TODO: Update EVERYTHING to use DSA
// Seriously, OpenGL is not usable without DSA

typedef struct {
    GLuint index;
    GLint vector_size;
    GLsizei stride;
    GLvoid* offset;
} VertexAttribute;

inline std::ostream& operator<< (std::ostream& out, const glm::bvec2& bvec) {
    out << "bvec2("
        << bvec.x << ", " << bvec.y
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::bvec3& bvec) {
    out << "bvec3("
        << bvec.x << ", " << bvec.y << ", " << bvec.z
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::bvec4& bvec) {
    out << "bvec4("
        << bvec.x << ", " << bvec.y << ", " << bvec.z << ", " << bvec.w
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec2& vec) {
    out << "vec2("
        << vec.x << ", " << vec.y
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    out << "vec3("
        << vec.x << ", " << vec.y << ", " << vec.z
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec4& vec) {
    out << "vec4("
        << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const float vec[4]) {
    out << "vec4("
        << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3]
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec2& ivec) {
    out << "ivec2("
        << ivec.x << ", " << ivec.y
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec3& ivec) {
    out << "ivec3("
        << ivec.x << ", " << ivec.y << ", " << ivec.z
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec4& ivec) {
    out << "ivec4("
        << ivec.x << ", " << ivec.y << ", " << ivec.z << ", " << ivec.w
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const int ivec[4]) {
    out << "ivec4("
        << ivec[0] << ", " << ivec[1] << ", " << ivec[2] << ", " << ivec[3]
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::uvec2& uvec) {
    out << "uvec2("
        << uvec.x << ", " << uvec.y
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::uvec3& uvec) {
    out << "uvec3("
        << uvec.x << ", " << uvec.y << ", " << uvec.z
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::uvec4& uvec) {
    out << "uvec4("
        << uvec.x << ", " << uvec.y << ", " << uvec.z << ", " << uvec.w
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::dvec2& dvec) {
    out << "dvec2("
        << dvec.x << ", " << dvec.y
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::dvec3& dvec) {
    out << "dvec3("
        << dvec.x << ", " << dvec.y << ", " << dvec.z
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::dvec4& dvec) {
    out << "dvec4("
        << dvec.x << ", " << dvec.y << ", " << dvec.z << ", " << dvec.w
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::mat2& mat2) {
    out << "mat2(\n"
        << "vec2(" << mat2[0][0] << ", " << mat2[0][1] << "),"
        << "\n "
        << "vec2(" << mat2[1][0] << ", " << mat2[1][1] << ")"
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::mat3& mat3) {
    out << "mat3(\n"
        << "vec3(" << mat3[0][0] << ", " << mat3[0][1] << ", " << mat3[0][2] << "),"
        << "\n "
        << "vec3(" << mat3[1][0] << ", " << mat3[1][1] << ", " << mat3[1][2] << "),"
        << "\n "
        << "vec3(" << mat3[2][0] << ", " << mat3[2][1] << ", " << mat3[2][2] << ")"
        << ")";

    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::mat4& mat4) {
    out << "mat4(\n"
        << "vec4(" << mat4[0][0] << ", " << mat4[0][1] << ", " << mat4[0][2] << ", " <<
        mat4[0][3] << "),"
        << "\n "
        << "vec4(" << mat4[1][0] << ", " << mat4[1][1] << ", " << mat4[1][2] << ", " <<
        mat4[1][3] << "),"
        << "\n "
        << "vec4(" << mat4[2][0] << ", " << mat4[2][1] << ", " << mat4[2][2] << ", " <<
        mat4[2][3] << "),"
        << "\n "
        << "vec4(" << mat4[3][0] << ", " << mat4[3][1] << ", " << mat4[3][2] << ", " <<
        mat4[3][3] << ")"
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

class Buffer {
  public:
    Buffer() :
        target(GL_ARRAY_BUFFER) {
        //glGenBuffers(1, &id);
        glCreateBuffers(1, &id);
    }

    Buffer(const Buffer& other) :
        id(other.id),
        target(other.target) {
    }

    ~Buffer() {
        unbind();
    }

    void load(const GLenum& target,
              void* data,
              const size_t& num_bytes,
              const GLenum& usage_type = GL_STATIC_COPY) {
        //bool not_bound = GLContext::is_buffer_bound(id);
        //bind(target);
        /*glBufferData(target,
                     num_bytes,
                     data,
                     usage_type);*/
        glNamedBufferData(id,
                          num_bytes,
                          data,
                          usage_type);
        /*if (not_bound) {
            unbind();
        }*/
    }

    void update(const GLenum& target,
                void* data,
                const size_t& num_bytes) {
        load(target, data, num_bytes, GL_DYNAMIC_COPY);
    }

    void bind(const GLenum& target) {
        this->target = target;
        if (GLContext::add_bound_buffer(target)) {
            glBindBuffer(target, id);
        }
    }

    void unbind() {
        if (GLContext::remove_bound_buffer(target)) {
            glBindBuffer(target, 0);
        }
    }

    GLuint id;
    GLenum target;
};

class VBO {
  public:
    VBO() {

    }

    VBO(const std::vector<GLfloat*>& vertex_data,
        const std::vector<VertexAttribute>& vertex_attributes,
        GLuint num_vertices,
        GLenum primitive_type) :
        _vertex_buffer(),
        _vertex_attributes(vertex_attributes),
        _primitive_type(primitive_type),
        _num_vertices(num_vertices) {
        for (size_t i = 0; i < vertex_data.size(); i++) {
            Buffer vertex_buffer;
            //vertex_buffer.bind(GL_ARRAY_BUFFER);

            auto va = vertex_attributes[i];
            /*glBufferData(GL_ARRAY_BUFFER,
                         num_vertices * va.vector_size * sizeof(GLfloat),
                         &(vertex_data[i][0]),
                         GL_STATIC_DRAW);*/
            glNamedBufferData(vertex_buffer.id,
                              num_vertices * va.vector_size * sizeof(GLfloat),
                              &(vertex_data[i][0]),
                              GL_STATIC_DRAW);
            //vertex_buffer.unbind();
            _vertex_buffer.push_back(vertex_buffer);
        }
        assert(_vertex_attributes.size() == vertex_data.size());
    }

    VBO(const VBO& other) {
        this->_num_vertices = other._num_vertices;
        this->_vertex_buffer = std::vector<Buffer>(other._vertex_buffer);
        this->_primitive_type = other._primitive_type;
        this->_vertex_attributes = other._vertex_attributes;
    }

    ~VBO() {

    }

    bool draw(GLuint& vao) {
        std::unordered_set<GLuint> indices;
        for (size_t i = 0; i < _vertex_attributes.size(); i++) {
            const VertexAttribute va = _vertex_attributes[i];

            if (indices.count(va.index)) {
                ERROR("Vertex attribute " << va <<
                      " has an index conflict with another vertex attribute!");
                return false;
            }

            // Eventually replace this with something more DSA
            glEnableVertexArrayAttrib(vao, va.index);
            _vertex_buffer[i].bind(GL_ARRAY_BUFFER);
            glVertexAttribPointer(
                i,
                va.vector_size,
                GL_FLOAT,
                GL_FALSE,
                va.stride,
                va.offset
                );
            _vertex_buffer[i].unbind();
            indices.insert(va.index);
        }

        glDrawArrays(_primitive_type, 0, _num_vertices);

        for (size_t i = 0; i < _vertex_attributes.size(); i++) {
            const VertexAttribute va = _vertex_attributes[i];

            glDisableVertexArrayAttrib(vao, va.index);
        }

        return true;
    }
  private:
    std::vector<Buffer> _vertex_buffer;
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
    // Don't use this constructor. Creates zombie object.
    Texture() {}

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
        this->format = format;
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
                format, _type,
                data);
        }
    }

    void bind(const bool& active_texture = true) {
        if (active_texture) {
            this->texture_image_unit = GLContext::get_texturing_unit();
            glActiveTexture(texture_image_unit);
        }

        glBindTexture(_texture_enum, id);
    }

    GLuint id;
    GLint texture_image_unit;
    int width, height, depth;

    GLenum format;
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

    GLenum _type;
    GLint _internalFormat;
};

class Program {
  public:
    Program() :
        ssbo_binding_map(),
        last_ssbo_binding_point(0) {
        id = glCreateProgram();
    }

    bool operator==(const Program& other) const {
        return (this->id == other.id);
    }

    ~Program() {
        glDeleteProgram(id);
        for (auto& shader : shader_ids)
            glDeleteShader(shader);
    }

    std::string compile_shader(const std::string& filename_or_str,
                               GLenum shader_type,
                               const bool& is_filename = true,
                               const bool& error_log = false) {
        GLuint shader = glCreateShader(shader_type);

        const GLchar* source;
        std::string source_str;

        if (is_filename) {
            std::ifstream t(filename_or_str);
            source_str = std::string((std::istreambuf_iterator<char>(t)),
                                     std::istreambuf_iterator<char>());
            source = (const GLchar*) source_str.c_str();
        } else {
            source = filename_or_str.c_str();
        }

        glShaderSource(shader, 1, &source, 0);

        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        std::string log;
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> info_log(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &info_log[0]);

            glDeleteShader(shader);

            if (is_filename) {
                log = "Failed to compile shader " + filename_or_str
                      + "! Info log:\n\n";
            } else {
                log = "Failed to compile shader! Info log:\n\n";
            }
            for (auto& a : info_log)
                log += a;

            if (error_log) {
                ERROR(log);
            }

            return log;
        }

        this->shader_ids.push_back(shader);

        glAttachShader(id, shader);

        return log;
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


    void attach_ssbo(Buffer& buffer,
                     const std::string& buffer_name) {
        GLuint block_index = 0;
        block_index = glGetProgramResourceIndex(id,
                                                GL_SHADER_STORAGE_BLOCK,
                                                buffer_name.c_str());

        ssbo_binding_map[buffer.id] = last_ssbo_binding_point++;

        // Cache stuff so that this gets more efficient
        glShaderStorageBlockBinding(id,
                                    block_index,
                                    ssbo_binding_map[buffer.id]);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
                         ssbo_binding_map[buffer.id],
                         buffer.id);
    }

    void remove_ssbo(Buffer& buffer) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
                         ssbo_binding_map[buffer.id],
                         0);
    }

    void dispatch_compute(const glm::uvec3& workgroup_count) {
        glDispatchCompute(workgroup_count.x,
                          workgroup_count.y,
                          workgroup_count.z);
    }

    template <typename T>
    void set_uniform(const std::string& name,
                     T value,
                     const bool& validate = true) {
        GLint location;
        if (uniform_cache.count(name))
            location = uniform_cache[name];
        else {
            location = glGetUniformLocation(id, name.c_str());
            uniform_cache[name] = location;
        }
        if (location == -1 && validate) {
            ERROR("Could not find uniform " << name << "!" <<
                  " Error: " << glGetError());
            exit(1);
        }
        _set_uniform(location, value);
    }

    GLuint id;
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

    void _set_uniform(const GLint& location, float value[4]) {
        glUniform4f(location, value[0], value[1], value[2], value[3]);
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

    void _set_uniform(const GLint& location, int value[4]) {
        glUniform4i(location, value[0], value[1], value[2], value[3]);
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

    std::vector<GLint> shader_ids;
    std::unordered_map<std::string, GLint> uniform_cache;
    std::unordered_map<GLuint, GLuint> ssbo_binding_map;
    int last_ssbo_binding_point;
};

class Framebuffer {
  public:
    explicit Framebuffer() :
        textures(),
        draw_buffers(),
        width(-1),
        height(-1) {
        glGenFramebuffers(1, &id);
    }

    explicit Framebuffer(const int w, const int h, const bool& typical,
                         const GLenum& format = GL_RGBA,
                         const GLenum& internal_format = GL_RGBA32F,
                         const GLenum& type = GL_UNSIGNED_BYTE) :
        textures(),
        draw_buffers(),
        width(w),
        height(h) {
        glGenFramebuffers(1, &id);
        if (typical) {
            typical_fbo(format, internal_format, type);
        }
    }

    void on_resize(const int& width, const int& height) {
        this->width = width;
        this->height = height;
    }

    void bind_textures(std::unordered_map<std::string, Texture> attachments) {
        int index = 0;
        for (auto attachment = attachments.begin();
                attachment != attachments.end();
                attachment++) {
            if (attachment->second.format != GL_DEPTH_COMPONENT) {
                GLenum attachment_point = get_color_attachment_point(index++);
                bind_texture(attachment->first,
                             attachment_point,
                             attachment->second);
            } else {
                bind_texture(attachment->first,
                             GL_DEPTH_ATTACHMENT,
                             attachment->second);
            }
        }
    }

    void bind_texture(const std::string name,
                      const GLenum& attachment_point,
                      Texture& texture,
                      const int& level = 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        texture.bind(false);
        glFramebufferTexture(GL_FRAMEBUFFER,
                             attachment_point,
                             texture.id,
                             level);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        assert(get_attachment_name(attachment_point) == std::string());
        draw_buffers[name] = attachment_point;
        textures[attachment_point] = texture;

        update_draw_buffers();
    }

    void create_renderbuffer(const int& width,
                             const int& height) {
        GLuint depthrenderbuffer;
        glGenRenderbuffers(1, &depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER,
                              GL_DEPTH_COMPONENT,
                              width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER,
                                  depthrenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        assert(get_attachment_name(GL_DEPTH_ATTACHMENT) == std::string());
        draw_buffers["renderbuffer"] = GL_DEPTH_ATTACHMENT;
        update_draw_buffers();
    }

    Texture get_texture(const GLenum& attachment) {
        if (textures.count(attachment) == 1) {
            return textures[attachment];
        } else {
            throw std::runtime_error(
                "Framebuffer does not have attachment");
        }
    }

    Texture get_texture(const std::string& name) {
        if (draw_buffers.count(name) == 1) {
            return get_texture(draw_buffers[name]);
        } else {
            throw std::runtime_error(
                "Framebuffer does not have texture " + name);
        }
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
        glViewport(0, 0, width, height);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void validate() {
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
               GL_FRAMEBUFFER_COMPLETE);
    }

    void typical_fbo(const GLenum& format = GL_RGBA,
                     const GLenum& internal_format = GL_RGBA32F,
                     const GLenum& type = GL_FLOAT) {
        std::unordered_map<std::string, Texture> attachments;
        Texture color_texture(GL_TEXTURE_2D,
                              width,
                              height);
        Texture depth_texture(GL_TEXTURE_2D,
                              width,
                              height);

        auto tex_params =
        TextureParameterSet({
            WRAP_ST_CLAMP_TO_BORDER, FILTER_MIN_MAG_NEAREST
        });
        color_texture.init(tex_params,
                           0,
                           internal_format,
                           format,
                           type,
                           NULL,
                           false);
        depth_texture.init(tex_params,
                           0,
                           GL_DEPTH_COMPONENT32,
                           GL_DEPTH_COMPONENT,
                           GL_FLOAT,
                           NULL,
                           false);
        attachments["color"] = color_texture;
        attachments["depth"] = depth_texture;
        bind_textures(attachments);
    }

    GLuint id;
    std::unordered_map<GLenum, Texture> textures;
    std::unordered_map<std::string, GLenum> draw_buffers;
    int width;
    int height;

  private:
    std::string get_attachment_name(const GLenum& attachment) {
        for (auto it = draw_buffers.begin(); it != draw_buffers.end(); it++) {
            if (it->second == attachment)
                return it->first;
        }
        return std::string();
    }

    void update_draw_buffers() {
        std::vector<GLenum> tmp;
        for (auto it = draw_buffers.begin(); it != draw_buffers.end(); it++)
            tmp.push_back(it->second);
        glDrawBuffers(tmp.size(), &tmp[0]);
    }


    GLenum get_color_attachment_point(const int& index) {
        switch (index) {
            case 0:
                return GL_COLOR_ATTACHMENT0;
            case 1:
                return GL_COLOR_ATTACHMENT1;
            case 2:
                return GL_COLOR_ATTACHMENT2;
            case 3:
                return GL_COLOR_ATTACHMENT3;
            case 4:
                return GL_COLOR_ATTACHMENT4;
            case 5:
                return GL_COLOR_ATTACHMENT5;
            case 6:
                return GL_COLOR_ATTACHMENT6;
            case 7:
                return GL_COLOR_ATTACHMENT7;
            case 8:
                return GL_COLOR_ATTACHMENT8;
            case 9:
                return GL_COLOR_ATTACHMENT9;
            case 10:
                return GL_COLOR_ATTACHMENT10;
            case 11:
                return GL_COLOR_ATTACHMENT11;
            case 12:
                return GL_COLOR_ATTACHMENT12;
            case 13:
                return GL_COLOR_ATTACHMENT13;
            case 14:
                return GL_COLOR_ATTACHMENT14;
            case 15:
                return GL_COLOR_ATTACHMENT15;
            default:
                throw std::runtime_error("Invalid color attachment!");
        }
    }
};
