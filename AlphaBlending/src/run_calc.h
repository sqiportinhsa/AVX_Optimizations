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
    long   time  = 0;
    double error = 0;
};

const int REPEAT_BENCH = 100;

const int COUNT_TIMES[] = {100, 250, 500, 750, 1000};
const int TIMES_SIZE    = sizeof(COUNT_TIMES) / sizeof(int);

const int XSHIFT = 400;
const int YSHIFT = 0;

void run_blending(void (*get_pixels)(Image *front, Image *background, uint32_t *dest, 
                                                 const int xshift, const int yshift), 
                                     Image *front, Image *back, const char *output);
#endif