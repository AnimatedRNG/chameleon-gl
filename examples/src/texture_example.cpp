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

#include "gl_context.hpp"
#include "input.hpp"
#include "graphics_context.hpp"
#include "texture_renderer.hpp"
#include "opengl_utils.hpp"

STATIC_INIT()

int main(int argc, char** args) {
    InputController input;
    GraphicsContext context(input);
    ImageData data = GLContext::get_image(
                         GLContext::load_image("assets/tex_0.jpg"));
    Texture texture("assets/tex_0.jpg",
                    GL_RGBA32F,
    TextureParameterSet({
        WRAP_ST_CLAMP_TO_BORDER, FILTER_MIN_MAG_NEAREST
    }));

    TextureRenderer renderer(input, texture);
    context.start(renderer);
}
