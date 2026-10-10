#include "functions.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Использование: %s <eps>\n", argv[0]);
        return 1;
    }

    double eps;
    int status = strtodouble(argv[1], &eps);

    switch (status) {
        case 0:
            break;
        case 3:
            printf("Ошибка: NaN/Inf/переполнение ввода\n");
            return 1;
        case 4:
            printf("Ошибка: внутренняя ошибка (NULL-указатель)\n");
            return 1;
        case 5:
            printf("Ошибка: не является числом\n");
            return 1;
        default:
            printf("Ошибка парсинга: неизвестный код %d\n", status);
            return 1;
    }

    if (isnan(eps) || eps <= 0.0 || eps >= 1.0) {
        printf("Ошибка: eps должно быть в диапазоне (0; 1)\n");
        return 1;
    }

    double r = 0.0;
    long double rl = 0.0L;
    int s;

    s = e_lim(eps, &r);
    print_result("e-lim", s, r);
    s = e_sum(eps, &r);
    print_result("e-sum", s, r);
    s = e_equation(eps, &r);
    print_result("e-equation", s, r);

    s = pi_lim(eps, &r);
    print_result("pi-lim", s, r);
    s = pi_sum(eps, &r);
    print_result("pi-sum", s, r);
    s = pi_equation(eps, &r);
    print_result("pi-equation", s, r);

    s = ln2_lim(eps, &r);
    print_result("ln2-lim", s, r);
    s = ln2_sum(eps, &r);
    print_result("ln2-sum", s, r);
    s = ln2_equation(eps, &r);
    print_result("ln2-equation", s, r);

    s = sqrt2_lim(eps, &r);
    print_result("sqrt2-lim", s, r);
    s = sqrt2_product(eps, &r);
    print_result("sqrt2-product", s, r);
    s = sqrt2_equation(eps, &r);
    print_result("sqrt2-equation", s, r);

    s = gamma_lim((long double)eps, &rl);
    print_resultl("gamma-lim", s, rl);
    s = gamma_sum((long double)eps, &rl);
    print_resultl("gamma-sum", s, rl);
    s = gamma_equation((long double)eps, &rl);
    print_resultl("gamma-equation", s, rl);

    return 0;
}