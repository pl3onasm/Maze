#include "include/queue.h"
#include "include/maze.h"

void doubleQueueSize (Queue *qp) {
  unsigned oldSize = qp -> size;
  qp -> size = 2*oldSize;
  qp -> array = realloc (qp -> array, qp -> size*sizeof(node));
  assert (qp -> array != NULL) ;
  for (unsigned i =0; i < qp -> back ; i ++) 
    {qp -> array [oldSize + i] = qp -> array [i];}
  qp -> back = qp -> back + oldSize; 
}

Queue newQueue (unsigned s) {
  Queue q; q.array = calloc (s,sizeof (node));
  assert (q.array != NULL);
  q.back = 0; q.front = 0; q.size = s;
  return q;
}

int isEmptyQueue (Queue q) {return (q.back == q.front);}

void queueEmptyError () {printf ("queue empty\n"); abort ();}

void enqueue (Node item , Queue * qp) {
  qp -> array[qp -> back] = item;
  qp -> back = (qp -> back+1) % qp -> size;
  if (qp -> back == qp -> front) doubleQueueSize (qp);
}

Node dequeue (Queue *qp) {
  if (isEmptyQueue (*qp)) queueEmptyError ();
  Node item = qp -> array [qp -> front];
  qp -> front = (qp -> front+1) % qp -> size;
  return item;
}

void freeQueue (Queue q) {
  for (unsigned i=0; i < q.size; i++) free (q.array[i]);
  free (q.array);
}