#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#define EPSILON 1e-9

typedef struct {
    unsigned int id;
    char *name;
    char *surname;
    double salary;
} Employee;

typedef enum {
    STATUS_OK,
    ERR_ARGS_COUNT,
    ERR_INVALID_FLAG,
    ERR_MEMORY,
    ERR_FILE_OPEN_IN,
    ERR_FILE_OPEN_OUT,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_INVALID_DATA,
    ERR_SAME_FILE
} Status;

Status parse_arguments(int argc, char *argv[], char **input_path, char **output_path, int *sort_desc);
Status check_same_file(const char *path1, const char *path2);
Status read_employees(const char *input_path, Employee **employees, size_t *count);
int compare_employees(const void *a, const void *b);
Status write_employees(const char *output_path, Employee *employees, size_t count);
void free_employees(Employee *employees, size_t count);
