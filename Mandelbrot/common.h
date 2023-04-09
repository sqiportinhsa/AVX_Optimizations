#ifndef MANDELBROTE_COMMON
#define MANDELBROTE_COMMON

#include <stdio.h>
#include <stdlib.h>

const int ITERATIONS  = 1000;
const int COUNT_TIMES = 100;

const int SCREEN_WIDTH  = 1000;
const int SCREEN_HEIGHT = 1000;

const float SCALE = 150;       // pixels for period of length 1
const float STEP  = 1/SCALE;  // length of period between nearby pixels

const float X_SHIFT = -800;      // in pixels
const float Y_SHIFT = -250;      // in pixels

const float MAX_R2 = 4;

const int BYTES_PER_PIXEL = 3;

#endif