#include <time.h>
#include <unistd.h>

#include "run_calc.h"
#include "get_bmp.h"

static Window *window_init(int height, int width);
static void end_window(Window *window);
static void draw_picture(Window *window, uint32_t *image, int height, int width);

static Time get_time(int count_times, Image *front, Image *back, uint32_t *dest,
                   void (*get_pixels)(Image *front, Image *back, uint32_t *dest, 
                                           const int xshift, const int yshift));

static void print_time(Time time);
uint32_t *reverse(uint32_t *image, unsigned int height, unsigned int width);


void run_blending(void (*get_pixels)(Image *front, Image *background, uint32_t *dest, 
                                                 const int xshift, const int yshift), 
                                     Image *front, Image *back, const char *output) {

    Window *window = window_init(back->height, back->width);
    uint32_t *dest = (uint32_t*) aligned_alloc(256, back->size * sizeof(uint32_t));


    get_time(1, front, back, dest, get_pixels);
    draw_picture(window, dest, back->height, back->width);

    Time time = {};
    
    time = get_time(COUNT_TIMES, front, back, dest, get_pixels);
    draw_picture(window, dest, back->height, back->width);
    print_time(time);
    sleep(5);

    uint32_t *temp = back->pixels;
    back->pixels = dest;
    save(back, output);
    back->pixels = temp;

    end_window(window);
    free(dest);
}

static Time get_time(int count_times, Image *front, Image *back, uint32_t *dest,
                   void (*get_pixels)(Image *front, Image *back, uint32_t *dest, 
                                             const int xshift, const int yshift)) {

    long times[REPEAT_BENCH] = {};
    volatile u_int32_t avoid_skip_loop_optimization = 0;
    memcpy(dest, back->pixels, back->size * sizeof(uint32_t));

    for (int i = 0; i < REPEAT_BENCH; ++i) {
        clock_t begin = clock();
        for (int j = 0; j < count_times; ++j) {
            get_pixels(front, back, dest, XSHIFT, YSHIFT);
            avoid_skip_loop_optimization = dest[j];
        }
        clock_t end = clock();

        double time = end - begin;
        time = (time * 1000000) / (CLOCKS_PER_SEC); // time in usec 

        times[i] = time;
    }

    Time time = {};
    for (int i = 0; i < REPEAT_BENCH; ++i) {
        time.time += times[i];
    }
    time.time /= REPEAT_BENCH;
    for (int i = 0; i < REPEAT_BENCH; ++i) {
        double delta = (times[i] - time.time);
        time.error += delta * delta;
    }
    time.error /= REPEAT_BENCH;
    time.error = sqrt(time.error);

    return time;
}

static void print_time(Time time) {
    printf("%f, %f\n", time.time, time.error);
}

static void draw_picture(Window *window, uint32_t *image, int height, int width) {
    int   pitch  = 0;
    void *pixels = nullptr;

    uint32_t *reversed = reverse(image, height, width);

    SDL_LockTexture(window->tex, NULL, &pixels, &pitch);
    memcpy(pixels, reversed, height * width * sizeof(uint32_t));
    SDL_UnlockTexture(window->tex);

    SDL_RenderClear(window->ren);
    SDL_RenderCopy(window->ren, window->tex, NULL, NULL);
    SDL_RenderPresent(window->ren);
    free(reversed);
}

uint32_t *reverse(uint32_t *image, unsigned int height, unsigned int width) {
    uint32_t *reversed = (uint32_t*) calloc(height * width, sizeof(uint32_t));
    for (unsigned int i = 0; i < height; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            reversed[i*width + j] = image[(height - i)*width + j];
        }
    }
    return reversed;
}

static Window *window_init(int height, int width) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return nullptr;
    }

    Window *window = (Window*) calloc(1, sizeof(Window));

    window->win = SDL_CreateWindow("Alpha Blending", SDL_WINDOWPOS_UNDEFINED, 
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