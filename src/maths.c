#include "maths.h"

int add(int a, int b) { return a + b; }

int subtract(int a, int b) { return a - b; }

int multiply(int a, int b) { return a * b; }

double divide(int a, int b) {
    if (b == 0) {
        return 0.0; /* TODO: handle divide-by-zero properly */
    }
    return (double)a / b;
}
