#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

struct Image {
    unsigned int width;
    unsigned int height;
    unsigned int size;
    uint32_t *pixels;
};

#endif