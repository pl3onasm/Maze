#ifndef PATH_H
#define PATH_H

#include "queue.h"
#include "maze.h"

char *getShortestPath(Maze *maze); 
char *convertPath(char* p);

typedef struct {char key; int x, y;} step;
static step steps[] = {{'^',-1,0},{'v',1,0},{'>',0,1},{'<',0,-1}};

typedef struct {char key1, key2, dir;} dir;
static dir dirs[] = {
  {'^','>','R'},{'^','v','B'},{'^','<','L'},
  {'v','>','L'},{'v','^','B'},{'v','<','R'},
  {'>','v','R'},{'>','^','L'},{'>','<','B'},
  {'<','v','L'},{'<','^','R'},{'<','>','B'}};

#endif