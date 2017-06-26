#pragma once

#include <iostream>
#include <thread>

#include "event_handler.hpp"
#include "sdl_helpers.hpp"

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "renderer.hpp"

class GraphicsContext {
  public:
    GraphicsContext(EventHandler& ev_handler) :
        handler(ev_handler) {
        SDL_Init(SDL_INIT_EVERYTHING);
        this->wp.window =
            SDL_CreateWindow("SDF Renderer", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        this->wp.width = WIDTH;
        this->wp.height = HEIGHT;

        set_gl_attributes();
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
        SDL_Quit();
    }
  private:
    void set_gl_attributes() {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

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

        SDL_Rect viewport = {0, 0, WIDTH, HEIGHT};
        renderer(viewport.w, viewport.h);
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

    SDL_Texture* render_texture;
    SDL::window_params wp;

    EventHandler& handler;

    constexpr static int WIDTH = 1600;
    constexpr static int HEIGHT = 900;
    constexpr static int IDEAL_FRAME_TIME = 1000 / 60;
};
