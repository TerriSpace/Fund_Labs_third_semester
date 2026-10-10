#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include <limits.h>


#define ARRAY_SIZE 100
#define MIN_SIZE 10
#define MAX_SIZE 10000
#define MIN_VAL -1000
#define MAX_VAL 1000
#define PRINT_LIMIT 10 

typedef enum {
    STATUS_OK,
    ERR_INVALID_INPUT,
    ERR_OVERFLOW,
    STATUS_INVALID_ARGC,
    STATUS_INVALID_ARG_FORMAT,
    STATUS_INVALID_RANGE,
    STATUS_NULL_POINTER,
    STATUS_SIZE_ERROR,
    STATUS_ALLOC_ERROR
} Status;

Status strtoi(const char *str, int *result);
Status parse_arguments(int argc, char *argv[], int *a, int *b);
Status fill_array(int *arr, size_t size, int a, int b);
Status swap_min_max(int *arr, size_t size, size_t *min_idx, size_t *max_idx);
void print_array(const int *arr, size_t size);

size_t get_random_size(void);
Status create_and_fill_array(int **arr, size_t size);
void free_array(int **arr);
Status sort_array(int *arr, size_t size);
int find_closest_value(const int *B, size_t size_B, int target);
Status create_array_C(const int *A, size_t size_A, const int *B, size_t size_B, int **C);
void print_array_limited(const int *arr, size_t size);

int run_task_1(int argc, char *argv[]);
int run_task_2(void);
