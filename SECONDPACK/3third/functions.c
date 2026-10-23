#include "functions.h"


static size_t* compute_lps(const char* sub, size_t sub_len) {
    size_t* lps = (size_t*)malloc(sub_len * sizeof(size_t));
    if (!lps) return NULL;

    size_t len = 0;
    lps[0] = 0;
    size_t i = 1;

    while (i < sub_len) {
        if (sub[i] == sub[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

static Status process_file(const char* filepath, const char* sub, size_t sub_len, const size_t* lps, FILE* out_stream) {
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        return STATUS_ERR_FILE_OPEN;
    }

    Coord* coords = (Coord*)malloc(sub_len * sizeof(Coord));
    if (!coords) {
        fclose(f);
        return STATUS_ERR_MEMORY;
    }

    size_t j = 0;
    size_t line = 1;
    size_t col = 1;
    size_t coord_idx = 0;
    char buf[4096];
    size_t bytes_read;
    Status status = STATUS_OK;

    while ((bytes_read = fread(buf, 1, sizeof(buf), f)) > 0) {
        for (size_t i = 0; i < bytes_read; ++i) {
            char ch = buf[i];

            coords[coord_idx].line = line;
            coords[coord_idx].col = col;
            coord_idx = (coord_idx + 1) % sub_len;

            while (j > 0 && ch != sub[j]) {
                j = lps[j - 1];
            }
            if (ch == sub[j]) {
                j++;
            }

            if (j == sub_len) {
                Coord start = coords[coord_idx];
                if (fprintf(out_stream, "File: %s, Line: %zu, Col: %zu\n", filepath, start.line, start.col) < 0) {
                    status = STATUS_ERR_OUTPUT;
                    break;
                }
                j = lps[j - 1];
            }

            if (ch == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
        }
        if (status != STATUS_OK) {
            break;
        }
    }

    if (status == STATUS_OK && ferror(f)) {
        status = STATUS_ERR_FILE_READ;
    }

    free(coords);
    fclose(f);
    return status;
}

Status search_in_files(const char* sub, int num_files, FILE* out_stream, ...) {
    if (!sub || !out_stream || num_files <= 0) {
        return STATUS_ERR_INVALID_ARGS;
    }

    size_t sub_len = strlen(sub);
    if (sub_len == 0) {
        return STATUS_ERR_EMPTY_SUBSTRING;
    }

    size_t* lps = compute_lps(sub, sub_len);
    if (!lps) {
        return STATUS_ERR_MEMORY;
    }

    va_list args;
    va_start(args, out_stream);

    Status status = STATUS_OK;
    for (int i = 0; i < num_files; ++i) {
        const char* filepath = va_arg(args, const char*);
        if (!filepath) {
            status = STATUS_ERR_INVALID_ARGS;
            break;
        }

        status = process_file(filepath, sub, sub_len, lps, out_stream);
        if (status != STATUS_OK) {
            break;
        }
    }

    va_end(args);
    free(lps);
    return status;
}