#include <SDL2/SDL.h>

struct Window {
    SDL_Window   *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture  *tex = nullptr;
};

struct Time {
    long time  = 0;
    long error = 0;
};

void run_mandelbrote(void (*get_picture)(u_int32_t *counters));