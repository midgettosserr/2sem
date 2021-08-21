#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 10 // число элементов в массиве-элементе узла списка

typedef struct Patient {  // структура с данными пациента
    char *id;
    int time;
} Patient;

typedef struct Node { // структура узла списка
    Patient *data;
    int firstInd, lastInd;
    struct Node* next;
} Node;

typedef struct Dequeue {  // структура двусторонняя очередь
    struct Node* first, *last;
} Dequeue;

int isempty(Dequeue *q);
Node* newNode(void);
Dequeue* createQueue(void);
void printQueue(Dequeue* q);
void addValueToEnd(Dequeue* q, const char* id, int time);
void addValueToBegin(Dequeue* q, const char* id, int time);
Patient *removeBegin(Dequeue *q);
