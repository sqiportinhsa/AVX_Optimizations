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
    long time  = 0;
    long error = 0;
};

const int REPEAT_BENCH = 5;

const int COUNT_TIMES[] = {10, 25, 50, 75, 100};
const int TIMES_SIZE    = sizeof(COUNT_TIMES) / sizeof(int);

void run_blending(void (*get_pixels)(Image *front, Image *background, uint32_t *dest), 
                                     Image *front, Image *back);
#endif