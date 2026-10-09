#include "functions.h"

long long flag_f(long long int number, long long *result){
    if (result == NULL) return 3;
    if (number < 0) return 1;

    long long p = 1;

    for (long long i = 2; i <= number; i++) {
        if (p > LLONG_MAX / i) {
            return 2;
        }
        p *= i;
    }

    *result = p;
    return 0;
}


short int strtoi(const char *str, int *result) {

    if (str == NULL || *str == '\0') return 1;

    const char *p = str;

    int sign = 1;
    if (*p == '-') {
        sign = -1;
        p++;
    } else if (*p == '+') {
        p++;
    }

    if (*p == '\0') return 1;

    const char *temp = p;
    while (*temp != '\0') {
        if (*temp < '0' || *temp > '9') {
            return 3;
        }
        temp++;
    }
    
    int num = 0;
    
    if (sign == 1) {

        while (*p >= '0' && *p <= '9') {
            int digit = *p - '0';
            if (num > (INT_MAX - digit) / 10) return 2;
            num = num * 10 + digit;
            p++;
        }
        *result = num;
        return 0;
    } else {
        
        while (*p >= '0' && *p <= '9') {
            int digit = *p - '0';
            if (num < (INT_MIN + digit) / 10) return 2;
            num = num * 10 - digit;
            p++;
        }
        *result = num;
        return 0;
    }
    
}


int flag_h(int number, int *result, int *count){
    if (result == NULL || count == NULL) return 1;

    *count = 0;

    if (number == 0) return 0;

    int abs_num = (number < 0) ? -number : number;

    if (abs_num > 100) return 0;

    int i = 0;
    for (int k = 1; k * abs_num <= 100; k++) {
        if (i >= MAX_MULTIPLES) break;
        result[i++] = k * abs_num;
    }

    *count = i;
    return 0;
}


int flag_p(int number){
    if (number <= 0) return 3;
    if (number == 1) return 11;

    for(long long i = 2; i * i <= number; i++){
        if(number % i == 0) return 1;
    }
    return 2;
}


int flag_s(int number, char *digits, int *count, int *is_negative){
    if (digits == NULL || count == NULL || is_negative == NULL) return 1;

    unsigned int unum;
    if (number < 0) {
        *is_negative = 1;
        unum = (unsigned int)(-(long long)number);
    } else {
        *is_negative = 0;
        unum = (unsigned int)number;
    }

    int i = 0;

    if (unum == 0) {
        digits[i++] = '0';
    } 
    else{
        while (unum > 0) {
            int digit = unum % 16;
            if (digit < 10) {
                digits[i++] = (char)('0' + digit);
            } else {
                digits[i++] = (char)('A' + (digit - 10));
            }
            unum /= 16;
            if (i >= MAX_HEX_DIGITS) return 2;
        }

    }

    *count = i;
    return 0;
}


int flag_e(int number, long long table[MAX_BASE][MAX_EXP]){
    if (number < 1 || number > 10) return 1;

    for (int base = 1; base <= MAX_BASE; base++) {
        long long val = 1;
        for (int exp = 1; exp <= number; exp++) {
  
            if (val > LLONG_MAX / base) return 2;

            val *= base;
            table[base - 1][exp - 1] = val;
        }
    }

    return 0;
}


int flag_a(int number, long long *result){
    if (result == NULL) return 2;
    if (number < 1) return 1;
    int total = 0;

    *result = (long long)number * (number + 1) / 2;
    return 0;
}
