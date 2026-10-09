#include "functions.h"

Status parse_arguments(int argc, char *argv[], char *flag, char **input_path1, char **input_path2, char **output_path) {

    *flag = '\0';
    *input_path1 = NULL;
    *input_path2 = NULL;
    *output_path = NULL;

    if (argc < 3) {
        return ERR_ARGS_COUNT;
    }

    const char *flag_str = argv[1];
    size_t flen = strlen(flag_str);

    if (flen != 2 || (flag_str[0] != '-' && flag_str[0] != '/')) {
        return ERR_INVALID_FLAG;
    }

    char op = flag_str[1];
    if (op != 'r' && op != 'a') {
        return ERR_INVALID_FLAG;
    }
    *flag = op;

    if (op == 'r') {
        if (argc != 5) {
            return ERR_ARGS_COUNT;
        }
        *input_path1 = argv[2];
        *input_path2 = argv[3];
        *output_path = argv[4];
    } else { 
        if (argc != 4) {
            return ERR_ARGS_COUNT;
        }
        *input_path1 = argv[2];
        *input_path2 = NULL; 
        *output_path = argv[3];
    }

    if (strcmp(*input_path1, *output_path) == 0 || (*input_path2 != NULL && strcmp(*input_path2, *output_path) == 0)) {
        return ERR_SAME_FILE;
    }

    return STATUS_OK;
}
char* read_lexeme(FILE *fin, Status *st) {
    size_t capacity = 128;
    size_t length = 0;
    char *buffer = malloc(capacity);

    if (buffer == NULL) {
        *st = ERR_MEMORY;
        return NULL;
    }

    int c;
    while ((c = fgetc(fin)) != EOF && isspace(c)) {
        //пробелы впереди
    }

    if (c == EOF) {
        free(buffer);
        *st = STATUS_OK;
        return NULL; 
    }

    while (c != EOF && !isspace(c)) {

        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_buffer = realloc(buffer, capacity);
            if (new_buffer == NULL) {
                free(buffer);
                *st = ERR_MEMORY;
                return NULL;
            }
            buffer = new_buffer;
        }

        buffer[length++] = (char)c;
        c = fgetc(fin);
    }

    if (ferror(fin)) {
        free(buffer);
        *st = ERR_FILE_READ;
        return NULL;
    }

    buffer[length] = '\0';
    *st = STATUS_OK;
    return buffer;
}

Status write_lexeme(FILE *fout, const char *lexeme, int *is_first) {
    if (!(*is_first)) {
        if (fputc(' ', fout) == EOF) {
            return ERR_FILE_WRITE;
        }
    }

    if (fputs(lexeme, fout) == EOF) {
        return ERR_FILE_WRITE;
    }

    *is_first = 0;
    return STATUS_OK;
}

Status process_flag_r(FILE *f1, FILE *f2, FILE *fout) {
    Status st;
    char *lexeme = NULL;
    int is_first = 1;
    int end1 = 0, end2 = 0;

    while (!end1 || !end2) {

        if (!end1) {
            lexeme = read_lexeme(f1, &st);
            if (st != STATUS_OK) {
                return st;
            }

            if (lexeme == NULL) {
                end1 = 1;
            } else {
                Status wst = write_lexeme(fout, lexeme, &is_first);
                free(lexeme);
                lexeme = NULL;
                if (wst != STATUS_OK) {
                    return wst;
                }
            }
        }

        if (!end2) {
            lexeme = read_lexeme(f2, &st);
            if (st != STATUS_OK) {
                return st;
            }

            if (lexeme == NULL) {
                end2 = 1;
            } else {
                Status wst = write_lexeme(fout, lexeme, &is_first);
                free(lexeme);
                lexeme = NULL;
                if (wst != STATUS_OK) {
                    return wst;
                }
            }
        }
    }

    return STATUS_OK;
}

void to_lowercase_latin(char *str) {
    if (str == NULL) {
        return;
    }
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = (char)(str[i] - 'A' + 'a');
        }
    }
}

Status convert_ascii_to_base(const char *str, int base, char **result) {
    if (str == NULL || result == NULL) {
        return ERR_ARGS_COUNT;
    }
    if (base != 4 && base != 8) {
        return ERR_INVALID_FLAG;
    }

    size_t max_per_char = (base == 4) ? 4 : 3;
    size_t str_len = strlen(str);
    size_t capacity = str_len * max_per_char + 1;

    char *buf = malloc(capacity);
    if (buf == NULL) {
        return ERR_MEMORY;
    }

    size_t pos = 0;
    for (size_t i = 0; i < str_len; i++) {
        unsigned char code = (unsigned char)str[i];

        if (code == 0) {
            buf[pos++] = '0';
            continue;
        }

        char temp[16];
        int tlen = 0;
        unsigned char tmp = code;
        while (tmp > 0) {
            temp[tlen++] = (char)('0' + (tmp % base));
            tmp /= base;
        }
        while (tlen > 0) {
            buf[pos++] = temp[--tlen];
        }
    }
    buf[pos] = '\0';

    *result = buf;
    return STATUS_OK;
}

Status process_flag_a(FILE *fin, FILE *fout) {
    Status st = STATUS_OK;
    char *lexeme = NULL;
    int is_first = 1;
    unsigned long counter = 0;
    while (1) {
        lexeme = read_lexeme(fin, &st);
        if (st != STATUS_OK) {
            return st;
        }
        if (lexeme == NULL) {
            break;
        }

        counter++;
        Status wst = STATUS_OK;
        char *to_write = lexeme;
        char *allocated = NULL;
        if (counter % 10 == 0) {
            to_lowercase_latin(lexeme);
            Status cst = convert_ascii_to_base(lexeme, 4, &allocated);
            if (cst != STATUS_OK) {
                free(lexeme);
                return cst;
            }
            to_write = allocated;

        } else if (counter % 2 == 0) {
            to_lowercase_latin(lexeme);
            
        } else if (counter % 5 == 0) {
            Status cst = convert_ascii_to_base(lexeme, 8, &allocated);
            if (cst != STATUS_OK) {
                free(lexeme);
                return cst;
            }
            to_write = allocated;
        }

        wst = write_lexeme(fout, to_write, &is_first);

        free(lexeme);
        if (allocated != NULL) {
            free(allocated);
        }

        if (wst != STATUS_OK) {
            return wst;
        }
    }

    return STATUS_OK;
}