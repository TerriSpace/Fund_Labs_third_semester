#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <limits.h>

typedef enum {
    STATUS_OK = 0,
    ERR_ARGC,
    ERR_INVALID_INPUT,
    ERR_OVERFLOW,
    ERR_DIV_BY_ZERO,

    SOL_INFINITE,
    SOL_NONE,
    SOL_ONE_ROOT,
    SOL_TWO_ROOTS,

    RES_MULTIPLE,
    RES_NOT_MULTIPLE,

    RES_RIGHT_TRIANGLE,
    RES_NOT_RIGHT_TRIANGLE,
    RES_NOT_TRIANGLE

} Status;

Status strtodouble(const char *str, double *result);
Status strtoi(const char *str, int *result);
int already_was(const double was[][3], int count, double a, double b, double c, double epsilon);
Status solve_quadratic(double a, double b, double c, double epsilon, double *x1, double *x2);
Status flag_m(int first, int second);
Status flag_t(double eps, double a, double b, double c);

