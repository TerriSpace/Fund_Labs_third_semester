#include "functions.h"

int main(int argc, char *argv[])
{

    if (argc != 3) {
        printf("Неверное количество аргументов\n");
        return 1;
    }


    int number;
    int parse_res = strtoi(argv[1], &number);

    if (parse_res == 1) {
        printf("Нет числа после знака\n");
        return 2;
    } else if (parse_res == 2) {
        printf("Переполнение типа int\n");
        return 3;
    } else if (parse_res == 3) {
        printf("Недопустимый символ в числовом аргументе\n");
        return 10;
    } 
    
    char *flag = argv[2];
    if (flag[0] != '-' && flag[0] != '/') {
        printf("Флаг должен начинаться с '-' или '/'\n");
        return 9;
    }



    if (strcmp(flag, "-h") == 0 || strcmp(flag, "/h") == 0) {
        int result[MAX_MULTIPLES];
        int count = 0;

        int res = flag_h(number, result, &count);

        if (res == 1) {
            printf("Внутренняя ошибка: нулевой указатель\n");
            return 5;
        }

        if (count == 0) {
            printf("Нет натуральных чисел в пределах 100, кратных %d\n", number);
            return 4;
        }

        for (int i = 0; i < count; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        return 0;
    }


    else if(strcmp(flag, "-p") == 0 || strcmp(flag, "/p") == 0){
        int res = flag_p(number);
        if (res == 1) { printf("Составное\n"); return 0; }
        if (res == 2) { printf("Простое\n"); return 0; }
        if (res == 3) { printf("Простым или составным может быть только натуральное\n"); return 6; }
        if (res == 11) { printf("Единица - не простое\n"); return 11; }
    }


    else if(strcmp(flag, "-s") == 0 || strcmp(flag, "/s") == 0){
        char digits[MAX_HEX_DIGITS];
        int count = 0;
        int is_negative = 0;

        int res = flag_s(number, digits, &count, &is_negative);

        if (res == 1) {
            printf("Внутренняя ошибка: нулевой указатель\n");
            return 16;
        }
        if (res == 2) {
            printf("Переполнение буфера цифр\n");
            return 17;
        }

        if (is_negative) printf("- ");
        for (int j = count - 1; j >= 0; j--) {
            printf("%c ", digits[j]);
        }
        printf("\n");
        return 0;      
    }


     else if(strcmp(flag, "-e") == 0 || strcmp(flag, "/e") == 0){
        long long table[MAX_BASE][MAX_EXP];
        int res = flag_e(number, table);

        if (res == 1) {
            printf("Неверное число: x должен быть от 1 до 10\n");
            return 7;
        }
        if (res == 2) {
            printf("Переполнение при вычислении степеней\n");
            return 13;
        }

        for (int base = 1; base <= MAX_BASE; base++) {
            for (int exp = 1; exp <= number; exp++) {
                printf("%d^%d = %lld ", base, exp, table[base - 1][exp - 1]);
            }
            printf("\n");
        }
        return 0;

    }

     else if(strcmp(flag, "-a") == 0 || strcmp(flag, "/a") == 0){
        long long total;
        int res = flag_a(number, &total);

        if (res == 1) {
            printf("Число должно быть натуральным (x >= 1)\n");
            return 14;
        }
        if (res == 2) {
            printf("Внутренняя ошибка: нулевой указатель\n");
            return 15;
        }

        printf("%lld\n", total);
        return 0;
    }
// флаг f - доп ф-я factorial

    else if(strcmp(flag, "-f") == 0 || strcmp(flag, "/f") == 0){
        long long result;
        int res = flag_f(number, &result);

        if (res == 1){
            printf("Факториал отрицательного не определён\n");
            return 8;
        }

        if (res == 2){
            printf("Переполнение типа ll\n");
            return 12;
        }
        if (res == 3){
            printf("Внутренняя ошибка: нулевой указатель\n");
            return 18;
        }
    printf("%lld\n", result);
    return 0;
} 
    else {
        printf("Неверный флаг\n");
        return 9;
    }
    
}