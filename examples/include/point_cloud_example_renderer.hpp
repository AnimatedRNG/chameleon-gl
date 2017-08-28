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
#include <utility>
#include <vector>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "util.hpp"
#include "opengl_utils.hpp"
#include "point_cloud.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "command.hpp"
#include "draw_command.hpp"
#include "clear_command.hpp"
#include "uniform_map.hpp"

class PointCloudExampleRenderer : public Renderer {
  public:
    explicit PointCloudExampleRenderer() :
        program(),
        render_state( {
        GL_MULTISAMPLE, GL_DITHER, GL_DEPTH_TEST
    }),
    point_cloud(),
    frame_count(0) {
        srand((int) time(0));

        program.compile_shader("shaders/simple_shader.vs", GL_VERTEX_SHADER,
                               true, true);
        program.compile_shader("shaders/simple_point_shader.fs", GL_FRAGMENT_SHADER,
                               true, true);
        program.link_program();

        auto points = random_data(1000);
        for (auto point : points) {
            DEBUG(point.position);
        }
        point_cloud.init(points);

        render_state.set_param(DepthFunction({GL_LESS}));
    }

    virtual CommandList operator()(AbstractSurfacePtr surface) override {
        CommandPtr clear_screen(new ClearCommand(surface,
                                ClearCommand::CLEAR_COLOR |
                                ClearCommand::CLEAR_DEPTH,
                                glm::vec4(0.0)));

        UniformMap render_map;
        CommandPtr render(new DrawCommand(point_cloud,
                                          program,
                                          surface,
                                          render_map,
                                          render_state));

        return CommandList({clear_screen, render});
    }

  private:
    std::vector<Point> random_data(const int& num_elements) {
        int size = num_elements;
        std::vector<Point> data;
        data.reserve(size);
        for (int i = 0; i < size; i += 4) {
            glm::vec4 position;
            position.x = (float)(rand() % GRANULARITY - (GRANULARITY / 2)) /
                        (GRANULARITY / 2.0);
            position.y = (float)(rand() % GRANULARITY - (GRANULARITY / 2)) /
                        (GRANULARITY / 2.0);
            position.z = (float)(rand() % GRANULARITY - (GRANULARITY / 2)) /
                        (GRANULARITY / 2.0);
            position.w = 0;
            data.push_back(Point({position, nullptr, nullptr}));
        }
        return data;
    }

    Program program;
    RenderState render_state;
    PointCloud point_cloud;
    int frame_count;
    constexpr static int GRANULARITY = 10000;
};
