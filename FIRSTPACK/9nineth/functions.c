#include "functions.h"


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
    } else {
        while (*p >= '0' && *p <= '9') {
            int digit = *p - '0';
            if (num < (INT_MIN + digit) / 10) return ERR_OVERFLOW;
            num = num * 10 - digit;
            p++;
        }
        *result = num;
    }
    return STATUS_OK;
}

Status parse_arguments(int argc, char *argv[], int *a, int *b) {
    if (argc != 3) {
        return STATUS_INVALID_ARGC;
    }

    Status status_a = strtoi(argv[1], a);
    if (status_a != STATUS_OK) {
        return (status_a == ERR_OVERFLOW) ? ERR_OVERFLOW : STATUS_INVALID_ARG_FORMAT;
    }

    Status status_b = strtoi(argv[2], b);
    if (status_b != STATUS_OK) {
        return (status_b == ERR_OVERFLOW) ? ERR_OVERFLOW : STATUS_INVALID_ARG_FORMAT;
    }

    if (*a > *b) {
        return STATUS_INVALID_RANGE;
    }

    return STATUS_OK;
}

Status fill_array(int *arr, size_t size, int a, int b) {
    if (arr == NULL) return STATUS_NULL_POINTER;
    if (size == 0) return STATUS_SIZE_ERROR;

    int range = b - a + 1;
    for (size_t i = 0; i < size; i++) {
        arr[i] = a + rand() % range;
    }
    return STATUS_OK;
}

Status swap_min_max(int *arr, size_t size, size_t *min_idx, size_t *max_idx) {
    if (arr == NULL || min_idx == NULL || max_idx == NULL) return STATUS_NULL_POINTER;
    if (size < 2) return STATUS_SIZE_ERROR;

    *min_idx = 0;
    *max_idx = 0;

    for (size_t i = 1; i < size; i++) {
        if (arr[i] < arr[*min_idx]) {
            *min_idx = i;
        } else if (arr[i] > arr[*max_idx]) {
            *max_idx = i;
        }
    }

    if (*min_idx != *max_idx) {
        int tmp = arr[*min_idx];
        arr[*min_idx] = arr[*max_idx];
        arr[*max_idx] = tmp;
    }
    return STATUS_OK;
}

void print_array(const int *arr, size_t size) {
    if (arr == NULL) return;
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

size_t get_random_size(void) {
    return (size_t)(MIN_SIZE + rand() % (MAX_SIZE - MIN_SIZE + 1));
}

Status create_and_fill_array(int **arr, size_t size) {
    if (arr == NULL) return STATUS_NULL_POINTER;
    if (size == 0)   return STATUS_SIZE_ERROR;

    *arr = (int *)malloc(size * sizeof(int));
    if (*arr == NULL) return STATUS_ALLOC_ERROR;

    long long range = (long long)MAX_VAL - (long long)MIN_VAL + 1;
    for (size_t i = 0; i < size; i++) {
        (*arr)[i] = MIN_VAL + (int)(rand() % range);
    }
    return STATUS_OK;
}

void free_array(int **arr) {
    if (arr != NULL && *arr != NULL) {
        free(*arr);
        *arr = NULL;
    }
}

static int compare_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

Status sort_array(int *arr, size_t size) {
    if (arr == NULL) return STATUS_NULL_POINTER;
    if (size < 2)    return STATUS_SIZE_ERROR;
    qsort(arr, size, sizeof(int), compare_int);
    return STATUS_OK;
}

int find_closest_value(const int *B, size_t size_B, int target) {
    size_t left = 0;
    size_t right = size_B;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (B[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    if (left == 0) return B[0];
    if (left == size_B) return B[size_B - 1];

    int diff_left  = abs(B[left] - target);
    int diff_right = abs(B[left - 1] - target);

    return (diff_right <= diff_left) ? B[left - 1] : B[left];
}

Status create_array_C(const int *A, size_t size_A, const int *B, size_t size_B, int **C) {
    if (A == NULL || B == NULL || C == NULL) return STATUS_NULL_POINTER;
    if (size_A == 0 || size_B == 0) return STATUS_SIZE_ERROR;

    *C = (int *)malloc(size_A * sizeof(int));
    if (*C == NULL) return STATUS_ALLOC_ERROR;

    for (size_t i = 0; i < size_A; i++) {
        int closest = find_closest_value(B, size_B, A[i]);
        (*C)[i] = A[i] + closest;
    }
    return STATUS_OK;
}

void print_array_limited(const int *arr, size_t size) {
    if (arr == NULL || size == 0) return;

    size_t limit = (size > PRINT_LIMIT) ? PRINT_LIMIT : size;
    for (size_t i = 0; i < limit; i++) {
        printf("%d ", arr[i]);
    }
    if (size > PRINT_LIMIT) {
        printf("... (total %zu )", size);
    }
    printf("\n");
}