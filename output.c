#include "include/path.h"
#include "include/queue.h"
#include "include/maze.h"
#include <stdio.h>
#include <stdlib.h>

void print(char *path, Maze *maze){
  if (path == NULL) {printf("No path found\n"); return;}
  printf("Initial problem:\n\n");
  for (int i = 0; i < maze->height; i++) {
    for (int j = 0; j < maze->width; j++) {
      printf("%c", maze->grid[i][j]);
    }
    printf("\n");
  }
  printf("\nMaze with path:\n\n");
  for (int i = 0; i < maze->height; i++) {
    for (int j = 0; j < maze->width; j++) 
      printf("%c", maze->sol[i][j]);
    printf("\n");
  }
  printf("\nPath with directions:");
  for (int i=0; i < strlen(path); i++) {
    if (!(i%maze->width)) printf("\n");
    printf("%c", path[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  Maze *maze = initMaze(maze); 
  char *path = getShortestPath(maze);
  print(path, maze);
  freeMaze(maze); free(path);
  return 0;
}
