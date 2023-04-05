#include "screen.h"
#include "common.h"

static Window *window_init();
static void end_window(Window *window);
static void draw_picture(Window *window, u_int16_t *picture);

void run_mandelbrote(void (*get_pixels)(u_int16_t *screen)) {
    Window *window = window_init();
    u_int16_t screen[SCREEN_HEIGHT * SCREEN_HEIGHT * BYTES_PER_PIXEL] = {};

    bool run = true;
    SDL_Event event;

    for (int i = 0; i < COUNT_TIMES; ++i) {
        get_pixels(screen);
        draw_picture(window, screen);
    }

    end_window(window);
}

static void draw_picture(Window *window, u_int16_t *picture) {

    int   pitch  = 0;
    void *pixels = nullptr;

    SDL_LockTexture(window->tex, NULL, &pixels, &pitch);
    memcpy(pixels, picture, SCREEN_HEIGHT * SCREEN_HEIGHT * BYTES_PER_PIXEL);
    SDL_UnlockTexture(window->tex);

    SDL_RenderClear(window->ren);
    SDL_RenderCopy(window->ren, window->tex, NULL, NULL);
    SDL_RenderPresent(window->ren);
}


static Window *window_init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return nullptr;
    }

    Window *window = (Window*) calloc(1, sizeof(Window));

    window->win = SDL_CreateWindow("Mandelbrote", SDL_WINDOWPOS_UNDEFINED, 
                                    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
                                    SDL_WINDOW_SHOWN);
    if (window->win == nullptr) {
        free(window);
        return nullptr;
    }

    window->ren = SDL_CreateRenderer(window->win, -1, 0);
    window->tex = SDL_CreateTexture(window->ren, SDL_PIXELFORMAT_RGB24, 
                                                 SDL_TEXTUREACCESS_STREAMING, 
                                                 SCREEN_WIDTH, SCREEN_HEIGHT);

    return window;
}

static void end_window(Window *window) {
    SDL_DestroyTexture(window->tex);
    SDL_DestroyRenderer(window->ren);
    SDL_DestroyWindow(window->win);
    SDL_Quit();
    free(window);
}