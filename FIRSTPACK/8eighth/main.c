#include "functions.h"


int main(int argc, char *argv[]) {
    const char *in_path = NULL;
    const char *out_path = NULL;

    Status st = parse_arguments(argc, argv, &in_path, &out_path);
    if (st != STATUS_OK) {
        fprintf(stderr, "Error: invalid arguments\n");
        return (int)st;
    }

    if (strcmp(in_path, out_path) == 0) {
        fprintf(stderr, "Error: input and output paths must differ\n");
        return (int)ERR_SAME_FILE;
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

    return (int)st;
}