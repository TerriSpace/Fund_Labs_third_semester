#include "functions.h"


static void print_error(Status err, const char *context) {
    switch (err) {
        case ERR_ARGS_COUNT:
            fprintf(stderr, "Error: invalid args count\n");
            break;

        case ERR_INVALID_FLAG:
            fprintf(stderr, "Error: unknown flag '%s'.\n", context);
            break;

        case ERR_MISSING_OUTPUT:
            fprintf(stderr, "Error: name output file.\n");
            break;

        case ERR_MEMORY:
            fprintf(stderr, "Memory error\n");
            break;

        case ERR_FILE_OPEN_IN:
            fprintf(stderr, "Error: open input'%s'.\n", context);
            break;

        case ERR_FILE_OPEN_OUT:
            fprintf(stderr, "Error: open output'%s'.\n", context);
            break;

        case ERR_FILE_READ:
            fprintf(stderr, "File read error '%s'.\n", context);
            break;

        case ERR_FILE_WRITE:
            fprintf(stderr, "Write file error'%s'.\n", context);
            break;

        case ERR_OUTPUT_NAME:
            fprintf(stderr, "Generate output error\n");
            break;

        case ERR_SAME_FILE:
            fprintf(stderr, "Error: same file\n");
            break;

        default:
            fprintf(stderr, "Unknown error");
            break;
    }
}

int main(int argc, char *argv[]) {
    
    char  *input_path  = NULL;
    char  *output_path = NULL;
    char   flag        = '\0';
    Status err;

    err = parse_arguments(argc, argv, &input_path, &output_path, &flag);
    if (err != STATUS_OK) {
        const char *ctx = "";
        if (err == ERR_INVALID_FLAG && argc > 1) {
            ctx = argv[1];
        } else if (err == ERR_SAME_FILE && argc > 2) {
            ctx = argv[2];
        }
        print_error(err, ctx);
        return EXIT_FAILURE;
    }

    FILE *fin = fopen(input_path, "r");
    if (fin == NULL) {
        print_error(ERR_FILE_OPEN_IN, input_path);
        free(input_path);
        free(output_path);
        return EXIT_FAILURE;
    }

    FILE *fout = fopen(output_path, "w");
    if (fout == NULL) {
        print_error(ERR_FILE_OPEN_OUT, output_path);
        fclose(fin);
        free(input_path);
        free(output_path);
        return EXIT_FAILURE;
    }

    err = process_file(fin, fout, flag);
    if (err != STATUS_OK) {
        print_error(err, input_path);
    }

    fclose(fin);
    fclose(fout);
    free(input_path);
    free(output_path);

    return (err == STATUS_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}