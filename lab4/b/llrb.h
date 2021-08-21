#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Info {
    float x;
    char *str;
    struct Info* next;
} Info;

typedef struct Node { //узел дерева
    unsigned int key;
    Info *info;
    struct Node *left;
    struct Node *right;
    char c;
} Node;

void printInfo(Info *info);

void removeInfo(Info *info);

Info *makeInfo(char *str, float x);

Node* makeNode(unsigned int key, Info *info);

void addToList(Node *node, Info *info);

Node *rotateRight(Node *node);

Node *rotateLeft(Node *node);

void flipColors(Node *node);

int isRed(Node *node);

Node* fixUp(Node *node);

Node *moveRedLeft(Node *node);

Node *moveRedRight(Node *node);

Node* minNode(Node *node);

Node* maxNode(Node *node);

Node* insert(Node *node, unsigned int key, Info *info);

Node* removeMin(Node *node);

Node* removeNode(Node* node, unsigned int key);

Info* find(Node* node, unsigned key);

Node* deleteTree(Node* node);

void printTree(Node* leaf, unsigned int a, unsigned int b);

void printTree2(Node *node, unsigned int b);

void printTree3(Node *node);

void print(Node* leaf, int level);

Node* inputFromFile(FILE *file);

unsigned int absDiff(unsigned int a, unsigned int b);

void closestKey(Node *node, unsigned int key, Node **result);

void test(int n);

void DOT(Node *node);
