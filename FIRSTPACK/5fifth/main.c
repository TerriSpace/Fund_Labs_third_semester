#include "functions.h"

int main(const int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "usage: %s <x> <eps>\n", argv[0]);
        return ERR_ARGC;
    }

    double x = 0.0;
    double eps = 0.0;
    double result = 0.0;
    Status st;

    st = strtodouble(argv[1], &x);
    switch (st) {
        case STATUS_OK:
            break;
        case ERR_INVALID_INPUT:
            fprintf(stderr, "x: invalid input\n");
            return st;
        case ERR_OVERFLOW:
            fprintf(stderr, "x: overflow\n");
            return st;
        default:
            fprintf(stderr, "x: unknown error\n");
            return st;
    }

    st = strtodouble(argv[2], &eps);
    switch (st) {
        case STATUS_OK:
            break;
        case ERR_INVALID_INPUT:
            fprintf(stderr, "eps: invalid input\n");
            return st;
        case ERR_OVERFLOW:
            fprintf(stderr, "eps: overflow\n");
            return st;
        default:
            fprintf(stderr, "eps: unknown error\n");
            return st;
    }

    st = sum_a(eps, x, &result);
    switch (st) {
        case STATUS_OK:
            printf("sum_a(x = %.10g, eps = %.10g) = %.15g\n", x, eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "sum_a: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "sum_a: eps must be positive and finite\n");
            break;
        case ERR_X:
            fprintf(stderr, "sum_a: invalid x\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "sum_a: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "sum_a: unknown error\n");
            break;
    }

    st = sum_b(eps, x, &result);
    switch (st) {
        case STATUS_OK:
            printf("sum_b(x = %.10g, eps = %.10g) = %.15g\n", x, eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "sum_b: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "sum_b: eps must be positive and finite\n");
            break;
        case ERR_X:
            fprintf(stderr, "sum_b: invalid x\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "sum_b: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "sum_b: unknown error\n");
            break;
    }

    st = sum_c(eps, x, &result);
    switch (st) {
        case STATUS_OK:
            printf("sum_c(x = %.10g, eps = %.10g) = %.15g\n", x, eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "sum_c: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "sum_c: eps must be positive and finite\n");
            break;
        case ERR_X:
            fprintf(stderr, "sum_c: invalid x\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "sum_c: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "sum_c: unknown error\n");
            break;
    }

    st = sum_d(eps, x, &result);
    switch (st) {
        case STATUS_OK:
            printf("sum_d(x = %.10g, eps = %.10g) = %.15g\n", x, eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "sum_d: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "sum_d: eps must be positive and finite\n");
            break;
        case ERR_X:
            fprintf(stderr, "sum_d: invalid x\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "sum_d: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "sum_d: unknown error\n");
            break;
    }

    st = integral_a(eps, &result);
    switch (st) {
        case STATUS_OK:
            printf("integral_a(eps = %.10g) = %.15g\n", eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "integral_a: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "integral_a: eps must be positive and finite\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "integral_a: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "integral_a: unknown error\n");
            break;
    }

    st = integral_b(eps, &result);
    switch (st) {
        case STATUS_OK:
            printf("integral_b(eps = %.10g) = %.15g\n", eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "integral_b: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "integral_b: eps must be positive and finite\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "integral_b: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "integral_b: unknown error\n");
            break;
    }

    st = integral_c(eps, &result);
    switch (st) {
        case STATUS_OK:
            printf("integral_c(eps = %.10g) = %.15g\n", eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "integral_c: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "integral_c: eps must be positive and finite\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "integral_c: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "integral_c: unknown error\n");
            break;
    }

    st = integral_d(eps, &result);
    switch (st) {
        case STATUS_OK:
            printf("integral_d(eps = %.10g) = %.15g\n", eps, result);
            break;
        case ERR_RESULT:
            fprintf(stderr, "integral_d: null result pointer\n");
            break;
        case ERR_EPS:
            fprintf(stderr, "integral_d: eps must be positive and finite\n");
            break;
        case ERR_OVERFLOW:
            fprintf(stderr, "integral_d: overflow or no convergence\n");
            break;
        default:
            fprintf(stderr, "integral_d: unknown error\n");
            break;
    }

    return STATUS_OK;
}