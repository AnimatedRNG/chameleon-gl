#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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

    inline virtual float sdf(const glm::vec4& location) const;
  private:
    inline virtual float sphere_sdf(const glm::vec4& location,
                                    const float& radius) const;
    inline virtual float fancy_torus_sdf(const glm::vec4& location,
                                         const glm::vec2& t) const;
    inline float length8(const glm::vec2& q) const {
        return pow(pow(q.x, 8.) + pow(q.y, 8.), 1.0 / 8.0);
    }

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;

    constexpr static float FOVY = 60.0;
    constexpr static float NEAR = 0.1;
    constexpr static float FAR = 100;
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
        for (long accum = 0; accum < 5000;) {
            long dt = mainloop(renderer);
            if (dt == -1)
                return;
            accum += dt;
        }
    }

    ~GraphicsContext() {
        SDL_DestroyTexture(render_texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }
  private:
    long mainloop(Renderer& renderer) {
        auto start = GET_TIME();
        SDL_Event event;
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT)
            return -1;

        // Handle input here

        const SDL_Rect rect {0, 0, WIDTH, HEIGHT};
        uint32_t* bytes = nullptr;
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

        return diff;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* render_texture;

    constexpr static int WIDTH = 640;
    constexpr static int HEIGHT = 480;
    constexpr static int IDEAL_FRAME_TIME = 1000 / 60;
};

std::ostream& operator<< (std::ostream& out, const glm::vec2& vec) {
    out << "("
        << vec.x << ", " << vec.y
        << ")";

    return out;
}

std::ostream& operator<< (std::ostream& out, const glm::vec3& vec) {
    out << "("
        << vec.x << ", " << vec.y << ", " << vec.z
        << ")";

    return out;
}

std::ostream& operator<< (std::ostream& out, const glm::vec4& vec) {
    out << "("
        << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w
        << ")";

    return out;
}
