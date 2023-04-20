#include "common.h"
#include "blending1.h"
#include "get_bmp.h"
#include "run_calc.h"

int main() {
    Image *back = get_image("images/back.bmp");
    Image *front = get_image("images/front.bmp");
    run_blending(blend1, front, back);
    free_image(back);
    free_image(front);
}