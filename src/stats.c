#include <stdlib.h>
#include <string.h>
#include "stats.h"

static int compare_doubles(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

/* Returns a sorted copy of values; the caller must free() it. */
static double *sorted_copy(const double *values, size_t n) {
    double *copy = malloc(n * sizeof *copy);
    if (copy == NULL) return NULL;
    memcpy(copy, values, n * sizeof *copy);
    qsort(copy, n, sizeof *copy, compare_doubles);
    return copy;
}

double mean(const double *values, size_t n) {
    double sum = 0;
    for (size_t i = 0; i < n; i++) sum += values[i];
    return sum / n;
}

double median(const double *values, size_t n) {
    double *s = sorted_copy(values, n);
    if (s == NULL) return 0.0;
    double result = (n % 2 == 1) ? s[n / 2] : (s[n / 2 - 1] + s[n / 2]) / 2;
    free(s);
    return result;
}

double mode(const double *values, size_t n) {
    double *s = sorted_copy(values, n);
    if (s == NULL) return 0.0;
    double best = s[0];
    size_t best_count = 0;
    size_t i = 0;
    while (i < n) { /* equal values sit next to each other after sorting */
        size_t j = i;
        while (j < n && s[j] == s[i]) j++;
        if (j - i > best_count) {
            best_count = j - i;
            best = s[i];
        }
        i = j;
    }
    free(s);
    return best;
}
