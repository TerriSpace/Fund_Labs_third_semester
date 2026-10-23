#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
    STATUS_OK = 0,
    STATUS_ERR_INVALID_ARGS,
    STATUS_ERR_EMPTY_SUBSTRING,
    STATUS_ERR_FILE_OPEN,
    STATUS_ERR_MEMORY,
    STATUS_ERR_FILE_READ,
    STATUS_ERR_OUTPUT
} Status;


typedef struct {
    size_t line;
    size_t col;
} Coord;

Status search_in_files(const char* sub, int num_files, FILE* out_stream, ...);
