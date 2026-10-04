#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include "number_types.h"

unsigned long long sum_proper_divisors(unsigned long long n) {
    if (n < 2) return 0;
    unsigned long long sum = 1;
    /* Divisors come in pairs d and n/d, so only test up to the square root. */
    for (unsigned long long d = 2; d <= n / d; d++) {
        if (n % d != 0) continue;
        unsigned long long pair = (d == n / d) ? d : d + n / d;
        if (sum > ULLONG_MAX - pair) return ULLONG_MAX; /* too big to hold */
        sum += pair;
    }
    return sum;
}

int is_prime(unsigned long long n) {
    if (n < 2) return 0;
    if (n < 4) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    /* Every prime above 3 is one more or one less than a multiple of 6. */
    for (unsigned long long d = 5; d <= n / d; d += 6) {
        if (n % d == 0 || n % (d + 2) == 0) return 0;
    }
    return 1;
}

int is_perfect(unsigned long long n) {
    return n > 0 && sum_proper_divisors(n) == n;
}

int is_deficient(unsigned long long n) {
    return n > 0 && sum_proper_divisors(n) < n;
}

int is_amicable(unsigned long long n) {
    if (n == 0) return 0;
    unsigned long long partner = sum_proper_divisors(n);
    return partner != n && partner != ULLONG_MAX
        && sum_proper_divisors(partner) == n;
}

Matrix *special_numbers_below(unsigned long long n) {
    if (n > SIZE_MAX) return NULL;
    size_t limit = (size_t)n;

    /* sums[k] = sum of proper divisors of k, for every k below n: add each
       d to all of its multiples 2d, 3d, ... instead of factoring every k. */
    unsigned long long *sums = calloc(limit > 0 ? limit : 1, sizeof *sums);
    if (sums == NULL) return NULL;
    for (size_t d = 1; d < limit / 2 + 1; d++) {
        for (size_t k = 2 * d; k < limit; k += d) sums[k] += d;
    }

    /* Count first so the matrix can be made the right width. */
    size_t counts[SPECIAL_NUMBER_ROWS] = { 0 };
    unsigned char *kinds = calloc(limit > 0 ? limit : 1, 1);
    if (kinds == NULL) {
        free(sums);
        return NULL;
    }
    for (size_t k = 1; k < limit; k++) {
        unsigned long long s = sums[k];
        /* The partner of an amicable number can be n or more, so its sum may
           not be in the table. */
        unsigned long long back = (s < limit) ? sums[s] : sum_proper_divisors(s);
        int flags[SPECIAL_NUMBER_ROWS] = {
            [ROW_PRIME] = (s == 1),
            [ROW_AMICABLE] = (s != k && back == k),
            [ROW_DEFICIENT] = (s < k),
            [ROW_PERFECT] = (s == k),
        };
        for (int row = 0; row < SPECIAL_NUMBER_ROWS; row++) {
            if (flags[row]) {
                kinds[k] |= 1u << row;
                counts[row]++;
            }
        }
    }

    size_t cols = 1;
    for (int row = 0; row < SPECIAL_NUMBER_ROWS; row++) {
        if (counts[row] > cols) cols = counts[row];
    }
    Matrix *result = matrix_create(SPECIAL_NUMBER_ROWS, cols);
    if (result != NULL) {
        size_t filled[SPECIAL_NUMBER_ROWS] = { 0 };
        for (size_t k = 1; k < limit; k++) {
            for (int row = 0; row < SPECIAL_NUMBER_ROWS; row++) {
                if (kinds[k] & (1u << row)) {
                    matrix_set(result, row, filled[row]++, (double)k);
                }
            }
        }
    }
    free(kinds);
    free(sums);
    return result;
}
