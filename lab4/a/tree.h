#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 80

typedef struct Info {
    float x, y;
    char *str;
    struct Info* next;
} Info;

void printInfo(Info *info);

void removeInfo(Info *info);

typedef struct Node {
    char key[N];
    Info *info;
    struct Node *left;
    struct Node *right;
} Node;

Info *makeInfo(char *str, float x, float y);

Node* makeNode(char *key, Info *info);

void addToList(Node *node, Info *info);

Node* minEl(Node *node);

Node* maxEl(Node *node);

Node* insert(Node *node, char *key, Info *info);

Node* removeNode(Node* node, char *key);

Info* find(Node* node, char *key);

void printTree(Node* node, char *a, char *b);

void printTreeInverse(Node* node, char *a, char *b);

void print(Node* node, int level);

Node* inputFromFile(FILE *file);

int myStrcmp(const char *str1, const char *str2);

Node* findMostDiff(Node *node, char *key);

void test(int n);
