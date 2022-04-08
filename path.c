#include "include/path.h"
#include "include/queue.h"
#include "include/maze.h"

char *convertPath(char* p){
  int j=0; char *q = malloc(sizeof(char) * 2*strlen(p));
  for (int i = strlen(p)-1; i>0; i--) {
    if (p[i] == p[i-1]) q[j++] = 'F'; 
    else {
      for (int k=0; k<12; ++k) 
        if (p[i] == dirs[k].key1 && p[i-1] == dirs[k].key2) 
          {q[j++] = dirs[k].dir; q[j++] = 'F'; break;}
    }
  }
  q[j++] = 'F'; q[j] = '\0'; free(p); return q;
}

char *getShortestPath(Maze *maze){
  unsigned h = maze->height, w = maze->width;
  char *dirPath = malloc(sizeof(char)*(2*h*w+1));
  Queue q = newQueue(h*w);
  Node start = getStart(maze);
  enqueue(start, &q);
   
  while (!isEmptyQueue(q)) {
    Node n = dequeue(&q);
    for (int i = 0; i < 4; i++) {
      int c = n->col + steps[i].y, r = n->row + steps[i].x;
      Node b = newNode(r,c,n,steps[i].key); 
      if ((b->row == 0 || b->col == w-1 || b->col == 0 
      || b->row == h-1) && maze->bitGraph[b->row][b->col]) {
        Node p; int i = 1; dirPath[0] = b->dir; maze->grid[b->row][b->col] = b->dir;
        while ((p = b->parent) != NULL) 
          {dirPath[i++] = p->dir; maze->grid[p->row][p->col] = p->dir;b = p;}
        dirPath[i] = '\0'; freeQueue(q);   
        return convertPath(dirPath);
      }
      if (isValidNode(b, maze))
        {enqueue(b, &q); maze->bitGraph[b->row][b->col] = 0;}
      else free(b); 
    }
  } 
  freeQueue(q); 
  return NULL; 
}

