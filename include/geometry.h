#ifndef GEOMETRY_H
#define GEOMETRY_H

#define PI 3.14159265358979323846

/* 2D shapes */
double rectangle_area(double l, double w);
double rectangle_perimeter(double l, double w);
double square_area(double s);
double square_perimeter(double s);
double circle_area(double r);
double circle_circumference(double r);
double triangle_area(double base, double height);
double triangle_perimeter(double a, double b, double c);
double trapezoid_area(double b1, double b2, double height);
double trapezoid_perimeter(double a, double b1, double b2, double c);

/* 3D shapes */
double cube_volume(double s);
double cube_surface_area(double s);
double rect_prism_volume(double l, double w, double h);
double rect_prism_surface_area(double l, double w, double h);
double sphere_volume(double r);
double sphere_surface_area(double r);
double cylinder_volume(double r, double h);
double cylinder_surface_area(double r, double h);
/* b, h = base and height of the triangle; l = length of the prism;
   s1, s2, s3 = the triangle's three sides */
double tri_prism_volume(double b, double h, double l);
double tri_prism_surface_area(double b, double h, double l,
                              double s1, double s2, double s3);

#endif
