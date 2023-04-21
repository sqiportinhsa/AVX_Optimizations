#include "common.h"
#include "screen.h"
#include "scalar.h"
#include "vector.h"

int main() {
    #ifdef M1
        run_mandelbrote(calc_iterations_scalar);
    #elif defined(M2)
        run_mandelbrote(calc_iterations_vector);
    #else
        #error 
    #endif
    return 0;
}