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

#include <vector>
#include <exception>
#include <assert.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "util.hpp"
#include "opengl_utils.hpp"
#include "drawable.hpp"

template <typename POSITION, typename NORMAL, typename COLOR>
struct PointT {
    POSITION position;
    NORMAL normal;
    COLOR color;
};

using Point = PointT<glm::vec4, void*, void*>;
using PointNormal = PointT<glm::vec4, glm::vec4, void*>;
using PointColor = PointT<glm::vec4, void*, glm::vec4>;
using PointNormalColor = PointT<glm::vec4, glm::vec4, glm::vec4>;

class PointCloud : public Drawable {
  public:
    PointCloud() {
    }

    PointCloud(VAO& vao) : PointCloud() {
        load_from_vao(vao);
    }

    // TODO: Make a fully generic version of this
    void init(std::vector<Point> points) {
        const std::vector<VertexAttribute> attribs({
            {0, 4, 0, 0}
        });
        std::vector<GLfloat*> vertex_data;
        vertex_data.push_back(&(points[0].position.x));
        this->vao = VAO(vertex_data,
                        attribs,
                        points.size(),
                        GL_POINTS);
    }

    void load_from_vao(VAO& vao) {
        this->vao = vao;
    }

    virtual void on_draw() override {

    };

    virtual VAO get_vao() override {
        return vao;
    };

  private:
    VAO vao;
};
