#ifndef GETBMP_H
#define GETBMP_H

#include <stdint.h>
#include "common.h"

typedef unsigned int uint;

Image *get_image(const char *filename);
void free_image(Image *img);
int save(Image *img, const char *filename);

struct __attribute__((__packed__)) bitmap_file_header {
    uint16_t bitmap;
    uint32_t file_size;
    uint32_t reserved;
    uint32_t pixel_data_offset;
};

struct __attribute__((__packed__)) bitmap_info_header {
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;

    // Unused data
    uint32_t compression_level;
    uint32_t image_size;       // = 0 if compression = 0
    uint32_t x_pels_per_meter; // horizontal pixels per meter
    uint32_t y_pels_per_meter; // vertical   pixels per meter 
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;
};

const int BYTES_PER_PIXEL = 4;
const int BITMAP = 0x4D42;
const int PIXELS_OFFSET = sizeof(bitmap_file_header) + sizeof(bitmap_info_header);
const int HEADER_SIZE = sizeof(bitmap_info_header);

#endif
