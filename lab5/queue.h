#ifndef queue_h
#define queue_h

#include <stdlib.h>
#include <string.h>
#include "graph.h"

typedef struct QueueNode {
    Path *path;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *first, *last;
} Queue;

Queue* initQueue(void);
void push(Queue *queue, Path *path);
Path* pop(Queue *queue);
int isEmpty(Queue *queue);
void removeQueue(Queue *queue);

#endif
