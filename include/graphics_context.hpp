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
#include <thread>
#include <unordered_map>
#include <assert.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "SDL2/SDL.h"
#include "SDL/SDL_image.h"

#include "event_handler.hpp"
#include "sdl_helpers.hpp"
#include "gl_context.hpp"
#include "draw_command.hpp"
#include "renderer.hpp"

class GraphicsContext {
  public:
    GraphicsContext(EventHandler& ev_handler,
                    const std::unordered_map<std::string, void*>& options) :
        handler(ev_handler) {
        SDL_Init(SDL_INIT_EVERYTHING);
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

        this->wp.width = default_value("width", WIDTH, options);
        this->wp.height = default_value("height", HEIGHT, options);
        this->wp.window =
            SDL_CreateWindow("SDF Renderer", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             this->wp.width,
                             this->wp.height,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);


        set_gl_attributes(default_value("opengl_version", "4.3", options));
        this->wp.gl_context = SDL_GL_CreateContext(this->wp.window);
        if (this->wp.gl_context == NULL) {
            DEBUG("OpenGL context could not be created!");
            exit(1);
        }
        SDL_GL_MakeCurrent(this->wp.window, this->wp.gl_context);

        GLenum err = glewInit();
        if (err != GLEW_OK) {
            ERROR("GLEW could not be initialized!");
            exit(1);
        }

        if (!GLEW_VERSION_2_1) {
            ERROR("GLEW could not be initialized!");
            exit(1);
        }

        SDL_GL_SetSwapInterval(1);

        this->wp.renderer = SDL_CreateRenderer(this->wp.window, -1, 0);
        this->render_texture =
            SDL_CreateTexture(
                this->wp.renderer, SDL_PIXELFORMAT_RGB888,
                SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
        GLContext::gl_init();
    }

    GraphicsContext(EventHandler& ev_handler) :
        GraphicsContext(ev_handler,
                        std::unordered_map<std::string, void*>()) {
    }

    void start(Renderer& renderer) {
        for (long accum = 0; accum >= 0;) {
            long dt = mainloop(renderer);
            if (dt == -1)
                return;
            accum += dt;
        }
    }

    ~GraphicsContext() {
        SDL_GL_DeleteContext(this->wp.gl_context);
        SDL_DestroyTexture(render_texture);
        SDL_DestroyRenderer(this->wp.renderer);
        SDL_DestroyWindow(this->wp.window);
        IMG_Quit();
        SDL_Quit();
    }
  private:
    void set_gl_attributes(const std::string& version) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

        // TODO: Replace with something more elegant
        assert(version.size() == 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                            (int)(version[0] - '0'));
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                            (int)(version[2] - '0'));

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

    long mainloop(Renderer& renderer) {
        auto start = GET_TIME();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handler.on_event(event, &wp);
            if (event.type == SDL_QUIT)
                return -1;
        }

        GLContext::gl_refresh();
        float viewport[4] = {0, 0, WIDTH, HEIGHT};
        DrawCommand::set_uniform("chml_viewport", viewport);
        renderer(viewport[2], viewport[3]);
        SDL_GL_SwapWindow(this->wp.window);

        auto end = GET_TIME();
        unsigned long diff = DIFF(end, start);
        if (diff < IDEAL_FRAME_TIME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                                            IDEAL_FRAME_TIME - diff));
        }
        DEBUG("Frame time: " << diff);

        return diff;
    }

    template <typename T>
    T default_value(std::string name,
                    T default_value,
                    std::unordered_map<std::string, void*> options) {
        if (options.count(name) == 1) {
            return *((T*)(options[name]));
        } else {
            return default_value;
        }
    }

    SDL_Texture* render_texture;
    SDL::WindowParams wp;

    EventHandler& handler;

    constexpr static int WIDTH = 1600;
    constexpr static int HEIGHT = 900;
    constexpr static int IDEAL_FRAME_TIME = 1000 / 60;
};
