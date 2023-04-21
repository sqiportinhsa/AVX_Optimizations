#include "common.h"
#include "blending.h"
#include "get_bmp.h"
#include "run_calc.h"

int main() {
    Image *back = get_image("images/back1.bmp");
    Image *front = get_image("images/front2.bmp");

    #ifdef A1
        run_blending(scalar_blending, front, back, "images/output.bmp");
    #elif defined(A2)
        run_blending(vector_blending, front, back, "images/output.bmp");
    #else
        #error "Undefined mode"
    #endif

    free_image(back);
    free_image(front);
}