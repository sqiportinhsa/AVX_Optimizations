#ifndef BLENDING_H
#define BLENDING_H

#include <immintrin.h>
#include "common.h"

void blend1(Image *front, Image *background, uint32_t *dest, const int xshift, const int yshift);
void blend2(Image *front, Image *background, uint32_t *dest, const int xshift, const int yshift);

#endif