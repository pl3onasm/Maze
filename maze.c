/* The returned path is guaranteed to be the shortest one */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct Maze{
  unsigned height, width; char** grid; int **bitGraph;} Maze;

typedef struct node *Node;

typedef struct node {
  int row, col; Node parent; char dir;} node;

typedef struct Queue {
  Node *array; int back, front, size;} Queue;

typedef struct {char key; int x, y;} step;
static step steps[] = {{'^',-1,0},{'v',1,0},{'>',0,1},{'<',0,-1}};

typedef struct {char key1, key2, dir;} dir;
static dir dirs[] = {
  {'^','>','R'},{'^','v','B'},{'^','<','L'},
  {'v','>','L'},{'v','^','B'},{'v','<','R'},
  {'>','v','R'},{'>','^','L'},{'>','<','B'},
  {'<','v','L'},{'<','^','R'},{'<','>','B'}};

void doubleQueueSize (Queue *qp) {
  int oldSize = qp -> size;
  qp -> size = 2*oldSize;
  qp -> array = realloc (qp -> array, qp -> size*sizeof(node));
  assert (qp -> array != NULL) ;
  for (int i =0; i < qp -> back ; i ++) 
    {qp -> array [oldSize + i] = qp -> array [i];}
  qp -> back = qp -> back + oldSize; 
}

Queue newQueue (int s) {
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
  for (int i=0; i < q.size; i++) free (q.array[i]);
  free (q.array);
}

Node newNode(int row, int col, Node parent, char dir) {
  Node n = malloc (sizeof (node)); assert (n != NULL);
  n -> row = row; n -> col = col; 
  n -> parent = parent; n -> dir = dir;
  return n;
}

Node getStart(Maze *src){
  unsigned i, j; char c, start[] = "<>^v";
  for(i=0; i<src->height; i++) 
    for(j=0; j<src->width; j++) {
      c = src->grid[i][j]; 
      if(strchr(start,c)) goto jump;
    }
  jump:; return newNode(i,j,NULL,c); 
}

int isValidNode(Node b, Maze *maze){
  return (b->row >= 0 && b->row < maze->height && b->col >= 0 
          && b->col < maze->width && maze->bitGraph[b->row][b->col]);
}

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

Maze *initMaze (){
  Maze *m = malloc(sizeof(Maze));
  unsigned w, h; int i=0, j=0;  
  scanf("%u %u\n", &h, &w); 
  m->height = h; m->width = w;
  char **grid = calloc(h, sizeof(char*));
  int **bitGraph = calloc(h, sizeof(int*));
  for (i = 0; i < h; i++) {
    grid[i] = calloc(w+2,sizeof(char));
    bitGraph[i] = calloc(w, sizeof(int));
    for (j = 0; j < w; j++) {
      grid[i][j] = getchar(); 
      if (grid[i][j] == ' ') bitGraph[i][j] = 1;
    }
    getchar(); grid[i][j] = '\0';
  }
  m->grid = grid; m->bitGraph = bitGraph;
  return m;
}

void freeMaze (Maze *m) {
  for (int i = 0; i < m->height; i++) {
    free(m->grid[i]); free(m->bitGraph[i]);}
  free(m->grid); free(m->bitGraph); free(m);
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

void print(char *path, Maze *maze){
  if (path == NULL) {printf("No path found\n"); return;}
  printf("Maze with path:\n\n");
  for (int i = 0; i < maze->height; i++) {
    for (int j = 0; j < maze->width; j++) 
      printf("%c", maze->grid[i][j]);
    printf("\n");
  }
  printf("\nPath with directions:");
  for (int i=0; i < strlen(path); i++) {
    if (!(i%maze->width)) printf("\n");
    printf("%c", path[i]);
  }
}

int main(int argc, char *argv[]) {
  Maze *maze = initMaze(maze); 
  char *path = getShortestPath(maze);
  print(path, maze);
  freeMaze(maze); free(path);
  return 0;
}
