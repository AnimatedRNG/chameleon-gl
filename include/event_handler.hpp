#pragma once

#include "SDL.h"

class EventHandler {
  public:
    virtual void on_event(SDL_Event& event,
                          SDL::window_params* wp) = 0;
    virtual void update() = 0;
};
