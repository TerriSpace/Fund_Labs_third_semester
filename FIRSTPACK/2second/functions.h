#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int strtodouble(const char *str, double *result);
void print_resultl(const char *name, int status, long double value);
void print_result(const char *name, int status, double value);
int e_lim(const double eps, double * const result);
int e_sum(const double eps, double * const result);
int e_equation(const double eps, double * const result);
int pi_lim(const double eps, double * const result);
int pi_sum(const double eps, double * const result);
int pi_equation(const double eps, double * const result);
int ln2_lim(const double eps, double * const result);
int ln2_sum(const double eps, double * const result);
int ln2_equation(const double eps, double * const result);
int sqrt2_lim(const double eps, double * const result);
int sqrt2_product(const double eps, double * const result);
int sqrt2_equation(const double eps, double * const result);
int gamma_lim(const long double eps, long double * const result);
int gamma_sum(const long double eps, long double * const result);
int gamma_equation(const long double eps, long double * const result);