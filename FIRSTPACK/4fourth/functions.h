#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/stat.h>


typedef enum {
    STATUS_OK,
    ERR_ARGS_COUNT,
    ERR_INVALID_FLAG,
    ERR_MISSING_OUTPUT,
    ERR_MEMORY,
    ERR_FILE_OPEN_IN,
    ERR_FILE_OPEN_OUT,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_OUTPUT_NAME,
    ERR_SAME_FILE
} Status;

Status check_same_file(const char *in_path, const char *out_path, int *is_same);

Status parse_arguments(int argc, char *argv[], char **input_path, char **output_path, char *flag);

Status generate_output_filename(const char *input_path, char **output_path);

char* read_line(FILE *fin, Status *st, size_t *out_len);

Status process_file(FILE *fin, FILE *fout, char flag);

Status flag_d(const char *line, size_t len, char **out_line, size_t *out_len);
Status flag_i(const char *line, size_t len, size_t *count);
Status flag_s(const char *line, size_t len, size_t *count);
Status flag_a(const char *line, size_t len, char **out_line, size_t *out_len);
