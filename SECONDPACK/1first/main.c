#include "functions.h"


void print_error(Status status) {
    switch (status) {
        case STATUS_ERR_INVALID_ARGC:
            fprintf(stderr, "Error: Invalid number of arguments.\n");
            break;
        case STATUS_ERR_INVALID_FLAG:
            fprintf(stderr, "Error: Unknown flag. Available flags: -l, -r, -u, -p, -c\n");
            break;
        case STATUS_ERR_INVALID_SEED:
            fprintf(stderr, "Error: Invalid seed value for -c flag. Must be an unsigned integer.\n");
            break;
        case STATUS_ERR_MEMORY:
            fprintf(stderr, "Error: Memory allocation failed.\n");
            break;
        case STATUS_ERR_EMPTY_STRING:
            fprintf(stderr, "Error: Input string is empty or NULL.\n");
            break;
        case STATUS_ERR_NON_ASCII:
            fprintf(stderr, "Error: Input string contains non-ASCII characters.\n");
            break;
        case STATUS_ERR_UNKNOWN:
        default:
            fprintf(stderr, "Error: An unknown error occurred.\n");
            break;
    }
}

static bool parse_uint(const char *str, unsigned int *out) {
    if (str == NULL || str[0] == '\0') {
        return false;
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (!is_digit(str[i])) {
            return false;
        }
    }

    unsigned long result = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        unsigned long digit = (unsigned long)(str[i] - '0');
        if (result > (UINT_MAX - digit) / 10) {
            return false;
        }
        result = result * 10 + digit;
    }

    *out = (unsigned int)result;
    return true;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_error(STATUS_ERR_INVALID_ARGC);
        return 1;
    }

    if (argv[1][0] != '-' || argv[1][1] == '\0' || argv[1][2] != '\0') {
        print_error(STATUS_ERR_INVALID_FLAG);
        return 1;
    }

    char flag = argv[1][1];

    if (!is_ascii(argv[2])) {
        print_error(STATUS_ERR_NON_ASCII);
        return 1;
    }

    Status status = STATUS_SUCCESS;
    char *result = NULL;

    switch (flag) {
        case 'l':
            result = process_l(argv[2], &status);
            break;

        case 'r':
            result = process_r(argv[2], &status);
            break;

        case 'u':
            result = process_u(argv[2], &status);
            break;

        case 'n':
            result = process_n(argv[2], &status);
            break;

        case 'c': {
            if (argc < 4) {
                print_error(STATUS_ERR_INVALID_ARGC);
                return 1;
            }

            for (int i = 2; i < argc; i++) {
                if (i == 3) {
                    continue;
                }
                if (!is_ascii(argv[i])) {
                    print_error(STATUS_ERR_NON_ASCII);
                    return 1;
                }
            }

            unsigned int seed = 0;
            if (!parse_uint(argv[3], &seed)) {
                print_error(STATUS_ERR_INVALID_SEED);
                return 1;
            }

            result = process_c(argc, argv, 2, seed, &status);
            break;
        }

        default:
            print_error(STATUS_ERR_INVALID_FLAG);
            return 1;
    }

    if (status != STATUS_SUCCESS) {
        print_error(status);
        return 1;
    }

    printf("%s\n", result);
    free(result);

    return 0;
}