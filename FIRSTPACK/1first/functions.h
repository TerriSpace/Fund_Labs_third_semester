#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>
#include <ctype.h>
#include <limits.h>

#define FUNCTIONS_H
#define MAX_BASE 10
#define MAX_EXP  10
#define MAX_HEX_DIGITS 16
#define MAX_MULTIPLES 100

long long flag_f(long long int number, long long *result);
short int strtoi(const char *str, int *result);
int flag_h(int number, int *result, int *count);
int flag_p(int number);
int flag_s(int number, char *digits, int *count, int *is_negative);
int flag_e(int number, long long table[MAX_BASE][MAX_EXP]);
int flag_a(int number, long long *result);