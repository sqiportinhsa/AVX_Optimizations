#include <stdlib.h>
#include "blending.h"
#include <stdio.h>

union pixel_t {
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    } rgba;

    uint32_t val;
};

void blend1(Image *front, Image *background, uint32_t *dest, const int xshift, const int yshift) {
    unsigned int front_width  = front->width;
    unsigned int front_height = front->height;
    unsigned int back_width = background->width;

    uint32_t *front_src = front->pixels + xshift + front_width * yshift;
    uint32_t *back_src  = background->pixels; 

    for (unsigned int y = 0; y < front_height; ++y) {
        for (unsigned int x = 0; x < front_width; ++x) {
            pixel_t back_pixel  = {};
            pixel_t front_pixel = {};

            back_pixel.val  =  back_src[y*back_width + x];
            front_pixel.val = front_src[y*front_width + x];

            uint8_t alpha = front_pixel.rgba.a;
            uint8_t not_alpha = 255 - alpha;

            pixel_t pixel;
            pixel.rgba.a = 255;
            pixel.rgba.r = (back_pixel.rgba.r * not_alpha + front_pixel.rgba.r * alpha) >> 8;
            pixel.rgba.g = (back_pixel.rgba.g * not_alpha + front_pixel.rgba.g * alpha) >> 8; 
            pixel.rgba.b = (back_pixel.rgba.b * not_alpha + front_pixel.rgba.b * alpha) >> 8;

            dest[y*back_width + x] = (uint32_t) pixel.val;
        }
    }
}