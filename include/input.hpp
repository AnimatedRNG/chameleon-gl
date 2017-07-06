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

#include <cmath>
#include <set>
#include <atomic>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "SDL.h"

#include "util.hpp"
#include "sdl_helpers.hpp"
#include "event_handler.hpp"
#include "opengl_utils.hpp"

class InputController : public EventHandler {

  public:
    InputController() :
        keypress_map(),
        current_mouse_position(0, 0),
        center(-1, -1),
        dont_update(false),
        model(),
        view(),
        projection() {
        auto up = [](Uint32 interval, void* param) -> Uint32 {
            SDL_Event event;
            SDL_UserEvent userevent;

            userevent.type = SDL_USEREVENT;
            userevent.code = 0;

            event.type = SDL_USEREVENT;
            event.user = userevent;

            SDL_PushEvent(&event);

            return interval;
        };
        SDL_AddTimer(INPUT_DELAY, up, this);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    ~InputController() {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }

    glm::mat4 get_model() const {
        return this->model;
    }

    glm::mat4 get_view() const {
        return this->view;
    }
    glm::mat4 get_projection() const {
        return this->projection;
    }

    void set_aspect_ratio(const float& ratio) {
        this->aspect_ratio = ratio;
    }

    void on_event(SDL_Event& event, SDL::WindowParams* wp) {
        this->wp = wp;
        switch (event.type) {
            case SDL_KEYDOWN:
                keypress_map.insert(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keypress_map.erase(event.key.keysym.sym);
                break;
            case SDL_MOUSEMOTION:
                current_mouse_position +=
                    glm::ivec2(event.motion.xrel, event.motion.yrel);
                break;
            case SDL_USEREVENT:
                this->update();
            default:
                return;
        }
    }

    void update() override {
        if (this->center == glm::ivec2(-1, -1)) {
            this->center = glm::ivec2(wp->width / 2, wp->height / 2);
        }

        uint64_t dt = INPUT_DELAY;
        glm::ivec2 mouse_delta = center - current_mouse_position;
        if (mouse_delta.x + mouse_delta.y < 200) {
            horizontal_angle += MOUSE_SPEED * dt * ((float) mouse_delta.x);
            vertical_angle += MOUSE_SPEED * dt * ((float) mouse_delta.y);
        }

        // Clamps vertical viewing angles
        vertical_angle = std::min(vertical_angle, 3.14f / 2.0f);
        vertical_angle = std::max(vertical_angle, -3.14f / 2.0f);

        glm::vec3 direction(cos(vertical_angle) * sin(horizontal_angle),
                            sin(vertical_angle),
                            cos(vertical_angle) * cos(horizontal_angle));
        glm::vec3 right_vector(sin(horizontal_angle - 3.14f / 2.0f),
                               0,
                               cos(horizontal_angle - 3.14f / 2.0f));
        glm::vec3 up_vector = glm::cross(right_vector, direction);
        for (int key : keypress_map) {
            if (key == SDLK_w) {
                position += direction * (float) dt * MOVE_SPEED;
            }
            if (key == SDLK_s) {
                position -= direction * (float) dt * MOVE_SPEED;
            }
            if (key == SDLK_a) {
                position -= right_vector * (float) dt * MOVE_SPEED;
            }
            if (key == SDLK_d) {
                position += right_vector * (float) dt * MOVE_SPEED;
            }
            if (key == SDLK_z) {
                position -= up_vector * (float) dt * MOVE_SPEED;
            }
            if (key == SDLK_x) {
                position += up_vector * (float) dt * MOVE_SPEED;
            }
        }

        if (dont_update)
            return;

        glm::mat4 projection;
        glm::mat4 view;
        projection = glm::perspective(initial_fov, aspect_ratio,
                                      InputController::NEAR_PLANE,
                                      InputController::FAR_PLANE);
        view = glm::lookAt(position,
                           position + direction,
                           up_vector);

        this->model = glm::mat4();
        this->projection = projection;
        this->view = view;

        current_mouse_position = center;
    };

    static constexpr int INPUT_DELAY = 10;
    static constexpr float MOUSE_SPEED = 0.0004f;
    static constexpr float MOVE_SPEED = 0.01f;

    static constexpr float NEAR_PLANE = 0.1;
    static constexpr float FAR_PLANE = 100.0;

    glm::vec3 position = glm::vec3(0, 0, 10);
    float horizontal_angle = 3.14f;
    float vertical_angle = 0.0f;
    float initial_fov = 20.0f;
    float aspect_ratio = 16 / 9.;

    SDL::WindowParams* wp;

  private:
    std::set<SDL_Keycode> keypress_map;
    glm::ivec2 current_mouse_position;
    glm::ivec2 center;
    bool dont_update;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};
