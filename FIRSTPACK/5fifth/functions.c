#include "functions.h"

Status strtodouble(const char *str, double *result){
    if (str == NULL || result == NULL) return ERR_INVALID_INPUT;
    
    char *endptr;
    double value = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') return ERR_INVALID_INPUT;
    if (isinf(value) || isnan(value)) return ERR_OVERFLOW; 
    
    *result = value;
    return STATUS_OK;
}


Status sum_a(const double eps, const double x, double * const result){
    if (result == NULL) return ERR_RESULT;
    if (isnan(eps) || eps <= 0.0) return ERR_EPS;
    if (isnan(x) || isinf(x)) return ERR_X;

    long long n = 0;
    double sum = 1.0;
    double term = 1.0;
    const long long MAX_N = 1000000LL;

    do{
        n++;
        term = term * x / n;
        if (isinf(term) || isnan(term)) return ERR_OVERFLOW;
        sum += term;
        if (n > MAX_N) return ERR_OVERFLOW;
    } while(fabs(term) > eps);

    if (isnan(sum) || isinf(sum)) return ERR_OVERFLOW;

    *result = sum;
    return STATUS_OK;
}

Status sum_b(const double eps, const double x, double * const result){
    if (result == NULL) return ERR_RESULT;
    if (isnan(eps) || eps <= 0.0) return ERR_EPS;
    if (isnan(x) || isinf(x)) return ERR_X;

    long long n = 0;
    double sum = 1.0;
    double term = 1.0;
    const long long MAX_N = 1000000LL;

    do{
        n++;
        term = term * (-x * x) / ((2.0 * n - 1.0) * (2.0 * n));
        if (isinf(term) || isnan(term)) return ERR_OVERFLOW;
        sum += term;
        if (n > MAX_N) return ERR_OVERFLOW;
    } while(fabs(term) > eps);

    if (isnan(sum) || isinf(sum)) return ERR_OVERFLOW;

    *result = sum;
    return STATUS_OK;
}

Status sum_c(const double eps, const double x, double * const result){
    if (result == NULL) return ERR_RESULT;
    if (isnan(eps) || eps <= 0.0) return ERR_EPS;
    if (isnan(x) || isinf(x)) return ERR_X;

    long long n = 0;
    double sum = 1.0;
    double term = 1.0;
    const long long MAX_N = 1000000LL;

    do{
        n++;
        term = term * (27.0 * n * n * n * x * x) / ((3.0 * n - 2.0) * (3.0 * n - 1.0) * (3.0 * n));
        if (isinf(term) || isnan(term)) return ERR_OVERFLOW;
        sum += term;
        if (n > MAX_N) return ERR_OVERFLOW;
    } while(fabs(term) > eps);

    if (isnan(sum) || isinf(sum)) return ERR_OVERFLOW;

    *result = sum;
    return STATUS_OK;
}

Status sum_d(const double eps, const double x, double * const result){
    if (result == NULL) return ERR_RESULT;
    if (isnan(eps) || eps <= 0.0) return ERR_EPS;
    if (isnan(x) || isinf(x)) return ERR_X;

    long long n = 1;
    double term = -(x * x) / 2.0;
    if (isinf(term) || isnan(term)) return ERR_OVERFLOW;
    double sum = term;
    const long long MAX_N = 1000000LL;

    while(fabs(term) > eps){
        n++;
        term = term * (-x * x) * (2.0 * n - 1.0) / (2.0 * n);
        if (isinf(term) || isnan(term)) return ERR_OVERFLOW;
        sum += term;
        if (isinf(sum) || isnan(sum)) return ERR_OVERFLOW;
        if (n > MAX_N) return ERR_OVERFLOW;
    }

    if (isnan(sum) || isinf(sum)) return ERR_OVERFLOW;

    *result = sum;
    return STATUS_OK;
}

typedef double (*integrand_t)(double);

static double rectangles_mid(integrand_t f, const double a, const double b, const long long n) {
    const double h = (b - a) / n;
    double s = 0.0;

    for (long long i = 0; i < n; i++) {
        const double x = a + (i + 0.5) * h;
        s += f(x);
    }

    return s * h;
}

static double f_a(const double x) {
    if (fabs(x) < 1e-12) return 1.0;
    return log(1.0 + x) / x;
}

static double f_b(const double x) {
    return exp(-x * x / 2.0);
}

static double f_c(const double x) {
    return -log(1.0 - x);
}

static double f_d(const double x) {
    if (x <= 0.0) return 1.0;
    return pow(x, x);
}


static Status integrate(integrand_t f, const double eps, double * const result) {
    if (result == NULL) return ERR_RESULT;
    if (isnan(eps) || eps <= 0.0) return ERR_EPS;

    const double a = 0.0;
    const double b = 1.0;

    long long n = 4;
    const long long MAX_N = 1000000000LL;

    double I_prev = rectangles_mid(f, a, b, n);
    if (isnan(I_prev) || isinf(I_prev)) return ERR_OVERFLOW;

    while (1) {
        if (n > MAX_N / 2) return ERR_OVERFLOW;
        n *= 2;

        const double I_curr = rectangles_mid(f, a, b, n);
        if (isnan(I_curr) || isinf(I_curr)) return ERR_OVERFLOW;

        if (fabs(I_curr - I_prev) < eps) {
            *result = I_curr;
            return STATUS_OK;
        }

        I_prev = I_curr;
    }
}

Status integral_a(const double eps, double * const result) {
    return integrate(f_a, eps, result);
}
Status integral_b(const double eps, double * const result) {
    return integrate(f_b, eps, result);
}
Status integral_c(const double eps, double * const result) {
    return integrate(f_c, eps, result);
}
Status integral_d(const double eps, double * const result) {
    return integrate(f_d, eps, result);
}