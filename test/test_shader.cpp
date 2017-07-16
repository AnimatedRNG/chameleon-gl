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

#include <unordered_map>

#include "catch.hpp"

#include "chameleon_gl.hpp"
#include "input.hpp"
#include "graphics_context.hpp"
#include "gl_context.hpp"
#include "shader.hpp"
#include "util.hpp"

STATIC_INIT()

TEST_CASE("shader function creation works", "[shader]") {
    InputController input;
    std::pair<int, int> viewport(100, 100);
    std::unordered_map<std::string, void*> options;
    std::string version = "4.3";
    options["width"] = &(viewport.first);
    options["height"] = &(viewport.second);
    options["opengl_version"] = &version;
    GraphicsContext context(input, options);

    std::string simple_example =
        read_file("test/test_shaders/shader_function_simple.glsl");
    std::string simple_example_with_comments =
        read_file("test/test_shaders/shader_function_comments.glsl");

    SECTION("creating functions") {
        ShaderFunction simple_example_func("test", simple_example);
        simple_example_func(SO(3.0F), SO(2.0F));
        assert(simple_example_func.get_function_call() ==
               "test(float(3), float(2))");
    }

    SECTION("creating functions with comments") {
        ShaderFunction simple_example_with_comments_func("test",
                simple_example_with_comments);
        simple_example_with_comments_func(SO(3.0F), SO(2.0F));
        assert(simple_example_with_comments_func.get_function_call() ==
               "test(float(3), float(2))");
    }
}
