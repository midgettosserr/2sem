#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "graph.h"
#include "set.h"
#include "queue.h"
#include "common.h"

Graph *initGraph() {    //создание пустого графа
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->first = NULL;
    graph->size = graph->edges = 0;
    return graph;
}

Node* makeNode(const char *name, double x, double y) {      //создание вершины
    Node *node = (Node*)malloc(sizeof(Node));
    node->name = (char*)malloc(sizeof(char) * (strlen(name)+1));
    strcpy(node->name, name);
    node->x = x;
    node->y = y;
    node->list = NULL;
    node->next = NULL;
    node->color = 'w';
    return node;
}

void removeFromList(ListNode **list, Node *node) {      //удаление вершины из списка смежностей
    ListNode *prev = NULL;
    ListNode *current = *list;
    while (current) {
        if (current->node == node) {
            if (prev) {
                prev->next = current->next;
            }
            else {
                (*list) = current->next;
            }
            free(current);
            break;
        }
        else {
            prev = current;
            current = current->next;
        }
    }
}

int addNode(Graph *graph, const char* name, double x, double y) {     //добавление вершины
    if (!graph) {
        return 0;
    }
    Node *last = NULL, *current = graph->first;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return 0;
        }
        last = current;
        current = current->next;
    }
    Node *node = makeNode(name, x, y);
    if (last) {
        last->next = node;
    }
    else {
        graph->first = node;
    }
    graph->size++;
    return 1;
}

int addEdge(Graph *graph, const char* name1, const char* name2) {     //добавление ребра
    if (!graph) {
        return 0;
    }
    Node *node1 = NULL, *node2 = NULL, *current = graph->first;
    while (current) {
        if (strcmp(current->name, name1) == 0) {
            node1 = current;
            if (node2) {
                break;
            }
        }
        if (strcmp(current->name, name2) == 0) {
            node2 = current;
            if (node1) {
                break;
            }
        }
        current = current->next;
    }
    if (!node1 || !node2) {
        return 0;
    }
    ListNode *list = node1->list;
    while (list) {
        if (list->node == node2) {
            return 0;
        }
        list = list->next;
    }
    double w = sqrt((node2->x-node1->x)*(node2->x-node1->x) + (node2->y-node1->y)*(node2->y-node1->y));
    ListNode *lnode = (ListNode*)malloc(sizeof(ListNode));
    lnode->node = node2;
    lnode->w = w;
    lnode->next = node1->list;
    node1->list = lnode;
    graph->edges++;
    return 1;
}

int removeNode(Graph *graph, const char* name) {
    if (!graph) {
        return 0;
    }
    Node *prevNode = NULL;
    Node *currentNode = graph->first;
    Node *nodeToDel = NULL;
    while (currentNode) {
        if (strcmp(currentNode->name, name) == 0) {
            nodeToDel = currentNode;
        }
        if (!nodeToDel) {
            prevNode = currentNode;
        }
        currentNode = currentNode->next;
    }
    if (!nodeToDel) {
        return 0;
    }
    currentNode = graph->first;
    while (currentNode) {
        if (currentNode != nodeToDel) {
            removeFromList(&currentNode->list, nodeToDel);
        }
        currentNode = currentNode->next;
    }
    ListNode *listToDel = nodeToDel->list;
    while (listToDel) {
        nodeToDel->list = listToDel->next;
        free(listToDel);
        listToDel = nodeToDel->list;
    }
    if (prevNode) {
        prevNode->next = nodeToDel->next;
    }
    else {
        graph->first = nodeToDel->next;
    }
    free(nodeToDel->name);
    free(nodeToDel);
    
    return 1;
}

int removeEdge(Graph *graph, const char* name1, const char* name2) {
    if (!graph) {
        return 0;
    }
    Node *node1 = NULL, *node2 = NULL, *current = graph->first;
    while (current) {
        if (strcmp(current->name, name1) == 0) {
            node1 = current;
            if (node2) {
                break;
            }
        }
        if (strcmp(current->name, name2) == 0) {
            node2 = current;
            if (node1) {
                break;
            }
        }
        current = current->next;
    }
    
    if (!node1 || !node2) {
        return 0;
    }  
    removeFromList(&(node1->list), node2); 
    return 1;
}

void print(Graph *graph) {
    if (!graph) {
        return;
    }
    Node *node = graph->first;
    while (node) {
        printf("Вершина %s (%.2lf, %.2lf), список смежных вершин:\n", node->name, node->x, node->y);
        ListNode *list = node->list;
        while (list) {
            printf("%s, вес %lf\n", list->node->name, list->w);
            list = list->next;
        }
        node = node->next;
    }
}

Node* findNode(Graph *graph, const char* name) {      //поиск вершины по имени
    Node *node = graph->first;
    while (node && strcmp(node->name, name) != 0) {
        node = node->next;
    }
    return node;
}

Path* BFS(Graph *graph, const char* name1, const char* name2) {   //поиск в ширину

    Node *nodeName1 = findNode(graph, name1);
    if (!nodeName1) {
        return NULL;
    }
    SetNode *visited = NULL;
    Queue *q = initQueue();
    Path *path = initPath(graph->size);
    visited = insert(visited, nodeName1->name);
    path->v[path->ind++] = nodeName1;
    push(q, path);
    while(!isEmpty(q))
    {
        path = pop(q);
        Node *last = path->v[path->ind-1];
        if (strcmp(last->name, name2) == 0) {
            removeSet(visited);
            removeQueue(q);
            return path;
        }
        ListNode *list = last->list;
        while (list) {
            if (!find(visited, list->node->name)) {
                insert(visited, list->node->name);
                Path *newPath = copyPath(path);
                newPath->v[newPath->ind++] = list->node;
                push(q, newPath);
            }
            list = list->next;
        }
        removePath(path);
    }
    removeQueue(q);
    removeSet(visited);
    return NULL;
}

void BFSRecursive_(Queue *q, SetNode *visited, const char* name2, Path **result) { 
    if (isEmpty(q)) {
        return;
    }
    
    Path *path = pop(q);
    Node *last = path->v[path->ind-1];
    if (strcmp(last->name, name2) == 0) {
        *result = path;
        return;
    }
    ListNode *list = last->list;
    while (list) {
        if (!find(visited, list->node->name)) {
            insert(visited, list->node->name);
            Path *newPath = copyPath(path);
            newPath->v[newPath->ind++] = list->node;
            push(q, newPath);
        }
        list = list->next;
    }
    removePath(path);
    BFSRecursive_(q, visited, name2, result);
}

Path* BFSRecursive(Graph *graph, const char* name1, const char* name2) {    //поиск в ширину с использованием рекурсии
    Node *nodeName1 = findNode(graph, name1);
    if (!nodeName1) {
        return NULL;
    }
    
    Path *result = NULL;
    SetNode *visited = NULL;
    Queue *q = initQueue();
    Path *path = initPath(graph->size);
    visited = insert(visited, nodeName1->name);
    path->v[path->ind++] = nodeName1;
    push(q, path);
    
    BFSRecursive_(q, visited, name2, &result);
    
    removeSet(visited);
    removeQueue(q);
    return result;
}

ShortestPath* BellmanFord(Graph *graph, const char *name1, const char* name2) {
    SetNode *distMap = NULL;
    Node *node = graph->first;
    while (node) {
        distMap = insert(distMap, node->name);
        node = node->next;
    }
    SetNode *src = find(distMap, name1);
    src->dist = 0;
    src->isInf = 0;
    SetNode *dst;
    for (int i = 1; i <= graph->size - 1; i++) {
        node = graph->first;
        while (node) {
            src = find(distMap, node->name);
            ListNode *list = node->list;
            while (list) {
                dst = find(distMap, list->node->name);
                double w = list->w;
                if (!src->isInf && (src->dist + w < dst->dist || dst->isInf)) {
                    dst->isInf = 0;
                    dst->dist = src->dist + w;
                    dst->predecessor = src->key;
                }
                list = list->next;
            }
            node = node->next;
        }
    }
    
    dst = find(distMap, name2);
    
    if (!dst || dst->isInf) {
        removeSet(distMap);
        return NULL;
    }
    
    ShortestPath *result = (ShortestPath*)malloc(sizeof(ShortestPath));
    result->dist = dst->dist;
    result->path = (char**)malloc(sizeof(char*)*graph->size);
    result->size = 0;
    result->path[result->size++] = dst->key;
    while (dst->predecessor) {
        result->path[result->size++] = dst->predecessor;
        dst->predecessor = find(distMap, dst->predecessor)->predecessor;
    }
    removeSet(distMap);
    return result;
}

void DFS(Node *node, int *cycle, struct Path *list) {     //поиск в глубину
    if (*cycle) {
        return;
    }
    node->color = 'g';
    ListNode *listNode = node->list;
    while (listNode) {
        if (listNode->node->color == 'g') {
            *cycle = 1;
            return;
        }
        if (listNode->node->color == 'w') {
            DFS(listNode->node, cycle, list);
        }
        listNode = listNode->next;
    }
    node->color = 'b';
    if (list) {
        list->v[list->ind++] = node;
    }
}

void makeWhite(Graph *graph) {
    Node *node = graph->first;
    while (node) {
        node->color = 'w';
        node = node->next;
    }
}

Path* sorting(Graph *graph) {
    makeWhite(graph);
    Path *list = initPath(graph->size);
    int cycle = 0;
    Node *node = graph->first;
    while (node) {
        if (node->color == 'w') {
            DFS(node, &cycle, list);
        }
        if (cycle) {
            break;
        }
        node = node->next;
    }

    if (cycle) {
        removePath(list);
        return NULL;
    }

    for (int i = 0; i < list->ind/2; i++) {
        Node *tmp = list->v[i];
        list->v[i] = list->v[list->ind - i - 1];
        list->v[list->ind - i - 1] = tmp;
    }
    return list;
}

int isCycle(Graph *graph) {   //проверка на цикличность
    makeWhite(graph);
    int cycle = 0;
    DFS(graph->first, &cycle, NULL);
    return cycle;
}

Graph* inputFromFile(FILE *file) {
    Graph *graph = initGraph();
    int n, m, res;
    double x, y;
    char *name, *name1, *name2;
    
    res = fscanf(file, "%d%*c", &n);
    if (res != 1) {
        removeGraph(graph);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        res = fscanf(file, "%lf%lf%*c", &x, &y);
        if (res != 2) {
            removeGraph(graph);
            return NULL;
        }
        name = get_str(file);
        if (!name) {
            removeGraph(graph);
            return NULL;
        }
        addNode(graph, name, x, y);
        free(name);
    }
    res = fscanf(file, "%d%*c", &m);
    if (res != 1) {
        removeGraph(graph);
        return NULL;
    }
    for (int i = 0; i < m; i++) {
        name1 = get_str(file);
        if (!name1) {
            removeGraph(graph);
            return NULL;
        }
        name2 = get_str(file);
        if (!name2) {
            removeGraph(graph);
            return NULL;
        }
        addEdge(graph, name1, name2);
        free(name1);
        free(name2);
    }
    return graph;
}

void outputToFile(FILE *file, Graph *graph) {
    fprintf(file, "%d\n", graph->size);
    Node *node = graph->first;
    while (node) {
        fprintf(file, "%lf %lf %s\n", node->x, node->y, node->name);
        node = node->next;
    }
    fprintf(file, "%d\n", graph->edges);
    node = graph->first;
    while (node) {
        ListNode *list = node->list;
        while (list) {
            fprintf(file, "%s\n", node->name);
            fprintf(file, "%s\n", list->node->name);
            list = list->next;
        }
        node = node->next;
    }
}

void removeGraph(Graph *graph) {
    if (!graph)
        return;
    Node *node = graph->first;
    while (node) {
        graph->first = node->next;
        ListNode *list = node->list;
        while (list) {
            node->list = list->next;
            free(list);
            list = node->list;
        }
        free(node->name);
        free(node);
        node = graph->first;
    }
    free(graph);
}

Graph* makeRand(unsigned int n, unsigned int m) {   //создание случайного графа
    Graph *graph = initGraph();
    char name1[80], name2[80];
    for (int i = 1; i <= n; i++) {
        sprintf(name1, "%d", i);
        addNode(graph, name1, rand()%100, rand()%100);
    }
    int i = 0;
    while (i < m) {
        int a = rand()%n + 1;
        int b = rand()%n + 1;
        sprintf(name1, "%d", a);
        sprintf(name2, "%d", b);
        if (addEdge(graph, name1, name2)) {
            i++;
        }
    }
    return graph;
}

//функции для работы с массивом вершин

Path* initPath(int n) {
    Path *path = (Path*)malloc(sizeof(Path));
    path->size = n;
    path->ind = 0;
    path->v = (Node**)malloc(sizeof(Node*)*n);
    return path;
}

Path* copyPath(Path *path) {
    Path *cpy = (Path*)malloc(sizeof(Path));
    cpy->size = path->size;
    cpy->ind = path->ind;
    cpy->v = (Node**)malloc(sizeof(Node*)*cpy->size);
    for (int i = 0; i < cpy->ind; i++) {
        cpy->v[i] = path->v[i];
    }
    return cpy;
}

void printPath(Path *path) {
    for (int i = 0; i < path->ind; i++) {
        printf("%s\n", path->v[i]->name);
    }
}

void removePath(Path *path) {
    if (path) {
        free(path->v);
        free(path);
    }
}
