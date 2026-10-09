#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef enum {
    STATUS_OK = 0,
    ERR_MEMORY,
    ERR_NULL_PTR,
    ERR_INVALID_BASE,
    ERR_INVALID_INPUT,
    ERR_OVERFLOW,
    ERR_DIVIDE_BY_ZERO
} Status;

typedef struct {
    int sign;
    size_t size;
    size_t capacity;
    uint8_t *digits;
} BigInt;


BigInt* bigint_create(size_t initial_capacity);
void bigint_free(BigInt *num);
BigInt* bigint_clone(const BigInt *src);


Status bigint_sum(const BigInt *a, const BigInt *b, BigInt *result);
int bigint_cmp_abs(const BigInt *a, const BigInt *b);


Status bigint_from_string(const char *str, int base, BigInt *result);


Status bigint_to_base_string(const BigInt *num, int base, char **result); 


Status bigint_print_decimal(const BigInt *num);


Status io_read_base(int *base);
Status io_validate_number_string(const char *str, int base);
Status io_read_number(BigInt **num, int base, bool *is_stop);


Status io_print_results(const BigInt *max_abs, const BigInt *sum);