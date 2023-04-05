#include <SDL2/SDL.h>

struct Window {
    SDL_Window   *win = nullptr;
    SDL_Renderer *ren = nullptr;
    SDL_Texture  *tex = nullptr;
};

void run_mandelbrote(void (*get_picture)(u_int16_t *screen));