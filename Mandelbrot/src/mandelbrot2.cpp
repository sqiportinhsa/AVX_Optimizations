#include "mandelbrot2.h"
#include "common.h"

void calc_iterations_2(u_int32_t *screen) {
    __m256 arr_shifts = _mm256_set_ps(7 * STEP, 6 * STEP, 5 * STEP, 4 * STEP, 
                                      3 * STEP, 2 * STEP, 1 * STEP, 0 * STEP); 
    __m256 max_r2     = _mm256_set1_ps(MAX_R2);

    for (int screen_y = 0; screen_y < SCREEN_HEIGHT; ++screen_y) {
        for (int screen_x = 0; screen_x < SCREEN_WIDTH; screen_x += 8) {
            __m256 x_0 = _mm256_set1_ps(((float)screen_x + X_SHIFT) / SCALE);
            __m256 y_0 = _mm256_set1_ps(((float)screen_y + Y_SHIFT) / SCALE);
            x_0 = _mm256_add_ps(x_0, arr_shifts);

            __m256 x = x_0;
            __m256 y = y_0;  

            __m256i counters =  _mm256_setzero_si256();         

            int i = 0;
            for (; i < ITERATIONS; ++i) {
                __m256 x_quadr = _mm256_mul_ps(x, x);                    
                __m256 y_quadr = _mm256_mul_ps(y, y);                    
                __m256 xy_2    = _mm256_mul_ps(x, y);

                xy_2 = _mm256_add_ps(xy_2, xy_2);

                __m256 r2  = _mm256_add_ps(x_quadr, y_quadr);
                __m256 cmp = _mm256_cmp_ps(r2, max_r2, _CMP_LT_OS);

                counters = _mm256_sub_epi32(counters, (__m256i) cmp);

                if (_mm256_testz_ps(cmp, cmp)) {
                    break;
                }

                x = _mm256_sub_ps(x_quadr, y_quadr);
                x = _mm256_add_ps(x, x_0);

                y = _mm256_add_ps(xy_2, y_0);
            }

            int pos_in_arr = screen_y * SCREEN_WIDTH + screen_x;
            
            _mm256_store_si256((__m256i*) (screen + pos_in_arr), counters);
        }
    }
}
