#include "common.h"
#include "mandelbrot1.h"

void calc_colors_1(u_int16_t *screen) {
    for (int screen_y = 0; screen_y < SCREEN_HEIGHT; ++screen_y) {
        for (int screen_x = 0; screen_x < SCREEN_WIDTH; ++screen_x) {
            float x_0 = ((float)screen_x + X_SHIFT) / SCALE;
            float y_0 = ((float)screen_y + Y_SHIFT) / SCALE;

            float x = x_0;
            float y = y_0;            

            int i = 0;
            for (; i < ITERATIONS; ++i) {
                float x_quadr = x * x;
                float y_quadr = y * y;
                float xy_2    = x * y * 2;

                if (x_quadr + y_quadr >= MAX_R2) {
                    break;
                }

                x = x_quadr - y_quadr + x_0;
                y = xy_2 + y_0; 
            }

            int pixel_pos = screen_y * SCREEN_WIDTH * BYTES_PER_PIXEL + screen_x * BYTES_PER_PIXEL;
            
            screen[pixel_pos + 0] = (i * 256 * 128 / ITERATIONS) % 256;
            screen[pixel_pos + 1] = (i * 256 * 128 / ITERATIONS) % 256;
            screen[pixel_pos + 2] = 0;
        }
    }
}
