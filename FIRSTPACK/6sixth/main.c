#include "functions.h"


#define MAX_TASK1_N 6
#define MAX_TASK2_N 5
#define MAX_TASK3_STR 8
#define MAX_TASK4_N 6
#define MAX_STR_LEN 64


static void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static Status read_int(const char *prompt, int *out) {
    if (prompt != NULL) printf("%s", prompt);

    long long tmp;
    if (scanf("%lld", &tmp) != 1) {
        clear_input();
        return STATUS_ERR_INVALID_ARG;
    }
    clear_input();

    if (tmp < INT_MIN || tmp > INT_MAX) {
        return STATUS_ERR_INVALID_ARG;
    }

    *out = (int)tmp;
    return STATUS_OK;
}

static Status read_double(const char *prompt, double *out) {
    if (prompt != NULL) printf("%s", prompt);
    if (scanf("%lf", out) != 1) {
        clear_input();
        return STATUS_ERR_INVALID_ARG;
    }
    clear_input();
    return STATUS_OK;
}

static Status read_line(const char *prompt, char *buf, size_t size) {
    if (prompt != NULL) printf("%s", prompt);
    if (fgets(buf, (int)size, stdin) == NULL) return STATUS_ERR_INVALID_ARG;
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        clear_input();
    }
    return STATUS_OK;
}

static const char *status_str(Status st) {
    switch (st) {
        case STATUS_OK:                 return "OK";
        case STATUS_ERR_INVALID_ARG:    return "INVALID_ARG";
        case STATUS_ERR_MEMORY:         return "MEMORY";
        case STATUS_ERR_NO_ROOT:        return "NO_ROOT";
        case STATUS_ERR_DOMAIN:         return "DOMAIN";
        case STATUS_ERR_INVALID_BASE:   return "INVALID_BASE";
        case STATUS_ERR_INVALID_STRING: return "INVALID_STRING";
        case STATUS_ERR_CAPACITY:       return "CAPACITY";
        default:                        return "UNKNOWN";
    }
}

static double f_sq_minus4(double x)      { return x * x - 4.0; }
static double f_sin_minus_half(double x) { return sin(x) - 0.5; }
static double f_exp_minus3(double x)     { return exp(x) - 3.0; }
static double f_no_root(double x)        { return x * x + 1.0; }

static void run_task1(void) {
    int n;
    if (read_int("n (3..6): ", &n) != STATUS_OK) { printf("input error\n"); return; }
    if (n < 3 || n > MAX_TASK1_N) { printf("bad n\n"); return; }

    double eps;
    if (read_double("eps: ", &eps) != STATUS_OK || eps <= 0.0) { printf("bad eps\n"); return; }

    double c[2 * MAX_TASK1_N];
    for (int i = 0; i < n; i++) {
        char p[32];
        snprintf(p, sizeof(p), "x%d: ", i + 1);
        if (read_double(p, &c[2 * i]) != STATUS_OK) { printf("input error\n"); return; }
        snprintf(p, sizeof(p), "y%d: ", i + 1);
        if (read_double(p, &c[2 * i + 1]) != STATUS_OK) { printf("input error\n"); return; }
    }

    bool convex = false;
    Status st;
    switch (n) {
        case 3: st = is_convex(3, eps, &convex, c[0],c[1],c[2],c[3],c[4],c[5]); break;
        case 4: st = is_convex(4, eps, &convex, c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7]); break;
        case 5: st = is_convex(5, eps, &convex, c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],c[8],c[9]); break;
        case 6: st = is_convex(6, eps, &convex, c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],c[8],c[9],c[10],c[11]); break;
        default: st = STATUS_ERR_INVALID_ARG; break;
    }
    if (st == STATUS_OK) printf("status: %s, convex=%d\n", status_str(st), convex);
    else {
        printf("status: %s\n", status_str(st));
    }
}

static void run_task2(void) {
    double x;
    if (read_double("x: ", &x) != STATUS_OK) { printf("input error\n"); return; }

    int n;
    if (read_int("degree n (0..5): ", &n) != STATUS_OK) { printf("input error\n"); return; }
    if (n < 0 || n > MAX_TASK2_N) { printf("bad n\n"); return; }

    double a[MAX_TASK2_N + 1];
    for (int i = 0; i <= n; i++) {
        char p[32];
        snprintf(p, sizeof(p), "a[%d]: ", i);
        if (read_double(p, &a[i]) != STATUS_OK) { printf("input error\n"); return; }
    }

    double result = 0.0;
    Status st;
    switch (n) {
        case 0: st = evaluate_poly(x, 0, &result, a[0]); break;
        case 1: st = evaluate_poly(x, 1, &result, a[0], a[1]); break;
        case 2: st = evaluate_poly(x, 2, &result, a[0], a[1], a[2]); break;
        case 3: st = evaluate_poly(x, 3, &result, a[0], a[1], a[2], a[3]); break;
        case 4: st = evaluate_poly(x, 4, &result, a[0], a[1], a[2], a[3], a[4]); break;
        case 5: st = evaluate_poly(x, 5, &result, a[0], a[1], a[2], a[3], a[4], a[5]); break;
        default: st = STATUS_ERR_INVALID_ARG; break;
    }
    if (st == STATUS_OK) printf("status: %s, P(%.6g) = %.10g\n", status_str(st), x, result);
    else                 printf("status: %s\n", status_str(st));
}

static void run_task3(void) {
    int base;
    if (read_int("base (2..36): ", &base) != STATUS_OK) { printf("input error\n"); return; }
    if (base < 2 || base > 36) { printf("bad base\n"); return; }

    int cnt;
    if (read_int("count (1..8): ", &cnt) != STATUS_OK) { printf("input error\n"); return; }
    if (cnt < 1 || cnt > MAX_TASK3_STR) { printf("bad count\n"); return; }

    char s[MAX_TASK3_STR][MAX_STR_LEN];
    for (int i = 0; i < cnt; i++) {
        char p[32];
        snprintf(p, sizeof(p), "num %d: ", i + 1);
        if (read_line(p, s[i], MAX_STR_LEN) != STATUS_OK) { printf("input error\n"); return; }
    }

    long long *res = NULL;
    size_t found = 0;
    Status st;
    switch (cnt) {
        case 1: st = find_kaprekar(base, &res, &found, s[0], NULL); break;
        case 2: st = find_kaprekar(base, &res, &found, s[0], s[1], NULL); break;
        case 3: st = find_kaprekar(base, &res, &found, s[0], s[1], s[2], NULL); break;
        case 4: st = find_kaprekar(base, &res, &found, s[0], s[1], s[2], s[3], NULL); break;
        case 5: st = find_kaprekar(base, &res, &found, s[0], s[1], s[2], s[3], s[4], NULL); break;
        case 6: st = find_kaprekar(base, &res, &found, s[0], s[1], s[2], s[3], s[4], s[5], NULL); break;
        case 7: st = find_kaprekar(base, &res, &found, s[0], s[1], s[2], s[3], s[4], s[5], s[6], NULL); break;
        case 8: st = find_kaprekar(base, &res, &found, s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7], NULL); break;
        default: st = STATUS_ERR_INVALID_ARG; break;
    }

    printf("status: %s, count=%zu\n", status_str(st), found);
    for (size_t i = 0; i < found; i++) printf("%lld\n", res[i]);
    free(res);
}

static void run_task4(void) {
    int n;
    if (read_int("n (1..6): ", &n) != STATUS_OK) { printf("input error\n"); return; }
    if (n < 1 || n > MAX_TASK4_N) { printf("bad n\n"); return; }

    double v[MAX_TASK4_N];
    for (int i = 0; i < n; i++) {
        char p[32];
        snprintf(p, sizeof(p), "x%d: ", i + 1);
        if (read_double(p, &v[i]) != STATUS_OK) { printf("input error\n"); return; }
    }

    double result = 0.0;
    Status st;
    switch (n) {
        case 1: st = geometric_mean(1, &result, v[0]); break;
        case 2: st = geometric_mean(2, &result, v[0], v[1]); break;
        case 3: st = geometric_mean(3, &result, v[0], v[1], v[2]); break;
        case 4: st = geometric_mean(4, &result, v[0], v[1], v[2], v[3]); break;
        case 5: st = geometric_mean(5, &result, v[0], v[1], v[2], v[3], v[4]); break;
        case 6: st = geometric_mean(6, &result, v[0], v[1], v[2], v[3], v[4], v[5]); break;
        default: st = STATUS_ERR_INVALID_ARG; break;
    }
    if (st == STATUS_OK) printf("status: %s, mean=%.10g\n", status_str(st), result);
    else                 printf("status: %s\n", status_str(st));
}

static void run_task5(void) {
    double base;
    if (read_double("base: ", &base) != STATUS_OK) { printf("input error\n"); return; }

    int exp;
    if (read_int("exp: ", &exp) != STATUS_OK) { printf("input error\n"); return; }

    double result = 0.0;
    Status st = fast_pow(base, exp, &result);
    if (st == STATUS_OK) printf("status: %s, result=%.10g\n", status_str(st), result);
    else                 printf("status: %s\n", status_str(st));
}

static void run_task6(void) {
    printf("f: 1) x^2-4   2) sin(x)-0.5   3) e^x-3   4) x^2+1\n");
    int choice;
    if (read_int("choice (1..4): ", &choice) != STATUS_OK) { printf("input error\n"); return; }

    MathFunc f = NULL;
    switch (choice) {
        case 1: f = f_sq_minus4; break;
        case 2: f = f_sin_minus_half; break;
        case 3: f = f_exp_minus3; break;
        case 4: f = f_no_root; break;
        default: printf("bad choice\n"); return;
    }

    double a, b, eps;
    if (read_double("a: ", &a) != STATUS_OK) { printf("input error\n"); return; }
    if (read_double("b: ", &b) != STATUS_OK) { printf("input error\n"); return; }
    if (read_double("eps: ", &eps) != STATUS_OK) { printf("input error\n"); return; }

    double root = 0.0;
    Status st = bisection(a, b, eps, f, &root);
    if (st == STATUS_OK) {
        printf("status: %s, root=%.10g, f(root)=%.3e\n", status_str(st), root, f(root));
    } else {
        printf("status: %s\n", status_str(st));
    }
}

int main(void) {
    int choice;
    for (;;) {
        printf("\n1-convex 2-poly 3-kaprekar 4-geomean 5-pow 6-bisect 0-exit\n");
        if (read_int("choice: ", &choice) != STATUS_OK) { printf("input error\n"); continue; }
        if (choice == 0) break;
        switch (choice) {
            case 1: run_task1(); break;
            case 2: run_task2(); break;
            case 3: run_task3(); break;
            case 4: run_task4(); break;
            case 5: run_task5(); break;
            case 6: run_task6(); break;
            default: printf("bad choice\n"); break;
        }
    }
    return 0;
}