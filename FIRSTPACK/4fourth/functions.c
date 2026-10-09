#include "functions.h"

Status generate_output_filename(const char *input_path, char **output_path) {
    size_t len = strlen(input_path);

    const char *last_slash = strrchr(input_path, '/');
    const char *name_start = last_slash ? last_slash + 1 : input_path;
    const char *last_dot = strrchr(name_start, '.');

    const char *insert_pos;
    if (last_dot == NULL || last_dot == name_start) {
        insert_pos = input_path + len;
    } else {
        insert_pos = last_dot;
    }

    size_t prefix_len = (size_t)(insert_pos - input_path);
    size_t suffix_len = len - prefix_len;
    size_t new_len = prefix_len + 4 + suffix_len;

    
    *output_path = malloc(new_len + 1);
    if (*output_path == NULL) {
        return ERR_MEMORY;
    }

    memcpy(*output_path, input_path, prefix_len);
    memcpy(*output_path + prefix_len, "_out", 4);
    memcpy(*output_path + prefix_len + 4, insert_pos, suffix_len);
    (*output_path)[new_len] = '\0';

    return STATUS_OK;
}

Status parse_arguments(int argc, char *argv[], char **input_path, char **output_path, char *flag) {
    if (argc < 3 || argc > 4) {
        return ERR_ARGS_COUNT;
    }

    const char *flag_str = argv[1];
    size_t flen = strlen(flag_str);

    if (flen < 2 || (flag_str[0] != '-' && flag_str[0] != '/')) {
        return ERR_INVALID_FLAG;
    }

    char op = '\0';
    int has_n = 0;

    if (flen == 2) {
        char c = flag_str[1];
        if (c == 'd' || c == 'i' || c == 's' || c == 'a') {
            op = c;
            has_n = 0;
        } else {
            return ERR_INVALID_FLAG;
        }
    } 
    else if (flen == 3) {
        if (flag_str[1] != 'n') {
            return ERR_INVALID_FLAG;
        }
        char c = flag_str[2];
        if (c == 'd' || c == 'i' || c == 's' || c == 'a') {
            op = c;
            has_n = 1;
        } else {
            return ERR_INVALID_FLAG;
        }
    } 
    else {
        return ERR_INVALID_FLAG;
    }

    *flag = op;

    *input_path = malloc(strlen(argv[2]) + 1);
    if (*input_path == NULL) {
        return ERR_MEMORY;
    }
    strcpy(*input_path, argv[2]);

    if (has_n) {
        if (argc < 4) {
            free(*input_path);
            *input_path = NULL;
            return ERR_MISSING_OUTPUT;
        }
        *output_path = malloc(strlen(argv[3]) + 1);
        if (*output_path == NULL) {
            free(*input_path);
            *input_path = NULL;
            return ERR_MEMORY;
        }
        strcpy(*output_path, argv[3]);
    } else {
        if (argc > 3) {
            free(*input_path);
            *input_path = NULL;
            return ERR_ARGS_COUNT;
        }
        Status s = generate_output_filename(argv[2], output_path);
        if (s != STATUS_OK) {
            free(*input_path);
            *input_path = NULL;
            return s;
        }
    }

    if (strcmp(*input_path, *output_path) == 0) {
        free(*input_path);
        free(*output_path);
        *input_path = NULL;
        *output_path = NULL;
        return ERR_SAME_FILE;
    }


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

Status process_file(FILE *fin, FILE *fout, char flag) {
    char *line = NULL;
    Status st = STATUS_OK;
    Status read_st = STATUS_OK;

    while ((line = read_line(fin, &read_st)) != NULL) {
        switch (flag) {
            case 'd': st = flag_d(line, fout); break;
            case 'i': st = flag_i(line, fout); break;
            case 's': st = flag_s(line, fout); break;
            case 'a': st = flag_a(line, fout); break;
            default:  st = ERR_INVALID_FLAG;   break;
        }

        free(line);

        if (st != STATUS_OK) {
            return st;
        }
    }

    return read_st;
}

Status flag_d(const char *line, FILE *fout) {
    size_t len = strlen(line);

    char *buffer = malloc(len + 1);
    if (buffer == NULL) {
        return ERR_MEMORY;
    }

    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)line[i])) {
            buffer[j++] = line[i];
        }
    }
    buffer[j] = '\0';

    if (fputs(buffer, fout) == EOF) {
        free(buffer);
        return ERR_FILE_WRITE;
    }

    free(buffer);
    return STATUS_OK;
}

Status flag_i(const char *line, FILE *fout) {
    size_t len = strlen(line);
    size_t count = 0;

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)line[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            count++;
        }
    }

    if (fprintf(fout, "%zu\n", count) < 0) {
        return ERR_FILE_WRITE;
    }

    return STATUS_OK;
}

Status flag_s(const char *line, FILE *fout) {
    size_t len = strlen(line);
    size_t count = 0;

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)line[i];

        int is_latin = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        int is_digit = (c >= '0' && c <= '9');
        int is_space = (c == ' ');

        if (!is_latin && !is_digit && !is_space) {
            count++;
        }
    }

    if (fprintf(fout, "%zu\n", count) < 0) {
        return ERR_FILE_WRITE;
    }

    return STATUS_OK;
}

Status flag_a(const char *line, FILE *fout) {
    size_t len = strlen(line);

    size_t content_len = len;
    if (content_len > 0 && line[content_len - 1] == '\n') {
        content_len--;
    }

    char *buffer = malloc(2 * content_len + 2);
    if (buffer == NULL) {
        return ERR_MEMORY;
    }

    size_t j = 0;
    for (size_t i = 0; i < content_len; i++) {
        unsigned char c = (unsigned char)line[i];

        if (c >= '0' && c <= '9') {
            buffer[j++] = (char)c;
        } else {
            unsigned char hi = c >> 4;
            unsigned char lo = c & 0x0F;
            buffer[j++] = (hi < 10) ? ('0' + hi) : ('A' + hi - 10);
            buffer[j++] = (lo < 10) ? ('0' + lo) : ('A' + lo - 10);
        }
    }

    buffer[j++] = '\n';
    buffer[j] = '\0';

    if (fputs(buffer, fout) == EOF) {
        free(buffer);
        return ERR_FILE_WRITE;
    }

    free(buffer);
    return STATUS_OK;
}