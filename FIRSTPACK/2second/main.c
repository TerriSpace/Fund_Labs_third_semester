#include "functions.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Использование: %s <eps>\n", argv[0]);
        return 1;
    }

    double eps;
    short int status = strtodouble(argv[1], &eps);

    switch (status) {
        case 0: break;
        case 1: printf("Ошибка: пустая строка после знака\n"); return 1;
        case 3: printf("Ошибка: '%s' не является числом\n", argv[1]); return 1;
        default: printf("Ошибка парсинга: неизвестный код %d\n", status); return 1;
    }

    if (isnan(eps) || eps <= 0.0) {
        printf("Ошибка: eps должно быть > 0\n");
        return 1;
    }

    double r = 0.0;
    long double rl = 0.0L;
    int s;

    // e
    s = e_lim(eps, &r);          
    print_result("e-lim", s, r);  r = 0.0;
    s = e_sum(eps, &r);          
    print_result("e-sum", s, r);  r = 0.0;
    s = e_equation(eps, &r);     
    print_result("e-equation", s, r);  r = 0.0;

    // pi
    s = pi_lim(eps, &r);         
    print_result("pi-lim", s, r);  r = 0.0;
    s = pi_sum(eps, &r);         
    print_result("pi-sum", s, r);  r = 0.0;
    s = pi_equation(eps, &r);    
    print_result("pi-equation", s, r);  r = 0.0;

    // ln 2
    s = ln2_lim(eps, &r);        
    print_result("ln2-lim", s, r);  r = 0.0;
    s = ln2_sum(eps, &r);        
    print_result("ln2-sum", s, r);  r = 0.0;
    s = ln2_equation(eps, &r);   
    print_result("ln2-equation", s, r);  r = 0.0;

    // sqrt 2
    s = sqrt2_lim(eps, &r);      
    print_result("sqrt2-lim", s, r);  r = 0.0;
    s = sqrt2_product(eps, &r);  
    print_result("sqrt2-product", s, r);  r = 0.0;
    s = sqrt2_equation(eps, &r); 
    print_result("sqrt2-equation", s, r); r = 0.0;

    // gamma
    s = gamma_lim(eps, &rl);      
    print_resultl("gamma-lim", s, rl);  rl = 0.0;
    s = gamma_sum(eps, &rl);      
    print_resultl("gamma-sum", s, rl);  rl = 0.0;
    s = gamma_equation(eps, &rl); 
    print_resultl("gamma-equation", s, rl);

    return 0;
}