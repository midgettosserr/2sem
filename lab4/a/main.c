#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tree.h"
#include "utility.h"

int main() {
    Node *root = NULL;
    char *str1 = NULL, *str2 = NULL;
    float x, y;
    Info *info;
    FILE *file;
    Node *node;
    unsigned int n;
    int menu;
    do {
        printf("1 - ввод таблицы из файла\n");
        printf("2 - включение нового элемента в таблицу\n");
        printf("3 - удаление элемента из таблицы\n");
        printf("4 - поиск информации по ключу\n");
        printf("5 - вывод в диапазоне\n");
        printf("6 - поиск наиболее отличающегося элемента\n");
        printf("7 - вывод ключей в виде дерева\n");
        printf("8 - таймирование операций\n");
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
                str1 = get_str(stdin);
                printf("Введите информацию:\n");
                printf("Вещественное число: ");
                x = getFloat();
                printf("Вещественное число: ");
                y = getFloat();
                printf("Строка: ");
                str2 = get_str(stdin);
                info = makeInfo(str2, x, y);
                root = insert(root, str1, info);
                printf("Элемент добавлен\n");
                break;
            case 3: // удаление элемента
                printf("Введите ключ: ");
                str1 = get_str(stdin);
                root = removeNode(root, str1);
                printf("Элемент удален\n");
                break;
            case 4:
                printf("Введите ключ: ");
                str1 = get_str(stdin);
                info = find(root, str1);
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
                printf("Строка a: ");
                str1 = get_str(stdin);
                printf("Строка b: ");
                str2 = get_str(stdin);
                if (strcmp(str1, str2) <= 0) {
                    printTree(root, str1, str2);
                }
                else {
                    printTreeInverse(root, str2, str1);
                }
                break;
            case 6:
                printf("Введите ключ: ");
                str1 = get_str(stdin);
                node = findMostDiff(root, str1);
                if (node) {
                    printf("Наиболее отличающийся ключ %s:\n", node->key);
                    printInfo(node->info);
                }
                else {
                    printf("Не найдено\n");
                }
                break;
            case 7:
                print(root, 0);
                break;
            case 8:
                printf("Введите количество узлов: ");
                n = getInt();
                test(n);
                break;
            case 0:
                break;
            default:
                printf("Неверно выбран пункт меню\n");
                break;
        }
	free(str1);
	free(str2);
	str1 = str2 = NULL;
    } while (menu != 0);
    deleteTree(root);
    return 0;
}
