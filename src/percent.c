#include "percent.h"

double percentage(double part, double whole) { return part / whole * 100; }

double part_from_percentage(double percentage, double whole) {
    return percentage * whole / 100;
}

double whole_from_percentage(double part, double percentage) {
    return part * 100 / percentage;
}

double percent_increase(double original, double new_value) {
    return (new_value - original) / original * 100;
}

double percent_decrease(double original, double new_value) {
    return (original - new_value) / original * 100;
}
