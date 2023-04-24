#ifndef RUNCALC_H
#define RUNCALC_H
#include <SDL2/SDL.h>

#include "common.h"

struct Window {
    SDL_Window   *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture  *tex = nullptr;
};

struct Time {
    double time  = 0;
    double error = 0;
};

const int REPEAT_BENCH = 100;

const int COUNT_TIMES = 100000;

const int XSHIFT = 400;
const int YSHIFT = 0;

void run_blending(void (*get_pixels)(Image *front, Image *background, uint32_t *dest, 
                                                 const int xshift, const int yshift), 
                                     Image *front, Image *back, const char *output);
#endif