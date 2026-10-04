#ifndef FRACTION_H
#define FRACTION_H

typedef struct {
    long num; /* numerator */
    long den; /* denominator, never 0 */
} Fraction;

typedef struct {
    long whole;
    long num;
    long den;
} Mixed;

Fraction fraction_make(long num, long den);
Fraction fraction_simplify(Fraction f);

/* a/b + c/d = (ad + bc) / bd */
Fraction fraction_add(Fraction x, Fraction y);
/* a/b - c/d = (ad - bc) / bd */
Fraction fraction_subtract(Fraction x, Fraction y);
/* a/b * c/d = ac / bd */
Fraction fraction_multiply(Fraction x, Fraction y);
/* a/b / c/d = ad / bc */
Fraction fraction_divide(Fraction x, Fraction y);

/* a/b = q r/b (q = quotient, r = remainder) */
Mixed improper_to_mixed(Fraction f);
/* a b/c = (a*c + b) / c */
Fraction mixed_to_improper(Mixed m);

#endif
