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

#include <unordered_map>
#include <functional>

#include "drawable.hpp"
#include "uniform_map.hpp"
#include "opengl_utils.hpp"

#define DRAW_STATIC_INIT() \
    std::vector<std::reference_wrapper<Program>> DrawCommand::_programs;

class DrawCommand {
  public:
    DrawCommand(Drawable& drawable,
                Program& program,
                UniformMap uniform_map = UniformMap(),
                Framebuffer framebuffer = Framebuffer()) :
        _drawable(drawable),
        _program(program),
        _framebuffer(framebuffer),
        _uniform_map(uniform_map),
        _use_framebuffer(false) {
        for (Program& program : _programs) {
            if (program == _program)
                return;
        }

        this->_use_framebuffer = (framebuffer.textures->size() > 0);

        _programs.push_back(std::ref(program));
    }

    void operator()() {
        _program.bind();
        _drawable.on_draw();
        VAO vao = _drawable.get_vao();

        if (_use_framebuffer) {
            _framebuffer.bind();
        }

        _uniform_map.apply(_program);
        vao.draw();
        _uniform_map.post_render();

        if (_use_framebuffer) {
            _framebuffer.unbind();
        }
    }

    template <typename T>
    static void set_uniform(const std::string& name,
                            T value) {
        for (Program& program : _programs) {
            program.set_uniform(name, value, false);
        }
    }

    static void exec(DrawCommand& drawCommand) {
        drawCommand();
    }

  private:
    Drawable& _drawable;
    Program& _program;
    Framebuffer _framebuffer;
    UniformMap _uniform_map;

    bool _use_framebuffer;

    static std::vector<std::reference_wrapper<Program>> _programs;
};
