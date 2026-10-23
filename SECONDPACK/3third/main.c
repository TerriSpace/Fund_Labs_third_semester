#include "functions.h"


int main(void) {
    const char* file1 = "test1.txt";
    const char* file2 = "test2.txt";
    const char* search_str = "ab\ncd";

    Status status = search_in_files(search_str, 2, stdout, file1, file2);

    switch (status) {
        case STATUS_OK:
            printf("Status: OK\n");
            break;
        case STATUS_ERR_INVALID_ARGS:
            fprintf(stderr, "Error: Invalid arguments\n");
            break;
        case STATUS_ERR_EMPTY_SUBSTRING:
            fprintf(stderr, "Error: Empty substring\n");
            break;
        case STATUS_ERR_FILE_OPEN:
            fprintf(stderr, "Error: Cannot open file\n");
            break;
        case STATUS_ERR_MEMORY:
            fprintf(stderr, "Error: Out of memory\n");
            break;
        case STATUS_ERR_FILE_READ:
            fprintf(stderr, "Error: File read error\n");
            break;
        case STATUS_ERR_OUTPUT:
            fprintf(stderr, "Error: Output error\n");
            break;
        default:
            fprintf(stderr, "Error: Unknown error\n");
            break;
    }

    return (status == STATUS_OK) ? 0 : 1;
}