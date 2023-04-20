#include <time.h>
#include <unistd.h>

#include "run_calc.h"

static Window *window_init(int height, int width);
static void end_window(Window *window);
static void draw_picture(Window *window, uint32_t *image, int height, int width);

static Time get_time(int count_times, Image *front, Image *back, u_int32_t *dest,
                   void (*get_pixels)(Image *front, Image *back, uint32_t *dest)) ;
static void print_time(int count_time, long time, long error);

void run_blending(void (*get_pixels)(Image *front, Image *background, uint32_t *dest), 
                                     Image *front, Image *back) {

    Window *window = window_init(back->height, back->width);
    uint32_t *dest = (uint32_t*) calloc(back->size, sizeof(uint32_t));

    Time time = {};
    for (int i = 0; i < TIMES_SIZE; ++i) {
        time = get_time(COUNT_TIMES[i], front, back, dest, get_pixels);
        draw_picture(window, dest, back->height, back->width);
        print_time(COUNT_TIMES[i], time.time, time.error);
    }
    sleep(5);
    end_window(window);
    free(dest);
}

static Time get_time(int count_times, Image *front, Image *back, uint32_t *dest,
                   void (*get_pixels)(Image *front, Image *back, uint32_t *dest)) {

    long times[REPEAT_BENCH] = {};
    volatile u_int32_t avoid_skip_loop_optimization = 0;
    memcpy(dest, back->pixels, back->size * sizeof(uint32_t));

    for (int i = 0; i < REPEAT_BENCH; ++i) {
        clock_t begin = clock();
        for (int j = 0; j < count_times; ++j) {
            get_pixels(front, back, dest);
            avoid_skip_loop_optimization = dest[j];
        }
        clock_t end = clock();

        long time = end - begin;
        time = (time * 1000) / (CLOCKS_PER_SEC); // time in msec 

        times[i] = time;
    }

    Time time = {};
    for (int i = 0; i < REPEAT_BENCH; ++i) {
        time.time += times[i];
    }
    time.time /= REPEAT_BENCH;
    for (int i = 0; i < REPEAT_BENCH; ++i) {
        times[i] -= time.time;
        time.error += times[i];
    }
    time.error /= REPEAT_BENCH;
    return time;
}

static void print_time(int count_time, long time, long error) {
    printf("%d, %ld, %ld\n", count_time, time, error);
}

static void draw_picture(Window *window, uint32_t *image, int height, int width) {

    int   pitch  = 0;
    void *pixels = nullptr;

    SDL_LockTexture(window->tex, NULL, &pixels, &pitch);
    memcpy(pixels, image, height * width * sizeof(uint32_t));
    SDL_UnlockTexture(window->tex);

    SDL_RenderClear(window->ren);
    SDL_RenderCopy(window->ren, window->tex, NULL, NULL);
    SDL_RenderPresent(window->ren);
}


static Window *window_init(int height, int width) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return nullptr;
    }

    Window *window = (Window*) calloc(1, sizeof(Window));

    window->win = SDL_CreateWindow("Mandelbrote", SDL_WINDOWPOS_UNDEFINED, 
                                    SDL_WINDOWPOS_UNDEFINED, width, height, 
                                    SDL_WINDOW_SHOWN);
    if (window->win == nullptr) {
        free(window);
        return nullptr;
    }

    window->ren = SDL_CreateRenderer(window->win, -1, 0);
    window->tex = SDL_CreateTexture(window->ren, SDL_PIXELFORMAT_BGRA32, 
                                                 SDL_TEXTUREACCESS_STREAMING, 
                                                 width, height);

    return window;
}

static void end_window(Window *window) {
    SDL_DestroyTexture(window->tex);
    SDL_DestroyRenderer(window->ren);
    SDL_DestroyWindow(window->win);
    SDL_Quit();
    free(window);
}