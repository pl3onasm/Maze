#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Maze{
  unsigned height, width, steps; 
  char **grid, **sol; int **bitGraph;} Maze;

typedef struct node *Node;

typedef struct node {
  int row, col; Node parent; char dir;} node;

Node newNode(int row, int col, Node parent, char dir); 

Node getStart(Maze *src); 

Maze *initMaze(); 

void freeMaze (Maze *m); 

#endif