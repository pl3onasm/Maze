#include "include/maze.h"

Maze *initMaze (){
  Maze *m = malloc(sizeof(Maze));
  unsigned w, h, i=0, j=0;  
  scanf("%u %u\n", &h, &w); 
  m->height = h; m->width = w; m->steps = 0; 
  char **grid = calloc(h, sizeof(char*));
  char **sol = calloc(h, sizeof(char*));
  int **bitGraph = calloc(h, sizeof(int*));
  for (i = 0; i < h; i++) {
    grid[i] = calloc(w+2,sizeof(char));
    sol[i] = calloc(w+2,sizeof(char));
    bitGraph[i] = calloc(w, sizeof(int));
    for (j = 0; j < w; j++) {
      grid[i][j] = sol[i][j] = getchar();
      if (grid[i][j] == ' ') bitGraph[i][j] = 1;
    }
    getchar(); grid[i][j] = '\0'; sol[i][j] = '\0';
  }
  m->grid = grid; m->sol = sol; m->bitGraph = bitGraph;
  return m;
}

void freeMaze (Maze *m) {
  for (unsigned i = 0; i < m->height; i++) {
    free(m->grid[i]); free(m->bitGraph[i]);free(m->sol[i]);}
  free(m->grid); free(m->bitGraph); free(m->sol); free(m);
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


