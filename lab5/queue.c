#include "queue.h"
#include <stdio.h>

Queue* initQueue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->first = queue->last = NULL;
    return queue;
}

void push(Queue *queue, Path* path) {
    QueueNode *node = (QueueNode*)malloc(sizeof(QueueNode));
    node->path = path;
    node->next = NULL;
    if (queue->first) {
        queue->last->next = node;
        queue->last = node;
    }
    else {
        queue->first = queue->last = node;
    }
}

Path* pop(Queue *queue) {
    Path *path = queue->first->path;
    QueueNode *node = queue->first;
    queue->first = queue->first->next;
    if (!queue->first) {
        queue->last = NULL;
    }
    free(node);
    return path;
}

int isEmpty(Queue *queue) {
    return queue->first == NULL;
}

void removeQueue(Queue *queue) {
    while (queue->first) {
        queue->last = queue->first->next;
        removePath(queue->first->path);
        free(queue->first);
        queue->first = queue->last;
    }
    free(queue);
}
