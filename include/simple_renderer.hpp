#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "SDL.h"
#include "SDL_image.h"

const std::string red("\033[1m\033[31m");
const std::string reset_code("\033[0m");

#define GET_TIME()       std::chrono::duration_cast<std::chrono::milliseconds>\
    (std::chrono::system_clock::now().time_since_epoch())
#define DIFF(a, b)       (a - b).count()
#define DEBUG(a)         std::cout << a << std::endl
#define ERROR(a)         std::cerr << red << a << reset_code << std::endl

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

typedef struct {
    GLuint index;
    GLint vector_size;
    GLsizei stride;
    GLvoid* offset;
} VertexAttribute;

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

    ~Program() {
        glDeleteProgram(id);
        for (auto& shader : shader_ids)
            glDeleteShader(shader);
    }

  private:
    GLuint id;
    std::vector<GLuint> shader_ids;
};

class Renderer {
  public:
    virtual void operator()(const int& width,
                            const int& height) = 0;
};

class SDFRenderer : public Renderer {
  public:
    explicit SDFRenderer();

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

class GraphicsContext {
  public:
    GraphicsContext() {
        SDL_Init(SDL_INIT_EVERYTHING);
        this->window =
            SDL_CreateWindow("SDF Renderer", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        set_gl_attributes();
        this->context = SDL_GL_CreateContext(this->window);
        if (this->context == NULL) {
            DEBUG("OpenGL context could not be created!");
            exit(1);
        }
        SDL_GL_MakeCurrent(this->window, this->context);

        GLenum err = glewInit();
        if (err != GLEW_OK) {
            ERROR("GLEW could not be initialized!");
            exit(1);
        }

        if (!GLEW_VERSION_2_1) {
            ERROR("GLEW could not be initialized!");
            exit(1);
        }

        SDL_GL_SetSwapInterval(1);

        this->renderer = SDL_CreateRenderer(window, -1, 0);
        this->render_texture =
            SDL_CreateTexture(
                renderer, SDL_PIXELFORMAT_RGB888,
                SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    }

    void start(Renderer& renderer) {
        for (long accum = 0; accum < 5000;) {
            long dt = mainloop(renderer);
            if (dt == -1)
                return;
            accum += dt;
        }
    }

    ~GraphicsContext() {
        SDL_GL_DeleteContext(context);
        SDL_DestroyTexture(render_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
  private:
    void set_gl_attributes() {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

    long mainloop(Renderer& renderer) {
        auto start = GET_TIME();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return -1;
        }

        // Handle input here

        SDL_Rect viewport = {0, 0, WIDTH, HEIGHT};
        renderer(viewport.w, viewport.h);
        SDL_GL_SwapWindow(this->window);

        auto end = GET_TIME();
        unsigned long diff = DIFF(end, start);
        if (diff < IDEAL_FRAME_TIME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                                            IDEAL_FRAME_TIME - diff));
        }
        DEBUG("Frame time: " << diff);

        return diff;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* render_texture;
    SDL_GLContext context;

    constexpr static int WIDTH = 640;
    constexpr static int HEIGHT = 480;
    constexpr static int IDEAL_FRAME_TIME = 1000 / 60;
};

std::ostream& operator<< (std::ostream& out, const glm::vec2& vec) {
    out << "("
        << vec.x << ", " << vec.y
        << ")";

    return out;
}
