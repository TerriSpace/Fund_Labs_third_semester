#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <sys/stat.h>

typedef enum {
    STATUS_OK,
    ERR_ARGS_COUNT,
    ERR_MEMORY,
    ERR_FILE_OPEN_IN,
    ERR_FILE_OPEN_OUT,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_INVALID_DATA,
    ERR_OVERFLOW,
    ERR_SAME_FILE
} Status;

Status parse_arguments(int argc, char *argv[], const char **input_path, const char **output_path);

Status check_same_file(const char *path1, const char *path2);
char* read_line(FILE *fin, Status *st);


Status process_file(FILE *fin, FILE *fout);


Status find_min_base(const char *str, int *base);
Status convert_to_decimal(const char *str, int base, unsigned long long *result);