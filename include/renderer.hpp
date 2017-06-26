#pragma once

class Renderer {
  public:
    virtual void operator()(const int& width,
                            const int& height) = 0;
};
