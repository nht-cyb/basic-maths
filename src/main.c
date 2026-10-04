#include <stdio.h>
#include "maths.h"
#include "fraction.h"
#include "consumer.h"
#include "geometry.h"
#include "percent.h"
#include "stats.h"
#include "matrix.h"
#include "advanced_ops.h"

static void print_fraction(const char *label, Fraction f) {
    printf("%s = %ld/%ld\n", label, f.num, f.den);
}

int main(void) {
    printf("== Whole numbers ==\n");
    printf("7 + 3 = %d\n", add(7, 3));
    printf("7 - 3 = %d\n", subtract(7, 3));
    printf("7 * 3 = %d\n", multiply(7, 3));
    printf("7 / 3 = %.2f\n", divide(7, 3));

    printf("\n== Fractions ==\n");
    Fraction half = fraction_make(1, 2);
    Fraction third = fraction_make(1, 3);
    print_fraction("1/2 + 1/3", fraction_add(half, third));
    print_fraction("1/2 - 1/3", fraction_subtract(half, third));
    print_fraction("1/2 * 1/3", fraction_multiply(half, third));
    print_fraction("1/2 / 1/3", fraction_divide(half, third));
    Mixed m = improper_to_mixed(fraction_make(7, 2));
    printf("7/2 as mixed = %ld %ld/%ld\n", m.whole, m.num, m.den);
    Mixed two_and_third = { 2, 1, 3 };
    print_fraction("2 1/3 as improper", mixed_to_improper(two_and_third));

    printf("\n== Consumer math ==\n");
    printf("Discount on $80 at 25%%   = $%.2f\n", discount(80, 0.25));
    printf("Sale price               = $%.2f\n", sale_price(80, 0.25));
    printf("Discount rate ($20/$80)  = %.2f\n", discount_rate(20, 80));
    printf("Sales tax on $50 at 8%%   = $%.2f\n", sales_tax(50, 0.08));
    printf("Tip on $40 at 15%%        = $%.2f\n", tip(40, 0.15));
    printf("Commission $200 at 5%%    = $%.2f\n", commission(200, 0.05));
    printf("Interest $1000, 5%%, 3 yr = $%.2f\n", simple_interest(1000, 0.05, 3));

    printf("\n== Geometry 2D ==\n");
    printf("Rectangle 4x3: area %.2f, perimeter %.2f\n",
           rectangle_area(4, 3), rectangle_perimeter(4, 3));
    printf("Square 5: area %.2f, perimeter %.2f\n",
           square_area(5), square_perimeter(5));
    printf("Circle r=2: area %.2f, circumference %.2f\n",
           circle_area(2), circle_circumference(2));
    printf("Triangle b=6 h=4: area %.2f; sides 3,4,5: perimeter %.2f\n",
           triangle_area(6, 4), triangle_perimeter(3, 4, 5));
    printf("Trapezoid b1=3 b2=5 h=4: area %.2f; sides 4,3,5,4: perimeter %.2f\n",
           trapezoid_area(3, 5, 4), trapezoid_perimeter(4, 3, 5, 4));

    printf("\n== Geometry 3D ==\n");
    printf("Cube s=3: volume %.2f, surface %.2f\n",
           cube_volume(3), cube_surface_area(3));
    printf("Box 2x3x4: volume %.2f, surface %.2f\n",
           rect_prism_volume(2, 3, 4), rect_prism_surface_area(2, 3, 4));
    printf("Sphere r=2: volume %.2f, surface %.2f\n",
           sphere_volume(2), sphere_surface_area(2));
    printf("Cylinder r=2 h=5: volume %.2f, surface %.2f\n",
           cylinder_volume(2, 5), cylinder_surface_area(2, 5));
    printf("Triangular prism (3-4-5 triangle, l=10): volume %.2f, surface %.2f\n",
           tri_prism_volume(3, 4, 10), tri_prism_surface_area(3, 4, 10, 3, 4, 5));

    printf("\n== Percentages ==\n");
    printf("15 of 60         = %.2f%%\n", percentage(15, 60));
    printf("20%% of 150       = %.2f\n", part_from_percentage(20, 150));
    printf("30 is 25%% of     = %.2f\n", whole_from_percentage(30, 25));
    printf("50 -> 65 increase = %.2f%%\n", percent_increase(50, 65));
    printf("80 -> 60 decrease = %.2f%%\n", percent_decrease(80, 60));

    printf("\n== Statistics ==\n");
    double data[] = { 4, 8, 6, 5, 3, 8, 9 };
    size_t n = sizeof data / sizeof data[0];
    printf("Data: 4 8 6 5 3 8 9\n");
    printf("Mean   = %.2f\n", mean(data, n));
    printf("Median = %.2f\n", median(data, n));
    printf("Mode   = %.2f\n", mode(data, n));

    printf("\n== Matrices ==\n");
    Matrix *a = matrix_from_array(2, 3, (double[]){ 1, 2, 3,
                                                    4, 5, 6 });
    Matrix *b = matrix_from_array(2, 3, (double[]){ 6, 5, 4,
                                                    3, 2, 1 });
    Matrix *c = matrix_from_array(3, 2, (double[]){ 7,  8,
                                                    9, 10,
                                                   11, 12 });
    printf("A (2x3):\n");     matrix_print(a);
    printf("B (2x3):\n");     matrix_print(b);
    printf("C (3x2):\n");     matrix_print(c);

    Matrix *sum = matrix_add(a, b);
    Matrix *diff = matrix_subtract(a, b);
    Matrix *product = matrix_multiply(a, c);
    Matrix *bad = matrix_multiply(a, b); /* 2x3 times 2x3 is not allowed */
    printf("A + B:\n");       matrix_print(sum);
    printf("A - B:\n");       matrix_print(diff);
    printf("A x C (2x2):\n"); matrix_print(product);
    printf("A x B:\n");       matrix_print(bad);

    matrix_free(a);
    matrix_free(b);
    matrix_free(c);
    matrix_free(sum);
    matrix_free(diff);
    matrix_free(product);
    matrix_free(bad);

    printf("\n== Advanced multiplication ==\n");
    ull p;
    lattice_multiply(42, 35, &p, stdout);
    partial_products_multiply(26, 45, &p, stdout);
    russian_peasant_multiply(37, 42, &p, stdout);
    duplication_multiply(14, 12, &p, stdout);

    printf("\n== Advanced division ==\n");
    ull q, r, low, high, estimate;
    divide_repeated_subtraction(28, 5, &q, &r, stdout);
    divide_partial_quotients(496, 4, &q, &r, stdout);
    estimate_quotient(152, 6, &low, &high, &estimate, stdout);
    quotient_and_remainder(27, 4, &q, &r, stdout);

    printf("\n== Interpreting the remainder ==\n");
    ull whole, num, den, answer;
    remainder_as_fraction(16, 5, &whole, &num, &den);
    printf("16 in of candy for 5 people: %llu %llu/%llu in each\n", whole, num, den);
    quotient_round_up(30, 4, &answer);
    printf("30 people, 4 per car: %llu cars\n", answer);
    remainder_only(20, 3, &answer);
    printf("$20 among 3 friends, sister gets the rest: $%llu\n", answer);
    quotient_drop_remainder(50, 9, &answer);
    printf("$50 for $9 meals: %llu meals\n", answer);

    return 0;
}
