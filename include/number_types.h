#ifndef NUMBER_TYPES_H
#define NUMBER_TYPES_H

#include "matrix.h"

/* Special kinds of whole numbers, all based on the proper divisors of n
   (every divisor of n except n itself; for 12 that is 1, 2, 3, 4, 6).

   The kinds overlap: every prime is also deficient, and an amicable number
   can be deficient too (220 is amicable and abundant, 284 is amicable and
   deficient). 0 is none of them. */

/* The sum of the proper divisors of n. 12 -> 1 + 2 + 3 + 4 + 6 = 16.
   0 and 1 give 0. */
unsigned long long sum_proper_divisors(unsigned long long n);

/* Exactly two divisors, 1 and itself. 2, 3, 5, 7, 11, ... */
int is_prime(unsigned long long n);
/* Proper divisors add up to n itself. 6 = 1 + 2 + 3; 28, 496, 8128 */
int is_perfect(unsigned long long n);
/* Proper divisors add up to less than n. 8 -> 1 + 2 + 4 = 7 */
int is_deficient(unsigned long long n);
/* Has a different partner m where the proper divisors of n add up to m and
   the proper divisors of m add up to n. 220 and 284 */
int is_amicable(unsigned long long n);

/* Rows of the matrix returned by special_numbers_below(). */
enum {
    ROW_PRIME,
    ROW_AMICABLE,
    ROW_DEFICIENT,
    ROW_PERFECT,
    SPECIAL_NUMBER_ROWS
};

/* Every special number smaller than n, one kind per row (see the ROW_ names),
   in increasing order. Rows are as long as the longest list; the shorter ones
   are padded with 0 at the end, so stop reading a row at the first 0.
   For n = 10:
     prime     | 2 3 5 7 0 0 0 0 |
     amicable  | 0 0 0 0 0 0 0 0 |
     deficient | 1 2 3 4 5 7 8 9 |
     perfect   | 6 0 0 0 0 0 0 0 |
   For n <= 1 there is nothing to list: the matrix is one column of zeros.
   Call matrix_free() when done. Returns NULL if memory runs out. */
Matrix *special_numbers_below(unsigned long long n);

#endif
