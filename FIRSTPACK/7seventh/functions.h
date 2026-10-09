#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    STATUS_OK,
    ERR_ARGS_COUNT,
    ERR_INVALID_FLAG,
    ERR_MEMORY,
    ERR_FILE_OPEN_IN,
    ERR_FILE_OPEN_OUT,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_SAME_FILE
} Status;

Status parse_arguments(int argc, char *argv[], char *flag, char **input_path1, char **input_path2, char **output_path);

char* read_lexeme(FILE *fin, Status *st);

Status write_lexeme(FILE *fout, const char *lexeme, int *is_first);

Status process_flag_r(FILE *f1, FILE *f2, FILE *fout);

Status process_flag_a(FILE *fin, FILE *fout);

void to_lowercase_latin(char *str);

Status convert_ascii_to_base(const char *str, int base, char **result);