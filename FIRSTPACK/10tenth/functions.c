#include "functions.h"


static void bigint_normalize(BigInt *num) {

    if (!num) return;
    while (num->size > 1 && num->digits[num->size - 1] == 0) {
        num->size--;
    }

    if (num->size == 1 && num->digits[0] == 0) {
        num->sign = 1;
    }

}

static Status bigint_ensure_capacity(BigInt *num, size_t required_capacity) {
    if (!num) return ERR_NULL_PTR;
    if (num->capacity >= required_capacity) return STATUS_OK;

    size_t new_capacity = required_capacity + 10;
    uint8_t *new_digits = (uint8_t *)realloc(num->digits, new_capacity * sizeof(uint8_t));
    
    if (!new_digits) {
        return ERR_MEMORY;
    }

    memset(new_digits + num->capacity, 0, (new_capacity - num->capacity) * sizeof(uint8_t));
    
    num->digits = new_digits;
    num->capacity = new_capacity;
    return STATUS_OK;
}


BigInt* bigint_create(size_t initial_capacity) {
    BigInt *num = (BigInt *)malloc(sizeof(BigInt));
    if (!num) return NULL;

    if (initial_capacity == 0) initial_capacity = 1;

    num->digits = (uint8_t *)calloc(initial_capacity, sizeof(uint8_t));
    if (!num->digits) {
        free(num);
        return NULL;
    }

    num->sign = 1;
    num->size = 1;
    num->capacity = initial_capacity;
    return num;
}

void bigint_free(BigInt *num) {
    if (!num) return;
    free(num->digits);
    free(num);
}

BigInt* bigint_clone(const BigInt *src) {
    if (!src) return NULL;

    BigInt *copy = bigint_create(src->capacity);
    if (!copy) return NULL;

    copy->sign = src->sign;
    copy->size = src->size;
    copy->capacity = src->capacity;
    
    memcpy(copy->digits, src->digits, src->size * sizeof(uint8_t));
    return copy;
}

int bigint_cmp_abs(const BigInt *a, const BigInt *b) {
    if (!a || !b) return 0;
    if (a->size > b->size) return 1;
    if (a->size < b->size) return -1;

    for (size_t i = a->size; i > 0; i--) {
        if (a->digits[i - 1] > b->digits[i - 1]) return 1;
        if (a->digits[i - 1] < b->digits[i - 1]) return -1;
    }
    return 0;
}

static Status bigint_add_abs(const BigInt *a, const BigInt *b, BigInt *result) {
    size_t max_size = (a->size > b->size) ? a->size : b->size;
    Status status = bigint_ensure_capacity(result, max_size + 1);
    if (status != STATUS_OK) return status;

    int carry = 0;
    for (size_t i = 0; i < max_size || carry; i++) {
        int sum = carry;
        if (i < a->size) sum += a->digits[i];
        if (i < b->size) sum += b->digits[i];

        result->digits[i] = sum % 10;
        carry = sum / 10;
        result->size = i + 1;
    }

    result->sign = 1;
    bigint_normalize(result);
    return STATUS_OK;
}

static Status bigint_sub_abs(const BigInt *a, const BigInt *b, BigInt *result) {
    Status status = bigint_ensure_capacity(result, a->size);
    if (status != STATUS_OK) return status;

    int borrow = 0;
    for (size_t i = 0; i < a->size; i++) {
        int sub = a->digits[i] - borrow;
        if (i < b->size) sub -= b->digits[i];

        if (sub < 0) {
            sub += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result->digits[i] = (uint8_t)sub;
        result->size = i + 1;
    }

    result->sign = 1;
    return STATUS_OK;
}

Status bigint_sum(const BigInt *a, const BigInt *b, BigInt *result) {
    if (!a || !b || !result) return ERR_NULL_PTR;

    if (a->sign == b->sign) {
        Status status = bigint_add_abs(a, b, result);
        if (status != STATUS_OK) return status;
        result->sign = a->sign;
    } 
    else {
        int cmp = bigint_cmp_abs(a, b);
        if (cmp == 0) {
            result->size = 1;
            result->digits[0] = 0;
            result->sign = 1;
            return STATUS_OK;
        } else if (cmp > 0) {
            Status status = bigint_sub_abs(a, b, result);
            if (status != STATUS_OK) return status;
            result->sign = a->sign;
        } else {
            Status status = bigint_sub_abs(b, a, result);
            if (status != STATUS_OK) return status;
            result->sign = b->sign;
        }
    }

    bigint_normalize(result);
    return STATUS_OK;
}

static Status bigint_mul_small(BigInt *num, int multiplier) {
    if (!num) return ERR_NULL_PTR;
    if (multiplier < 0) return ERR_INVALID_INPUT;

    if (multiplier == 0) {
        num->size = 1;
        num->digits[0] = 0;
        num->sign = 1;
        return STATUS_OK;
    }

    int carry = 0;
    for (size_t i = 0; i < num->size; i++) {
        int prod = num->digits[i] * multiplier + carry;
        num->digits[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }

    while (carry > 0) {
        Status status = bigint_ensure_capacity(num, num->size + 1);
        if (status != STATUS_OK) return status;
        num->digits[num->size++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
    return STATUS_OK;
}

static Status bigint_add_small(BigInt *num, int addend) {
    if (!num) return ERR_NULL_PTR;
    if (addend < 0) return ERR_INVALID_INPUT;

    int carry = addend;
    size_t i = 0;
    while (carry > 0 || i < num->size) {
        if (i == num->size) {
            Status status = bigint_ensure_capacity(num, num->size + 1);
            if (status != STATUS_OK) return status;
            num->size++;
        }
        int sum = num->digits[i] + carry;
        num->digits[i] = (uint8_t)(sum % 10);
        carry = sum / 10;
        i++;
    }
    return STATUS_OK;
}

static int bigint_div_small(BigInt *num, int divisor) {
    int remainder = 0;
    for (size_t i = num->size; i > 0; i--) {
        int current = remainder * 10 + num->digits[i - 1];
        num->digits[i - 1] = (uint8_t)(current / divisor);
        remainder = current % divisor;
    }
    bigint_normalize(num);
    return remainder;
}

Status bigint_from_string(const char *str, int base, BigInt *result) {
    if (!str || !result) return ERR_NULL_PTR;
    if (base < 2 || base > 36) return ERR_INVALID_BASE;

    result->size = 1;
    result->digits[0] = 0;
    result->sign = 1;

    size_t len = strlen(str);
    if (len == 0) return ERR_INVALID_INPUT;

    size_t start_idx = 0;
    if (str[0] == '-') {
        result->sign = -1;
        start_idx = 1;
    } else if (str[0] == '+') {
        start_idx = 1;
    }

    if (start_idx == len) return ERR_INVALID_INPUT;

    for (size_t i = start_idx; i < len; i++) {
        char c = str[i];
        int val = 0;

        if (c >= '0' && c <= '9') {
            val = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            val = c - 'A' + 10;
        } else {
            return ERR_INVALID_INPUT; 
        }

        if (val >= base) {
            return ERR_INVALID_INPUT;
        }

        Status status = bigint_mul_small(result, base);
        if (status != STATUS_OK) return status;
        
        status = bigint_add_small(result, val);
        if (status != STATUS_OK) return status;
    }

    bigint_normalize(result);
    return STATUS_OK;
}

Status bigint_to_base_string(const BigInt *num, int base, char **result) {
    if (!num || !result) return ERR_NULL_PTR;
    if (base < 2 || base > 36) return ERR_INVALID_BASE;
    *result = NULL;

    BigInt *temp = bigint_clone(num);
    if (!temp) return ERR_MEMORY;

    size_t max_len = temp->size * 4 + 2;
    char *buffer = (char *)malloc(max_len);
    if (!buffer) {
        bigint_free(temp);
        return ERR_MEMORY;
    }

    char *ptr = buffer + max_len - 1;
    *ptr = '\0';

    if (temp->size == 1 && temp->digits[0] == 0) {
        *(--ptr) = '0';
    } else {
        while (!(temp->size == 1 && temp->digits[0] == 0)) {
            int rem = bigint_div_small(temp, base);
            char c;
            if (rem < 10) {
                c = (char)('0' + rem);
            } else {
                c = (char)('A' + (rem - 10));
            }
            *(--ptr) = c; 
        }
        
        if (num->sign == -1) {
            *(--ptr) = '-';
        }
    }

    size_t actual_len = (buffer + max_len - 1) - ptr;
    memmove(buffer, ptr, actual_len + 1);
    bigint_free(temp);
    *result = buffer;
    return STATUS_OK;
}

Status bigint_print_decimal(const BigInt *num) {
    if (!num) return ERR_NULL_PTR;
    
    char *str = NULL;
    Status status = bigint_to_base_string(num, 10, &str);
    if (status != STATUS_OK) return status;

    printf("%s", str);
    free(str);
    return STATUS_OK;
}

Status io_read_base(int *base) {
    if (!base) return ERR_NULL_PTR;

    char buffer[256];
    char *endptr;
    long val;

    printf("Enter the base of the number system [2..36]: ");
    fflush(stdout);

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return ERR_INVALID_INPUT;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    val = strtol(buffer, &endptr, 10);

    if (endptr == buffer || *endptr != '\0') {
        return ERR_INVALID_INPUT;
    }

    if (val < 2 || val > 36) {
        return ERR_INVALID_BASE;
    }

    *base = (int)val;
    return STATUS_OK;
}

Status io_validate_number_string(const char *str, int base) {
    if (!str) return ERR_NULL_PTR;
    if (base < 2 || base > 36) return ERR_INVALID_BASE;

    size_t len = strlen(str);
    if (len == 0) return ERR_INVALID_INPUT;

    size_t start_idx = 0;
    if (str[0] == '-' || str[0] == '+') {
        start_idx = 1;
    }

    if (start_idx == len) return ERR_INVALID_INPUT;

    for (size_t i = start_idx; i < len; i++) {
        char c = str[i];
        int val = -1;

        if (c >= '0' && c <= '9') {
            val = c - '0';
        } else if (c >= 'A' && c <= 'Z') {
            val = c - 'A' + 10;
        } else {
            return ERR_INVALID_INPUT;
        }

        if (val >= base) {
            return ERR_INVALID_INPUT;
        }
    }
    return STATUS_OK;
}

Status io_read_number(BigInt **num, int base, bool *is_stop) {
    if (!num || !is_stop) return ERR_NULL_PTR;
    
    *num = NULL;
    *is_stop = false;

    char buffer[1024];
    printf("Enter a number in base %d (or 'Stop' to finish): ", base);
    fflush(stdout);

    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return ERR_INVALID_INPUT;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strcmp(buffer, "Stop") == 0) {
        *is_stop = true;
        return STATUS_OK;
    }

    Status status = io_validate_number_string(buffer, base);
    if (status != STATUS_OK) {
        return status;
    }

    BigInt *result = bigint_create(1);
    if (!result) return ERR_MEMORY;

    status = bigint_from_string(buffer, base, result);
    if (status != STATUS_OK) {
        bigint_free(result);
        return status;
    }

    *num = result;
    return STATUS_OK;
}