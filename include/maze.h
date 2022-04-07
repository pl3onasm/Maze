#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "queue.h"
#include "path.h"

typedef struct Maze{
  unsigned height, width;
  char** grid;} Maze;

typedef struct node *Node;

typedef struct node {
  int row, col; Node parent; char dir;} node;

Node newNode(int row, int col, Node parent, char dir); 

Node getStart(const Maze *src); 

int isValidNode(Node b, const Maze *maze, int **grid); 

Maze *initMaze(Maze *m, unsigned height, unsigned width, const char *cellsStr); 

int **initBitGraph(const Maze *src); 

void freeGraph(int **bitGraph, int len); 

#endif