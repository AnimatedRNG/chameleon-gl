#pragma once

#include "SDL.h"

#include "util.hpp"

namespace SDL {
    typedef struct {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_GLContext gl_context;
        int width;
        int height;
    } window_params;
}
