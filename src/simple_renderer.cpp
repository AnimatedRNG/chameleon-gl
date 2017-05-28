#include <iostream>
#include <algorithm>
#include "simple_renderer.hpp"

SDFRenderer::SDFRenderer() :
    projection_matrix(),
    view_matrix(glm::inverse(glm::translate(glm::mat4(1), glm::vec3(0, 0, -5)))) {

}

inline float SDFRenderer::sdf(const glm::vec4& location) const {
    return fancy_torus_sdf(location, glm::vec2(2, 1));
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

void SDFRenderer::operator()(uint32_t* bytes,
                             const int& pitch,
                             const int& width,
                             const int& height) {
    this->projection_matrix = glm::perspective(FOVY,
                              (float) width / (float) height,
                              NEAR,
                              FAR);
    glm::vec4 viewport(0, 0, width, height);

    glm::vec3 origin(glm::inverse(view_matrix) * glm::vec4(0, 0, 0, 1));
    glm::mat4 inverse(glm::inverse(projection_matrix * view_matrix));

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int offset = j * (pitch / sizeof(uint32_t)) + i;
            glm::vec4 world_space_vec((i - viewport[0]) / viewport[2],
                                      ((height - j) - viewport[1]) / viewport[3],
                                      1,
                                      1);
            world_space_vec = world_space_vec * 2.0f - 1.0f;

            glm::vec4 tmp = inverse * world_space_vec;
            tmp /= tmp.w;
            tmp -= glm::vec4(origin, 0);
            glm::vec3 ray_vec(tmp);
            ray_vec = glm::normalize(ray_vec);

            glm::vec3 reprojected_1 = glm::vec3(glm::unProject(glm::vec3(i, height - j,
                                                0.1),
                                                view_matrix, projection_matrix, viewport));
            glm::vec3 reprojected_2 = glm::vec3(glm::unProject(glm::vec3(i, height - j,
                                                0.2),
                                                view_matrix, projection_matrix, viewport));
            glm::vec3 r2(glm::normalize(reprojected_2 - reprojected_1));
            glm::vec3 r3(glm::normalize(reprojected_1 - origin));
            glm::vec3 r4(origin + 1.0f * ray_vec);
            glm::vec3 re_rasterized =
                glm::project(
                    r4,
                    view_matrix,
                    projection_matrix,
                    viewport);
            if (glm::length(glm::vec2(re_rasterized) - glm::vec2(i, height - j)) > 0.2) {
                DEBUG("Original: " << glm::vec2(i, height - j) <<
                      "; rasterized: " << re_rasterized);
            }
            DEBUG("r2: " << r2 << " ray_vec: " << ray_vec << " reprojected_1: " <<
                  reprojected_1 << " r3: " << r3);

            glm::vec3 current_point = origin + ray_vec * NEAR;
            float radius = 0;
            for (int k = 0; k < 0; k++) {
                radius = sdf(glm::vec4(current_point, 1));
                current_point += ray_vec * radius;
            }

            if (radius < 1) {
                bytes[offset] = (unsigned char)
                                std::max(radius * 256, (float) 255.0);
            }
        }
    }
}

int main(int argc, char** args) {
    GraphicsContext context;
    SDFRenderer renderer;
    context.start(renderer);
}
