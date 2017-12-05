#include <StackArray.h>
#define Unexplored 0
#define Explored 1


#define North 0
#define East 1
#define South 2
#define West 3


#ifndef DFS_h
#define DFS_h

#include "Arduino.h"


class DFS{

  public:
  char currentOrientation;
  DFS();
  char currPos[2]; 
  char frontier[20]; 
  StackArray <char> visitedStack;
  void resetMaze();
  void initializeCurrPos();
  void addWalls();
  char convertCoordsToChar(char coodinateArray[]);
  boolean containsFrontier(char coordinate);
  void addToFrontier(char coordinate);
  void removeFromFrontier(char coordinate);
  boolean frontierIsEmpty();
};

#endif
