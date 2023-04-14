#ifndef MANDELBROTE_COMMON
#define MANDELBROTE_COMMON

#include <stdio.h>
#include <stdlib.h>

const int ITERATIONS  = 1000;

const int COUNT_TIMES[] = {10, 25, 50, 75, 100};
const int TIMES_SIZE    = sizeof(COUNT_TIMES) / sizeof(int);

const int REPEAT_BENCH = 5;

const int SCREEN_WIDTH  = 1024;
const int SCREEN_HEIGHT = 1024;

const float SCALE = 300;       // pixels for period of length 1
const float STEP  = 1/SCALE;  // length of period between nearby pixels

const float X_SHIFT = -600;      // in pixels
const float Y_SHIFT = -512;      // in pixels

const float MAX_R2 = 4;

const int BYTES_PER_PIXEL = 3;

#endif