#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

Matrix *matrix_create(size_t rows, size_t cols) {
    Matrix *m = malloc(sizeof *m);
    if (m == NULL) return NULL;
    m->rows = rows;
    m->cols = cols;
    m->data = calloc(rows * cols, sizeof *m->data);
    if (m->data == NULL) {
        free(m);
        return NULL;
    }
    return m;
}

Matrix *matrix_from_array(size_t rows, size_t cols, const double *values) {
    Matrix *m = matrix_create(rows, cols);
    if (m == NULL) return NULL;
    memcpy(m->data, values, rows * cols * sizeof *m->data);
    return m;
}

void matrix_free(Matrix *m) {
    if (m == NULL) return;
    free(m->data);
    free(m);
}

double matrix_get(const Matrix *m, size_t row, size_t col) {
    return m->data[row * m->cols + col];
}

void matrix_set(Matrix *m, size_t row, size_t col, double value) {
    m->data[row * m->cols + col] = value;
}

static int same_size(const Matrix *a, const Matrix *b) {
    return a->rows == b->rows && a->cols == b->cols;
}

Matrix *matrix_add(const Matrix *a, const Matrix *b) {
    if (!same_size(a, b)) return NULL;
    Matrix *result = matrix_create(a->rows, a->cols);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] + b->data[i];
    }
    return result;
}

Matrix *matrix_subtract(const Matrix *a, const Matrix *b) {
    if (!same_size(a, b)) return NULL;
    Matrix *result = matrix_create(a->rows, a->cols);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < a->rows * a->cols; i++) {
        result->data[i] = a->data[i] - b->data[i];
    }
    return result;
}

Matrix *matrix_multiply(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) return NULL;
    Matrix *result = matrix_create(a->rows, b->cols);
    if (result == NULL) return NULL;
    /* Each result cell = row i of A "dotted" with column j of B. */
    for (size_t i = 0; i < a->rows; i++) {
        for (size_t j = 0; j < b->cols; j++) {
            double sum = 0;
            for (size_t k = 0; k < a->cols; k++) {
                sum += matrix_get(a, i, k) * matrix_get(b, k, j);
            }
            matrix_set(result, i, j, sum);
        }
    }
    return result;
}

void matrix_print(const Matrix *m) {
    if (m == NULL) {
        printf("(no result: sizes don't match)\n");
        return;
    }
    for (size_t i = 0; i < m->rows; i++) {
        printf("| ");
        for (size_t j = 0; j < m->cols; j++) {
            printf("%7.2f ", matrix_get(m, i, j));
        }
        printf("|\n");
    }
}
