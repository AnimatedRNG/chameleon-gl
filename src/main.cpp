#include "gl_context.hpp"
#include "input.hpp"
#include "graphics_context.hpp"
#include "simple_renderer.hpp"
#include "mesh_renderer.hpp"
#include "mesh.hpp"
#include "texture_renderer.hpp"
#include "opengl_utils.hpp"

STATIC_INIT()

int main(int argc, char** args) {
    InputController input;
    GraphicsContext context(input);
    Mesh mesh;
    mesh.load("assets/sculpt.obj");
    ImageData data = GLContext::get_image(
                         GLContext::load_image("assets/tex_0.jpg"));
    Texture texture("assets/tex_0.jpg",
                    GL_RGBA32F,
                    TextureParameterSet({WRAP_ST_CLAMP_TO_BORDER, FILTER_MIN_MAG_NEAREST
                                        }));

    //SDFRenderer renderer(input);
    MeshRenderer renderer(input, mesh);
    //TextureRenderer renderer(input, texture);
    context.start(renderer);
}
