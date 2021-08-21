#include "table.h"

char *get_str() {                     //ввод строки произвольной длины
        char buf[81] = {0};
        char *str = NULL;
        int len = 0;
        int n = 0;
        do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
                if (!str) {
                return NULL;
                }
        } else if (n > 0) {
                int chunk_len = strlen(buf);
                int str_len = len + chunk_len;
                str = realloc(str, str_len + 1);
                memcpy(str + len, buf, chunk_len);
                len = str_len;
        } else {
                scanf("%*c");
        }

        } while (n > 0);
        if (len > 0) {
        str[len] = '\0';
        } else {
        str = calloc(1, sizeof(char));
        }
        return str;
}

InfoType* inputInfo() {
    InfoType *info = (InfoType*)malloc(sizeof(InfoType));
    *info = get_str();
    return info;
}

InfoType* copyInfo(InfoType *info) {
    InfoType *cpy = (InfoType*)malloc(sizeof(InfoType));
    *cpy = (InfoType)malloc(strlen(*info)+1);
    strcpy(*cpy, *info);
    return cpy;
}

void printInfo(InfoType *info) {
    printf("%s", *info);
}

void freeInfo(InfoType *info) {
    if (info)
        free(*info);
    free(info);
}

Item* makeItem(InfoType *info, KeyType1 key1, KeyType2 key2) {      //создание элемента таблицы
    Item *item = (Item*)malloc(sizeof(Item));
    item->info = copyInfo(info);
    item->key1 = key1;
    item->key2 = key2;
    return item;
}

// первое пространство ключей

Node1* findKeyKS1(KeySpace1 *ks1, KeyType1 key) {     //поиск подсписка с заданным ключом
    while (ks1) {
        if (ks1->key == key)
            return ks1->node;
        ks1 = ks1->link;
    }
    return NULL;
}

void addElementKS1(KeySpace1 **ks1, Item *item) {     //добавление элемента
    Node1 *node1 = (Node1*)malloc(sizeof(Node1));
    node1->info = item;
    node1->next = NULL;
    Node1 *node = findKeyKS1(*ks1, item->key1);
    if (node) {
        while (node->next) {
            node = node->next;
        }
        node->next = node1;
        node1->release = node->release + 1;
    }
    else {
        KeySpace1 *ks = (KeySpace1*)malloc(sizeof(KeySpace1));
        ks->key = item->key1;
        ks->node = node1;
        ks->node->release = 1;
        ks->link = NULL;
        if (*ks1 == NULL) {
            *ks1 = ks;
        }
        else {
            KeySpace1 *last = *ks1;
            while (last->link) {
                last = last->link;
            }
            last->link = ks;
        }
    }
}

KeySpace1* getKey1(KeySpace1 *ks1, KeyType1 key) {      //поиск в таблице всех версий элемента, заданного ключом
    KeySpace1 *newKs1 = NULL;
    Node1 *node1 = findKeyKS1(ks1, key);
    while (node1) {
        Item *item = makeItem(node1->info->info, node1->info->key1, 0);
        addElementKS1(&newKs1, item);
        node1 = node1->next;
    }
    return newKs1;
}

KeySpace1* getKeyRel1(KeySpace1 *ks1, KeyType1 key, RelType1 release) {   //поиск конкрeтной версии элемента, заданного ключом
    KeySpace1 *newKs1 = NULL;
    Node1 *node1 = findKeyKS1(ks1, key);
    while (node1) {
        if (node1->release == release) {
            Item *item = makeItem(node1->info->info, node1->info->key1, 0);
            addElementKS1(&newKs1, item);
            break;
        }
        node1 = node1->next;
    }
    return newKs1;
}

int removeKey(KeySpace1 **ks1, KeyType1 key) {      //удаление из таблицы всех элементов с заданным ключом
    KeySpace1 *current = *ks1;
    KeySpace1 *prev = NULL;
    while (current) {
        if (current->key == key) {
            Node1 *node1 = current->node;
            Node1 *todel = node1;
            while (node1) {
                node1 = node1->next;
                if (todel->info->key2 == 0) {
                    freeInfo(todel->info->info);
                    free(todel->info);
                }
                else {
                    todel->info->key1 = 0;
                }
                free(todel);
                todel = node1;
            }
            if (prev) {
                prev->link = current->link;
            }
            else {
                *ks1 = current->link;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->link;
    }
    return 0;
}

KeyType2 removeKeyRel(KeySpace1 **ks1, KeyType1 key, RelType1 release) {    //удаление из таблицы элемента определенной версии с заданным ключом
    KeySpace1 *current = *ks1;
    KeySpace1 *prev = NULL;
    while (current) {
        if (current->key == key) {
            Node1 *node1 = current->node;
            Node1 *prev_node = NULL;
            while (node1 && node1->release != release ) {
                prev_node = node1;
                node1 = node1->next;
            }
            if (node1) {
                if (prev == NULL && node1->next == NULL) {
                    *ks1 = (*ks1)->link;
                    free(current);
                }
                else if (node1->next == NULL) {
                    prev = current->link;
                    free(current);
                }
                else {
                    if (prev_node == NULL) {
                        (*ks1)->node = node1->next;
                    }
                    else {
                        prev_node->next = node1->next;
                    }
                }
                KeyType2 key2 = node1->info->key2;
                if (node1->info->key2 != 0) {
                    node1->info->key1 = 0;
                }
                else {
                    freeInfo(node1->info->info);
                    free(node1->info);
                }
                free(node1);
                return key2;
            }
            else {
                return 0;
            }
        }
        prev = current;
        current = current->link;
    }
    return 0;
}

//второе пространство ключей
//перемешанная таблица, перемешивание сцеплением

int addElementKS2(KeySpace2 **ks2, Item *item, IndexType2 size) {     //добавление элемента
    KeySpace2 *element = (KeySpace2*)malloc(sizeof(KeySpace2));
    element->info = item;
    element->key = item->key2;
    element->next = NULL;
    IndexType2 ind = item->key2 % size;
    if (ks2[ind]) {
        KeySpace2 *current = ks2[ind];
        RelType2 rel = 1;
        while (current->next) {
            if (current->key == item->key2) {
                rel = current->release + 1;
            }
            current = current->next;
        }
        if (current->key == item->key2) {
            rel = current->release + 1;
        }
        current->next = element;
        element->release = rel;
    }
    else {
        element->release = 1;
        ks2[ind] = element;
    }
    
    return 0;
}

void cleanKS1(KeySpace1 **ks) {
    KeySpace1 *ks1 = *ks;
    KeySpace1 *prev = NULL;
    while (ks1) {
        Node1 *node = ks1->node;
        Node1 *nprev = NULL;
        while (node) {
            if (node->info->key2 == 0) {
                if (!nprev && !node->next) {
                    if (!prev) {
                        *ks = ks1->link;
                    }
                    else {
                        prev->link = ks1->link;
                    }
                    freeInfo(node->info->info);
                    free(node->info);
                    free(node);
                    free(ks1);
                    ks1 = prev;
                    break;
                }
                else {
                    removeKeyRel(ks, ks1->key, node->release);
                    node = nprev ? nprev->next : ks1->node;
                }
            }
            else {
                nprev = node;
                node = node->next;
            }
        }
        prev = ks1;
        ks1 = ks1 ? ks1->link : *ks;
    }
}

KeySpace2** getKey2(KeySpace2 **ks2, KeyType2 key, IndexType2 size) {   //поиск в таблице всех версий элемента, заданного ключом
    KeySpace2 **newKs2 = (KeySpace2**)malloc(size * sizeof(KeySpace2*));
    for (int i = 0; i < size; i++) {
        newKs2[i] = NULL;
    }
    KeySpace2 *current = ks2[key % size];
    while (current) {
        if (current->key == key) {
            Item *item = makeItem(current->info->info, 0, current->info->key2);
            addElementKS2(newKs2, item, size);
        }
        current = current->next;
    }
    return newKs2;
}

KeySpace2** getKeyRel2(KeySpace2 **ks2, KeyType2 key, RelType2 release, IndexType2 size) {  //поиск конкретной версии элемента, заданного ключом
    KeySpace2 **newKs2 = (KeySpace2**)malloc(size * sizeof(KeySpace2*));
    for (int i = 0; i < size; i++) {
        newKs2[i] = NULL;
    }
    KeySpace2 *current = ks2[key % size];
    while (current) {
        if (current->key == key && current->release == release) {
            Item *item = makeItem(current->info->info, 0, current->info->key2);
            addElementKS2(newKs2, item, size);
        }
        current = current->next;
    }
    return newKs2;
}

// чистка таблицы - удаление всех версий кроме последних

void removeKs2El(KeySpace2 **ks2, IndexType2 size, KeyType2 key2, RelType2 rel) {   //вспомогательная функция: удаление элементов по паре ключ - версия
    IndexType2 ind = key2 % size;
    KeySpace2 *current = ks2[ind];
    KeySpace2 *prev = current;
    
    if (current->key == key2 && current->release == rel) {
        ks2[ind] = current->next;
    }
    else {
        while (current && !(current->key == key2 && current->release == rel)) {
            prev = current;
            current = current->next;
        }
        prev->next = current->next;
    }
    if (current->info->key1 == 0) {
        freeInfo(current->info->info);
        free(current->info);
    }
    else {
        current->info->key2 = 0;
    }
    free(current);
}

void cleanKs2(KeySpace2 **ks2, IndexType2 size) {     //основная функция
    for (int i = 0; i < size; i++) {
        KeySpace2 *current = ks2[i];
        while (current) {
            if (current->release > 1) {
                removeKs2El(ks2, size, current->key, 1);
                current->release = 1;
            }
            current = current->next;
        }
    }
}

Table* initTable(IndexType2 msize2) {     //инициализация таблицы
    Table *table = (Table*)malloc(sizeof(Table));
    table->ks1 = NULL;
    table->msize2 = msize2;
    table->ks2 = (KeySpace2**)malloc(msize2 * sizeof(KeySpace2*));
    for (int i = 0; i < msize2; i++) {
        table->ks2[i] = NULL;
    }
    return table;
}

Item* findElement(Table *table, KeyType1 key1, KeyType2 key2) {   //поиск в таблице элемента, заданного составным ключом
    IndexType2 ind = key2 % table->msize2;
    KeySpace2 *element = table->ks2[ind];
    while (element) {
        if (element->info->key1 == key1 && element->info->key2 == key2)
            return element->info;
        element = element->next;
    }
    return NULL;
}

int addElement(Table *table, Item *item) {    //включение нового элемента в таблицу
    if (findElement(table, item->key1, item->key2))
        return 0;
    addElementKS1(&table->ks1, item);
    addElementKS2(table->ks2, item, table->msize2);
    
    return 1;
}

int removeElement(Table *table, KeyType1 key1, KeyType2 key2) {   //удаление из таблицы элемента, заданного составным ключом
    //первое пространство 
    KeySpace1 *ks1 = table->ks1;
    KeySpace1 *ksprev = ks1;
    while (ks1 && ks1->key != key1) {
        ksprev = ks1;
        ks1 = ks1->link;
    }
    if (!ks1)
        return 0;
    Node1 *node = ks1->node;
    Node1 *prev = NULL;
    while (node && node->info->key2 != key2) {
        prev = node;
        node = node->next;
    }
    
    if (!node)
        return 0;
    if (ks1 == table->ks1 && node->next == NULL) {
        table->ks1 = table->ks1->link;
        free(ks1);
    }
    else if (node->next == NULL) {
        ksprev->link = ks1->link;
        free(ks1);
    }
    else {
        if (prev == NULL) {
            ks1->node = node->next;
        }
        else {
            prev->next = node->next;
        }
    }
    //второе пространство
    IndexType2 ind = key2 % table->msize2;
    KeySpace2 *element = table->ks2[ind];
    KeySpace2 *prev2 = element;
    if (element->info->key1 == key1) {
        table->ks2[ind] = element->next;
    }
    else while (element) {
        if (element->info->key1 == key1) {
            prev2->next = element->next;
            break;
        }
        prev2 = element;
        element = element->next;
    }
    freeInfo(element->info->info);
    free(element->info);
    free(element);
    free(node);
    return 1;
}

// поиск в таблице элемента по любому заданному ключу

Node1* getNext1(Table *table, KeyType1 key1, Node1 *prev) {     //в первом пространстве
    if (prev == NULL) {
        return findKeyKS1(table->ks1, key1);
    }
    else {
        return prev->next;
    }
}

KeySpace2* getNext2(Table *table, KeyType2 key2, KeySpace2 *prev) {     //во втором пространстве
    if (prev == NULL) {
        IndexType2 ind = key2 % table->msize2;
        prev = table->ks2[ind];
    }
    else {
        prev = prev->next;
    }
    while (prev && prev->key != key2) {
        prev = prev->next;
    }
    return prev;
}

void removeAll1(Table *table, KeyType1 key1) {    //удаление из таблицы всех элементов, заданного ключом в одном из ключевых пространств
    removeKey(&(table->ks1), key1);
    for (int i = 0; i < table->msize2; i++) {
        KeySpace2 *ks2 = table->ks2[i];
        KeySpace2 *prev = NULL;
        while (ks2) {
            if (ks2->info->key1 == 0) {
                if (prev) {
                    prev->next = ks2->next;
                }
                else {
                    table->ks2[i] = ks2->next;
                }
                free(ks2->info);
                free(ks2);
                ks2 = prev ? prev->next : table->ks2[i];
            }
            else {
                prev = ks2;
                ks2 = ks2->next;
            }
        }
    }
}

void removeAll2(Table *table, KeyType2 key2) {
    IndexType2 ind = key2 % table->msize2;
    KeySpace2 *ks = table->ks2[ind];
    KeySpace2 *prev = NULL;
    while (ks) {
        if (ks->key == key2) {
            if (prev) {
                prev->next = ks->next;
            }
            else {
                table->ks2[ind] = ks->next;
            }
            if (ks->info->key1 == 0) {
                freeInfo(ks->info->info);
                free(ks->info);
            }
            else {
                ks->info->key2 = 0;
            }
            free(ks);
            ks = prev ? prev->next : table->ks2[ind];
        }
        else {
            prev = ks;
            ks = ks->next;
        }
    }
    cleanKS1(&table->ks1);
}

void removeKS1(KeySpace1 *ks1) {      //удаление таблицы
    KeySpace1 *current = ks1;
    while (current) {
        ks1 = ks1->link;
        Node1 *node1 = current->node;
        while (node1) {
            current->node = current->node->next;
            if (node1->info->key2 != 0) {
                node1->info->key1 = 0;
            }
            else {
                freeInfo(node1->info->info);
                free(node1->info);
            }
            free(node1);
            node1 = current->node;
        }
        free(current);
        current = ks1;
    }
}

void printKS1(KeySpace1 *ks1) {       //вывод таблицы
    if (!ks1) {
        printf("Table is empty\n");
        return;
    }
    KeySpace1 *current = ks1;
    printf("%7s | %7s | %7s | %15s\n", "Key", "Release", "Key2", "Info");
    printf("----------------------------------------------------------------\n");
    while (current) {
        Node1 *node1 = current->node;
        while (node1) {
            printf("%7ld | %7ld | %7ld | ", current->key, node1->release, node1->info->key2);
            printInfo(node1->info->info);
            printf("\n");
            node1 = node1->next;
            printf("----------------------------------------------------------------\n");
        }
        current = current->link;
    }
}


void removeKS2(KeySpace2 **ks2, IndexType2 size) {
    for (int i = 0; i < size; i++) {
        KeySpace2 *ks = ks2[i];
        while (ks) {
            ks2[i] = ks2[i]->next;
            if (ks->info->key1 != 0) {
                ks->info->key2 = 0;
            }
            else {
                freeInfo(ks->info->info);
                free(ks->info);
            }
            free(ks);
            ks = ks2[i];
        }
    }
}

void printKS2(KeySpace2 **ks2, IndexType2 size) {
    int empty = 1;
    for (int i = 0; i < size; i++) {
        KeySpace2 *ks = ks2[i];
        if (ks) {
            if (empty) {
                empty = 0;
                printf("%7s | %7s | %7s | %7s | %15s\n", "Num", "Key", "Release", "Key1", "Info");
                printf("----------------------------------------------------------------\n");
            }
            while (ks) {
                printf("%7d | %7ld | %7ld | %7ld | ", i, ks->key, ks->release, ks->info->key1);
                printInfo(ks->info->info);
                printf("\n");
                ks = ks->next;
                printf("----------------------------------------------------------------\n");
            }
        }
    }
    if (empty)
        printf("Table is empty\n");
}

void printTable(Table *table, int type) {     //вывод содержимого таблицы
    if (type == 1) {
        printKS1(table->ks1);
    }
    else {
        printKS2(table->ks2, table->msize2);
    }
}

int removeKeyRel1(Table *table, KeyType1 key1, RelType1 rel) {
    KeyType2 key2 = removeKeyRel(&table->ks1, key1, rel);
    if (!key2) {
        return 0;
    }
    else {
        IndexType2 ind = key2 % table->msize2;
        KeySpace2 *ks = table->ks2[ind];
        KeySpace2 *prev = NULL;
        while (ks) {
            if (ks->key == key2 && ks->info->key1 == 0) {
                if (prev) {
                    prev->next = ks->next;
                }
                else {
                    table->ks2[ind] = ks->next;
                }
                freeInfo(ks->info->info);
                free(ks->info);
                free(ks);
                break;
            }
            prev = ks;
            ks = ks->next;
        }
        return 1;
    }
}

void cleanTableKs2(Table *table) {
    cleanKs2(table->ks2, table->msize2);
    cleanKS1(&table->ks1);
}

void removeTable(Table *table) {      //удаление таблицы
    removeKS1(table->ks1);
    removeKS2(table->ks2, table->msize2);
    free(table->ks2);
}
