#include "functions.h"


static int is_ascii_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static char *read_line(FILE *f, Status *st)
{
    *st = STATUS_OK;
    char *buf = NULL;
    size_t cap = 0;
    ssize_t n = getline(&buf, &cap, f);
    if (n < 0) {
        if (feof(f)) { free(buf); return NULL; }
        free(buf);
        *st = ERR_FILE_READ;
        return NULL;
    }
    if (n > 0 && buf[n - 1] == '\n') buf[n - 1] = '\0';
    return buf;
}

static Status parse_employee(const char *line, Employee *emp)
{
    const char *p = line;
    char *end;

    while (*p == ' ' || *p == '\t') p++;

    const char *id_start = p;
    while (isdigit((unsigned char)*p)) p++;
    if (p == id_start) return ERR_INVALID_DATA;
    unsigned long long v = strtoull(id_start, &end, 10);
    if (end != p) return ERR_INVALID_DATA;
    if (v > UINT_MAX) return ERR_INVALID_DATA;
    emp->id = (unsigned int)v;
    
    const char *name_start = p;
    while (is_ascii_alpha(*p)) p++;
    size_t name_len = (size_t)(p - name_start);
    if (name_len == 0) return ERR_INVALID_DATA;
    emp->name = malloc(name_len + 1);
    if (!emp->name) return ERR_MEMORY;
    memcpy(emp->name, name_start, name_len);
    emp->name[name_len] = '\0';

    if (*p != ' ' && *p != '\t') {
        free(emp->name); emp->name = NULL; return ERR_INVALID_DATA;
    }
    while (*p == ' ' || *p == '\t') p++;

    const char *sur_start = p;
    while (is_ascii_alpha(*p)) p++;
    size_t sur_len = (size_t)(p - sur_start);
    if (sur_len == 0) {
        free(emp->name); emp->name = NULL; return ERR_INVALID_DATA;
    }
    emp->surname = malloc(sur_len + 1);
    if (!emp->surname) {
        free(emp->name); emp->name = NULL; return ERR_MEMORY;
    }
    memcpy(emp->surname, sur_start, sur_len);
    emp->surname[sur_len] = '\0';

    if (*p != ' ' && *p != '\t') {
        free(emp->name); free(emp->surname);
        emp->name = NULL; emp->surname = NULL;
        return ERR_INVALID_DATA;
    }
    while (*p == ' ' || *p == '\t') p++;

    const char *sal_start = p;
    int has_digit = 0;
    while (isdigit((unsigned char)*p)) { p++; has_digit = 1; }
    if (*p == '.') {
        p++;
        while (isdigit((unsigned char)*p)) { p++; has_digit = 1; }
    }
    if (!has_digit) {
        free(emp->name); free(emp->surname);
        emp->name = NULL; emp->surname = NULL;
        return ERR_INVALID_DATA;
    }
    double d = strtod(sal_start, &end);
    if (end != p || !isfinite(d) || d < 0) {
        free(emp->name); free(emp->surname);
        emp->name = NULL; emp->surname = NULL;
        return ERR_INVALID_DATA;
    }
    emp->salary = d;

    while (*p == ' ' || *p == '\t' || *p == '\r') p++;
    if (*p != '\0') {
        free(emp->name); free(emp->surname);
        emp->name = NULL; emp->surname = NULL;
        return ERR_INVALID_DATA;
    }
    return STATUS_OK;
}

Status parse_arguments(int argc, char *argv[], char **input_path, char **output_path, int *sort_desc)
{
    if (argc != 4) return ERR_ARGS_COUNT;
    const char *flag = argv[2];
    if (flag[0] != '-' && flag[0] != '/') return ERR_INVALID_FLAG;
    if (flag[1] == 'a' && flag[2] == '\0') *sort_desc = 0;
    else if (flag[1] == 'd' && flag[2] == '\0') *sort_desc = 1;
    else return ERR_INVALID_FLAG;
    *input_path = argv[1];
    *output_path = argv[3];
    return STATUS_OK;
}

Status check_same_file(const char *path1, const char *path2)
{
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

Status read_employees(const char *input_path, Employee **employees, size_t *count)
{
    *employees = NULL;
    *count = 0;

    FILE *f = fopen(input_path, "r");
    if (!f) return ERR_FILE_OPEN_IN;

    size_t cap = 16, n = 0;
    Employee *arr = malloc(cap * sizeof(Employee));
    if (!arr) { fclose(f); return ERR_MEMORY; }

    Status st;
    while (1) {
        char *line = read_line(f, &st);
        if (!line) {
            if (st != STATUS_OK) {
                for (size_t i = 0; i < n; i++) {
                    free(arr[i].name); free(arr[i].surname);
                }
                free(arr); fclose(f);
                return st;
            }
            break;
        }

        int only_ws = 1;
        for (const char *q = line; *q; q++) {
            if (*q != ' ' && *q != '\t' && *q != '\r') { only_ws = 0; break; }
        }
        if (only_ws) { free(line); continue; }

        if (n == cap) {
            size_t new_cap = cap * 2;
            Employee *tmp = realloc(arr, new_cap * sizeof(Employee));
            if (!tmp) {
                free(line);
                for (size_t i = 0; i < n; i++) {
                    free(arr[i].name); free(arr[i].surname);
                }
                free(arr); fclose(f);
                return ERR_MEMORY;
            }
            arr = tmp; cap = new_cap;
        }

        st = parse_employee(line, &arr[n]);
        free(line);
        if (st != STATUS_OK) {
            for (size_t i = 0; i < n; i++) {
                free(arr[i].name); free(arr[i].surname);
            }
            free(arr); fclose(f);
            return st;
        }
        n++;
    }

    if (fclose(f) != 0) {
        for (size_t i = 0; i < n; i++) {
            free(arr[i].name); free(arr[i].surname);
        }
        free(arr);
        return ERR_FILE_READ;
    }

    if (n == 0) { free(arr); return STATUS_OK; }

    Employee *shrunk = realloc(arr, n * sizeof(Employee));
    if (shrunk) arr = shrunk;

    *employees = arr;
    *count = n;
    return STATUS_OK;
}

int compare_employees(const void *a, const void *b)
{
    const Employee *ea = (const Employee *)a;
    const Employee *eb = (const Employee *)b;

    if (fabs(ea->salary - eb->salary) >= EPSILON) {
        return (ea->salary < eb->salary) ? -1 : 1;
    }
    int c = strcmp(ea->surname, eb->surname);
    if (c != 0) return c;
    c = strcmp(ea->name, eb->name);
    if (c != 0) return c;
    if (ea->id < eb->id) return -1;
    if (ea->id > eb->id) return 1;
    return 0;
}

Status write_employees(const char *output_path, Employee *employees, size_t count)
{
    FILE *f = fopen(output_path, "w");
    if (!f) return ERR_FILE_OPEN_OUT;

    for (size_t i = 0; i < count; i++) {
        if (fprintf(f, "%u %s %s %g\n", employees[i].id, employees[i].name, employees[i].surname, employees[i].salary) < 0) {
            fclose(f);
            return ERR_FILE_WRITE;
        }
    }

    if (fclose(f) != 0) return ERR_FILE_WRITE;
    return STATUS_OK;
}

void free_employees(Employee *employees, size_t count)
{
    if (!employees) return;
    for (size_t i = 0; i < count; i++) {
        free(employees[i].name);
        free(employees[i].surname);
    }
    free(employees);
}