#include "functions.h"


int strtodouble(const char *str, double *result){
    if (str == NULL || result == NULL) return 4;
    
    char *endptr;
    double value = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') return 5;
    if (isinf(value) || isnan(value)) return 3;
    
    *result = value;
    return 0;
}

int e_lim(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    long long n = 1;
    double prev = 0.0;
    double curr = exp(1.0 * log1p(1.0 / (double)n));
    const long long MAX_I = 10000000LL;
    long long i = 0;

    do {
        prev = curr;
        n += 10;
        curr = exp((double)n * log1p(1.0 / (double)n));
        i++;
        if (isnan(curr) || isinf(curr)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(curr - prev) > eps);

    *result = curr;
    return 0;
}

int e_sum(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double sum = 1.0;
    double temp = 1.0;
    int n = 1;
    const long long MAX_I = 1000;
    long long i = 0;

    do {
        temp = temp / n;
        n++;
        sum += temp;
        i++;
        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (temp > eps);

    *result = sum;
    return 0;
}

int e_equation(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double x = 2.0;
    double lx = log(x);
    double x_new = x - x * (lx - 1.0);
    const long long MAX_I = 100;
    long long i = 0;

    do {
        x = x_new;
        lx = log(x);
        x_new = x - x * (lx - 1.0);
        i++;
        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

int pi_lim(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    int n = 1;
    double d = 2.0 * n + 1.0;
    double pi_current = 4.0;
    double pi_new = pi_current * (4.0 * n * (n + 1.0) / (d * d));
    const long long MAX_I = 10000000LL;
    long long i = 0;

    do {
        pi_current = pi_new;
        n++;
        d = 2.0 * n + 1.0;
        pi_new = pi_current * (4.0 * n * (n + 1.0) / (d * d));
        i++;
        if (isnan(pi_new) || isinf(pi_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(pi_new - pi_current) > eps);

    *result = pi_new;
    return 0;
}

int pi_sum(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    short int sign = 1;
    long long n = 1;
    double sum = 1.0;
    double temp = 1.0;
    const long long MAX_I = 1000000LL;
    long long i = 0;

    do {
        sign = -sign;
        n++;
        temp = 1.0 / (2.0 * n - 1.0);
        if (sign > 0) sum += temp;
        else          sum -= temp;
        i++;
        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(temp) > eps);

    *result = 4.0 * sum;
    return 0;
}

int pi_equation(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double x = 3.0;
    double s = sin(x), c = cos(x);
    double x_new = x + (c + 1.0) / s;
    const long long MAX_I = 1000;
    long long i = 0;

    do {
        x = x_new;
        s = sin(x);
        c = cos(x);
        x_new = x + (c + 1.0) / s;
        i++;
        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

int ln2_lim(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    const double ln2 = log(2.0);
    long long n = 1;
    double ln2_current = 1.0;
    double ln2_next = (double)n * expm1(ln2 / (double)n);
    const long long MAX_I = 100000000LL;
    long long i = 0;

    do {
        n += 10;
        ln2_current = ln2_next;
        ln2_next = (double)n * expm1(ln2 / (double)n);
        i++;
        if (isnan(ln2_next) || isinf(ln2_next)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(ln2_next - ln2_current) > eps);

    *result = ln2_next;
    return 0;
}

int ln2_sum(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    short int sign = 1;
    long long n = 1;
    double sum = 1.0;
    double temp = 1.0;
    const long long MAX_I = 1000000LL;
    long long i = 0;

    do {
        sign = -sign;
        n++;
        temp = 1.0 / (double)n;
        if (sign > 0) {
            sum += temp;
        }
        else {
            sum -= temp;
        }
        i++;
        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(temp) > eps);

    *result = sum;
    return 0;
}

int ln2_equation(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double x = 1.0;
    double ex = exp(x);
    double x_new = x - (ex - 2.0) / ex;
    const long long MAX_I = 100;
    long long i = 0;

    do {
        x = x_new;
        ex = exp(x);
        x_new = x - (ex - 2.0) / ex;
        i++;
        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

int sqrt2_lim(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double x = -0.5;
    double x_new = x - x * x / 2.0 + 1.0;
    const long long MAX_I = 1000;
    long long i = 0;

    do {
        x = x_new;
        x_new = x - x * x / 2.0 + 1.0;
        i++;
        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

int sqrt2_product(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double prod = 1.0;
    long long k = 2;
    double prod_next = prod * exp2(ldexp(1.0, -(int)k));
    const long long MAX_I = 1000;
    long long i = 0;

    do {
        prod = prod_next;
        k++;
        prod_next = prod * exp2(ldexp(1.0, -(int)k));
        i++;
        if (isnan(prod_next) || isinf(prod_next)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(prod_next - prod) > eps);

    *result = prod_next;
    return 0;
}

int sqrt2_equation(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) return 1;

    double x = 1.0;
    double x_new = 0.5 * (x + 2.0 / x);
    const long long MAX_I = 100;
    long long i = 0;

    do {
        x = x_new;
        x_new = 0.5 * (x + 2.0 / x);
        i++;
        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

int gamma_lim(const long double eps, long double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0L || eps >= 1.0L) return 1;

    long double m = 1.0L;
    long double sum = 0.0L;
    long double sum_prev = 0.0L;
    const long long MAX_I = 1000;
    long long i = 0;

    do {
        sum_prev = sum;
        m++;
        sum = 0.0L;
        long double k = 1.0L;
        long double C = 1.0L;
        long double ln_current = 0.0L;
        short int sign = 1;
        while (k <= m) {
            C *= (m - k + 1.0L) / k;
            ln_current += logl(k);
            sign = -sign;
            sum += (long double)sign * C * ln_current / k;
            k += 1.0L;
        }
        i++;
        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabsl(sum - sum_prev) > eps);

    *result = sum;
    return 0;
}

int gamma_sum(const long double eps, long double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0L || eps >= 1.0L) return 1;

    long double sum = 0.0L;
    long double sum_start_block;
    long long n = 1;
    long double pi = acosl(-1.0L);
    const long long MAX_I = 10000LL;
    long long i = 0;

    do {
        sum_start_block = sum;
        long double inv_n2 = 1.0L / ((long double)n * (long double)n);
        long long k_start = n * n;
        long long k_end = (n + 1) * (n + 1);

        for (long long k = k_start; k < k_end; k++) {
            sum += inv_n2 - 1.0L / (long double)k;
        }
        n++;
        i++;
        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabsl(sum - sum_start_block) > eps);

    *result = sum - pi * pi / 6.0L;
    return 0;
}

int gamma_equation(const long double eps, long double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0L || eps >= 1.0L) return 1;

    const long long SIEVE_LIMIT = 1000000LL;

    unsigned char *sieve = calloc((size_t)SIEVE_LIMIT + 1, sizeof(unsigned char));
    if (sieve == NULL) return 5;

    sieve[0] = 1;
    sieve[1] = 1;
    for (long long i = 2; i * i <= SIEVE_LIMIT; i++) {
        if (!sieve[i]) {
            for (long long j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve[j] = 1;
        }
    }

    long double product = 1.0L;
    long double x_prev = 0.0L;
    long double x = 0.0L;
    long long t = 2;

    do {
        if (!sieve[t]) {
            product *= (long double)(t - 1) / (long double)t;
            long double L = logl((long double)t) * product;
            x_prev = x;
            x = -logl(L);
        }
        t++;
        if (isnan(x) || isinf(x)) { 
            free(sieve); 
            return 3; 
        }
        if (t > SIEVE_LIMIT) {
            free(sieve); 
            return 2;
        }
        
    } while (fabsl(x - x_prev) > eps);

    free(sieve);
    *result = x;
    return 0;
}

void print_result(const char *name, int status, double value) {
    if (status == 0) {
        printf("%-20s = %.12f\n", name, value);
        return;
    }
    switch (status) {
        case 1: printf("%-20s некорректный eps\n", name);    break;
        case 2: printf("%-20s не сошлось за разумное количество итераций\n", name); break;
        case 3: printf("%-20s переполнение / NaN\n", name);   break;
        case 4: printf("%-20s NULL-указатель\n", name);       break;
        case 5: printf("%-20s ошибка выделения памяти\n", name); break;
        default: printf("%-20s неизвестный код %d\n", name, status); break;
    }
}

void print_resultl(const char *name, int status, long double value) {
    if (status == 0) {
        printf("%-20s = %.15Lf\n", name, value);
        return;
    }
    switch (status) {
        case 1: printf("%-20s некорректный eps\n", name);    break;
        case 2: printf("%-20s не сошлось за разумное количество итераций\n", name); break;
        case 3: printf("%-20s переполнение / NaN\n", name);   break;
        case 4: printf("%-20s NULL-указатель\n", name);       break;
        case 5: printf("%-20s ошибка выделения памяти\n", name); break;
        default: printf("%-20s неизвестный код %d\n", name, status); break;
    }
}