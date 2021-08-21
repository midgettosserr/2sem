#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_str(FILE *file) {
    char *str = NULL;
    char buf[81];
    size_t size = 0;
    int res;
    do {
        res = fscanf(file, "%80[^\n]", buf);
        if (res == 0) {
            fscanf(file, "%*c"); 
            if (!str) {
                str = malloc(sizeof(char));
                str[0] = '\0';
            }
        } else if (res > 0) {
            size_t len = strlen(buf);
            str = realloc(str, (size+len+1)*sizeof(char));
            memcpy(str+size, buf, len);
            size += len;
            str[size] = '\0';
        }
    } while (res > 0);
    return str;
}

int getInt() {
    int number;
    int result;
    do {
        result = scanf("%d", &number);
        scanf("%*[^\n]s");
        scanf("%*c");
        if (result != 1) {
            printf("Ошибка! Повторите ввод\n");
        }
    } while (result != 1);
    return number;
}

float getFloat() {
    float number;
    int result;
    do {
        result = scanf("%f", &number);
        scanf("%*[^\n]s");
        scanf("%*c");
        if (result != 1) {
            printf("Ошибка! Повторите ввод\n");
        }
    } while (result != 1);
    return number;
}

unsigned int getUInt() {
    long number;
    int result;
    do {
        result = scanf("%ld", &number);
        scanf("%*[^\n]s");
        scanf("%*c");
        if (result != 1) {
            printf("Ошибка! Повторите ввод\n");
        }
        else if (number < 0) {
            printf("Число должно быть положительное, повторите ввод\n");
            result = 0;
        }
    } while (result != 1);
    return (unsigned int)number;
}
