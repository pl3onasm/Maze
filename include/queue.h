#ifndef QUEUE_H
#define QUEUE_H

#include "path.h"
#include "maze.h"
#include <assert.h>

typedef struct Queue {
  Node *array; int back, front, size;
} Queue;

int isEmptyQueue(Queue q);

void enqueue(Node item, Queue *qp);

Node dequeue(Queue *qp);

Queue newQueue(unsigned s);

void freeQueue(Queue q);

#endif