#include "common.h"
#include "scalar.h"

void calc_iterations_scalar(u_int32_t *counters) {
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

                if (x_quadr + y_quadr >= MAX_R2) {
                    break;
                }

                float xy_2    = x * y * 2;
                x = x_quadr - y_quadr + x_0;
                y = xy_2 + y_0; 
            }

            counters[screen_y * SCREEN_WIDTH + screen_x] = i;
        }
    }
}
