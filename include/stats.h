#ifndef STATS_H
#define STATS_H

#include <stddef.h>

/* All of these need n > 0. The input array is not changed. */
double mean(const double *values, size_t n);
double median(const double *values, size_t n);
/* Most frequent value; if there is a tie, the smallest one wins. */
double mode(const double *values, size_t n);

#endif
