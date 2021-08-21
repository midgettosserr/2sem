#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

typedef struct Key{
    KeyType1 key1;
    KeyType2 key2;
} Key;

int getPositiveInt() {      //функция ввода целого положительного числа
    int a;
    do {
        char *str = get_str();
        a = atoi(str);
        if (a <= 0) {
            printf("Введите целое положительное число\n");
        }
        free(str);
    } while (a <= 0);
    return a;
}

int menu() {
    printf("1 - добавить элемент в таблицу\n");
    printf("2 - удалить элемент из таблицы\n");
    printf("3 - найти элемент в таблице\n");
    printf("4 - найти элементы по первому ключу\n");
    printf("5 - найти элементы по второму ключу\n");
    printf("6 - вывод таблицы (упорядоченной по первому ключу)\n");
    printf("7 - вывод таблицы (упорядоченной по второму ключу)\n");
    printf("8 - удаление элементов по первому ключу\n");
    printf("9 - удаление элементов по второму ключу\n");
    printf("Другие операции:\n");
    printf("11 - удаление элемента по первому ключу и номеру версии\n");
    printf("12 - чистка таблицы по второму ключу\n");
    printf("13 - получение новой таблицы по первому ключу\n");
    printf("14 - поиск по первому ключу и номеру версии\n");
    printf("15 - получение новой таблицы по второму ключу\n");
    printf("16 - поиск по второму ключу и номеру версии\n");
    printf("99 - выход\n");
    printf("> ");
    return getPositiveInt();
}

Key inputKey() {
    Key key;
    printf("Введите первый ключ: ");
    key.key1 = getPositiveInt();
    printf("Введите второй ключ: ");
    key.key2 = getPositiveInt();
    return key;
}

int main() {
    KeyType1 key1;
    RelType1 rel1;
    KeyType2 key2;
    RelType2 rel2;
    Key key;
    InfoType *info;
    Item *item;
    Node1 *it1;
    KeySpace2 *it2;
    KeySpace1 *ks1;
    KeySpace2 **ks2;
    
    Table *table = initTable(10);
    int m;
    
    do {
        m = menu();
        switch (m) {
            case 1:
                key = inputKey();
                printf("Введите информацию: ");
                info = inputInfo();
                item = makeItem(info, key.key1, key.key2);
                freeInfo(info);
                if (!addElement(table, item)) {
                    printf("Не удалось добавить элемент: составной ключ (%ld, %ld) не является уникальным\n", key.key1, key.key2);
                    freeInfo(item->info);
                    free(item);
                }
                else {
                    printf("Элемент успешно добавлен в таблицу\n");
                }
                break;
            case 2:
                key = inputKey();
                if (!removeElement(table, key.key1, key.key2)) {
                    printf("Не удалось удалить элемент: составной ключ (%ld, %ld) не найден\n", key.key1, key.key2);
                }
                else {
                    printf("Элемент успешно удален\n");
                }
                break;
            case 3:
                key = inputKey();
                item = findElement(table, key.key1, key.key2);
                if (item) {
                    printf("Надено: ");
                    printInfo(item->info);
                    printf("\n");
                }
                else {
                    printf("Элемент не найден\n");
                }
                break;
            case 4:
                printf("Введите ключ: ");
                key1 = getPositiveInt();
                it1 = getNext1(table, key1, NULL);
                if (!it1) {
                    printf("Ни одного элемента не найдено\n");
                }
                else {
                    printf("Найдены элементы:\n");
                    while (it1) {
                        printInfo(it1->info->info);
                        printf("\n");
                        it1 = getNext1(table, key1, it1);
                    }
                }
                break;
            case 5:
                printf("Введите ключ: ");
                key2 = getPositiveInt();
                it2 = getNext2(table, key2, NULL);
                if (!it2) {
                    printf("Ни одного элемента не найдено\n");
                }
                else {
                    printf("Найдены элементы:\n");
                    while (it2) {
                        printInfo(it2->info->info);
                        printf("\n");
                        it2 = getNext2(table, key2, it2);
                    }
                }
                break;
            case 6:
                printTable(table, 1);
                break;
            case 7:
                printTable(table, 2);
                break;
            case 8:
                printf("Введите ключ: ");
                key1 = getPositiveInt();
                removeAll1(table, key1);
                break;
            case 9:
                printf("Введите ключ: ");
                key2 = getPositiveInt();
                removeAll2(table, key2);
                break;
                
            case 11:
                printf("Введите ключ: ");
                key1 = getPositiveInt();
                printf("Введите номер версии: ");
                rel1 = getPositiveInt();
                if (!removeKeyRel1(table, key1, rel1)) {
                    printf("Элемент не найден\n");
                }
                else {
                    printf("Элемент успешно удален\n");
                }
                break;
            case 12:
                cleanTableKs2(table);
                break;
                
            case 13:
                printf("Введите ключ: ");
                key1 = getPositiveInt();
                ks1 = getKey1(table->ks1, key1);
                printKS1(ks1);
                removeKS1(ks1);
                break;
            case 14:
                printf("Введите ключ: ");
                key1 = getPositiveInt();
                printf("Введите номер версии: ");
                rel1 = getPositiveInt();
                ks1 = getKeyRel1(table->ks1, key1, rel1);
                printKS1(ks1);
                removeKS1(ks1);
                break;
            case 15:
                printf("Введите ключ: ");
                key2 = getPositiveInt();
                ks2 = getKey2(table->ks2, key2, table->msize2);
                printKS2(ks2, table->msize2);
                removeKS2(ks2, table->msize2);
                break;
            case 16:
                printf("Введите ключ: ");
                key2 = getPositiveInt();
                printf("Введите номер версии: ");
                rel2 = getPositiveInt();
                ks2 = getKeyRel2(table->ks2, key2, rel2, table->msize2);
                printKS2(ks2, table->msize2);
                removeKS2(ks2, table->msize2);
                break; 
            case 99:
                break;
            default:
                printf("Неверно выбран пункт меню\n");
                break;
        }
    } while (m != 99);
    removeTable(table);
    return 0;
}
