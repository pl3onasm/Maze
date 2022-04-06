/* 4kyu, Escape simple maze */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct s_Maze{
  unsigned height, width;
  char** grid;} Maze;

typedef struct node *Node;

typedef struct node {
  int row; int col; Node parent; char dir;} node;

typedef struct Queue {
  Node *array; int back; int front; int size;} Queue;

typedef struct {char key; int x; int y;} step;
static step steps[] = {{'^',-1,0},{'v',1,0},{'>',0,1},{'<',0,-1}};

typedef struct {char key1; char key2; char a; char b;} dir;
static dir dirs[] = {
  {'^','>','R','F'},{'^','v','B','F'},{'^','<','L','F'},
  {'v','>','L','F'},{'v','^','B','F'},{'v','<','R','F'},
  {'>','v','R','F'},{'>','^','L','F'},{'>','<','B','F'},
  {'<','v','L','F'},{'<','^','R','F'},{'<','>','B','F'}};

void doubleQueueSize (Queue *qp) {
  int i ;
  int oldSize = qp -> size;
  qp -> size = 2*oldSize;
  qp -> array = realloc (qp -> array, qp -> size*sizeof(node));
  assert (qp -> array != NULL) ;
  for (i =0; i < qp -> back ; i ++) 
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
  Node item ;
  if (isEmptyQueue (*qp)) queueEmptyError ();
  item = qp -> array [qp -> front];
  qp -> front = (qp -> front+1) % qp -> size;
  return item;
}

void freeQueue (Queue q) {
  for (int i=0; i < q.size; i++) free (q.array[i]);
  free (q.array);
}

Node newNode(int row, int col, Node parent, char dir) {
  Node n = malloc (sizeof (node));
  assert (n != NULL);
  n -> row = row; n -> col = col; 
  n -> parent = parent; n -> dir = dir;
  return n;
}

Node getStart(const Maze *src){
  int i, j; char start[] = "<>^v";
  for(i=0; i<src->height; i++) 
    for(j=0; j<src->width; j++) {
      char c = src->grid[i][j]; 
      if(strchr(start,c)) return newNode(i,j,NULL,c); 
    }
}

int isValidNode(Node b, const Maze *maze, int **grid){
  return (b->row >= 0 && b->row < maze->height && b->col >= 0 
          && b->col < maze->width && grid[b->row][b->col]);
}

Maze *loadMaze(Maze *m, unsigned height, unsigned width, const char *cellsStr){
  int i, j; m = malloc(sizeof(Maze));
  m->height = height; m->width = width;
  char **grid = malloc(sizeof(char*) * height);
  for (i = 0; i < height; i++) {
    grid[i] = malloc(sizeof(char) * (width+1));
    for (j = 0; j < width; j++) grid[i][j] = cellsStr[i*width+j];
    grid[i][j] = '\0';
  }
  m->grid = grid;
  return m;
}

int **initBitGraph(const Maze *src){
  int **grid = malloc(sizeof(int*) * src->height); int j; 
  for(int i=0; i<src->height; i++) {
    grid[i] = malloc(sizeof(int) * (src->width+1));
    for(j=0; j<src->width; j++) grid[i][j] = src->grid[i][j] == ' ' ? 1:0;
  }
  return grid; 
}

void freeGraph(int **bitGraph, int len){
  for (int i=0; i<len; i++) free(bitGraph[i]);
  free(bitGraph);
}

char *convertPath(char* p){
  int j=0; char *q = malloc(sizeof(char) * 2*strlen(p));
  for (int i = strlen(p)-1; i>0; i--) {
    if (p[i] == p[i-1]) q[j++] = 'F'; 
    else {
      for (int k=0; k<12; ++k) 
        if (p[i] == dirs[k].key1 && p[i-1] == dirs[k].key2) 
          {q[j++] = dirs[k].a; q[j++] = dirs[k].b; break;}
    }
  }
  q[j++] = 'F'; q[j] = '\0'; free(p); return q;
}

char *getShortestPath(const Maze *maze){
  unsigned h = maze->height, w = maze->width;
  char *path = malloc(sizeof(char)*(2*h*w+1));
  int **bitGraph = initBitGraph(maze); 
  Queue q = newQueue(h*w);
  Node start = getStart(maze);
  enqueue(start, &q);
   
  while (!isEmptyQueue(q)) {
    Node n = dequeue(&q);
    for (int i = 0; i < 4; i++) {
      int c = n->col + steps[i].y; 
      int r = n->row + steps[i].x;
      Node b = newNode(r,c,n,steps[i].key); 
      if ((b->row == 0 || b->col == w-1 || b->col == 0 
      || b->row == h-1) && bitGraph[b->row][b->col]) {
        Node p; int i = 1; path[0] = b->dir; 
        while ((p = b->parent) != NULL) 
          {path[i++] = p->dir; b = p;}
        path[i] = '\0'; freeQueue(q); 
        freeGraph(bitGraph, h);   
        return convertPath(path);
      }
      if (isValidNode(b, maze, bitGraph))
        {enqueue(b, &q); bitGraph[b->row][b->col] = 0;}
      else free(b); 
    }
  } 
  freeQueue(q); freeGraph(bitGraph, h); 
  return NULL; 
}

char *escape(const Maze *maze){
  if(maze->grid == NULL) return NULL;
  char *path = getShortestPath(maze);
  return path;
}

int main(int argc, char *argv[]) {
  Maze *m1;
  m1 = loadMaze(
        m1, 3, 11,
        "# #########" \
        "#        >#" \
        "###########" \
    );
  printf("%s\n", escape(m1));
  return 0;
}


/* TEST CASES 

m1 = loadMaze(
        m1, 3, 11,
        "# #########" \
        "#        >#" \
        "#### ######" \
    );

m1 = loadMaze(
        m1, 3, 10,
        "##########" \
        "#>       #" \
        "######## #" \
    );

m1 = loadMaze(
        m1, 21, 41,
        "#########################################" \
        "#<    #       #     #         # #   #   #" \
        "##### # ##### # ### # # ##### # # # ### #" \
        "# #   #   #   #   #   # #     #   #   # #" \
        "# # # ### # ########### # ####### # # # #" \
        "#   #   # # #       #   # #   #   # #   #" \
        "####### # # # ##### # ### # # # #########" \
        "#   #     # #     # #   #   # # #       #" \
        "# # ####### ### ### ##### ### # ####### #" \
        "# #             #   #     #   #   #   # #" \
        "# ############### ### ##### ##### # # # #" \
        "#               #     #   #   #   # #   #" \
        "##### ####### # ######### # # # ### #####" \
        "#   # #   #   # #         # # # #       #" \
        "# # # # # # ### # # ####### # # ### ### #" \
        "# # #   # # #     #   #     # #     #   #" \
        "# # ##### # # ####### # ##### ####### # #" \
        "# #     # # # #   # # #     # #       # #" \
        "# ##### ### # ### # # ##### # # ### ### #" \
        "#     #     #     #   #     #   #   #    " \
        "#########################################" \
    );

  m1 = loadMaze(
        m1, 9, 9,
        "  #####  " \
        " #     # " \
        "#       #" \
        "#       #" \
        "#   ^   #" \
        " #     # " \
        "  #   #  " \
        "   # #   " \
        "    #    " \
    );  

  m1 = loadMaze(
        m1, 8, 10,
        "##########" \
        "#        #" \
        "#  ##### #" \
        "#  #   # #" \
        "#  #^# # #" \
        "#  ### # #" \
        "#      # #" \
        "######## #" \
    );

*/