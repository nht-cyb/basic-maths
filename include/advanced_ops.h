#ifndef ADVANCED_OPS_H
#define ADVANCED_OPS_H

#include <stdio.h>

/* Advanced multiplication and division methods for whole numbers, following
   https://www.basic-mathematics.com/pre-algebra-lessons.html
   ("Advanced Multiplication and Division").

   Every function takes a `steps` stream: pass stdout to see the working
   written out like on paper, or NULL to just get the answer.
   They return 0 on success, or -1 if the answer doesn't fit in an
   unsigned long long (multiplication) or the divisor is 0 (division). */

typedef unsigned long long ull;

/* ---- Multiplication ---- */

/* Grid of single-digit products, then add down the diagonals. 42 x 35 = 1470 */
int lattice_multiply(ull a, ull b, ull *product, FILE *steps);
/* Split both numbers by place value and multiply every pair.
   26 x 45 = 800 + 100 + 240 + 30 = 1170 */
int partial_products_multiply(ull a, ull b, ull *product, FILE *steps);
/* Halve a, double b, add the b's where a is odd. 11 x 12 = 12 + 24 + 96 = 132 */
int russian_peasant_multiply(ull a, ull b, ull *product, FILE *steps);
/* Table of b doubled (1x, 2x, 4x, ...), write a as a sum of those powers of 2.
   14 x 12 = (8 + 4 + 2) x 12 = 96 + 48 + 24 = 168 */
int duplication_multiply(ull a, ull b, ull *product, FILE *steps);

/* ---- Division ---- */

/* Subtract the divisor one at a time and count. 28 / 5 = 5 r3
   Note: takes one step per unit of the quotient, so it is slow when the
   quotient is huge (e.g. 10^18 / 1). */
int divide_repeated_subtraction(ull dividend, ull divisor,
                                ull *quotient, ull *remainder, FILE *steps);
/* Subtract big, easy multiples (100x, 20x, 4x, ...) and add them up.
   496 / 4 = 100 + 20 + 4 = 124 */
int divide_partial_quotients(ull dividend, ull divisor,
                             ull *quotient, ull *remainder, FILE *steps);
/* Find the two multiples of the divisor around the dividend (table of
   multiples, skipping by 5s, 50s, ... for big quotients) and pick the
   closer one. 50 / 7 is between 7 and 8, about 7.
   Returns -1 if the upper multiple doesn't fit, which only happens when
   the quotient is within about 10% of the largest unsigned long long. */
int estimate_quotient(ull dividend, ull divisor,
                      ull *low, ull *high, ull *estimate, FILE *steps);
/* Share dividend counters equally into divisor groups.
   27 / 4 = 6 r3, check: 4 x 6 + 3 = 27 */
int quotient_and_remainder(ull dividend, ull divisor,
                           ull *quotient, ull *remainder, FILE *steps);

/* ---- Interpreting the remainder (word problems) ---- */

/* 16 inches of candy for 5 people: 16 / 5 = 3 1/5 (fraction is simplified) */
int remainder_as_fraction(ull dividend, ull divisor,
                          ull *whole, ull *numerator, ull *denominator);
/* 30 people, 4 per car: 30 / 4 = 7 r2, so 8 cars */
int quotient_round_up(ull dividend, ull divisor, ull *result);
/* $20 among 3 friends, sister gets what's left: 20 / 3 = 6 r2, so $2 */
int remainder_only(ull dividend, ull divisor, ull *result);
/* $50 for $9 meals: 50 / 9 = 5 r5, so 5 meals */
int quotient_drop_remainder(ull dividend, ull divisor, ull *result);

#endif
