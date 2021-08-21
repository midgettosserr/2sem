#include "set.h"

SetNode* insert(SetNode *node, char* key) {
    if (node == NULL) {
        node = (SetNode*)malloc(sizeof(SetNode));
        node->key = key;
        node->left = node->right = NULL;
        
        node->isInf = 1;
        node->predecessor = NULL;
    }
    else {
        int cmpResult = strcmp(key, node->key);
        if (cmpResult < 0) {
            node->left = insert(node->left, key);
        }
        else if (cmpResult > 0) {
            node->right = insert(node->right, key);
        }
    }
    return node;
}

SetNode* find(SetNode *node, const char* key) {
    if (node == NULL)
        return NULL;
    
    int cmpResult = strcmp(key, node->key);
    
    if(cmpResult == 0)
        return node;
    
    if(cmpResult < 0)
        return find(node->left, key);
    else
        return find(node->right, key);
}

void removeSet(SetNode *node) {
    if(node != NULL) {
        removeSet(node->left);
        removeSet(node->right);
        free(node);
    }
}
