#pragma once
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "util.hpp"

typedef struct {
    GLuint index;
    GLint vector_size;
    GLsizei stride;
    GLvoid* offset;
} VertexAttribute;

std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    out << "("
        << vec.x << ", " << vec.y << ", " << vec.z
        << ")";

    return out;
}

std::ostream& operator<< (std::ostream& out, const glm::vec4& vec) {
    out << "("
        << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
        << ")";

    return out;
}

std::ostream& operator<< (std::ostream& out, const VertexAttribute& va) {
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

std::ostream& operator<< (std::ostream& out, const glm::vec2& vec) {
    out << "("
        << vec.x << ", " << vec.y
        << ")";

    return out;
}

class VBO {
  public:
    VBO() {

    }

    VBO(const GLfloat* vertex_data,
        std::vector<VertexAttribute> vertex_attributes,
        GLuint num_vertices,
        GLenum primitive_type) :
        _vertex_data(vertex_data),
        _vertex_attributes(vertex_attributes),
        _primitive_type(primitive_type),
        _num_vertices(num_vertices) {
        glGenBuffers(1, &_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(GLfloat),
                     &(_vertex_data[0]), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
            glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);
            glVertexAttribPointer(
                i,
                va.vector_size,
                GL_FLOAT,
                GL_FALSE,
                va.stride,
                va.offset
            );
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
    GLuint _vertex_buffer;
    const GLfloat* _vertex_data;
    std::vector<VertexAttribute> _vertex_attributes;
    GLenum _primitive_type;
    GLuint _num_vertices;
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
        GLint location = glGetUniformLocation(id, name.c_str());
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

    GLuint id;
    std::vector<GLuint> shader_ids;
};
