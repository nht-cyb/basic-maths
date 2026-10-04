#include "fraction.h"

static long gcd(long a, long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

Fraction fraction_make(long num, long den) {
    Fraction f = { num, den };
    return fraction_simplify(f);
}

Fraction fraction_simplify(Fraction f) {
    long g = gcd(f.num, f.den);
    if (g != 0) {
        f.num /= g;
        f.den /= g;
    }
    if (f.den < 0) { /* keep the sign on the numerator */
        f.num = -f.num;
        f.den = -f.den;
    }
    return f;
}

Fraction fraction_add(Fraction x, Fraction y) {
    return fraction_make(x.num * y.den + y.num * x.den, x.den * y.den);
}

Fraction fraction_subtract(Fraction x, Fraction y) {
    return fraction_make(x.num * y.den - y.num * x.den, x.den * y.den);
}

Fraction fraction_multiply(Fraction x, Fraction y) {
    return fraction_make(x.num * y.num, x.den * y.den);
}

Fraction fraction_divide(Fraction x, Fraction y) {
    return fraction_make(x.num * y.den, x.den * y.num);
}

Mixed improper_to_mixed(Fraction f) {
    Mixed m;
    f = fraction_simplify(f);
    m.whole = f.num / f.den;
    m.num = f.num % f.den;
    m.den = f.den;
    if (m.whole != 0 && m.num < 0) { /* -7/2 -> -3 1/2, not -3 -1/2 */
        m.num = -m.num;
    }
    return m;
}

Fraction mixed_to_improper(Mixed m) {
    long sign = m.whole < 0 ? -1 : 1;
    return fraction_make(m.whole * m.den + sign * m.num, m.den);
}
