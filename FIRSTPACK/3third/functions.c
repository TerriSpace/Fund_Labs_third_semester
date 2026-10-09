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


Status strtoi(const char *str, int *result) {

    if (str == NULL || *str == '\0') return ERR_INVALID_INPUT;

    const char *p = str;

    int sign = 1;
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }
 
    if (*p == '\0') return ERR_INVALID_INPUT;

    const char *temp = p;
    while (*temp != '\0') {
        if (*temp < '0' || *temp > '9') {
            return ERR_INVALID_INPUT;
        }
        temp++;
    }

    int num = 0;

    if (sign == 1) {
        while (*p >= '0' && *p <= '9') {
            int digit = *p - '0';
            if (num > (INT_MAX - digit) / 10) return ERR_OVERFLOW;
            num = num * 10 + digit;
            p++;
        }
        *result = num;
        return STATUS_OK;
    } else {
        while (*p >= '0' && *p <= '9') {
            int digit = *p - '0';
            if (num < (INT_MIN + digit) / 10) return ERR_OVERFLOW;
            num = num * 10 - digit;
            p++;
        }
        *result = num;
        return STATUS_OK;
    }
}

int already_was(const double was[][3], int count, double a, double b, double c, double epsilon){
    for (int i = 0; i < count; i++) {
        if (fabs(a - was[i][0]) < epsilon && fabs(b - was[i][1]) < epsilon && fabs(c - was[i][2]) < epsilon) return 1;
    }
    return 0;
}

Status solve_quadratic(double a, double b, double c, double epsilon, double *x1, double *x2) {
    if (fabs(a) < epsilon) {
        if (fabs(b) < epsilon) {
            if (fabs(c) < epsilon) {
                return SOL_INFINITE;
            } else {
                return SOL_NONE;
            }
        } else {
            *x1 = -c / b;
        }
        return SOL_ONE_ROOT;
    }

    double D = b * b - 4 * a * c;

    if (fabs(D) < epsilon) {
        *x1 = -b / (2 * a);
        return SOL_ONE_ROOT;           
    } else if (D > epsilon) {
        double sqrtD = sqrt(D);
        *x1 = (-b + sqrtD) / (2 * a);
        *x2 = (-b - sqrtD) / (2 * a);
        return SOL_TWO_ROOTS;
    } else {
        return SOL_NONE;
    }
    
}

Status flag_m(int first, int second){
    if (first == 0 || second == 0) return ERR_INVALID_INPUT;
    if (first % second){
        return RES_NOT_MULTIPLE;
    }else{
        return RES_MULTIPLE;
    }
}

Status flag_t(double eps, double a, double b, double c){

    if (a <= eps || b <= eps || c <= eps){
        return RES_NOT_TRIANGLE;
    }

    if (a + b - c <= eps || a + c - b <= eps || b + c - a <= eps){
        return RES_NOT_TRIANGLE;
    }
    if (fabs(a*a + b*b - c*c) < eps || fabs(a*a + c*c - b*b) < eps || fabs(b*b + c*c - a*a) < eps){
        return RES_RIGHT_TRIANGLE;
    }

    return RES_NOT_RIGHT_TRIANGLE;
}