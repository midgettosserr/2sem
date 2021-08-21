#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "llrb.h"

void printInfo(Info *info) {
    while (info) {
        printf("%.2f\t\t%s\n", info->x, info->str);
        info = info->next;
    }
}

void removeInfo(Info *info) {
    Info *toDel = info;
    while (toDel) {
        info = info->next;
        free(toDel->str);
        free(toDel);
        toDel = info;
    }
}

Info *makeInfo(char *str, float x) {
    Info *info = (Info*)malloc(sizeof(Info));
    info->x = x;
    info->str = (char*)calloc(strlen(str)+1, sizeof(char));
    strcpy(info->str, str);
    info->next = NULL;
    return info;
}

Node* makeNode(unsigned int key, Info *info) {
    Node *node = malloc(sizeof(Node));
    node->left = node->right = NULL;
    node->key = key;
    node->info = info;
    node->c = 'r';
    return node;
}

void addToList(Node *node, Info *info) {
    Info *last = node->info;
    while (last->next) {
        last = last->next;
    }
    last->next = info;
}

Node *rotateRight(Node *node) { //вращенеи вправо
    Node *x = node->left;
    node->left = x->right;
    x->right = node;
    x->c = node->c;
    node->c = 'r';
    return x;
}

Node *rotateLeft(Node *node) {  //вращение влево
    Node *x = node->right;
    node->right = x->left;
    x->left = node;
    x->c = node->c;
    node->c = 'r';
    return x;
}

void flipColors(Node *node) { //переворот цветов
    node->c = node->c == 'r' ? 'b' : 'r';
    node->left->c = node->left->c == 'r' ? 'b' : 'r';
    node->right->c = node->right->c == 'r' ? 'b' : 'r';
}

int isRed(Node *node) {
    if (!node) {
        return 0;
    }
    return node->c == 'r';
}

Node* fixUp(Node *node) { //исправление правых красных связей
    if (isRed(node->right)) {
        node = rotateLeft(node);
    }
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    return node;
}

Node *moveRedLeft(Node *node) { //перемещение красного потомка влево
    flipColors(node);
    if (node->right && isRed(node->right->left)) {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
        flipColors(node);
    }
    return node;
}

Node *moveRedRight(Node *node) {  //перемещение красного потомка вправо
    flipColors(node);
    if (node->left && isRed(node->left->left)) {
        node = rotateRight(node);
        flipColors(node);
    }
    return node;
}

Node* minNode(Node *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Node* maxNode(Node *node) {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

Node* insert(Node *node, unsigned int key, Info *info) {  //включение нового элемента в таблицу
    if (node == NULL) {
        return makeNode(key, info);
    }
    if (key == node->key) {
        addToList(node, info);
    }
    else {
        if (key < node->key) {
            node->left = insert(node->left, key, info);
        }
        else {
            node->right = insert(node->right, key, info);
        }
        if (isRed(node->right) && !isRed(node->left)) {
            node = rotateLeft(node);
        }
        if (isRed(node->left) && isRed(node->left->left)) {
            node = rotateRight(node);
        }
    }
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    return node;
}

Node* removeMin(Node *node) { //удаление минимального элемента
    if (!node) {
        return NULL;
    }
    if (!node->left) {
        removeInfo(node->info);
        free(node);
        return NULL;
    }
    if (!isRed(node->left) && !isRed(node->left->left)) {
        node =  moveRedRight(node);
    }
    node->left = removeMin(node->left);
    return fixUp(node);
}

Node* removeNode(Node* node, unsigned int key) {  //удаление элемента из таблицы
    if (!node)
        return NULL;
    if (key < node->key) {
        if (node->left) {
            if (!isRed(node->left) && !isRed(node->left->left)) {
                node = moveRedLeft(node);
            }
            node->left = removeNode(node->left, key);
        }
    }
    else {
        if (key == node->key && node->info->next) {
            Info *toDel = node->info;
            node->info = node->info->next;
            free(toDel->str);
            free(toDel);
            return node;
        }
        if (isRed(node->left)) {
            node = rotateRight(node);
        }
        if (key == node->key && !node->right) {
            removeInfo(node->info);
            free(node);
            return NULL;
        }
        if (node->right) {
            if (!isRed(node->right) && !isRed(node->right->left)) {
                node = moveRedRight(node);
            }
            if (key == node->key) {
                Node *tmp = minNode(node->right);
                node->key = tmp->key;
                node->info = tmp->info;
                tmp->info = NULL;
                node->right = removeMin(node->right);
            }
            else {
                node->right = removeNode(node->right, key);
            }
        }
    }
    return fixUp(node);
}

Info* find(Node* node, unsigned key) {  //поиск информации по заданному ключу
    if (node == NULL)
        return NULL;
    if(key == node->key)
        return node->info;
    if(key < node->key)
        return find(node->left, key);
    else
        return find(node->right, key);
}

Node* deleteTree(Node* node) {
    if(node != NULL) {
        deleteTree(node->left);
        deleteTree(node->right);
        removeInfo(node->info);
        free(node);
        node = NULL;
    }
    return NULL;
}

void printTree(Node* leaf, unsigned int a, unsigned int b) {  //вывод в прямом порядке след ключей
    if (leaf != NULL) {
        if (leaf->key >= a) {
            printTree(leaf->left, a, b);
        }
        if (leaf->key >= a && leaf->key < b) {
            printf("%u\n", leaf->key);
            printInfo(leaf->info);
        }
        if (leaf->key < b) {
            printTree(leaf->right, a, b);
        }
    }
}

void printTree2(Node *node, unsigned int b) { //версия только с кл b
    printTree(node, 0, b);
}

void printTree3(Node *node) { //версия без ключей
    printTree(node, 0, UINT_MAX);
}

void print(Node* leaf, int level) { //вывод в виде дерева
    if(leaf) {
        print(leaf->left, level+1);
        for(int i = 0; i < level; i++)
            printf("\t\t");
        printf("%u(%c)\n", leaf->key, leaf->c);
        print(leaf->right, level+1);
    }
}

Node* inputFromFile(FILE *file) {
    Node *root = NULL;
    unsigned int key;
    float x;
    char *str;
    while (fscanf(file, "%u", &key) == 1) {
        fscanf(file, "%f", &x);
        str = get_str(file);
        Info *info = makeInfo(str, x);
        root = insert(root, key, info);
        free(str);
    }
    return root;
}

unsigned int absDiff(unsigned int a, unsigned int b) {
    return (unsigned int)labs((long)a - (long)b);
}

void closestKey(Node *node, unsigned int key, Node **result) {  //поиск элемента, наиболее близкого к заданному
    if (node == NULL) {
        return;
    }
    if(key == node->key) {
        Node *minN = minNode(node->right);
        Node *maxN = maxNode(node->left);
        if (minN && absDiff((*result)->key, key) > absDiff(minN->key, key)) {
            *result = minN;
        }
        if (maxN && absDiff((*result)->key, key) > absDiff(minN->key, key)) {
            *result = maxN;
        }
        return;
    }
    
    if (*result == NULL || absDiff((*result)->key, key) > absDiff(node->key, key)) {
        *result = node;
    }
    
    if(key < node->key)
        return closestKey(node->left, key, result);
    else
        return closestKey(node->right, key, result);
}

void test(int n) {
    Node *root = NULL;
    double avgTime = 0;
    for (int i = 0; i < n; i++) {
        Info *info = makeInfo("test", 1.1);
        clock_t t = clock();
        root = insert(root, i, info);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC * 1000;
    }
    printf("Общее время вставки элементов: %lf мс\n", avgTime);
    avgTime /= n;
    printf("Среднее время вставки элемента: %lf мс\n", avgTime);
    avgTime = 0;
    for (int i = 0; i < n; i++) {
        clock_t t = clock();
        find(root, i);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC * 1000;
    }
    printf("Общее время поиска элементов: %lf мс\n", avgTime);
    avgTime /= n;
    printf("Среднее время поиска элемента: %lf мс\n", avgTime);
    avgTime = 0;
    for (int i = 0; i < n; i++) {
        clock_t t = clock();
        root = removeNode(root, i);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC * 1000;
    }
    printf("Общее время удаления элементов: %lf мс\n", avgTime);
    avgTime /= n;
    printf("Среднее время удаления элемента: %lf мс\n", avgTime);
    deleteTree(root);
}

void DOT (Node* node) {
	if (node!=NULL) {
		DOT(node->left);
		printf("\t%d -> ", node->key);
		printf("{");
		if (node->right) {
			printf("%d ", node->right->key);
		}
		if (node->left) {
			printf("%d ", node->left->key);
		}
		printf("}\n");
		DOT(node->right);
	}
}
