#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

/* A rows x cols table of numbers, stored row by row. */
typedef struct {
    size_t rows;
    size_t cols;
    double *data;
} Matrix;

/* Creating a matrix allocates memory: call matrix_free() when done.
   Functions that return a Matrix* return NULL if memory runs out or the
   sizes don't fit together. */
Matrix *matrix_create(size_t rows, size_t cols);  /* filled with zeros */
Matrix *matrix_from_array(size_t rows, size_t cols, const double *values);
void matrix_free(Matrix *m);

double matrix_get(const Matrix *m, size_t row, size_t col);
void matrix_set(Matrix *m, size_t row, size_t col, double value);

/* A + B and A - B: A and B must be the same size. */
Matrix *matrix_add(const Matrix *a, const Matrix *b);
Matrix *matrix_subtract(const Matrix *a, const Matrix *b);
/* A x B: A's columns must equal B's rows. Result is A.rows x B.cols. */
Matrix *matrix_multiply(const Matrix *a, const Matrix *b);

void matrix_print(const Matrix *m);

#endif
