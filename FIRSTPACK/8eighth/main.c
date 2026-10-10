#include "functions.h"


int main(int argc, char *argv[]) {
    const char *in_path = NULL;
    const char *out_path = NULL;

    Status st = parse_arguments(argc, argv, &in_path, &out_path);
    if (st != STATUS_OK) {
        fprintf(stderr, "Error: invalid arguments\n");
        return (int)st;
    }

    st = check_same_file(in_path, out_path);
    if (st != STATUS_OK) {
        fprintf(stderr, "Error: input and output paths refer to the same file\n");
        return (int)st;
    }
    
    FILE *fin = fopen(in_path, "r");
    if (fin == NULL) {
        fprintf(stderr, "Error: cannot open input file\n");
        return ERR_FILE_OPEN_IN;
    }

    FILE *fout = fopen(out_path, "w");
    if (fout == NULL) {
        fclose(fin);
        fprintf(stderr, "Error: cannot open output file\n");
        return ERR_FILE_OPEN_OUT;
    }

st = process_file(fin, fout);

fclose(fin);
fclose(fout);

switch (st) {
    case STATUS_OK:
        break;
    case ERR_INVALID_DATA:
        fprintf(stderr, "Error: input file contains invalid characters\n");
        break;
    case ERR_OVERFLOW:
        fprintf(stderr, "Error: number is too large (overflow of unsigned long long)\n");
        break;
    case ERR_FILE_READ:
        fprintf(stderr, "Error: failed to read input file\n");
        break;
    case ERR_FILE_WRITE:
        fprintf(stderr, "Error: failed to write to output file\n");
        break;
    case ERR_MEMORY:
        fprintf(stderr, "Error: memory allocation failed\n");
        break;
    default:
        fprintf(stderr, "Error: unknown error (code %d)\n", (int)st);
        break;
}

return (int)st;
}