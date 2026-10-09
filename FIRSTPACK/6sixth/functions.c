#include "functions.h"
#define POLY_MAX_DEGREE 1000

Status is_convex(int n, double eps, bool *result, ...) {
    if (n < 3 || eps <= 0.0 || result == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }

    if (!isfinite(eps) || eps <= 0.0) {
        return STATUS_ERR_INVALID_ARG;
    }

    typedef struct { double x, y; } Point;
    Point *points = (Point *)malloc((size_t)n * sizeof(Point));
    if (points == NULL) {
        return STATUS_ERR_MEMORY;
    }

    va_list args;
    va_start(args, result);
    for (int i = 0; i < n; i++) {
        points[i].x = va_arg(args, double);
        points[i].y = va_arg(args, double);
        if (!isfinite(points[i].x) || !isfinite(points[i].y)) {  // ← новое
            va_end(args);
            free(points);
            return STATUS_ERR_INVALID_ARG;
        }
    }
    va_end(args);

    bool has_pos = false;
    bool has_neg = false;

    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;
        int next2 = (i + 2) % n;

        double x1 = points[i].x;
        double y1 = points[i].y;
        double x2 = points[next].x;
        double y2 = points[next].y;
        double x3 = points[next2].x;
        double y3 = points[next2].y;

        double dx1 = x2 - x1;
        double dy1 = y2 - y1;
        double dx2 = x3 - x2;
        double dy2 = y3 - y2;

        double cross = dx1 * dy2 - dy1 * dx2;

        if (!isfinite(cross)) {
            return STATUS_ERR_DOMAIN;
        }

        if (cross > eps) {
            has_pos = true;
        } else if (cross < -eps) {
            has_neg = true;
        }

        if (has_pos && has_neg) {
            free(points);
            *result = false;
            return STATUS_OK;
        }
    }

    free(points);
    *result = true;
    return STATUS_OK;
}

Status evaluate_poly(double x, int n, double *result, ...) {
    if (result == NULL || isnan(x)) {
        return STATUS_ERR_INVALID_ARG;
    }

    if (n < 0 || n > POLY_MAX_DEGREE) { 
        return STATUS_ERR_INVALID_ARG;
    }

    if (!isfinite(x)) {
        return STATUS_ERR_INVALID_ARG;
    }

    va_list args;
    va_start(args, result);

    double acc = 0.0;
    Status status = STATUS_OK;
    for (long long i = 0; i <= (long long)n; i++) {
        double coeff = va_arg(args, double);
        if (!isfinite(coeff)) {
            status = STATUS_ERR_INVALID_ARG;
            break;
        }
        acc = acc * x + coeff;
        if (!isfinite(acc)) {
            status = STATUS_ERR_DOMAIN;
            break;
        }
    }

    va_end(args);

    if (isnan(acc)) {
        return STATUS_ERR_DOMAIN;
    }

    *result = acc;
    return STATUS_OK;
}

static int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

static Status parse_in_base(const char *str, int base, long long *out) {
    if (str == NULL || out == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (*str == '\0') {
        return STATUS_ERR_INVALID_STRING;
    }

    long long value = 0;
    for (const char *p = str; *p != '\0'; p++) {
        int digit = char_to_digit(*p);
        if (digit < 0 || digit >= base) {
            return STATUS_ERR_INVALID_STRING;
        }
        if (value > (LLONG_MAX - digit) / base) {
            return STATUS_ERR_DOMAIN;
        }
        value = value * base + digit;
    }
    *out = value;
    return STATUS_OK;
}

static bool is_kaprekar(long long n, int base) {
    if (n <= 0) return false;
    if (n > 3037000499LL) return false;
    long long sq = n * n;

    long long divisor = 1;
    long long temp = n;
    while (temp > 0) {
        if (divisor > LLONG_MAX / base) {
            return false;
        }
        divisor *= base;
        temp /= base;
    }

    long long right = sq % divisor;
    long long left  = sq / divisor;

    return (left + right) == n;
}

Status find_kaprekar(int base, long long **results, size_t *count, ...) {
    if (results == NULL || count == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (base < 2 || base > 36) {
        return STATUS_ERR_INVALID_BASE;
    }

    *results = NULL;
    *count = 0;

    va_list args;
    va_start(args, count);

    size_t capacity = 4;
    long long *arr = (long long *)malloc(capacity * sizeof(long long));
    if (arr == NULL) {
        va_end(args);
        return STATUS_ERR_MEMORY;
    }

    const char *str;
    Status status = STATUS_OK;

    while ((str = va_arg(args, const char *)) != NULL) {
        long long n;
        Status ps = parse_in_base(str, base, &n);
        if (ps != STATUS_OK) {
            status = ps;
            break;
        }

        if (is_kaprekar(n, base)) {
            if (*count >= capacity) {
                size_t new_cap = capacity * 2;
                long long *tmp = (long long *)realloc(arr, new_cap * sizeof(long long));
                if (tmp == NULL) {
                    status = STATUS_ERR_MEMORY;
                    break;
                }
                arr = tmp;
                capacity = new_cap;
            }
            arr[*count] = n;
            (*count)++;
        }
    }

    va_end(args);

    if (status != STATUS_OK) {
        free(arr);
        *results = NULL;
        *count = 0;
        return status;
    }

    if (*count == 0) {
        free(arr);
        return STATUS_OK;
    }

    long long *tmp = (long long *)realloc(arr, (*count) * sizeof(long long));
    if (tmp != NULL) {
        arr = tmp;
    }

    *results = arr;
    return STATUS_OK;
}

Status geometric_mean(int n, double *result, ...) {
    if (n <= 0 || result == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }

    va_list args;
    va_start(args, result);

    double product = 1.0;
    bool has_zero = false;
    Status status = STATUS_OK;

    for (int i = 0; i < n; i++) {
        double value = va_arg(args, double);

        if (isnan(value)) {
            status = STATUS_ERR_INVALID_ARG;
            break;
        }
        if (isinf(value)) {
            status = STATUS_ERR_DOMAIN;
            break;
        }
        if (value < 0.0) {
            status = STATUS_ERR_DOMAIN;
            break;
        }
        if (value == 0.0) {
            has_zero = true;
        }
        product *= value;
    }

    va_end(args);

    if (status != STATUS_OK) {
        return status;
    }

    if (has_zero) {
        *result = 0.0;
        return STATUS_OK;
    }

    if (product == 0.0 || isinf(product)) {
        return STATUS_ERR_DOMAIN;
    }

    double mean = pow(product, 1.0 / (double)n);

    if (isnan(mean) || isinf(mean)) {
        return STATUS_ERR_DOMAIN;
    }

    *result = mean;
    return STATUS_OK;
}

static double pow_recursive(double base, long long exp) {
    if (exp == 0) {
        return 1.0;
    }
    if (exp < 0) {
        return 1.0 / pow_recursive(base, -exp);
    }
    if (exp % 2 == 0) {
        double half = pow_recursive(base, exp / 2);
        return half * half;
    }
    return base * pow_recursive(base, exp - 1);
}

Status fast_pow(double base, int exp, double *result) {
    if (result == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (isnan(base) || isinf(base)) {
        return STATUS_ERR_DOMAIN;
    }
    if (base == 0.0 && exp < 0) {
        return STATUS_ERR_DOMAIN;
    }

    double value = pow_recursive(base, (long long)exp);

    if (isnan(value) || isinf(value)) {
        return STATUS_ERR_DOMAIN;
    }

    *result = value;
    return STATUS_OK;
}

Status bisection(double a, double b, double eps, MathFunc f, double *root) {
    if (f == NULL || root == NULL) {
        return STATUS_ERR_INVALID_ARG;
    }
    if (!isfinite(a) || !isfinite(b)) { 
        return STATUS_ERR_INVALID_ARG;
    }
    if (!isfinite(eps) || eps <= 0.0) {
        return STATUS_ERR_INVALID_ARG;
    }

    if (a >= b) {
        return STATUS_ERR_INVALID_ARG;
    }

    double fa = f(a);
    double fb = f(b);

    if (!isfinite(fa) || !isfinite(fb)){
        return STATUS_ERR_DOMAIN;
    }

    if (fabs(fa) < eps) {
        *root = a;
        return STATUS_OK;
    }
    if (fabs(fb) < eps) {
        *root = b;
        return STATUS_OK;
    }

    if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0)) {
        return STATUS_ERR_NO_ROOT;
    }

    double left = a;
    double right = b;
    double mid = a;
    int max_iter = 1000;

    for (int i = 0; i < max_iter; i++) {
        mid = left + (right - left) / 2.0;
        double fmid = f(mid);

        if (isnan(fmid) || isinf(fmid)) {
            return STATUS_ERR_DOMAIN;
        }

        if (fabs(fmid) < eps || (right - left) / 2.0 < eps) {
            *root = mid;
            return STATUS_OK;
        }

        if ((fa > 0.0 && fmid < 0.0) || (fa < 0.0 && fmid > 0.0)) {
            right = mid;
            fb = fmid;
        } else {
            left = mid;
            fa = fmid;
        }
    }

    *root = mid;
    return STATUS_OK;
}