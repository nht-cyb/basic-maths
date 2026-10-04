#include <stdio.h>
#include "maths.h"

int main(void) {
    printf("7 + 3 = %d\n", add(7, 3));
    printf("7 - 3 = %d\n", subtract(7, 3));
    printf("7 * 3 = %d\n", multiply(7, 3));
    printf("7 / 3 = %.2f\n", divide(7, 3));
    return 0;
}
