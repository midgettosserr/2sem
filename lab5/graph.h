#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
struct ListNode;

typedef struct Node {
    char *name;
    char color;
    double x, y;
    struct ListNode *list;
    struct Node *next;
} Node;

typedef struct ListNode {
    Node *node;
    double w;
    struct ListNode *next;
} ListNode;

typedef struct Graph {
    Node *first;
    int size;
    int edges;
} Graph;

typedef struct ShortestPath {   //для реза алг Беллмана-Форда
    double dist;
    char **path;
    int size;
} ShortestPath;

typedef struct Path {     //массив вершин
    Node **v;
    int size;
    int ind;
} Path;

Path* initPath(int n);
Path* copyPath(Path *path);
void printPath(Path *path);
void removePath(Path *path);

Graph *initGraph(void);
int addNode(Graph *graph, const char* name, double x, double y);
int addEdge(Graph *graph, const char* name1, const char* name2);
int removeNode(Graph *graph, const char* name);
int removeEdge(Graph *graph, const char* name1, const char* name2);
void print(Graph *graph);
Path* BFS(Graph *graph, const char* name1, const char* name2);
Path* BFSRecursive(Graph *graph, const char* name1, const char* name2);
ShortestPath* BellmanFord(Graph *graph, const char *name1, const char* name2);
Path* sorting(Graph *graph);
int isCycle(Graph *graph);
Graph* inputFromFile(FILE *file);
void outputToFile(FILE *file, Graph *graph);
Graph* makeRand(unsigned int n, unsigned int m);
void removeGraph(Graph *graph);

#endif
