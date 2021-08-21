#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "common.h"
#include "llrb.h"

int main() {
    Node *root = NULL;
    unsigned int key, key1, key2, n;
    char *str1 = NULL;
    float x;
    Info *info;
    FILE *file;
    Node *node; 
    int menu;
    do {
        printf("1 - ввод таблицы из файла\n");
        printf("2 - включение нового элемента в таблицу\n");
        printf("3 - удаление элемента из таблицы\n");
        printf("4 - поиск информации по ключу\n");
        printf("5 - вывод в диапазоне\n");
        printf("6 - вывод всей таблицы\n");
        printf("7 - поиск элемента, наиболее близкого к заданному\n");
        printf("8 - вывод ключей в виде дерева\n");
        printf("9 - таймирование операций\n");
        printf("10 - graphviz\n");
        printf("0 - выход\n");
        menu = getInt();
        switch (menu) {
            case 1:
                printf("Введите имя файла: ");
                str1 = get_str(stdin);
                file = fopen(str1, "rt");
                if (file) {
                    root = deleteTree(root);
                    root = inputFromFile(file);
                    fclose(file);
                }
                else {
                    printf("Не удалось открыть файл\n");
                }
                break;
            case 2:
                printf("Введите ключ: ");
                key = getUInt();
                printf("Введите информацию:\n");
                printf("Вещественное число: ");
                x = getFloat();
                printf("Строка: ");
                str1 = get_str(stdin);
                info = makeInfo(str1, x);
                root = insert(root, key, info);
                root->c = 'b';
                printf("Элемент добавлен\n");
                break;
            case 3:
                printf("Введите ключ: ");
                key = getUInt();
                root = removeNode(root, key);
                if (root) {
                    root->c = 'b';
                }
                printf("Элемент удален\n");
                break;
            case 4:
                printf("Введите ключ: ");
                key = getUInt();
                info = find(root, key);
                if (info) {
                    printf("Найдено:\n");
                    printInfo(info);
                }
                else {
                    printf("Не найдено\n");
                }
                break;
            case 5:
                printf("Введите границы диапазона:\n");
                key1 = getUInt();
                key2 = getUInt();
                printTree(root, key1, key2);
                break;
            case 6:
                printTree3(root);
                break;
            case 7:
                printf("Введите ключ: ");
                key = getUInt();
                node = NULL;
                closestKey(root, key, &node);
                if (node) {
                    printf("Наиболее близкий ключ %u:\n", node->key);
                    printInfo(node->info);
                }
                else {
                    printf("Не найдено\n");
                }
                break;
            case 8:
                print(root, 0);
                break;
            case 9:
                printf("Введите количество узлов: ");
                n = getUInt();
                test(n);
                break;
            case 10:
                printf("\ndigraph G {\n");
                DOT(root);
                printf("}\n");
            case 0:
                break;
            default:
                printf("Неверно выбран пункт меню\n");
                break;
        }
        free(str1);
        str1 = NULL;
    } while (menu != 0);
    deleteTree(root);
    return 0;
}
