#include <stdlib.h>
#include "blending.h"
#include <stdio.h>

static inline __m256i _mm256_mulhi_epi8_own(__m256i a, __m256i b);

const __m256i      ALPHA_MASK = _mm256_set_epi8(4*3 + 3, 4*3 + 3, 4*3 + 3, 4*3 + 3, 
                                                4*2 + 3, 4*2 + 3, 4*2 + 3, 4*2 + 3,
                                                4*1 + 3, 4*1 + 3, 4*1 + 3, 4*1 + 3, 
                                                4*0 + 3, 4*0 + 3, 4*0 + 3, 4*0 + 3, 
                                                4*3 + 3, 4*3 + 3, 4*3 + 3, 4*3 + 3, 
                                                4*2 + 3, 4*2 + 3, 4*2 + 3, 4*2 + 3,
                                                4*1 + 3, 4*1 + 3, 4*1 + 3, 4*1 + 3, 
                                                4*0 + 3, 4*0 + 3, 4*0 + 3, 4*0 + 3);

const __m256i  SPLIT_LOW_MASK = _mm256_set_epi8(0x80, 4*1 + 3, 0x80, 4*1 + 2, 0x80, 4*1 + 1, 0x80, 4*1 + 0,
                                                0x80, 4*0 + 3, 0x80, 4*0 + 2, 0x80, 4*0 + 1, 0x80, 4*0 + 0, 
                                                0x80, 4*1 + 3, 0x80, 4*1 + 2, 0x80, 4*1 + 1, 0x80, 4*1 + 0,
                                                0x80, 4*0 + 3, 0x80, 4*0 + 2, 0x80, 4*0 + 1, 0x80, 4*0 + 0);
 
const __m256i SPLIT_HIGH_MASK = _mm256_set_epi8(0x80, 4*3 + 3, 0x80, 4*3 + 2, 0x80, 4*3 + 1, 0x80, 4*3 + 0,
                                                0x80, 4*2 + 3, 0x80, 4*2 + 2, 0x80, 4*2 + 1, 0x80, 4*2 + 0, 
                                                0x80, 4*3 + 3, 0x80, 4*3 + 2, 0x80, 4*3 + 1, 0x80, 4*3 + 0,
                                                0x80, 4*2 + 3, 0x80, 4*2 + 2, 0x80, 4*2 + 1, 0x80, 4*2 + 0);

const __m256i   MOVE_LOW_MASK = _mm256_set_epi8(   0x80,    0x80,    0x80,    0x80,
                                                   0x80,    0x80,    0x80,    0x80,
                                                4*3 + 3, 4*3 + 1, 4*2 + 3, 4*2 + 1,
                                                4*1 + 3, 4*1 + 1, 4*0 + 3, 4*0 + 1,
                                                   0x80,    0x80,    0x80,    0x80,
                                                   0x80,    0x80,    0x80,    0x80,
                                                4*3 + 3, 4*3 + 1, 4*2 + 3, 4*2 + 1,
                                                4*1 + 3, 4*1 + 1, 4*0 + 3, 4*0 + 1); 

const __m256i  MOVE_HIGH_MASK = _mm256_set_epi8(4*3 + 3, 4*3 + 1, 4*2 + 3, 4*2 + 1,
                                                4*1 + 3, 4*1 + 1, 4*0 + 3, 4*0 + 1,
                                                   0x80,    0x80,    0x80,    0x80, 
                                                   0x80,    0x80,    0x80,    0x80,
                                                4*3 + 3, 4*3 + 1, 4*2 + 3, 4*2 + 1,
                                                4*1 + 3, 4*1 + 1, 4*0 + 3, 4*0 + 1,
                                                   0x80,    0x80,    0x80,    0x80, 
                                                   0x80,    0x80,    0x80,    0x80); 

const __m256i const255 = _mm256_set1_epi8(255);


void vector_blending(Image *front, Image *background, uint32_t *dest, const int xshift, const int yshift) {
    unsigned int front_width  = front->width;
    unsigned int front_height = front->height;
    unsigned int back_width   = background->width;

    uint32_t *front_src = front->pixels;
    uint32_t *back_src  = background->pixels; 

    for (unsigned int y = 0; y < front_height; ++y) {
        for (unsigned int x = 0; x < front_width; x += 8) {
            __m256i  back_pixels = _mm256_loadu_si256((__m256i*)(back_src + (y+yshift) * back_width  + x + xshift));
            __m256i front_pixels = _mm256_loadu_si256((__m256i*)(front_src + y*front_width + x));

            __m256i alpha = _mm256_shuffle_epi8(front_pixels, ALPHA_MASK);
            front_pixels = _mm256_mulhi_epi8_own(front_pixels, alpha);

            alpha = _mm256_sub_epi8(const255, alpha);
            back_pixels = _mm256_mulhi_epi8_own(back_pixels, alpha);

            __m256i pixels = _mm256_add_epi8(front_pixels, back_pixels);

            _mm256_store_si256((__m256i*)(dest + (y+yshift) * back_width  + x + xshift), pixels);
        }
    }
}


static inline __m256i _mm256_mulhi_epi8_own(__m256i a, __m256i b) {
    __m256i low_a  = _mm256_shuffle_epi8(a, SPLIT_LOW_MASK);
    __m256i low_b  = _mm256_shuffle_epi8(b, SPLIT_LOW_MASK);
    __m256i high_a = _mm256_shuffle_epi8(a, SPLIT_HIGH_MASK);
    __m256i high_b = _mm256_shuffle_epi8(b, SPLIT_HIGH_MASK);   

    // a: [a31-a24 | a23-a16 | a15-a8 | a7-a0]
    // 
    // a_high:  [ a31 0 a30 0 ... 0 a24 0 | a15 0 a14 0 ... 0 a8 0]
    // a_low:   [ a23 0 a22 0 ... 0 a16 0 | a7  0 a6  0 ... 0 a0 0]

    __m256i mul_low  = _mm256_mullo_epi16(low_a,  low_b);
    __m256i mul_high = _mm256_mullo_epi16(high_a, high_b);

    // high: [ (a31 * b31)_hi | (a31 * b31)_lo | (a30 * b30)_hi | (a30 * b30)_lo | ... ]    
    // low:  [ (a23 * b23)_hi | (a23 * b23)_lo | (a22 * b22)_hi | (a22 * b22)_lo | ... ] 

    mul_low  = _mm256_shuffle_epi8(mul_low,  MOVE_LOW_MASK);
    mul_high = _mm256_shuffle_epi8(mul_high, MOVE_HIGH_MASK);

    // high: [ ab31_hi | ... | ab24_hi |  ----   | ... |   ----  | ab15_hi | ... ]
    // low:  [  ----   | ... |  ----   | ab23_hi | ... | ab16_hi |  ----   | ... ]
    __m256i res = _mm256_add_epi8(mul_high, mul_low);

    // res: [ ab31_hi - ab0_hi ]

    return res;
}