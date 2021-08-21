#include "tree.h"
#include "utility.h"

void printInfo(Info *info) {
    while (info) {
        printf("%.2f\t%.2f\t%s\n", info->x, info->y, info->str);
        info = info->next;
    }
}

void removeInfo(Info *info) {   //освобождение памяти
    Info *toDel = info;
    while (toDel) {
        info = info->next;
        free(toDel->str);
        free(toDel);
        toDel = info;
    }
}

Info *makeInfo(char *str, float x, float y) {
    Info *info = (Info*)malloc(sizeof(Info));
    info->x = x;
    info->y = y;
    info->str = (char*)calloc(strlen(str)+1, sizeof(char));
    strcpy(info->str, str);
    info->next = NULL;
    return info;
}

Node* makeNode(char *key, Info *info) {             //создание узла
    Node *node = malloc(sizeof(Node));
    node->left = node->right = NULL;
    strncpy(node->key, key, N);
    node->info = info;
    return node;
}

void addToList(Node *node, Info *info) {            //добавление элемента в список узла
    Info *last = node->info;
    while (last->next) {
        last = last->next;
    }
    last->next = info;
}

Node* minEl(Node *node) {   //определение мин по ключу элемента (самый левый потомок)
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Node* maxEl(Node *node) {     //определение макс по ключу элемента (самый правый потомок)
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

Node* insert(Node *node, char *key, Info *info) {     //включение нового элемента в таблицу
    if (node == NULL) {
        node = makeNode(key, info);
    }
    else {
        int cmpResult = strcmp(key, node->key);
        if (cmpResult < 0) {
            node->left = insert(node->left, key, info);
        }
        else if (cmpResult > 0) {
            node->right = insert(node->right, key, info);
        }
        else {
            addToList(node, info);
        }
    }
    return node;
}

Node* removeNode(Node* node, char *key) {       //удаление элемента из таблицы
    if (node == NULL)
        return node;
    
    int cmpResult = strcmp(key, node->key);
    
    if (cmpResult < 0)
        node->left = removeNode(node->left, key);
    
    else if (cmpResult > 0)
        node->right = removeNode(node->right, key);
    
    else {
        if (node->info->next) {
            Info *toDel = node->info;
            node->info = node->info->next;
            free(toDel->str);
            free(toDel);
        }
        else {
            if (node->left == NULL) {
                Node *tmp = node->right;
                removeInfo(node->info);
                free(node);
                return tmp;
            }
            else if (node->right == NULL) {
                Node *tmp = node->left;
                removeInfo(node->info);
                free(node);
                return tmp;
            }
            Node* tmp = node->right;
            while (tmp && tmp->left != NULL) {
                tmp = tmp->left;
            }
            strncpy(node->key, tmp->key, N);
            node->info = tmp->info;
            node->right = removeNode(node->right, tmp->key);
        }
    }
    return node;
}

Info* find(Node* node, char *key) {       //поиск информации по заданному ключу
    if (node == NULL)
        return NULL;
    int cmpResult = strcmp(key, node->key);
    if(cmpResult == 0)
        return node->info;
    if(cmpResult < 0)
        return find(node->left, key);
    else
        return find(node->right, key);
}

Node* deleteTree(Node* node) {      //удаление дерева
    if(node != NULL) {
        deleteTree(node->left);
        deleteTree(node->right);
        removeInfo(node->info);
        free(node);
        node = NULL;
    }
    return NULL;
}

void printTree(Node* node, char *a, char *b) {  //обход в прямом порядке в указанном диапазоне
    if (node != NULL) {
        int cmpA = strcmp(node->key, a);
        int cmpB = strcmp(node->key, b);
        if (cmpA > 0) {
            printTree(node->left, a, b);
        }
        if (cmpA > 0 && cmpB < 0) {
            printf("%s\n", node->key);
            printInfo(node->info);
        }
        if (cmpB < 0) {
            printTree(node->right, a, b);
        }
    }
}

void printTreeInverse(Node* node, char *a, char *b) { //обход в обратном порядке в указанном диапазоне
    if (node != NULL) {
        int cmpA = strcmp(node->key, a);
        int cmpB = strcmp(node->key, b);
        if (cmpB < 0) {
            printTreeInverse(node->right, a, b);
        }
        if (cmpA > 0 && cmpB < 0) {
            printf("%s\n", node->key);
            printInfo(node->info);
        }
        if (cmpA > 0) {
            printTreeInverse(node->left, a, b);
        }
    }
}

void print(Node* node, int level) {
    if(node) {
        print(node->left, level+1);
        for(int i = 0; i < level; i++)
            printf("\t");
        printf("%s\n", node->key);
        print(node->right, level+1);
    }
}

Node* inputFromFile(FILE *file) {   //ввод дерева из файла
    Node *root = NULL;
    char key[N];
    float x, y;
    char *str;
    while (fgets(key, N, file)) {
        if (key[strlen(key)-1] == '\n')
            key[strlen(key)-1] = '\0';
        fscanf(file, "%f %f " , &x, &y);
        str = get_str(file);
        Info *info = makeInfo(str, x, y);
        root = insert(root, key, info);
        free(str);
    }
    return root;
}

//поиск элемента, наиболее отличающегося по значению ключа от заданного

int myStrcmp(const char *str1, const char *str2) {  //сравнение разницы между 1-ым отличающимся символом
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str2-*str1;
        }
        str1++;
        str2++;
    }
    return *str2-*str1;
}

Node* findMostDiff(Node *node, char *key) {
    if (!node) {
        return NULL;
    }
    Node *max = maxEl(node);
    Node *min = minEl(node);
    if (strcmp(key, min->key) <= 0) {
        return max;
    }
    else if (strcmp(key, max->key) >= 0) {
        return min;
    }
    else if (myStrcmp(key, max->key) > myStrcmp(min->key, key)) {
        //printf("%d %d\n", myStrcmp(key, max->key), myStrcmp(min->key, key));
        return max;
    }
    else {
        //printf("%d %d\n", myStrcmp(key, max->key), myStrcmp(min->key, key));
        return min;
    }
}

void test(int n) {
    Node *root = NULL;
    char key[N];
    double avgTime = 0;
    for (int i = 0; i < n; i++) {
        sprintf(key, "%020d", i);
        Info *info = makeInfo("test", 1.1, 2.2);
        clock_t t = clock();
        insert(root, key, info);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC * 1000;
    }
    printf("Общее время вставки элементов: %lf мс\n", avgTime);
    avgTime /= n;
    printf("Среднее время вставки элемента: %lf мс\n", avgTime);
    avgTime = 0;
    for (int i = 0; i < n; i++) {
        sprintf(key, "%020d", i);
        clock_t t = clock();
        find(root, key);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC * 1000;
    }
    printf("Общее время поиска элементов: %lf мс\n", avgTime);
    avgTime /= n;
    printf("Среднее время поиска элемента: %lf мс\n", avgTime);
    avgTime = 0;
    for (int i = 0; i < n; i++) {
        sprintf(key, "%020d", i);
        clock_t t = clock();
        removeNode(root, key);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC * 1000;
    }
    printf("Общее время удаления элементов: %lf мс\n", avgTime);
    avgTime /= n;
    printf("Среднее время удаления элемента: %lf мс\n", avgTime);
    deleteTree(root);
}
