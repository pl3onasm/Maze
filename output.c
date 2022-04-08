#include "include/path.h"
#include "include/maze.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print(char *path, Maze *maze, double time){
  if (path == NULL) {printf("No path found\n"); return;}
  printf("==< Solution found in %f s, and in %u steps >==\n\n", time, maze->steps);
  printf("Initial problem:\n\n");
  for (unsigned i = 0; i < maze->height; i++) {
    for (unsigned j = 0; j < maze->width; j++) 
      printf("%c", maze->grid[i][j]);
    printf("\n");
  }
  printf("\nMaze with path:\n\n");
  for (unsigned i = 0; i < maze->height; i++) {
    for (unsigned j = 0; j < maze->width; j++) 
      printf("%c", maze->sol[i][j]);
    printf("\n");
  }
  printf("\nPath with directions:");
  for (unsigned i=0; i < strlen(path); i++) {
    if (!(i%maze->width)) printf("\n");
    printf("%c", path[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  Maze *maze = initMaze(maze); 
  clock_t start, end;
  start = clock();
  char *path = getShortestPath(maze);
  end = clock();
  double t = (double)(end - start) / CLOCKS_PER_SEC;
  print(path, maze, t);
  freeMaze(maze); free(path);
  return 0;
}
