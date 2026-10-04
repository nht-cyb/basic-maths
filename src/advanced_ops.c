#include "advanced_ops.h"

#define MAX_DIGITS 20 /* an unsigned long long has at most 20 digits */
#define MAX_PRINTED_STEPS 12

/* Writes the digits of n into out, most significant first; returns count. */
static int to_digits(ull n, int out[MAX_DIGITS]) {
    int tmp[MAX_DIGITS], count = 0;
    do {
        tmp[count++] = (int)(n % 10);
        n /= 10;
    } while (n > 0);
    for (int i = 0; i < count; i++) out[i] = tmp[count - 1 - i];
    return count;
}

static ull power_of_ten(int exponent) {
    ull p = 1;
    while (exponent-- > 0) p *= 10;
    return p;
}

/* *total += x; returns -1 if that would overflow. */
static int add_checked(ull *total, ull x) {
    if (*total > ~0ULL - x) return -1;
    *total += x;
    return 0;
}

/* ---- Multiplication ---- */

int lattice_multiply(ull a, ull b, ull *product, FILE *steps) {
    int top[MAX_DIGITS], side[MAX_DIGITS];
    int cols = to_digits(a, top);
    int rows = to_digits(b, side);

    /* Each cell holds a digit product split into tens (above the diagonal)
       and ones (below it). */
    int tens[MAX_DIGITS][MAX_DIGITS], ones[MAX_DIGITS][MAX_DIGITS];
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int p = side[r] * top[c];
            tens[r][c] = p / 10;
            ones[r][c] = p % 10;
        }
    }

    if (steps) {
        fprintf(steps, "Lattice %llu x %llu (tens/ones in each cell):\n    ", a, b);
        for (int c = 0; c < cols; c++) fprintf(steps, "   %d  ", top[c]);
        fprintf(steps, "\n");
        for (int r = 0; r < rows; r++) {
            fprintf(steps, "    ");
            for (int c = 0; c < cols; c++) fprintf(steps, "| %d/%d ", tens[r][c], ones[r][c]);
            fprintf(steps, "|  %d\n", side[r]);
        }
    }

    /* Diagonal k (0 = bottom-right) collects the ones of cells with
       (rows-1-r) + (cols-1-c) == k and the tens of cells one diagonal lower. */
    int diagonals = rows + cols;
    int answer[2 * MAX_DIGITS];
    int carry = 0;
    for (int k = 0; k < diagonals; k++) {
        int sum = carry;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int pos = (rows - 1 - r) + (cols - 1 - c);
                if (pos == k) sum += ones[r][c];
                if (pos + 1 == k) sum += tens[r][c];
            }
        }
        answer[k] = sum % 10;
        carry = sum / 10;
        if (steps) fprintf(steps, "  diagonal %d: sum %d -> write %d, carry %d\n",
                           k + 1, sum, sum % 10, carry);
    }

    /* Read the diagonal digits from the left. */
    ull result = 0;
    for (int k = diagonals - 1; k >= 0; k--) {
        if (result > (~0ULL - answer[k]) / 10) return -1;
        result = result * 10 + answer[k];
    }
    if (steps) fprintf(steps, "  answer: %llu\n", result);
    *product = result;
    return 0;
}

int partial_products_multiply(ull a, ull b, ull *product, FILE *steps) {
    int da[MAX_DIGITS], db[MAX_DIGITS];
    int na = to_digits(a, da);
    int nb = to_digits(b, db);
    ull total = 0;

    if (steps) fprintf(steps, "Partial products %llu x %llu:\n", a, b);
    for (int i = 0; i < na; i++) {
        if (da[i] == 0) continue;
        ull part_a = da[i] * power_of_ten(na - 1 - i); /* e.g. 2 tens = 20 */
        for (int j = 0; j < nb; j++) {
            if (db[j] == 0) continue;
            ull part_b = db[j] * power_of_ten(nb - 1 - j);
            if (part_b != 0 && part_a > ~0ULL / part_b) return -1;
            ull partial = part_a * part_b;
            if (add_checked(&total, partial) != 0) return -1;
            if (steps) fprintf(steps, "  %llu x %llu = %llu\n", part_a, part_b, partial);
        }
    }
    if (steps) fprintf(steps, "  sum of partial products: %llu\n", total);
    *product = total;
    return 0;
}

int russian_peasant_multiply(ull a, ull b, ull *product, FILE *steps) {
    ull total = 0;
    int overflow = 0;

    if (steps) fprintf(steps, "Russian peasant %llu x %llu:\n  %-20s %s\n", a, b,
                       "halving", "doubling");
    while (a > 0) {
        int keep = a % 2 == 1; /* rows with an even halving number are crossed out */
        if (steps) fprintf(steps, "  %-20llu %llu%s\n", a, b, keep ? "" : "  (crossed out)");
        if (keep && add_checked(&total, b) != 0) overflow = 1;
        a /= 2;
        if (a > 0) {
            if (b > ~0ULL / 2) overflow = 1;
            b *= 2;
        }
        if (overflow) return -1;
    }
    if (steps) fprintf(steps, "  sum of rows not crossed out: %llu\n", total);
    *product = total;
    return 0;
}

int duplication_multiply(ull a, ull b, ull *product, FILE *steps) {
    /* Doubling table: power[i] = 2^i, doubled[i] = 2^i * b. */
    ull power[64], doubled[64];
    int count = 0;
    power[0] = 1;
    doubled[0] = b;
    count = 1;
    while (power[count - 1] <= a / 2) {
        power[count] = power[count - 1] * 2;
        doubled[count] = doubled[count - 1] * 2;
        if (doubled[count - 1] > ~0ULL / 2) {
            /* Only a problem if we would actually use this row. */
            doubled[count] = 0;
        }
        count++;
    }

    if (steps) {
        fprintf(steps, "Duplication %llu x %llu, doubling table:\n", a, b);
        for (int i = 0; i < count; i++) {
            if (doubled[i] == 0 && b != 0) fprintf(steps, "  %llu x %llu = (too big)\n", power[i], b);
            else fprintf(steps, "  %llu x %llu = %llu\n", power[i], b, doubled[i]);
        }
        fprintf(steps, "  %llu =", a);
    }

    /* Take the biggest powers of 2 that still fit (a in binary). */
    ull left = a, total = 0;
    int first = 1;
    for (int i = count - 1; i >= 0 && a > 0; i--) {
        if (power[i] <= left) {
            if (doubled[i] == 0 && b != 0) return -1;
            left -= power[i];
            if (add_checked(&total, doubled[i]) != 0) return -1;
            if (steps) fprintf(steps, "%s %llu", first ? "" : " +", power[i]);
            first = 0;
        }
    }
    if (steps) {
        if (a == 0) fprintf(steps, " 0");
        fprintf(steps, ", so the answer is %llu\n", total);
    }
    *product = total;
    return 0;
}

/* ---- Division ---- */

int divide_repeated_subtraction(ull dividend, ull divisor,
                                ull *quotient, ull *remainder, FILE *steps) {
    if (divisor == 0) return -1;
    ull left = dividend, count = 0;

    if (steps) fprintf(steps, "Repeated subtraction %llu / %llu:\n", dividend, divisor);
    while (left >= divisor) {
        if (steps && count < MAX_PRINTED_STEPS)
            fprintf(steps, "  %llu - %llu = %llu\n", left, divisor, left - divisor);
        left -= divisor;
        count++;
    }
    if (steps) {
        if (count > MAX_PRINTED_STEPS)
            fprintf(steps, "  ... (%llu more subtractions)\n", count - MAX_PRINTED_STEPS);
        fprintf(steps, "  subtracted %llu times, %llu left: %llu r%llu\n",
                count, left, count, left);
    }
    *quotient = count;
    *remainder = left;
    return 0;
}

int divide_partial_quotients(ull dividend, ull divisor,
                             ull *quotient, ull *remainder, FILE *steps) {
    if (divisor == 0) return -1;
    ull left = dividend, total = 0;

    if (steps) fprintf(steps, "Partial quotients %llu / %llu:\n", dividend, divisor);
    while (left >= divisor) {
        /* Biggest easy multiple: digit x power of ten, like 100, 20 or 4. */
        ull place = 1;
        while (place <= left / divisor / 10) place *= 10;
        ull digit = left / divisor / place; /* 1..9 */
        ull partial = digit * place;
        ull amount = partial * divisor;
        if (steps) fprintf(steps, "  %llu - %llu (%llu x %llu) = %llu\n",
                           left, amount, partial, divisor, left - amount);
        left -= amount;
        total += partial;
    }
    if (steps) fprintf(steps, "  add the partial quotients: %llu r%llu\n", total, left);
    *quotient = total;
    *remainder = left;
    return 0;
}

int estimate_quotient(ull dividend, ull divisor,
                      ull *low, ull *high, ull *estimate, FILE *steps) {
    if (divisor == 0) return -1;
    ull q = dividend / divisor;

    /* Skip through the table: by 1s for small quotients, then by 5s, 50s,
       500s... so the table stays short (152 / 6 uses 10, 15, 20, 25, 30). */
    ull step = 1;
    if (q >= 10) {
        step = 5;
        while (step <= q / 100) step *= 10;
    }
    ull lo = q / step * step;
    if (lo > ~0ULL - step) return -1;
    ull hi = lo + step;

    if (steps) {
        /* Show a few rows of the table leading up to the answer. */
        fprintf(steps, "Estimate %llu / %llu with multiples of %llu:\n", dividend, divisor, divisor);
        ull start = lo > 3 * step ? lo - 3 * step : step;
        for (ull m = start; m <= hi; m += step) {
            if (divisor > ~0ULL / m) {
                fprintf(steps, "  %llu x %llu = (too big)\n", divisor, m);
                break;
            }
            fprintf(steps, "  %llu x %llu = %llu\n", divisor, m, divisor * m);
        }
    }

    /* Pick whichever multiple is closer to the dividend (ties go low). */
    ull below = dividend - divisor * lo;
    int high_fits = divisor <= ~0ULL / hi;
    ull above = high_fits ? divisor * hi - dividend : ~0ULL;
    ull best = above < below ? hi : lo;

    if (steps) fprintf(steps, "  %llu is between %llu x %llu and %llu x %llu, closest: about %llu\n",
                       dividend, divisor, lo, divisor, hi, best);
    *low = lo;
    *high = hi;
    *estimate = best;
    return 0;
}

int quotient_and_remainder(ull dividend, ull divisor,
                           ull *quotient, ull *remainder, FILE *steps) {
    if (divisor == 0) return -1;
    ull q = dividend / divisor, r = dividend % divisor;
    if (steps) {
        fprintf(steps, "Share %llu counters into %llu groups:\n", dividend, divisor);
        fprintf(steps, "  each group gets %llu, %llu left over (less than %llu)\n", q, r, divisor);
        fprintf(steps, "  check: (%llu x %llu) + %llu = %llu\n", divisor, q, r, divisor * q + r);
    }
    *quotient = q;
    *remainder = r;
    return 0;
}

/* ---- Interpreting the remainder ---- */

static ull gcd(ull a, ull b) {
    while (b != 0) {
        ull t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int remainder_as_fraction(ull dividend, ull divisor,
                          ull *whole, ull *numerator, ull *denominator) {
    if (divisor == 0) return -1;
    ull r = dividend % divisor;
    ull g = r ? gcd(r, divisor) : divisor;
    *whole = dividend / divisor;
    *numerator = r / g;
    *denominator = divisor / g;
    return 0;
}

int quotient_round_up(ull dividend, ull divisor, ull *result) {
    if (divisor == 0) return -1;
    *result = dividend / divisor + (dividend % divisor != 0);
    return 0;
}

int remainder_only(ull dividend, ull divisor, ull *result) {
    if (divisor == 0) return -1;
    *result = dividend % divisor;
    return 0;
}

int quotient_drop_remainder(ull dividend, ull divisor, ull *result) {
    if (divisor == 0) return -1;
    *result = dividend / divisor;
    return 0;
}
