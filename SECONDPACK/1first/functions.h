#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef enum {
    STATUS_SUCCESS = 0,
    STATUS_ERR_INVALID_ARGC,
    STATUS_ERR_INVALID_FLAG,
    STATUS_ERR_INVALID_SEED,
    STATUS_ERR_MEMORY,
    STATUS_ERR_EMPTY_STRING,
    STATUS_ERR_NON_ASCII,
    STATUS_ERR_UNKNOWN
} Status;

size_t my_strlen(const char *str);
char *my_strdup(const char *src, Status *status);

bool is_ascii(const char *str);
bool is_digit(char c);
bool is_alpha(char c);
bool is_upper(char c);
bool is_lower(char c);
char to_upper(char c);

char *process_l(const char *str, Status *status);
char *process_r(const char *str, Status *status);
char *process_u(const char *str, Status *status);
char *process_n(const char *str, Status *status);
char *process_c(int argc, char **argv, int start_index, unsigned int seed, Status *status);

void print_error(Status status);