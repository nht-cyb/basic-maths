# basic-maths

Basic maths functions in C: whole-number arithmetic, fractions, consumer
maths, geometry, percentages, statistics, matrices, numbers of any size,
special numbers (prime, amicable, deficient, perfect), and the advanced multiplication and division methods from
[basic-mathematics.com](https://www.basic-mathematics.com/pre-algebra-lessons.html).

This is a git practice repo.

## Build and run

```bash
make          # builds ./maths from every file in src/
./maths       # runs the demo in src/main.c
make clean
```

To use a topic in your own code, include its header and compile its `.c`
file with `-Iinclude`:

```c
#include "geometry.h"   /* compile with src/geometry.c */
```

## Project layout

| Header | Topic |
|---|---|
| [`maths.h`](include/maths.h) | Add, subtract, multiply, divide (`int`) |
| [`fraction.h`](include/fraction.h) | Fractions and mixed numbers |
| [`consumer.h`](include/consumer.h) | Discounts, tax, tips, commission, interest |
| [`geometry.h`](include/geometry.h) | Area, perimeter, volume, surface area |
| [`percent.h`](include/percent.h) | Percentages, increase and decrease |
| [`stats.h`](include/stats.h) | Mean, median, mode |
| [`matrix.h`](include/matrix.h) | Matrix add, subtract, multiply |
| [`bigint.h`](include/bigint.h) | Whole numbers of any size, and their means |
| [`advanced_ops.h`](include/advanced_ops.h) | Lattice, Russian peasant, partial quotients, and more |
| [`number_types.h`](include/number_types.h) | Prime, amicable, deficient and perfect numbers |

Every source file is in [`src/`](src) with the same name, e.g. `src/geometry.c`.

---

## Whole numbers — `maths.h`

| Function | Example | Result |
|---|---|---|
| `add(a, b)` | `add(7, 3)` | `10` |
| `subtract(a, b)` | `subtract(7, 3)` | `4` |
| `multiply(a, b)` | `multiply(7, 3)` | `21` |
| `divide(a, b)` | `divide(7, 2)` | `3.5` |

`divide` returns `0.0` if `b` is 0.

## Fractions — `fraction.h`

A `Fraction` is `{ num, den }` and a `Mixed` number is `{ whole, num, den }`.
Every result is simplified, with the sign kept on the numerator.

| Function | Example | Result |
|---|---|---|
| `fraction_make(num, den)` | `fraction_make(2, 4)` | `1/2` |
| `fraction_simplify(f)` | `fraction_simplify((Fraction){6, 8})` | `3/4` |
| `fraction_add(x, y)` | 1/2 + 1/3 | `5/6` |
| `fraction_subtract(x, y)` | 1/2 − 1/3 | `1/6` |
| `fraction_multiply(x, y)` | 2/3 × 3/4 | `1/2` |
| `fraction_divide(x, y)` | 1/2 ÷ 1/4 | `2/1` |
| `improper_to_mixed(f)` | 7/2 | `3 1/2` |
| `mixed_to_improper(m)` | 2 1/3 | `7/3` |

```c
Fraction sum = fraction_add(fraction_make(1, 2), fraction_make(1, 3));
printf("%ld/%ld\n", sum.num, sum.den);   /* 5/6 */
```

## Consumer maths — `consumer.h`

Rates are decimals: 25% is `0.25`.

| Function | Example | Result |
|---|---|---|
| `discount(list_price, rate)` | `discount(80, 0.25)` | `20` |
| `sale_price(list_price, rate)` | `sale_price(80, 0.25)` | `60` |
| `discount_rate(discount, list_price)` | `discount_rate(20, 80)` | `0.25` |
| `sales_tax(price, rate)` | `sales_tax(50, 0.08)` | `4` |
| `tip(meal_cost, rate)` | `tip(40, 0.15)` | `6` |
| `commission(service_cost, rate)` | `commission(200, 0.05)` | `10` |
| `simple_interest(principal, rate, time)` | `simple_interest(1000, 0.05, 3)` | `150` |

## Geometry — `geometry.h`

### 2D shapes

| Function | Example | Result |
|---|---|---|
| `rectangle_area(l, w)` | `rectangle_area(4, 3)` | `12` |
| `rectangle_perimeter(l, w)` | `rectangle_perimeter(4, 3)` | `14` |
| `square_area(s)` | `square_area(5)` | `25` |
| `square_perimeter(s)` | `square_perimeter(5)` | `20` |
| `circle_area(r)` | `circle_area(2)` | `12.57` |
| `circle_circumference(r)` | `circle_circumference(3)` | `18.85` |
| `triangle_area(base, height)` | `triangle_area(6, 4)` | `12` |
| `triangle_perimeter(a, b, c)` | `triangle_perimeter(3, 4, 5)` | `12` |
| `trapezoid_area(b1, b2, height)` | `trapezoid_area(3, 5, 4)` | `16` |
| `trapezoid_perimeter(a, b1, b2, c)` | `trapezoid_perimeter(4, 3, 5, 4)` | `16` |

### 3D shapes

| Function | Example | Result |
|---|---|---|
| `cube_volume(s)` | `cube_volume(3)` | `27` |
| `cube_surface_area(s)` | `cube_surface_area(3)` | `54` |
| `rect_prism_volume(l, w, h)` | `rect_prism_volume(2, 3, 4)` | `24` |
| `rect_prism_surface_area(l, w, h)` | `rect_prism_surface_area(2, 3, 4)` | `52` |
| `sphere_volume(r)` | `sphere_volume(2)` | `33.51` |
| `sphere_surface_area(r)` | `sphere_surface_area(2)` | `50.27` |
| `cylinder_volume(r, h)` | `cylinder_volume(2, 5)` | `62.83` |
| `cylinder_surface_area(r, h)` | `cylinder_surface_area(2, 5)` | `87.96` |
| `tri_prism_volume(b, h, l)` | `tri_prism_volume(3, 4, 10)` | `60` |
| `tri_prism_surface_area(b, h, l, s1, s2, s3)` | `tri_prism_surface_area(3, 4, 10, 3, 4, 5)` | `132` |

Decimal results are rounded to 2 places here; the functions return the full `double`.

## Percentages — `percent.h`

| Function | Example | Result |
|---|---|---|
| `percentage(part, whole)` | `percentage(15, 60)` | `25` (%) |
| `part_from_percentage(percentage, whole)` | `part_from_percentage(20, 150)` | `30` |
| `whole_from_percentage(part, percentage)` | `whole_from_percentage(30, 25)` | `120` |
| `percent_increase(original, new_value)` | `percent_increase(50, 65)` | `30` (%) |
| `percent_decrease(original, new_value)` | `percent_decrease(80, 60)` | `25` (%) |

## Statistics — `stats.h`

The examples use `double data[] = {4, 8, 6, 5, 3, 8, 9};` with `n = 7`.
Every function needs `n > 0` and leaves the array unchanged.

| Function | Example | Result |
|---|---|---|
| `mean(values, n)` | `mean(data, 7)` | `6.14` |
| `median(values, n)` | `median(data, 7)` | `6` |
| `mode(values, n)` | `mode(data, 7)` | `8` (ties pick the smallest value) |

## Matrices — `matrix.h`

A matrix is created on the heap: call `matrix_free()` on every one you create.
Operations return `NULL` when the sizes don't fit together.

```c
Matrix *a = matrix_from_array(2, 3, (double[]){ 1, 2, 3,
                                                4, 5, 6 });
Matrix *c = matrix_from_array(3, 2, (double[]){ 7,  8,
                                                9, 10,
                                               11, 12 });
Matrix *product = matrix_multiply(a, c);
matrix_print(product);
/* |   58.00   64.00 |
   |  139.00  154.00 | */
matrix_free(a); matrix_free(c); matrix_free(product);
```

| Function | Example | Result |
|---|---|---|
| `matrix_create(rows, cols)` | `matrix_create(2, 2)` | 2×2 of zeros |
| `matrix_from_array(rows, cols, values)` | `matrix_from_array(2, 3, ...)` | `A` above |
| `matrix_get(m, row, col)` | `matrix_get(a, 1, 2)` | `6` (rows and columns start at 0) |
| `matrix_set(m, row, col, value)` | `matrix_set(a, 0, 0, 9)` | top-left of `a` becomes `9` |
| `matrix_add(a, b)` | `[1 2; 3 4] + [5 6; 7 8]` | `[6 8; 10 12]` |
| `matrix_subtract(a, b)` | `[5 6; 7 8] − [1 2; 3 4]` | `[4 4; 4 4]` |
| `matrix_multiply(a, b)` | `A (2×3) × C (3×2)` | `[58 64; 139 154]` |
| `matrix_print(m)` | `matrix_print(product)` | prints the table above |
| `matrix_free(m)` | `matrix_free(a)` | frees `a` |

## Numbers of any size — `bigint.h`

`BigInt` holds whole numbers with as many digits as memory allows. Create them
from text, and free every `BigInt` with `bigint_free()` and every `char *`
with `free()`. Functions return `NULL` on bad input (like `"12a"` or dividing
by zero).

```c
BigInt *a = bigint_from_string("12345678901234567890");
BigInt *b = bigint_from_string("98765432109876543210");
BigInt *product = bigint_multiply(a, b);
char *text = bigint_to_string(product);
printf("%s\n", text);   /* 1219326311370217952237463801111263526900 */
free(text);
bigint_free(a); bigint_free(b); bigint_free(product);
```

| Function | Example | Result |
|---|---|---|
| `bigint_from_string(text)` | `bigint_from_string("-123456789012345678901234567890")` | a `BigInt` |
| `bigint_from_int(value)` | `bigint_from_int(42)` | a `BigInt` |
| `bigint_to_string(x)` | of `bigint_from_int(42)` | `"42"` |
| `bigint_free(x)` | `bigint_free(a)` | frees `a` |
| `bigint_compare(a, b)` | 12345678901234567890 vs 98765432109876543210 | `-1` (a < b) |
| `bigint_add(a, b)` | 99999999999999999999 + 1 | `100000000000000000000` |
| `bigint_subtract(a, b)` | 100000000000000000000 − 1 | `99999999999999999999` |
| `bigint_multiply(a, b)` | 12345678901234567890 × 98765432109876543210 | `1219326311370217952237463801111263526900` |
| `bigint_divmod(a, b, &q, &r)` | 100000000000000000000 ÷ 7 | q = `14285714285714285714`, r = `2` |
| `bigint_divide(a, b)` | 100000000000000000000 ÷ 7 | `14285714285714285714` |
| `bigint_mean(values, n, places)` | 10²⁰, 2×10²⁰, 4×10²⁰, 2 places | `"233333333333333333333.33"` |
| `bigint_weighted_mean(values, weights, n, places)` | 45, 50, 65, 95 with weights 1, 1, 2, 3, 2 places | `"72.86"` |
| `bigint_harmonic_mean(values, n, places)` | 1, 2, 4, 10, 4 places | `"2.1622"` |

Division rounds toward zero like C's `/` and `%`. The means are returned as
decimal text, rounded to `places` digits.

**Speed limits (measured):** add, subtract and mean handle 160,000+ digits in
well under a second. Weighted mean takes about 6 s at 40,000 digits. Harmonic
mean is the slowest: about 1.5 s for 2 numbers of 1,280 digits, and about 11 s
for 10 numbers of 200 digits.

## Advanced multiplication and division — `advanced_ops.h`

These follow the
[Advanced Multiplication and Division](https://www.basic-mathematics.com/pre-algebra-lessons.html)
lessons and work on `unsigned long long` (up to 20 digits). Each method takes
a last argument `steps`: pass `stdout` to print the working like on paper, or
`NULL` to just get the answer. They return `0` on success, or `-1` if the
answer doesn't fit or the divisor is 0.

```c
ull product;
russian_peasant_multiply(37, 42, &product, stdout);
/* Russian peasant 37 x 42:
     halving              doubling
     37                   42
     18                   84  (crossed out)
     9                    168
     4                    336  (crossed out)
     2                    672  (crossed out)
     1                    1344
     sum of rows not crossed out: 1554 */
```

| Function | Example | Result |
|---|---|---|
| `lattice_multiply(a, b, &product, steps)` | 42 × 35 | `1470` |
| `partial_products_multiply(a, b, &product, steps)` | 26 × 45 = 800 + 100 + 240 + 30 | `1170` |
| `russian_peasant_multiply(a, b, &product, steps)` | 37 × 42 = 42 + 168 + 1344 | `1554` |
| `duplication_multiply(a, b, &product, steps)` | 14 × 12 = 96 + 48 + 24 | `168` |
| `divide_repeated_subtraction(dividend, divisor, &q, &r, steps)` | 28 ÷ 5 | `5 r3` |
| `divide_partial_quotients(dividend, divisor, &q, &r, steps)` | 496 ÷ 4 = 100 + 20 + 4 | `124 r0` |
| `estimate_quotient(dividend, divisor, &low, &high, &estimate, steps)` | 152 ÷ 6 | between `25` and `30`, about `25` |
| `quotient_and_remainder(dividend, divisor, &q, &r, steps)` | 27 ÷ 4 | `6 r3` (check: 4 × 6 + 3 = 27) |
| `remainder_as_fraction(dividend, divisor, &whole, &num, &den)` | 16 in of candy for 5 people | `3 1/5` in each |
| `quotient_round_up(dividend, divisor, &result)` | 30 people, 4 per car | `8` cars |
| `remainder_only(dividend, divisor, &result)` | $20 among 3 friends, sister gets the rest | `$2` |
| `quotient_drop_remainder(dividend, divisor, &result)` | $50 for $9 meals | `5` meals |

`divide_repeated_subtraction` takes one step per unit of the quotient, so it is
very slow for huge quotients (e.g. 10¹⁸ ÷ 1). `estimate_quotient` returns `-1`
when the quotient is within about 10% of the largest `unsigned long long`.

## Special numbers — `number_types.h`

These are based on the *proper divisors* of a number: every divisor except the
number itself (for 12 that is 1, 2, 3, 4, 6). The kinds overlap: every prime is
also deficient, and 284 is both amicable and deficient. `0` is none of them.
The `is_` functions return `1` for yes and `0` for no.

`special_numbers_below(n)` returns a 4-row `Matrix` listing every special number
smaller than `n`, one kind per row in this order: prime, amicable, deficient,
perfect (`ROW_PRIME`, `ROW_AMICABLE`, `ROW_DEFICIENT`, `ROW_PERFECT`). Rows are
as long as the longest list and the shorter ones end in `0`s, so stop reading a
row at the first `0`. Call `matrix_free()` when done.

```c
Matrix *special = special_numbers_below(10);
matrix_print(special);
/* |    2.00    3.00    5.00    7.00    0.00    0.00    0.00    0.00 |   prime
   |    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00 |   amicable
   |    1.00    2.00    3.00    4.00    5.00    7.00    8.00    9.00 |   deficient
   |    6.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00 |   perfect */
double first_perfect = matrix_get(special, ROW_PERFECT, 0);   /* 6 */
matrix_free(special);
```

| Function | Example | Result |
|---|---|---|
| `sum_proper_divisors(n)` | 12 → 1 + 2 + 3 + 4 + 6 | `16` |
| `is_prime(n)` | `is_prime(29)` | `1` (only 1 and 29 divide it) |
| `is_amicable(n)` | `is_amicable(220)` | `1` (220 → 284 and 284 → 220) |
| `is_deficient(n)` | `is_deficient(8)` | `1` (1 + 2 + 4 = 7 < 8) |
| `is_perfect(n)` | `is_perfect(28)` | `1` (1 + 2 + 4 + 7 + 14 = 28) |
| `special_numbers_below(n)` | `special_numbers_below(10)` | the matrix above |

The `is_` functions test divisors up to √n, so they are quick for numbers up
to about 10¹² and slow (minutes) near the top of `unsigned long long`.
`special_numbers_below(n)` uses memory and time that grow with `n`; it works
well into the millions.

## License

[MIT](LICENSE)
