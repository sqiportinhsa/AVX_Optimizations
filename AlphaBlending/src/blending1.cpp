#include <stdlib.h>
#include "blending1.h"
#include <stdio.h>

void blend1(Image *front, Image *background, uint32_t *dest) {
    unsigned int front_width  = front->width;
    unsigned int front_height = front->height;
    unsigned int back_width = background->width;

    uint32_t *front_src =      front->pixels;
    uint32_t *back_src  = background->pixels; 

    for (unsigned int y = 0; y < front_height; ++y) {
        for (unsigned int x = 0; x < front_width; ++x) {
            uint32_t back_pixel  =  back_src[y*back_width + x];
            uint32_t front_pixel = front_src[y*front_width + x];

            uint8_t alpha = front_pixel >> 8*3;

            uint8_t r = (((uint8_t)(back_pixel >> 8*2))*(255-alpha) + ((uint8_t)(front_pixel >> 8*2) * alpha)) >> 8;
            uint8_t g = (((uint8_t)(back_pixel >> 8*1))*(255-alpha) + ((uint8_t)(front_pixel >> 8*1) * alpha)) >> 8;
            uint8_t b = (((uint8_t)(back_pixel >> 8*0))*(255-alpha) + ((uint8_t)(front_pixel >> 8*0) * alpha)) >> 8;   

            uint32_t pixel = (255 << 8*3) + (r << 8*2) + (g << 8*1) + (b << 8*0);
            dest[y*back_width + x] = pixel;
        }
    }
}