#include "functions.h"


static void print_error(Status st)
{
    switch (st) {
        case ERR_ARGS_COUNT:
            fprintf(stderr, "Error: invalid number of arguments.\n" "Usage: program <input_file> <flag> <output_file>\n""  flag: -a or /a for ascending, -d or /d for descending\n");
            break;
        case ERR_INVALID_FLAG:
            fprintf(stderr, "Error: invalid flag. Use -a, /a, -d or /d.\n");
            break;
        case ERR_FILE_OPEN_IN:
            fprintf(stderr, "Error: cannot open input file.\n");
            break;
        case ERR_FILE_OPEN_OUT:
            fprintf(stderr, "Error: cannot open output file.\n");
            break;
        case ERR_FILE_READ:
            fprintf(stderr, "Error: failed to read from input file.\n");
            break;
        case ERR_FILE_WRITE:
            fprintf(stderr, "Error: failed to write to output file.\n");
            break;
        case ERR_MEMORY:
            fprintf(stderr, "Error: memory allocation failed.\n");
            break;
        case ERR_INVALID_DATA:
            fprintf(stderr, "Error: invalid data format in input file.\n");
            break;
        case ERR_SAME_FILE:
            fprintf(stderr, "Error: input and output files are the same.\n");
            break;
        default:
            fprintf(stderr, "Error: unknown error code %d.\n", (int)st);
            break;
    }
}

int main(int argc, char *argv[])
{
    char *input_path = NULL;
    char *output_path = NULL;
    int sort_desc = 0;
    Status st;

    st = parse_arguments(argc, argv, &input_path, &output_path, &sort_desc);
    if (st != STATUS_OK) {
        print_error(st);
        return (int)st;
    }

    st = check_same_file(input_path, output_path);
    if (st != STATUS_OK) {
        print_error(st);
        return (int)st;
    }

    Employee *employees = NULL;
    size_t count = 0;
    st = read_employees(input_path, &employees, &count);
    if (st != STATUS_OK) {
        print_error(st);
        return (int)st;
    }

    if (count > 0) {
        qsort(employees, count, sizeof(Employee), compare_employees);
        if (sort_desc) {
            for (size_t i = 0; i < count / 2; i++) {
                Employee tmp = employees[i];
                employees[i] = employees[count - 1 - i];
                employees[count - 1 - i] = tmp;
            }
        }
    }

    st = write_employees(output_path, employees, count);
    if (st != STATUS_OK) {
        print_error(st);
        free_employees(employees, count);
        return (int)st;
    }

    free_employees(employees, count);
    return 0;
}