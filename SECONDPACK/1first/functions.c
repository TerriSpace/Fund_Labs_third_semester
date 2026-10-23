#include "functions.h"


size_t my_strlen(const char *str) {
    size_t len = 0;
    if (str == NULL) return 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char *my_strdup(const char *src, Status *status) {
    if (src == NULL) {
        if (status) *status = STATUS_ERR_EMPTY_STRING;
        return NULL;
    }

    size_t len = my_strlen(src);
    char *copy = (char *)malloc((len + 1) * sizeof(char));

    if (copy == NULL) {
        if (status) *status = STATUS_ERR_MEMORY;
        return NULL;
    }

    for (size_t i = 0; i <= len; i++) {
        copy[i] = src[i];
    }

    if (status) *status = STATUS_SUCCESS;
    return copy;
}

bool is_ascii(const char *str) {
    if (str == NULL) return false;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if ((unsigned char)str[i] > 127) {
            return false;
        }
    }
    return true;
}

bool is_digit(char c) {
    return (c >= '0' && c <= '9');
}

bool is_alpha(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

bool is_upper(char c) {
    return (c >= 'A' && c <= 'Z');
}

bool is_lower(char c) {
    return (c >= 'a' && c <= 'z');
}

char to_upper(char c) {
    if (is_lower(c)) {
        return (char)(c - 32);
    }
    return c;
}


char *process_l(const char *str, Status *status) {
    if (str == NULL) {
        if (status) *status = STATUS_ERR_EMPTY_STRING;
        return NULL;
    }

    size_t len = my_strlen(str);

    size_t temp = len;
    int digit_count = 0;

    do {
        digit_count++;
        temp /= 10;
    } while (temp > 0);

    char *result = (char *)malloc((digit_count + 1) * sizeof(char));
    if (result == NULL) {
        if (status) *status = STATUS_ERR_MEMORY;
        return NULL;
    }

    result[digit_count] = '\0';

    temp = len;
    for (int i = digit_count - 1; i >= 0; i--) {
        result[i] = (char)('0' + (temp % 10));
        temp /= 10;
    }

    if (status) *status = STATUS_SUCCESS;
    return result;
}

char *process_r(const char *str, Status *status) {
    if (str == NULL) {
        if (status) *status = STATUS_ERR_EMPTY_STRING;
        return NULL;
    }

    size_t len = my_strlen(str);

    char *result = my_strdup(str, status);
    if (result == NULL) {
        return NULL;
    }

    if (len < 2) {
        if (status) *status = STATUS_SUCCESS;
        return result;
    }

    size_t i = 0;
    size_t j = len - 1;
    char temp;

    while (i < j) {
        temp = result[i];
        result[i] = result[j];
        result[j] = temp;
        i++;
        j--;
    }

    if (status) *status = STATUS_SUCCESS;
    return result;
}

char *process_u(const char *str, Status *status) {
    if (str == NULL) {
        if (status) *status = STATUS_ERR_EMPTY_STRING;
        return NULL;
    }

    char *result = my_strdup(str, status);
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 1; result[i] != '\0'; i += 2) {
        result[i] = to_upper(result[i]);
    }

    if (status) *status = STATUS_SUCCESS;
    return result;
}

char *process_n(const char *str, Status *status) {
    if (str == NULL) {
        if (status) *status = STATUS_ERR_EMPTY_STRING;
        return NULL;
    }

    size_t len = my_strlen(str);
    char *result = (char *)malloc((len + 1) * sizeof(char));
    if (result == NULL) {
        if (status) *status = STATUS_ERR_MEMORY;
        return NULL;
    }

    size_t out = 0;

    for (size_t i = 0; i < len; i++) {
        if (is_digit(str[i])) {
            result[out++] = str[i];
        }
    }

    for (size_t i = 0; i < len; i++) {
        if (is_alpha(str[i])) {
            result[out++] = str[i];
        }
    }

    for (size_t i = 0; i < len; i++) {
        if (!is_digit(str[i]) && !is_alpha(str[i])) {
            result[out++] = str[i];
        }
    }

    result[out] = '\0';

    if (status) *status = STATUS_SUCCESS;
    return result;
}

char *process_c(int argc, char **argv, int start_index, unsigned int seed, Status *status) {
    if (argc <= start_index) {
        if (status) *status = STATUS_ERR_INVALID_ARGC;
        return NULL;
    }

    int count = 0;
    for (int i = start_index; i < argc; i++) {
        if (i == 3) continue;
        count++;
    }

    if (count == 0) {
        if (status) *status = STATUS_ERR_EMPTY_STRING;
        return NULL;
    }

    char **strings = (char **)malloc(count * sizeof(char *));
    if (strings == NULL) {
        if (status) *status = STATUS_ERR_MEMORY;
        return NULL;
    }

    int idx = 0;
    for (int i = start_index; i < argc; i++) {
        if (i == 3) continue;
        strings[idx++] = argv[i];
    }

    srand(seed);
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char *temp = strings[i];
        strings[i] = strings[j];
        strings[j] = temp;
    }

    size_t total_len = 0;
    for (int i = 0; i < count; i++) {
        total_len += my_strlen(strings[i]);
    }

    char *result = (char *)malloc((total_len + 1) * sizeof(char));
    if (result == NULL) {
        free(strings);
        if (status) *status = STATUS_ERR_MEMORY;
        return NULL;
    }

    size_t pos = 0;
    for (int i = 0; i < count; i++) {
        size_t len = my_strlen(strings[i]);
        for (size_t k = 0; k < len; k++) {
            result[pos++] = strings[i][k];
        }
    }
    result[pos] = '\0';

    free(strings);

    if (status) *status = STATUS_SUCCESS;
    return result;
}