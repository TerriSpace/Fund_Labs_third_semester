#include "functions.h"


int run_task_1(int argc, char *argv[]) {
    int a = 0, b = 0;
    Status status = parse_arguments(argc, argv, &a, &b);
    if (status != STATUS_OK) {
        switch (status) {
            case STATUS_INVALID_ARGC:
                fprintf(stderr, "Error: invalid argument count. Usage: %s <a> <b>\n", argv[0]);
                break;
            case STATUS_INVALID_ARG_FORMAT:
                fprintf(stderr, "Error: arguments must be integers.\n");
                break;
            case ERR_OVERFLOW:
                fprintf(stderr, "Error: integer overflow.\n");
                break;
            case STATUS_INVALID_RANGE:
                fprintf(stderr, "Error: a must be <= b.\n");
                break;
            default:
                fprintf(stderr, "Error: unknown.\n");
                break;
        }
        return 1;
    }

    int arr[ARRAY_SIZE];

    status = fill_array(arr, ARRAY_SIZE, a, b);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: fill_array failed.\n");
        return 1;
    }

    print_array(arr, ARRAY_SIZE);

    size_t min_idx = 0;
    size_t max_idx = 0;
    status = swap_min_max(arr, ARRAY_SIZE, &min_idx, &max_idx);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: swap_min_max failed.\n");
        return 1;
    }

    printf("swapped indices: min=%zu max=%zu\n", min_idx, max_idx);
    print_array(arr, ARRAY_SIZE);
    return 0;
}

int run_task_2(void) {
    size_t size_A = get_random_size();
    size_t size_B = get_random_size();

    int *A = NULL;
    int *B = NULL;
    int *C = NULL;

    Status status = create_and_fill_array(&A, size_A);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: array A allocation failed.\n");
        return 1;
    }

    status = create_and_fill_array(&B, size_B);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: array B allocation failed.\n");
        free_array(&A);
        return 1;
    }

    status = sort_array(B, size_B);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: sort_array failed.\n");
        free_array(&A);
        free_array(&B);
        return 1;
    }

    status = create_array_C(A, size_A, B, size_B, &C);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: array C allocation failed.\n");
        free_array(&A);
        free_array(&B);
        return 1;
    }

    printf("size_A=%zu size_B=%zu\n", size_A, size_B);
    print_array_limited(A, size_A);
    print_array_limited(B, size_B);
    print_array_limited(C, size_A);

    free_array(&A);
    free_array(&B);
    free_array(&C);

    return 0;
}

int main(int argc, char *argv[]) {
    srand((unsigned int)time(NULL));

    if (run_task_1(argc, argv) != 0) return 1;
    if (run_task_2() != 0) return 1;

    return 0;
}