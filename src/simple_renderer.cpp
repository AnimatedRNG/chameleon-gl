#include "simple_renderer.hpp"

SDFRenderer::SDFRenderer(InputController& controller) :
    projection_matrix(),
    view_matrix(glm::translate(glm::mat4(1), glm::vec3(0, 0, -10))),
    program(),
    ctrl(controller) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    program.compile_shader("shaders/simple_shader.vs", GL_VERTEX_SHADER);
    program.compile_shader("shaders/simple_shader.fs", GL_FRAGMENT_SHADER);
    program.link_program();

    const std::vector<VertexAttribute> attribs({{0, 3, 0, 0}});
    this->vbo = VBO(SDFRenderer::quad_vertex_buffer_data,
                    attribs,
                    sizeof(SDFRenderer::quad_vertex_buffer_data) /
                    sizeof(SDFRenderer::quad_vertex_buffer_data[0]),
                    GL_TRIANGLES);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0, 0.0, 0.0, 1.0);
}

inline float SDFRenderer::sdf(const glm::vec4& location) const {
    return fancy_torus_sdf(location, glm::vec2(3, 5));
}

inline float SDFRenderer::sphere_sdf(const glm::vec4& location,
                                     const float& radius) const {
    return glm::length(location) - radius;
}

inline float SDFRenderer::fancy_torus_sdf(
    const glm::vec4& location,
    const glm::vec2& t) const {
    glm::vec2 q(length8(glm::vec2(location.x, location.z)) - t.x,
                location.y);

    return length8(q) - t.y;
}

void SDFRenderer::operator()(const int& width,
                             const int& height) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    this->projection_matrix = ctrl.get_projection();
    this->view_matrix = ctrl.get_view();

    glm::vec4 viewport(0, 0, width, height);
    glm::vec3 origin(glm::inverse(view_matrix) * glm::vec4(0, 0, 0, 1));
    glm::mat4 inverse(glm::inverse(projection_matrix * view_matrix));

    program.set_uniform("origin", origin);
    program.set_uniform("inv", inverse);
    program.set_uniform("viewport", viewport);

    program.set_uniform("NEAR", NEAR);
    //program.set_uniform("FAR", FAR);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    program.bind();
    this->vbo.draw();
}

int main(int argc, char** args) {
    InputController input;
    GraphicsContext context(input);
    SDFRenderer renderer(input);
    context.start(renderer);
}
