#pragma once
#define TINYOBJLOADER_IMPLEMENTATION

#include <vector>
#include <exception>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "tiny_obj_loader.h"

#include "util.hpp"
#include "opengl_utils.hpp"

// Unoptimized mesh implementation
class Mesh {
  public:
    Mesh() {
        glGenVertexArrays(1, &vao);
    }

    Mesh(const std::string filename) : Mesh() {
        load(filename);
    }

    Mesh(VBO& vbo) : Mesh() {
        load_from_vbo(vbo);
    }

    void load(const std::string& filename) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                                    filename.c_str(), "assets/",
                                    true);
        if (!err.empty()) {
            throw std::runtime_error("Error in tinyobj: " + err);
        }

        DEBUG("Loading model...");

        std::vector<glm::vec4> positions;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec4> normals;

        for (size_t s = 0; s < shapes.size(); s++) {
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                int fv = shapes[s].mesh.num_face_vertices[f];
                assert(fv == 3);

                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    glm::vec4 v_pos;
                    v_pos.x = attrib.vertices[3 * idx.vertex_index + 0];
                    v_pos.y = attrib.vertices[3 * idx.vertex_index + 1];
                    v_pos.z = attrib.vertices[3 * idx.vertex_index + 2];
                    v_pos.w = 0.0;

                    glm::vec4 v_normal;
                    v_normal.x = attrib.vertices[3 * idx.normal_index + 0];
                    v_normal.y = attrib.vertices[3 * idx.normal_index + 1];
                    v_normal.z = attrib.vertices[3 * idx.normal_index + 2];
                    v_normal.w = 0.0;

                    glm::vec2 v_texcoord;
                    v_texcoord.x = attrib.vertices[3 * idx.texcoord_index + 0];
                    v_texcoord.y = attrib.vertices[3 * idx.texcoord_index + 1];

                    positions.push_back(v_pos);
                    normals.push_back(v_normal);
                    uvs.push_back(v_texcoord);
                }
                DEBUG("");
                index_offset += fv;
            }
        }
        DEBUG("Loading complete!");

        const std::vector<VertexAttribute> attribs({
            {0, 4, 0, 0},
            {1, 4, 0, 0},
            {2, 2, 0, 0},
        });
        std::vector<GLfloat*> vertex_data;
        vertex_data.push_back(&(positions[0].x));
        vertex_data.push_back(&(normals[0].x));
        vertex_data.push_back(&(uvs[0].x));
        glBindVertexArray(vao);
        this->vbo = VBO(vertex_data,
                        attribs,
                        positions.size(),
                        GL_TRIANGLES);
    }

    void load_from_vbo(VBO& vbo) {
        this->vbo = vbo;
    }

    static Mesh construct_fullscreen_quad() {
        const std::vector<VertexAttribute> attribs({{0, 3, 0, 0}});
        std::vector<GLfloat*> vertex_data;
        vertex_data.push_back((GLfloat*) GLContext::quad_vertex_buffer_data);
        VBO vbo(vertex_data,
                attribs,
                sizeof(GLContext::quad_vertex_buffer_data) /
                sizeof(GLContext::quad_vertex_buffer_data[0]),
                GL_TRIANGLES);
        return Mesh(vbo);
    }

    void draw() {
        glBindVertexArray(vao);
        this->vbo.draw();
    }

  private:
    GLuint vao;
    VBO vbo;
};
