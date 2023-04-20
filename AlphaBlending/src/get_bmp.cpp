#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "get_bmp.h"

static Image *create_img(unsigned int width, unsigned int height);

// ---------------------------------------------------------------------------------------------------------------------

#define CHECK(condition) {           \
    if (!(condition)) {              \
        fprintf(stderr, "Invalid file: expected " #condition "\n"); \
        return nullptr;         \
    }                           \
}

Image *get_image(const char *filename) {
    FILE *input = fopen(filename, "rb");
    CHECK(input)

    bitmap_file_header fheader = {};
    bitmap_info_header iheader = {};

    fread(&fheader, sizeof(bitmap_file_header), 1, input);
    CHECK(fheader.bitmap == BITMAP);

    fread(&iheader, sizeof(bitmap_info_header), 1, input);
    CHECK(iheader.compression_level == 0);
    CHECK(iheader.bits_per_pixel == 32);

    Image *img = create_img(iheader.width, iheader.height);
    fread(img->pixels, sizeof(img->pixels), img->size, input);

    fclose(input);
    return img;
}

#undef check_format

// ---------------------------------------------------------------------------------------------------------------------

static Image *create_img(unsigned int width, unsigned int height) {
    Image *img = (Image *) calloc(1, sizeof (Image));
    img->height = height;
    img->width  = width;
    img->size = width * height;

    img->pixels = (uint32_t*) calloc(width * height, sizeof(uint32_t));
    return img;
}

// ---------------------------------------------------------------------------------------------------------------------

void free_image(Image *img) {
    free(img->pixels);
    free(img);
}

// ---------------------------------------------------------------------------------------------------------------------

int save(Image *img, const char *filename) {
    int fd = creat (filename, O_RDWR);

    if (fd == -1) {
        fprintf(stderr, "failed to save file %s with error %s\n", filename, strerror(errno));
        return -1;
    }

    bitmap_file_header fheader = {
        .bitmap = BITMAP,
        .file_size = img->size * BYTES_PER_PIXEL + PIXELS_OFFSET,
        .reserved = 0,
        .pixel_data_offset = PIXELS_OFFSET,
    };

    bitmap_info_header iheader = {
        .header_size = HEADER_SIZE,
        .width  = img->width,
        .height = img->height,
        .planes = 1,                             // Specified by standart
        .bits_per_pixel = BYTES_PER_PIXEL * 8,   // 4 bytes of RGBA
        .compression_level = 0,
        .image_size = img->size * BYTES_PER_PIXEL,
    };

    write(fd, &fheader, sizeof(bitmap_file_header));
    write(fd, &iheader, sizeof(bitmap_info_header));
    write(fd, img->pixels, img->size);

    return 0;
}