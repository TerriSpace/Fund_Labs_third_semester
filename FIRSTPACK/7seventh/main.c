#include "functions.h"

int main(int argc, char *argv[]) {

    char *input1 = NULL;
    char *input2 = NULL;
    char *output = NULL;
    char flag = '\0';
    Status status = STATUS_OK;

    FILE *f1 = NULL;
    FILE *f2 = NULL;
    FILE *fout = NULL;

    status = parse_arguments(argc, argv, &flag, &input1, &input2, &output);
    if (status != STATUS_OK) {
        goto cleanup;
    }

    f1 = fopen(input1, "rb");
    if (f1 == NULL) {
        status = ERR_FILE_OPEN_IN;
        goto cleanup;
    }

    if (flag == 'r') {
        f2 = fopen(input2, "rb");
        if (f2 == NULL) {
            status = ERR_FILE_OPEN_IN;
            goto cleanup;
        }
    }

    fout = fopen(output, "wb");
    if (fout == NULL) {
        status = ERR_FILE_OPEN_OUT;
        goto cleanup;
    }

    if (flag == 'r') {
        status = process_flag_r(f1, f2, fout);
    } else if (flag == 'a') {
        status = process_flag_a(f1, fout);
    } else {
        status = ERR_INVALID_FLAG;
    }

    if (status == STATUS_OK) {
        if (fclose(fout) == EOF) {
            fout = NULL;
            status = ERR_FILE_WRITE;
        } else {
            fout = NULL;
        }
    }

cleanup:
    if (f1 != NULL) fclose(f1);
    if (f2 != NULL) fclose(f2);
    if (fout != NULL) fclose(fout);

    switch (status) {
        case STATUS_OK:
            break;

        case ERR_ARGS_COUNT:
            fprintf(stderr, "Ошибка: неверное количество аргументов.\n");
            fprintf(stderr, "Использование: %s <флаг> <файл1> <файл2> <выходной_файл>\n", argv[0]);
            break;

        case ERR_INVALID_FLAG:
            fprintf(stderr, "Ошибка: неверный флаг. Ожидается -r или -a (или /r, /a).\n");
            break;

        case ERR_MEMORY:
            fprintf(stderr, "Ошибка: не удалось выделить память.\n");
            break;

        case ERR_FILE_OPEN_IN:
            fprintf(stderr, "Ошибка: не удалось открыть входной файл.\n");
            break;

        case ERR_FILE_OPEN_OUT:
            fprintf(stderr, "Ошибка: не удалось открыть/создать выходной файл.\n");
            break;

        case ERR_FILE_READ:
            fprintf(stderr, "Ошибка: сбой при чтении из входного файла.\n");
            break;

        case ERR_FILE_WRITE:
            fprintf(stderr, "Ошибка: сбой при записи в выходной файл.\n");
            break;

        case ERR_SAME_FILE:
            fprintf(stderr, "Ошибка: выходной файл совпадает с одним из входных.\n");
            break;

        default:
            fprintf(stderr, "Ошибка: неизвестный статус (%d).\n", status);
            break;
    }

    return (int)status;
}