#include "consumer.h"

double discount(double list_price, double discount_rate) {
    return list_price * discount_rate;
}

double sale_price(double list_price, double discount_rate) {
    return list_price - discount(list_price, discount_rate);
}

double discount_rate(double discount, double list_price) {
    return discount / list_price;
}

double sales_tax(double price, double tax_rate) { return price * tax_rate; }

double tip(double meal_cost, double tip_rate) { return meal_cost * tip_rate; }

double commission(double service_cost, double commission_rate) {
    return service_cost * commission_rate;
}

double simple_interest(double principal, double rate, double time) {
    return principal * rate * time;
}
