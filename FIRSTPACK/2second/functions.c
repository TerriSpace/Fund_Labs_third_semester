#include "functions.h"

int strtodouble(const char *str, double *result){
    if (str == NULL || result == NULL) return 3;
    
    char *endptr;
    double value = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') return 2;
    if (isinf(value) || isnan(value)) return 3; 
    
    *result = value;
    return 0;
}

//функции
//для е
//е-предел
int e_lim(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;
    
    long long n = 1;
    double e_current = 2.0;
    double e_next = 2.25;

    const long long MAX_I = 100000000LL;
    long long i = 0;
    do{
        e_current = pow((1.0 + 1.0/n), n);
        n += 10;
        e_next = pow((1.0 + 1.0/n), n);

        i++;

        if (isnan(e_next) || isinf(e_next)) return 3;
        if (i > MAX_I) return 2;

    } while (fabs(e_next - e_current) > eps);
    *result = e_next;
    return 0;
}

//e-сумма
int e_sum(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double sum = 1.0;
    double temp = 1.0;
    int n = 1;

    const long long MAX_I = 1000;
    long long i = 0;
    do{
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
//e-уравнение
int e_equation(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double x = 2.0;
    double x_new = x - x * (log(x) - 1.0);

    const long long MAX_I = 100;
    long long i = 0;
    do{
        x = x_new;
        x_new = x - x * (log(x) - 1.0);

        i++;

        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

//для pi
//pi-предел
int pi_lim(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double pi_current = 4.0; //первый
    int n = 1;
    double pi_new = pi_current *(4.0 * n * (n + 1) / pow(2.0 * n + 1, 2));

    const long long MAX_I = 10000000LL;
    long long i = 0;
    do{
        pi_current = pi_new;
        n++;
        pi_new = pi_current * (4.0 * n * (n + 1) / pow(2.0 * n + 1, 2));
        
        i++;

        if (isnan(pi_new) || isinf(pi_new)) return 3;
        if (i > MAX_I) return 2;
    } while(fabs(pi_new - pi_current) > eps);

    *result = pi_new;
    return 0;

}
//pi-сумма
int pi_sum(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    short int sign = 1; //чередование зн в ряду
    long long n = 1;
    double sum = 1.0;
    double temp = 1.0; //первый

    const long long MAX_I = 1000000000LL;
    long long i = 0;
    do{
        sign = -sign;
        n++;
        temp = 1.0 / (2.0 * n - 1.0);
        if (sign > 0){
            sum += temp;

        }
        else{
            sum -= temp;
        }

        i++;

        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(temp) > eps);
    
    *result = 4.0 * sum;
    return 0;
}
//pi-уравнение
int pi_equation(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double x = 3.0;
    double x_new = x + (cos(x) + 1.0) / sin(x);

    const long long MAX_I = 1000;
    long long i = 0;
    do{
        x = x_new;
        x_new = x + (cos(x) + 1.0) / sin(x);

        i++;

        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}

//для ln(2)
//lu(2)-предел
int ln2_lim(const double eps, double * const result){

    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double ln2_current = 1.0;
    long long n = 1;
    double ln2_next = n * (pow(2.0, 1.0/n) - 1.0);

    const long long MAX_I = 100000000LL;
    long long i = 0;
    do{
        n += 10;
        ln2_current = ln2_next;
        ln2_next = n * (pow(2.0, 1.0/n) - 1.0);

        i++;

        if (isnan(ln2_next) || isinf(ln2_next)) return 3;
        if (i > MAX_I) return 2;
    } while(fabs(ln2_next - ln2_current) > eps);

    *result = ln2_next;
    return 0;
}
//ln(2)-сумма
int ln2_sum(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    short int sign = 1; //чередование зн в ряду
    long long n = 1;
    double sum = 1.0;
    double temp = 1.0; //первый

    const long long MAX_I = 1000000000LL;
    long long i = 0;
    do{
        sign = -sign;
        n++;
        temp = 1.0 / n;
        if (sign > 0){
            sum += temp;

        }
        else{
            sum -= temp;
        }

        i++;

        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(temp) > eps);
    
    *result = sum;
    return 0;
}
//ln(2)-уравнение
int ln2_equation(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double x = 1.0;
    double x_new = x - (exp(x) - 2.0) / exp(x);

    const long long MAX_I = 100;
    long long i = 0;
    do{
        x = x_new;
        x_new = x - (exp(x) - 2.0) / exp(x);

        i++;

        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while (fabs(x_new - x) > eps);

    *result = x_new;
    return 0;

}
//sqrt2
//sqrt2-предел
int sqrt2_lim(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double x = - 0.5;
    double x_new = x - x*x/2.0 + 1.0;

    const long long MAX_I = 1000;
    long long i = 0;
    do{
        x = x_new;
        x_new = x - x*x/2.0 + 1.0;

        i++;

        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while(fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}
//sqrt2-произведение
int sqrt2_product(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double prod = 1.0;
    long long k = 2;
    double prod_next = prod * pow(2.0, 1.0 / pow(2.0, k));

    const long long MAX_I = 1000;
    long long i = 0;
    do{
        prod = prod_next;
        k++;
        prod_next = prod * pow(2.0, 1.0 / pow(2.0, k));

        i++;

        if (isnan(prod_next) || isinf(prod_next)) return 3;
        if (i > MAX_I) return 2;
    } while(fabs(prod_next - prod) > eps);

    *result = prod_next;
    return 0;

}
//sqrt2-уравнение
int sqrt2_equation(const double eps, double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    double x = 1.0;
    double x_new = 0.5 * (x + 2.0 / x);

    const long long MAX_I = 100;
    long long i = 0;
    do{
        x = x_new;
        x_new = 0.5 * (x + 2.0 / x);

        i++;

        if (isnan(x_new) || isinf(x_new)) return 3;
        if (i > MAX_I) return 2;
    } while(fabs(x_new - x) > eps);

    *result = x_new;
    return 0;
}
//gamma
//gamma-предел
int gamma_lim(const long double eps, long double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    long double m = 1.0;
    long double sum = 0.0;
    long double sum_prev = 0.0;

    const long long MAX_I = 500;
    long long i = 0;
    do{
        sum_prev = sum;
        m++;
        sum = 0.0;
        long double k = 1.0;
        long double C = 1.0;
        long double ln_current = 0.0;
        short int sign = 1;
        while (k <= m){
            C *= (m - k + 1) / k;
            ln_current += logl(k);
            sign = -sign;
            sum += sign * C * ln_current / k;
            k++;
        }

        i++;

        if (isnan(sum) || isinf(sum)) return 3; // переполнение
        if (i > MAX_I) return 2;
    } while(fabsl(sum - sum_prev) > eps);

    *result = sum;
    return 0;

}
//gamma-сумма
int gamma_sum(const long double eps, long double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    long double sum = 0.0;
    long double sum_start_block;
    long long n = 1;
    long double pi = acosl(-1.0);

    const long long MAX_I = 1000000000LL;
    long long i = 0;
    do {
        sum_start_block = sum;

        long long k_start = n * n;
        long long k_end = (n + 1) * (n + 1);

        for (long long k = k_start; k < k_end; k++) {
            if (k == 1) continue;

            long long num = k - n * n;
            long long den = n * n * k;
            sum += (long double)num / (long double)den;
        }
        n++;

        i++;

        if (isnan(sum) || isinf(sum)) return 3;
        if (i > MAX_I) return 2;
    } while (fabsl(sum - sum_start_block) > eps);

    *result = sum - pi * pi / 6.0;
    return 0;

}
//gamma-уравнение
int is_prime(long long t) {
    if (t < 2) return 0;
    for (long long d = 2; d * d <= t; d++) {
        if (t % d == 0) return 0;
    }
    return 1;
}

int gamma_equation(const long double eps, long double * const result){
    if (result == NULL) return 4;
    if (isnan(eps) || eps <= 0.0) return 1;

    long double product = 1.0;
    long double x_prev = 0.0;
    long double x = 0.0;
    long long t = 2;

    const long long MAX_T = 10000000LL;
    
    do {
        if (is_prime(t)) {
            product *= (long double)(t - 1) / (long double)t;
            long double L = logl((long double)t) * product;
            x_prev = x;
            x = -logl(L);
        }
        t++;

        if (isnan(x) || isinf(x)) return 3;
        if (t > MAX_T) return 2;
    } while (fabsl(x - x_prev) > eps);

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
        default: printf("%-20s неизвестный код %d\n", name, status); break;
    }
}