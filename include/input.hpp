#pragma once

#include <cmath>
#include <set>
#include <atomic>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "SDL.h"

#include "util.hpp"
#include "sdl_helpers.hpp"

class InputController {

  public:
    InputController(SDL::window_params* wp);
    ~InputController();

    glm::mat4 get_model();
    glm::mat4 get_view();
    glm::mat4 get_projection();
    glm::mat4 get_mvp();
    void set_aspect_ratio(const float& ratio);

    void on_event(SDL_Event& event);
    void update();

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

  private:
    std::set<SDL_Keycode> keypress_map;
    glm::ivec2 current_mouse_position;
    glm::ivec2 center;
    bool dont_update;

    SDL_TimerID timer;

    SDL::window_params* wp;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
};
