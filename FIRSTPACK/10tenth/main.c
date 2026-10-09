#include "functions.h"


Status io_print_results(const BigInt *max_abs, const BigInt *sum) {
    if (!max_abs || !sum) return ERR_NULL_PTR;

    int bases[] = {9, 18, 27, 36};
    char *str = NULL;
    Status status;

    printf("\n--- Results ---\n");

    printf("Max modulo (decimal): ");
    status = bigint_print_decimal(max_abs);
    if (status != STATUS_OK) return status;
    printf("\n");

    printf("Sum (decimal):        ");
    status = bigint_print_decimal(sum);
    if (status != STATUS_OK) return status;
    printf("\n\n");

    for (int i = 0; i < 4; i++) {
        int base = bases[i];

        status = bigint_to_base_string(max_abs, base, &str);
        if (status != STATUS_OK) return status;
        printf("Max modulo (base %2d): %s\n", base, str);
        free(str);
        str = NULL;

        status = bigint_to_base_string(sum, base, &str);
        if (status != STATUS_OK) return status;
        printf("Sum (base %2d):        %s\n", base, str);
        free(str);
        str = NULL;
    }

    return STATUS_OK;
}

int main(void) {
    int base = 0;
    Status status;

    status = io_read_base(&base);
    if (status != STATUS_OK) {
        if (status == ERR_INVALID_BASE) {
            fprintf(stderr, "Error: Base must be in range [2..36].\n");
        } else if (status == ERR_INVALID_INPUT) {
            fprintf(stderr, "Error: Invalid input. Please enter an integer.\n");
        } else {
            fprintf(stderr, "Error: Unexpected error while reading base.\n");
        }
        return 1;
    }

    BigInt *sum = bigint_create(1);
    if (!sum) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 1;
    }

    BigInt *max_abs = NULL;

    while (1) {
        BigInt *current = NULL;
        bool is_stop = false;

        status = io_read_number(&current, base, &is_stop);

        if (status != STATUS_OK) {
            if (status == ERR_INVALID_INPUT) {
                fprintf(stderr, "Error: Invalid number for base %d. Try again.\n", base);
            } else if (status == ERR_MEMORY) {
                fprintf(stderr, "Error: Memory allocation failed.\n");
                bigint_free(sum);
                bigint_free(max_abs);
                return 1;
            } else {
                fprintf(stderr, "Error: Unexpected error.\n");
                bigint_free(sum);
                bigint_free(max_abs);
                return 1;
            }
            continue;
        }

        if (is_stop) {
            break;
        }

        BigInt *new_sum = bigint_create(sum->capacity);
        if (!new_sum) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            bigint_free(current);
            bigint_free(sum);
            bigint_free(max_abs);
            return 1;
        }

        status = bigint_sum(sum, current, new_sum);
        if (status != STATUS_OK) {
            fprintf(stderr, "Error: Math operation failed.\n");
            bigint_free(new_sum);
            bigint_free(current);
            bigint_free(sum);
            bigint_free(max_abs);
            return 1;
        }

        bigint_free(sum);
        sum = new_sum;

        if (max_abs == NULL) {
            max_abs = bigint_clone(current);
            if (!max_abs) {
                fprintf(stderr, "Error: Memory allocation failed.\n");
                bigint_free(current);
                bigint_free(sum);
                return 1;
            }
        } else {
            if (bigint_cmp_abs(current, max_abs) > 0) {
                BigInt *new_max = bigint_clone(current);
                if (!new_max) {
                    fprintf(stderr, "Error: Memory allocation failed.\n");
                    bigint_free(current);
                    bigint_free(sum);
                    bigint_free(max_abs);
                    return 1;
                }
                bigint_free(max_abs);
                max_abs = new_max;
            }
        }

        bigint_free(current);
    }

    if (max_abs == NULL) {
        printf("\nNo numbers were entered.\n");
        bigint_free(sum);
        return 0;
    }

    status = io_print_results(max_abs, sum);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: Failed to print results.\n");
        bigint_free(max_abs);
        bigint_free(sum);
        return 1;
    }

    bigint_free(max_abs);
    bigint_free(sum);

    return 0;
}