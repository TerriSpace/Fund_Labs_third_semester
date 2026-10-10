#include "functions.h"



Status check_same_file(const char *path1, const char *path2) {
    struct stat st1, st2;

    int res1 = stat(path1, &st1);
    int res2 = stat(path2, &st2);

    if (res1 != 0 || res2 != 0) {
        return STATUS_OK;
    }

    if (st1.st_dev == st2.st_dev && st1.st_ino == st2.st_ino) {
        return ERR_SAME_FILE;
    }

    return STATUS_OK;
}

Status parse_arguments(int argc, char *argv[], const char **input_path, const char **output_path) {
    if (argc != 3) {
        return ERR_ARGS_COUNT;
    }
    
    *input_path = argv[1];
    *output_path = argv[2];

    return STATUS_OK;
}

Status char_to_value(char c, int *value) {
    if (c >= '0' && c <= '9') {
        *value = c - '0';
        return STATUS_OK;
    }
    if (c >= 'A' && c <= 'Z') {
        *value = c - 'A' + 10;
        return STATUS_OK;
    }
    if (c >= 'a' && c <= 'z') {
        *value = c - 'a' + 10;
        return STATUS_OK;
    }
    return ERR_INVALID_DATA;
}

Status find_min_base(const char *str, int *base) {
    int max_digit = 0;
    const char *p = str;
    while (*p != '\0') {
        int v;
        Status st = char_to_value(*p, &v);
        if (st != STATUS_OK) return st;
        if (v > max_digit) max_digit = v;
        p++;
    }
    *base = (max_digit + 1 < 2) ? 2 : max_digit + 1;
    return STATUS_OK;
}

Status convert_to_decimal(const char *str, int base, unsigned long long *result) {
    unsigned long long res = 0;
    const char *p = str;
    while (*p != '\0') {
        int v;
        Status st = char_to_value(*p, &v);
        if (st != STATUS_OK) return st;
        
        if (res > (ULLONG_MAX - (unsigned long long)v) / (unsigned long long)base) {
            return ERR_OVERFLOW;
        }

        res = res * (unsigned long long)base + (unsigned long long)v;
        p++;
    }
    *result = res;
    return STATUS_OK;
}

char* read_line(FILE *fin, Status *st) {
    size_t capacity = 128;
    size_t length = 0;
    char *buffer = malloc(capacity);

    if (buffer == NULL) {
        *st = ERR_MEMORY;
        return NULL;
    }

    int c;
    while ((c = fgetc(fin)) != EOF) {
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

        if (c == '\n') {
            break;
        }
    }

    if (ferror(fin)) {
        free(buffer);
        *st = ERR_FILE_READ;
        return NULL;
    }

    if (length == 0 && c == EOF) {
        free(buffer);
        *st = STATUS_OK;
        return NULL;
    }

    buffer[length] = '\0';
    *st = STATUS_OK;
    return buffer;
}

Status process_file(FILE *fin, FILE *fout) {
    char *line = NULL;
    Status read_st = STATUS_OK;
    const char *delims = " \t\n\r";

    while ((line = read_line(fin, &read_st)) != NULL) {

        char *save_ptr = line;

        char *token = strtok(line, delims);
        while (token != NULL) {
            char *num = token;
            while (*num == '0' && *(num + 1) != '\0') {
                num++;
            }

            int base = 0;
            Status st = find_min_base(num, &base);
            if (st != STATUS_OK) {
                free(save_ptr);
                return st;
            }

            unsigned long long dec = 0;
            st = convert_to_decimal(num, base, &dec);
            if (st != STATUS_OK) {
                free(save_ptr);
                return st;
            }

            if (fprintf(fout, "%s %d %llu\n", num, base, dec) < 0) {
                free(save_ptr);
                return ERR_FILE_WRITE;
            }

            token = strtok(NULL, delims);
        }

        free(save_ptr);
    }

    return read_st;
}