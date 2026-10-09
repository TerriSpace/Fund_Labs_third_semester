#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>


typedef enum {
    STATUS_OK,
    STATUS_ERR_INVALID_ARG,
    STATUS_ERR_MEMORY,
    STATUS_ERR_NO_ROOT,
    STATUS_ERR_DOMAIN,
    STATUS_ERR_INVALID_BASE,
    STATUS_ERR_INVALID_STRING,
    STATUS_ERR_CAPACITY
} Status;

typedef double (*MathFunc)(double);

Status is_convex(int n, double eps, bool *result, ...);

Status evaluate_poly(double x, int n, double *result, ...);

Status find_kaprekar(int base, long long **results, size_t *count, ...);

Status geometric_mean(int n, double *result, ...);

Status fast_pow(double base, int exp, double *result);

Status bisection(double a, double b, double eps, MathFunc f, double *root);