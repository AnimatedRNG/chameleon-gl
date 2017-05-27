#pragma once

#include <iostream>
#include <chrono>
#include <thread>

#include "SDL.h"
#include "SDL_image.h"

#define GET_TIME()       std::chrono::duration_cast<std::chrono::milliseconds>\
    (std::chrono::system_clock::now().time_since_epoch())
#define DIFF(a, b)       (a - b).count()
#define DEBUG(a)         std::cout << a << std::endl

class Renderer {
  public:
    virtual void operator()(uint32_t* bytes,
                            const int& pitch,
                            const int& width,
                            const int& height) = 0;
};

class SDFRenderer : public Renderer {
  public:
    SDFRenderer();

    virtual void operator()(uint32_t* bytes,
                            const int& pitch,
                            const int& width,
                            const int& height) override;
};

class GraphicsContext {
  public:
    GraphicsContext() {
        SDL_Init(SDL_INIT_EVERYTHING);
        this->window =
            SDL_CreateWindow("SDF Renderer", SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                             SDL_WINDOW_SHOWN);
        this->renderer = SDL_CreateRenderer(window, -1, 0);
        this->render_texture =
            SDL_CreateTexture(
                renderer, SDL_PIXELFORMAT_RGB888,
                SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    }

    void start(Renderer& renderer) {
        while (mainloop(renderer));
    }

    ~GraphicsContext() {
        SDL_DestroyTexture(render_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
  private:
    bool mainloop(Renderer& renderer) {
        auto start = GET_TIME();
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
            return false;

        // Handle input here

        const SDL_Rect rect {0, 0, WIDTH, HEIGHT};
        uint32_t * bytes = nullptr;
        int pitch = 0;
        int result = SDL_LockTexture(
                         render_texture, nullptr,
                         reinterpret_cast<void**>(&bytes),
                         &pitch);
        renderer(bytes, pitch, rect.w, rect.h);
        SDL_UnlockTexture(render_texture);

        SDL_RenderCopy(this->renderer, render_texture, NULL, &rect);
        SDL_RenderPresent(this->renderer);

        auto end = GET_TIME();
        unsigned long diff = DIFF(end, start);
        if (diff < IDEAL_FRAME_TIME) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                                            IDEAL_FRAME_TIME - diff));
        }
        DEBUG("Frame time: " << diff);

        return true;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* render_texture;

    constexpr static int WIDTH = 1920;
    constexpr static int HEIGHT = 1080;
    constexpr static int IDEAL_FRAME_TIME = 1000 / 60;
};
