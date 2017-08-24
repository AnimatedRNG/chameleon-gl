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

#include "command.hpp"
#include "uniform_map.hpp"
#include "opengl_utils.hpp"

class ComputeCommand : public Command {
  public:
    ComputeCommand(Program& program,
                   const glm::uvec3& workgroup_count,
                   UniformMap uniform_map = UniformMap()) :
        _program(program),
        _workgroup_count(workgroup_count),
        _uniform_map(uniform_map) {

    }

    void operator()() override {
        _program.bind();
        _uniform_map.apply(_program);
        _program.dispatch_compute(_workgroup_count);
        _uniform_map.post_render();
    }

  private:
    Program& _program;
    const glm::uvec3 _workgroup_count;
    UniformMap _uniform_map;
};
