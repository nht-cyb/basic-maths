#include "geometry.h"

double rectangle_area(double l, double w) { return l * w; }
double rectangle_perimeter(double l, double w) { return 2 * l + 2 * w; }

double square_area(double s) { return s * s; }
double square_perimeter(double s) { return 4 * s; }

double circle_area(double r) { return PI * r * r; }
double circle_circumference(double r) { return 2 * PI * r; }

double triangle_area(double base, double height) { return 0.5 * base * height; }
double triangle_perimeter(double a, double b, double c) { return a + b + c; }

double trapezoid_area(double b1, double b2, double height) {
    return 0.5 * (b1 + b2) * height;
}
double trapezoid_perimeter(double a, double b1, double b2, double c) {
    return a + b1 + b2 + c;
}

double cube_volume(double s) { return s * s * s; }
double cube_surface_area(double s) { return 6 * s * s; }

double rect_prism_volume(double l, double w, double h) { return l * w * h; }
double rect_prism_surface_area(double l, double w, double h) {
    return 2 * (l * w + l * h + w * h);
}

double sphere_volume(double r) { return 4.0 / 3.0 * PI * r * r * r; }
double sphere_surface_area(double r) { return 4 * PI * r * r; }

double cylinder_volume(double r, double h) { return PI * r * r * h; }
double cylinder_surface_area(double r, double h) {
    return 2 * PI * r * r + 2 * PI * r * h;
}

double tri_prism_volume(double b, double h, double l) { return 0.5 * b * h * l; }
double tri_prism_surface_area(double b, double h, double l,
                              double s1, double s2, double s3) {
    return b * h + (s1 + s2 + s3) * l;
}
