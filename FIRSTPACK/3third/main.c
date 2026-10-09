#include "functions.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Использование: %s <флаг> <аргументы>\n", argv[0]);
        return 1;
    }

    const char *flag = argv[1];

    if (strcmp(flag, "-q") == 0 || strcmp(flag, "/q") == 0) {
        if (argc != 6) {
            printf("Ошибка: для флага -q нужно 4 аргумента\n");
            return 1;
        }

        double epsilon;
        if (strtodouble(argv[2], &epsilon) != STATUS_OK) {
            printf("Ошибка: некорректный эпсилон\n");
            return 1;
        }

        double nums[3];
        for (int i = 0; i < 3; i++) {
            if (strtodouble(argv[3 + i], &nums[i]) != STATUS_OK) {
                printf("Ошибка: некорректный коэффициент\n");
                return 1;
            }
        }

        double seen[6][3];
        int seen_count = 0;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (i == j || i == k || j == k) continue;

                    double a = nums[i];
                    double b = nums[j];
                    double c = nums[k];

                    if (already_was(seen, seen_count, a, b, c, epsilon) == 0) {
                        seen[seen_count][0] = a;
                        seen[seen_count][1] = b;
                        seen[seen_count][2] = c;
                        seen_count++;

                        printf("Уравнение: %gx^2 + %gx + %g = 0\n", a, b, c);

                        double x1 = 0.0, x2 = 0.0;
                        Status result = solve_quadratic(a, b, c, epsilon, &x1, &x2);

                        switch (result) {
                            case SOL_INFINITE:
                                printf("  Бесконечно много решений\n");
                                break;
                            case SOL_NONE:
                                printf("  Нет действительных корней\n");
                                break;
                            case SOL_ONE_ROOT:
                                printf("  x = %.6f\n", x1);
                                break;
                            case SOL_TWO_ROOTS:
                                printf("  x1 = %.6f, x2 = %.6f\n", x1, x2);
                                break;
                            default:
                                printf("  Неизвестный результат\n");
                                return 1;
                        }
                        printf("\n");
                    }
                }
            }
        }
    }
    else if (strcmp(flag, "-m") == 0 || strcmp(flag, "/m") == 0) {
        if (argc != 4) {
            printf("Ошибка: для флага -m нужно 2 аргумента\n");
            return 1;
        }

        int first, second;
        Status st1 = strtoi(argv[2], &first);
        Status st2 = strtoi(argv[3], &second);

        if (st1 != STATUS_OK || st2 != STATUS_OK) {
            printf("Ошибка: некорректные целые числа\n");
            return 1;
        }

        Status result = flag_m(first, second);

        switch (result) {
            case RES_MULTIPLE:
                printf("Первое число кратно второму\n");
                break;
            case RES_NOT_MULTIPLE:
                printf("Первое число не кратно второму\n");
                break;
            case ERR_INVALID_INPUT:
                printf("Ошибка: числа должны быть ненулевыми\n");
                return 1;
            default:
                printf("Неизвестный результат\n");
                return 1;
        }
    }
    else if (strcmp(flag, "-t") == 0 || strcmp(flag, "/t") == 0) {
        if (argc != 6) {
            printf("Ошибка: для флага -t нужно 4 аргумента\n");
            return 1;
        }

        double epsilon, a, b, c;
        if (strtodouble(argv[2], &epsilon) != STATUS_OK ||
            strtodouble(argv[3], &a) != STATUS_OK ||
            strtodouble(argv[4], &b) != STATUS_OK ||
            strtodouble(argv[5], &c) != STATUS_OK) {
            printf("Ошибка: некорректные вещественные числа\n");
            return 1;
        }

        Status result = flag_t(epsilon, a, b, c);

        switch (result) {
            case RES_RIGHT_TRIANGLE:
                printf("Треугольник прямоугольный\n");
                break;
            case RES_NOT_RIGHT_TRIANGLE:
                printf("Треугольник не прямоугольный\n");
                break;
            case RES_NOT_TRIANGLE:
                printf("Числа не являются сторонами треугольника\n");
                break;
            default:
                printf("Неизвестный результат\n");
                return 1;
        }
    }      
    else {
        printf("Неизвестный флаг: %s\n", flag);
        return 1;
    }

    return 0;
}