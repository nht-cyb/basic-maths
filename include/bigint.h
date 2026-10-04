#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>

/* A whole number of any size (limited only by memory), positive or negative.
   Every function that returns a BigInt* or char* allocates memory: free
   results with bigint_free() / free(). They return NULL on bad input
   (e.g. dividing by zero) or if memory runs out. */
typedef struct BigInt BigInt;

BigInt *bigint_from_string(const char *text);  /* e.g. "-123456789012345678901234567890" */
BigInt *bigint_from_int(long long value);
char *bigint_to_string(const BigInt *x);
void bigint_free(BigInt *x);

/* Returns -1, 0 or 1 for a < b, a == b, a > b. */
int bigint_compare(const BigInt *a, const BigInt *b);

BigInt *bigint_add(const BigInt *a, const BigInt *b);
BigInt *bigint_subtract(const BigInt *a, const BigInt *b);
BigInt *bigint_multiply(const BigInt *a, const BigInt *b);

/* Whole-number division like C's / and %: the quotient is rounded toward
   zero and the remainder has the sign of a. Either output may be NULL if
   you don't need it. Returns 0 on success, -1 if b is zero. */
int bigint_divmod(const BigInt *a, const BigInt *b,
                  BigInt **quotient, BigInt **remainder);
BigInt *bigint_divide(const BigInt *a, const BigInt *b);

/* The means are usually not whole numbers, so they come back as decimal
   text rounded to `places` digits after the point, e.g. "-12.35". */

/* (x1 + x2 + ... + xn) / n */
char *bigint_mean(BigInt *const *values, size_t n, unsigned places);
/* (w1*x1 + ... + wn*xn) / (w1 + ... + wn); NULL if the weights add to 0. */
char *bigint_weighted_mean(BigInt *const *values, BigInt *const *weights,
                           size_t n, unsigned places);
/* n / (1/x1 + ... + 1/xn); NULL if any x is 0 or the reciprocals add to 0. */
char *bigint_harmonic_mean(BigInt *const *values, size_t n, unsigned places);

#endif
