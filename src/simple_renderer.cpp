#include <iostream>
#include "simple_renderer.h"

SDFRenderer::SDFRenderer() {

}

void SDFRenderer::operator()(uint32_t* bytes,
                             const int& pitch,
                             const int& width,
                             const int& height) {
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int offset = j * (pitch / sizeof(uint32_t)) + i;
            if (i % 30 < 5 || j % 30 < 5)
                bytes[offset] = (255 << (16)) | (255 << (8)) | 200;
        }
    }
}

int main(int argc, char** args) {
    GraphicsContext context;
    SDFRenderer renderer;
    context.start(renderer);
}
