#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"

int isempty(Dequeue* q) { // проверка, пуста ли очередь
    if (q->first == 0)
        return 1;
    else
        return 0;
}

Node* newNode() // функция формирования нового узла очереди
{
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = (Patient*)malloc(sizeof(Patient) * MAXSIZE);
    temp->next = NULL;
    return temp;
}

Dequeue* createQueue()  // функция создания очереди
{
    Dequeue* q = (Dequeue*)malloc(sizeof(Dequeue));
    q->first = q->last = NULL;
    return q;
}

void printQueue(Dequeue* q) { // функция вывода очереди на экран
    if (isempty(q))
        printf("Очередь пуста");
    Node* temp = q->first;
    while (temp) {
        for (int i = temp->firstInd; i <= temp->lastInd; i++) {
            printf("%s ", temp->data[i].id);
        }
        temp = temp->next;
    }
    printf("\n");
}

void addValueToEnd(Dequeue* q, const char* id, int time)  // функция добавления значения в конец очереди
{
    if (q->last != NULL && q->last->lastInd < MAXSIZE - 1) {
        q->last->lastInd++;
    }
    else {
        Node* temp = newNode();
        if (q->last == NULL) {
            q->first = q->last = temp;
        }
        else {
            q->last->next = temp;
            q->last = temp;
        }
        q->last->firstInd = q->last->lastInd = 0;
    }
    q->last->data[q->last->lastInd].id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(q->last->data[q->last->lastInd].id, id);
    q->last->data[q->last->lastInd].time = time;
}

void addValueToBegin(Dequeue* q, const char* id, int time)  // функция добавления значения в начало очереди
{
    if (q->first != NULL && q->first->firstInd > 0) {
        q->first->firstInd--;
    }
    else {
        Node* temp = newNode();
        if (q->first == NULL) {
            q->first = q->last = temp;
        }
        else {
            temp->next = q->first;
            q->first = temp;
        }
        q->first->firstInd = q->first->lastInd = MAXSIZE - 1;
    }
    q->first->data[q->first->firstInd].id = (char*)malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(q->first->data[q->first->firstInd].id, id);
    q->first->data[q->first->firstInd].time = time;
}

Patient* removeBegin(Dequeue* q) {  // функция удаления элемента из начала очереди
    if (isempty(q) == 1) {
        return NULL;
    }
    Patient* patient = (Patient*)malloc(sizeof(Patient));
    patient->id = (char*)malloc(sizeof(char) * (strlen(q->first->data[q->first->firstInd].id) + 1));
    strcpy(patient->id, q->first->data[q->first->firstInd].id);
    patient->time = q->first->data[q->first->firstInd].time;
    if (q->first->firstInd == q->first->lastInd) {
        if (q->last == q->first)
            q->last = NULL;
        struct Node* temp;
        temp = q->first;
        q->first = q->first->next;
        free(temp->data[temp->firstInd].id);
        free(temp->data);
        free(temp);
    }
    else {
        free(q->first->data[q->first->firstInd].id);
        q->first->firstInd++;
    }

    return patient;
}
