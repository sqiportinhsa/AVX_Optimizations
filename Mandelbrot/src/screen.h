#include <SDL2/SDL.h>

struct Window {
    SDL_Window   *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture  *tex = nullptr;
};

struct Time {
    float time  = 0;
    float error = 0;
};

void run_mandelbrote(void (*get_picture)(u_int32_t *counters));