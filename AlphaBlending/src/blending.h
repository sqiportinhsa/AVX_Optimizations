#ifndef BLENDING_H
#define BLENDING_H

#include <immintrin.h>
#include "common.h"

void scalar_blending(Image *front, Image *background, uint32_t *dest, const int xshift, const int yshift);
void vector_blending(Image *front, Image *background, uint32_t *dest, const int xshift, const int yshift);

#endif