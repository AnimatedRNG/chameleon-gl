#pragma once

#include <vector>
#include <exception>
#include <assert.h>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

#include "SDL.h"
#include "SDL_image.h"

#include "util.hpp"

class ImageData {
  public:
    ImageData(const int& w,
              const int& h,
              const int& p_s,
              const GLenum& fmt,
              void* d) :
        width(w), height(h), pixel_size(p_s), format(fmt), data(d) { }

    ImageData(const ImageData& a) :
        width(a.width),
        height(a.height),
        pixel_size(a.pixel_size),
        format(a.format),
        data(a.data) { }

    ImageData& operator=(const ImageData& a) {
        this->data = a.data;
        return *this;
    }

    const int width;
    const int height;
    const int pixel_size;
    const GLenum format;

    void* data;
};

class ImagePool {
  public:
    ImagePool() :
        pool() {
    }

    ~ImagePool() {

    }

    int add(std::string filename) {
        SDL_Surface* surface = IMG_Load(filename.c_str());

        SDL_PixelFormat* fmt = surface->format;
        const int num_bytes = surface->h * surface->pitch;
        uint8_t* data = new uint8_t[num_bytes];

        assert(fmt->palette == NULL);
        assert(num_bytes == surface->w * surface->h * fmt->BytesPerPixel);

        uint32_t pixel, temp;
        uint8_t red, green, blue, alpha;
        SDL_LockSurface(surface);

        for (size_t j = 0; j < surface->h; j++) {
            for (size_t i = 0; i < surface->w; i++) {
                size_t byte_offset = (i + j * surface->w) * fmt->BytesPerPixel;

                uint8_t* tmp = new uint8_t[sizeof(uint32_t)];
                for (int i = 0; i < fmt->BytesPerPixel; i++) {
                    tmp[i] = ((uint8_t*) surface->pixels)[byte_offset + i];
                }
                pixel = *(uint32_t*) tmp;

                temp = pixel & fmt->Rmask;
                temp = temp >> fmt->Rshift;
                temp = temp << fmt->Rloss;
                red = (uint8_t) temp;
                data[byte_offset] = red;

                if (fmt->BytesPerPixel > 1) {
                    temp = pixel & fmt->Gmask;
                    temp = temp >> fmt->Gshift;
                    temp = temp << fmt->Gloss;
                    green = (uint8_t) temp;
                    data[byte_offset + 1] = green;

                    if (fmt->BytesPerPixel > 2) {
                        temp = pixel & fmt->Bmask;
                        temp = temp >> fmt->Bshift;
                        temp = temp << fmt->Bloss;
                        blue = (uint8_t) temp;
                        data[byte_offset + 2] = blue;

                        if (fmt->BytesPerPixel > 3) {
                            temp = pixel & fmt->Amask;
                            temp = temp >> fmt->Ashift;
                            temp = temp << fmt->Aloss;
                            alpha = (uint8_t) temp;
                            data[byte_offset + 3] = alpha;
                        }
                    }
                }
            }
        }
        SDL_UnlockSurface(surface);

        GLenum gl_fmt;
        switch (fmt->BytesPerPixel) {
        case 1:
            gl_fmt = GL_R;
            break;
        case 2:
            gl_fmt = GL_RG;
            break;
        case 3:
            gl_fmt = GL_RGB;
            break;
        case 4:
            gl_fmt = GL_RGBA;
            break;
        default:
            throw std::runtime_error("Invalid pixel format!");
        }

        ImageData d(surface->w, surface->h, fmt->BytesPerPixel, gl_fmt, data);

        for (auto it = pool.begin(); it != pool.end(); ++it) {
            if (it->data == NULL) {
                *it = d;
                return (int)(it - pool.begin());
            }
        }
        SDL_FreeSurface(surface);

        pool.push_back(d);
        return pool.size() - 1;
    }

    void remove(const int& index) {
        ImageData data = pool[index];
        free(data.data);
        data.data = NULL;
    }

    ImageData get(const int& index) {
        return pool[index];
    }

  private:
    std::vector<ImageData> pool;
};
