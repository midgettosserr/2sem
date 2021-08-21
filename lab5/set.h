#ifndef SET_H
#define SET_H

#include <stdlib.h>
#include <string.h>

typedef struct SetNode {    //множество для поиска в ширину
    char *key;
    double dist;
    int isInf;
    char *predecessor;
    struct SetNode *left;
    struct SetNode *right;
} SetNode;

SetNode* insert(SetNode *node, char* key);
SetNode* find(SetNode *node, const char* key);
void removeSet(SetNode *node);

#endif
