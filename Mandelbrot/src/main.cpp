#include "common.h"
#include "screen.h"
#include "mandelbrot1.h"
#include "mandelbrot2.h"

int main() {
    run_mandelbrote(calc_iterations_1);
    return 0;
}