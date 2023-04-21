#include <time.h>
#include <unistd.h>

#include "screen.h"
#include "common.h"

static Window *window_init();
static void end_window(Window *window);
static void draw_picture(Window *window, u_int8_t *picture);
static void calc_colors(u_int8_t *screen, u_int32_t *counters);

static Time get_time(int count_times, u_int32_t *counters, void (*get_pixels)(u_int32_t *counters));
static void print_time(int count_time, long time, long error);

void run_mandelbrote(void (*get_pixels)(u_int32_t *counters)) {
    Window *window = window_init();
    u_int32_t *counters = (u_int32_t*) aligned_alloc(32, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(u_int32_t));
    u_int8_t  *screen   =  (u_int8_t*) calloc(SCREEN_HEIGHT * SCREEN_WIDTH * BYTES_PER_PIXEL, sizeof(u_int16_t));

    SDL_Event event;

    get_time(1, counters, get_pixels);
    calc_colors(screen, counters);
    draw_picture(window, screen);

    Time time = {};
    for (int i = 0; i < TIMES_SIZE; ++i) {
        time = get_time(COUNT_TIMES[i], counters, get_pixels);
        calc_colors(screen, counters);
        draw_picture(window, screen);
        print_time(COUNT_TIMES[i], time.time, time.error);
    }

    end_window(window);

    free(counters);
    free(screen);
}

static Time get_time(int count_times, u_int32_t *counters, void (*get_pixels)(u_int32_t *counters)) {
    long times[REPEAT_BENCH] = {};
    volatile u_int32_t avoid_skip_loop_optimization = 0;

    for (int i = 0; i < REPEAT_BENCH; ++i) {
        clock_t begin = clock();
        for (int j = 0; j < count_times; ++j) {
            get_pixels(counters);
            avoid_skip_loop_optimization = counters[j];
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
        long delta = (times[i] - time.time);
        time.error += delta * delta;
    }
    time.error /= REPEAT_BENCH;
    time.error = sqrt(time.error);

    return time;
}

static void print_time(int count_time, long time, long error) {
    printf("%d, %ld, %ld\n", count_time, time, error);
}

static void calc_colors(u_int8_t *screen, u_int32_t *counters) {
    int size = SCREEN_HEIGHT * SCREEN_WIDTH;
    for (int i = 0; i < size; ++i) {
        int counter       = counters[i];
        int pos_in_screen = i * BYTES_PER_PIXEL;

        screen[pos_in_screen + 0] = (counter * 256 * 128 / ITERATIONS) % 256;
        screen[pos_in_screen + 1] = (counter * 256 * 128 / ITERATIONS) % 256;
        screen[pos_in_screen + 2] = (counter * 256 * 128 / ITERATIONS) % 256;
    }
}

static void draw_picture(Window *window, u_int8_t *picture) {

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