#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigint.h"

/* The number is stored in base 10^9: each "limb" holds 9 decimal digits,
   lowest limb first. Zero has len 0 and sign 0. */
#define BASE 1000000000u
#define BASE_DIGITS 9

struct BigInt {
    int sign;       /* -1, 0 or 1 */
    size_t len;     /* limbs in use */
    uint32_t *limb;
};

static BigInt *alloc_limbs(size_t len) {
    BigInt *x = malloc(sizeof *x);
    if (x == NULL) return NULL;
    x->limb = calloc(len ? len : 1, sizeof *x->limb);
    if (x->limb == NULL) {
        free(x);
        return NULL;
    }
    x->sign = 1;
    x->len = len;
    return x;
}

/* Drop leading zero limbs. */
static void trim(BigInt *x) {
    while (x->len > 0 && x->limb[x->len - 1] == 0) x->len--;
    if (x->len == 0) x->sign = 0;
}

static BigInt *copy(const BigInt *x) {
    BigInt *c = alloc_limbs(x->len);
    if (c == NULL) return NULL;
    memcpy(c->limb, x->limb, x->len * sizeof *x->limb);
    c->sign = x->sign;
    return c;
}

void bigint_free(BigInt *x) {
    if (x == NULL) return;
    free(x->limb);
    free(x);
}

BigInt *bigint_from_int(long long value) {
    unsigned long long mag = value < 0 ? 0ULL - (unsigned long long)value
                                       : (unsigned long long)value;
    BigInt *x = alloc_limbs(3); /* 2^64 has 20 digits = 3 limbs */
    if (x == NULL) return NULL;
    for (size_t i = 0; i < 3; i++) {
        x->limb[i] = (uint32_t)(mag % BASE);
        mag /= BASE;
    }
    x->sign = value < 0 ? -1 : 1;
    trim(x);
    return x;
}

BigInt *bigint_from_string(const char *text) {
    int sign = 1;
    if (*text == '+' || *text == '-') {
        if (*text == '-') sign = -1;
        text++;
    }
    size_t digits = strlen(text);
    if (digits == 0) return NULL;
    for (size_t i = 0; i < digits; i++) {
        if (text[i] < '0' || text[i] > '9') return NULL;
    }

    BigInt *x = alloc_limbs((digits + BASE_DIGITS - 1) / BASE_DIGITS);
    if (x == NULL) return NULL;
    /* Read 9 digits at a time, starting from the right-hand end. */
    size_t end = digits;
    for (size_t i = 0; i < x->len; i++) {
        size_t start = end >= BASE_DIGITS ? end - BASE_DIGITS : 0;
        uint32_t value = 0;
        for (size_t k = start; k < end; k++) value = value * 10 + (uint32_t)(text[k] - '0');
        x->limb[i] = value;
        end = start;
    }
    x->sign = sign;
    trim(x);
    return x;
}

char *bigint_to_string(const BigInt *x) {
    char *s = malloc(x->len * BASE_DIGITS + 2); /* digits + '-' + '\0' */
    if (s == NULL) return NULL;
    if (x->sign == 0) {
        strcpy(s, "0");
        return s;
    }
    char *p = s;
    if (x->sign < 0) *p++ = '-';
    p += sprintf(p, "%u", (unsigned)x->limb[x->len - 1]);
    for (size_t i = x->len - 1; i-- > 0;) {
        p += sprintf(p, "%09u", (unsigned)x->limb[i]); /* keep inner zeros */
    }
    return s;
}

/* Compare sizes, ignoring signs. */
static int compare_abs(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->limb[i] != b->limb[i]) return a->limb[i] < b->limb[i] ? -1 : 1;
    }
    return 0;
}

int bigint_compare(const BigInt *a, const BigInt *b) {
    if (a->sign != b->sign) return a->sign < b->sign ? -1 : 1;
    return a->sign * compare_abs(a, b);
}

/* |a| + |b| */
static BigInt *add_abs(const BigInt *a, const BigInt *b) {
    if (a->len < b->len) {
        const BigInt *t = a;
        a = b;
        b = t;
    }
    BigInt *r = alloc_limbs(a->len + 1);
    if (r == NULL) return NULL;
    uint32_t carry = 0;
    for (size_t i = 0; i < a->len; i++) {
        uint32_t sum = a->limb[i] + (i < b->len ? b->limb[i] : 0) + carry;
        carry = sum >= BASE;
        r->limb[i] = sum - (carry ? BASE : 0);
    }
    r->limb[a->len] = carry;
    trim(r);
    return r;
}

/* |a| - |b|, where |a| >= |b|; written into r (which may be a). */
static void sub_abs_into(BigInt *r, const BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; i++) {
        int64_t diff = (int64_t)a->limb[i] - (i < b->len ? b->limb[i] : 0) - borrow;
        borrow = diff < 0;
        r->limb[i] = (uint32_t)(diff + (borrow ? BASE : 0));
    }
    r->len = a->len;
    trim(r);
}

/* a + (b_sign * |b|) */
static BigInt *add_signed(const BigInt *a, const BigInt *b, int b_sign) {
    BigInt *r;
    if (b_sign == 0) return copy(a);
    if (a->sign == 0) {
        r = copy(b);
        if (r != NULL) r->sign = b_sign;
        return r;
    }
    if (a->sign == b_sign) {
        r = add_abs(a, b);
        if (r != NULL) r->sign = a->sign;
        return r;
    }
    /* Opposite signs: subtract the smaller size from the bigger one. */
    int cmp = compare_abs(a, b);
    if (cmp == 0) return bigint_from_int(0);
    const BigInt *big = cmp > 0 ? a : b;
    const BigInt *small = cmp > 0 ? b : a;
    r = alloc_limbs(big->len);
    if (r == NULL) return NULL;
    sub_abs_into(r, big, small);
    r->sign = cmp > 0 ? a->sign : b_sign;
    return r;
}

BigInt *bigint_add(const BigInt *a, const BigInt *b) {
    return add_signed(a, b, b->sign);
}

BigInt *bigint_subtract(const BigInt *a, const BigInt *b) {
    return add_signed(a, b, -b->sign);
}

BigInt *bigint_multiply(const BigInt *a, const BigInt *b) {
    if (a->sign == 0 || b->sign == 0) return bigint_from_int(0);
    BigInt *r = alloc_limbs(a->len + b->len);
    if (r == NULL) return NULL;
    /* Schoolbook multiplication, one limb of a at a time. */
    for (size_t i = 0; i < a->len; i++) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b->len; j++) {
            uint64_t cur = r->limb[i + j] + (uint64_t)a->limb[i] * b->limb[j] + carry;
            r->limb[i + j] = (uint32_t)(cur % BASE);
            carry = cur / BASE;
        }
        r->limb[i + b->len] = (uint32_t)carry;
    }
    r->sign = a->sign * b->sign;
    trim(r);
    return r;
}

/* r = |b| * m, for a single limb m. r must have room for b->len + 1 limbs. */
static void mul_small_into(BigInt *r, const BigInt *b, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; i++) {
        uint64_t cur = (uint64_t)b->limb[i] * m + carry;
        r->limb[i] = (uint32_t)(cur % BASE);
        carry = cur / BASE;
    }
    r->limb[b->len] = (uint32_t)carry;
    r->len = b->len + 1;
    r->sign = 1;
    trim(r);
}

int bigint_divmod(const BigInt *a, const BigInt *b,
                  BigInt **quotient, BigInt **remainder) {
    if (b->sign == 0) return -1;

    BigInt *q = alloc_limbs(a->len);
    BigInt *r = alloc_limbs(b->len + 1);    /* running remainder */
    BigInt *t = alloc_limbs(b->len + 1);    /* scratch: |b| * digit */
    if (q == NULL || r == NULL || t == NULL) {
        bigint_free(q);
        bigint_free(r);
        bigint_free(t);
        return -1;
    }
    r->len = 0;
    r->sign = 0;

    /* Long division, like on paper, but each "digit" is a whole limb. */
    for (size_t i = a->len; i-- > 0;) {
        /* Bring down the next limb: r = r * BASE + a[i]. */
        memmove(r->limb + 1, r->limb, r->len * sizeof *r->limb);
        r->limb[0] = a->limb[i];
        r->len++;
        r->sign = 1;
        trim(r);

        /* Find the biggest digit d with |b| * d <= r (binary search). */
        uint32_t lo = 0, hi = BASE - 1;
        while (lo < hi) {
            uint32_t mid = lo + (hi - lo + 1) / 2;
            mul_small_into(t, b, mid);
            if (compare_abs(t, r) <= 0) lo = mid;
            else hi = mid - 1;
        }
        if (lo > 0) {
            mul_small_into(t, b, lo);
            sub_abs_into(r, r, t);
        }
        q->limb[i] = lo;
    }
    bigint_free(t);

    q->sign = a->sign * b->sign;
    trim(q);
    if (r->len > 0) r->sign = a->sign;

    if (quotient != NULL) *quotient = q;
    else bigint_free(q);
    if (remainder != NULL) *remainder = r;
    else bigint_free(r);
    return 0;
}

BigInt *bigint_divide(const BigInt *a, const BigInt *b) {
    BigInt *q;
    if (bigint_divmod(a, b, &q, NULL) != 0) return NULL;
    return q;
}

/* ---- Means ---- */

static BigInt *abs_copy(const BigInt *x) {
    BigInt *c = copy(x);
    if (c != NULL && c->sign != 0) c->sign = 1;
    return c;
}

static BigInt *power_of_ten(unsigned exponent) {
    char *text = malloc(exponent + 2);
    if (text == NULL) return NULL;
    text[0] = '1';
    memset(text + 1, '0', exponent);
    text[exponent + 1] = '\0';
    BigInt *x = bigint_from_string(text);
    free(text);
    return x;
}

/* Writes num / den as decimal text, rounded half away from zero to
   `places` digits after the point. den must not be zero. */
static char *ratio_to_string(const BigInt *num, const BigInt *den, unsigned places) {
    char *result = NULL, *whole_text = NULL, *frac_text = NULL;
    BigInt *n = abs_copy(num), *d = abs_copy(den), *one = bigint_from_int(1);
    BigInt *whole = NULL, *rem = NULL, *scale = power_of_ten(places);
    BigInt *scaled = NULL, *frac = NULL, *frac_rem = NULL, *twice = NULL, *tmp = NULL;
    int negative = num->sign * den->sign < 0;

    if (n == NULL || d == NULL || one == NULL || scale == NULL) goto done;
    if (bigint_divmod(n, d, &whole, &rem) != 0) goto done;

    /* Digits after the point: (rem * 10^places) / den. */
    scaled = bigint_multiply(rem, scale);
    if (scaled == NULL || bigint_divmod(scaled, d, &frac, &frac_rem) != 0) goto done;

    /* Round: if what's left over is at least half of den, round up. */
    twice = bigint_add(frac_rem, frac_rem);
    if (twice == NULL) goto done;
    if (compare_abs(twice, d) >= 0) {
        tmp = bigint_add(frac, one);
        if (tmp == NULL) goto done;
        bigint_free(frac);
        frac = tmp;
        tmp = NULL;
        if (bigint_compare(frac, scale) == 0) { /* e.g. 0.999 -> 1.00 */
            bigint_free(frac);
            frac = bigint_from_int(0);
            tmp = bigint_add(whole, one);
            if (frac == NULL || tmp == NULL) goto done;
            bigint_free(whole);
            whole = tmp;
            tmp = NULL;
        }
    }

    whole_text = bigint_to_string(whole);
    frac_text = bigint_to_string(frac);
    if (whole_text == NULL || frac_text == NULL) goto done;
    if (whole->sign == 0 && frac->sign == 0) negative = 0; /* no "-0.00" */

    result = malloc(strlen(whole_text) + places + 3); /* '-' '.' '\0' */
    if (result == NULL) goto done;
    char *p = result;
    if (negative) *p++ = '-';
    p += sprintf(p, "%s", whole_text);
    if (places > 0) {
        /* Pad with leading zeros: 5 with places = 3 is ".005". */
        size_t frac_len = strlen(frac_text);
        *p++ = '.';
        memset(p, '0', places - frac_len);
        strcpy(p + (places - frac_len), frac_text);
    }

done:
    free(whole_text);
    free(frac_text);
    bigint_free(n);
    bigint_free(d);
    bigint_free(one);
    bigint_free(whole);
    bigint_free(rem);
    bigint_free(scale);
    bigint_free(scaled);
    bigint_free(frac);
    bigint_free(frac_rem);
    bigint_free(twice);
    bigint_free(tmp);
    return result;
}

/* Adds x to *total, replacing it. Returns 0 on success. */
static int accumulate(BigInt **total, const BigInt *x) {
    BigInt *sum = bigint_add(*total, x);
    if (sum == NULL) return -1;
    bigint_free(*total);
    *total = sum;
    return 0;
}

char *bigint_mean(BigInt *const *values, size_t n, unsigned places) {
    if (n == 0) return NULL;
    char *result = NULL;
    BigInt *sum = bigint_from_int(0);
    BigInt *count = bigint_from_int((long long)n);
    if (sum == NULL || count == NULL) goto done;
    for (size_t i = 0; i < n; i++) {
        if (accumulate(&sum, values[i]) != 0) goto done;
    }
    result = ratio_to_string(sum, count, places);
done:
    bigint_free(sum);
    bigint_free(count);
    return result;
}

char *bigint_weighted_mean(BigInt *const *values, BigInt *const *weights,
                           size_t n, unsigned places) {
    if (n == 0) return NULL;
    char *result = NULL;
    BigInt *weighted_sum = bigint_from_int(0);
    BigInt *weight_sum = bigint_from_int(0);
    if (weighted_sum == NULL || weight_sum == NULL) goto done;
    for (size_t i = 0; i < n; i++) {
        BigInt *term = bigint_multiply(weights[i], values[i]);
        int failed = term == NULL || accumulate(&weighted_sum, term) != 0
                     || accumulate(&weight_sum, weights[i]) != 0;
        bigint_free(term);
        if (failed) goto done;
    }
    if (weight_sum->sign != 0) result = ratio_to_string(weighted_sum, weight_sum, places);
done:
    bigint_free(weighted_sum);
    bigint_free(weight_sum);
    return result;
}

/* Greatest common divisor of |a| and |b|. */
static BigInt *gcd(const BigInt *a, const BigInt *b) {
    BigInt *x = abs_copy(a), *y = abs_copy(b);
    while (x != NULL && y != NULL && y->sign != 0) {
        BigInt *r;
        if (bigint_divmod(x, y, NULL, &r) != 0) r = NULL;
        bigint_free(x);
        x = y;
        y = r;
    }
    if (y == NULL) {
        bigint_free(x);
        return NULL;
    }
    bigint_free(y);
    return x;
}

char *bigint_harmonic_mean(BigInt *const *values, size_t n, unsigned places) {
    if (n == 0) return NULL;
    for (size_t i = 0; i < n; i++) {
        if (values[i]->sign == 0) return NULL; /* 1/0 is undefined */
    }

    /* Keep 1/x1 + ... + 1/xn as an exact fraction top/bottom:
       top/bottom + 1/x = (top*x + bottom) / (bottom*x), then simplify. */
    char *result = NULL;
    BigInt *top = bigint_from_int(0), *bottom = bigint_from_int(1);
    BigInt *count = bigint_from_int((long long)n), *numerator = NULL;
    if (top == NULL || bottom == NULL || count == NULL) goto done;

    for (size_t i = 0; i < n; i++) {
        BigInt *top_x = bigint_multiply(top, values[i]);
        BigInt *new_top = top_x ? bigint_add(top_x, bottom) : NULL;
        BigInt *new_bottom = bigint_multiply(bottom, values[i]);
        BigInt *g = (new_top && new_bottom) ? gcd(new_top, new_bottom) : NULL;
        BigInt *reduced_top = g ? bigint_divide(new_top, g) : NULL;
        BigInt *reduced_bottom = g ? bigint_divide(new_bottom, g) : NULL;
        bigint_free(top_x);
        bigint_free(new_top);
        bigint_free(new_bottom);
        bigint_free(g);
        if (reduced_top == NULL || reduced_bottom == NULL) {
            bigint_free(reduced_top);
            bigint_free(reduced_bottom);
            goto done;
        }
        bigint_free(top);
        bigint_free(bottom);
        top = reduced_top;
        bottom = reduced_bottom;
    }

    /* n / (top/bottom) = n*bottom / top */
    if (top->sign == 0) goto done;
    numerator = bigint_multiply(count, bottom);
    if (numerator != NULL) result = ratio_to_string(numerator, top, places);
done:
    bigint_free(top);
    bigint_free(bottom);
    bigint_free(count);
    bigint_free(numerator);
    return result;
}
