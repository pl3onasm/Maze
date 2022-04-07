#ifndef PATH_H
#define PATH_H

/* struct definition and function declarations
 * for all functions related to the queue 
 */

#include "path.h"
#include "queue.h"
#include "maze.h"

char *getShortestPath(const Maze *maze); 
char *convertPath(char* p);

#endif