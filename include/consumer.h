#ifndef CONSUMER_H
#define CONSUMER_H

/* Rates are decimals: 20% -> 0.20 */

double discount(double list_price, double discount_rate);
double sale_price(double list_price, double discount_rate);
double discount_rate(double discount, double list_price);
double sales_tax(double price, double tax_rate);
double tip(double meal_cost, double tip_rate);
double commission(double service_cost, double commission_rate);

/* I = P * r * t  (r as a decimal, t usually in years) */
double simple_interest(double principal, double rate, double time);

#endif
