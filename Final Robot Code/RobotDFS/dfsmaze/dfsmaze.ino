#include <StackArray.h>
#define Unexplored 0
#define Explored 1
#define Wall 2

#define North 0
#define East 1
#define South 2
#define West 3


void setup(){
  
}


char currentOrientation; //We'll create some way of computing the current orientation later

char maze [9] [11];
char currPos[2]; 
char frontier[20]; 
StackArray <char> visitedStack;
char reachableCells[4];
char wallsRelativeDirs[2];
char possibleWallPosition[2];

void resetMaze() {
  // Resets the arduino's underlying maze to be completely unexplored
  for (int i=0; i<11; i++){
    for (int j= 0; j<9; j++){
      maze[j][i]= Unexplored; 
      
    }
  }
 }




 void loop(){
  resetMaze();
  initializeCurrPos();
  addToFrontier(convertCoordsToChar(currPos));
  visitedStack.push(convertCoordsToChar(currPos));
  while (!frontierIsEmpty()){
    maze[currPos[0]][currPos[1]] = Explored;
    removeFromFrontier(convertCoordsToChar(currPos));
    getReachableCells();
    addWallsToMaze();
    addUnvisitedSurroundingNodesToFrontier();
    updateCurrPosAndVisitedSet();
    if (!frontierIsEmpty()){
      getReachableCells();
      addWallsToMaze();
    }
   
  }
  
  
 }
void getAdjacentWall(char dir){
  possibleWallPosition[0] = currPos[0];
  possibleWallPosition[1] = currPos[1];
  //go straight
  if ((currentOrientation==0 && dir==0) || //facing north, looking in front 
  (currentOrientation==1 && dir==3) || //facing east, looking left
  (currentOrientation==2 && dir==2) || // facing south, looking behind
  (currentOrientation==3 && dir==1)){ // facing west, looking right
    possibleWallPosition[1] = currPos[1] -1;
  }
  //go right
  if ((currentOrientation==0 && dir==1) || //facing north, looking right
  (currentOrientation==1 && dir==0) || //facing east, looking in front
  (currentOrientation==2 && dir==3) || // facing south, looking left
  (currentOrientation==3 && dir==2)){ //facing west, looking behind
    possibleWallPosition[0] = currPos[0] + 1;
  }
  //go backwards
  if ((currentOrientation==0 && dir==2) || //facing north, looking behind
  (currentOrientation==1 && dir==1) || //facing east, looking right
  (currentOrientation==2 && dir==0) || // facing south, looking in front
  (currentOrientation==3 && dir==3)){ // facing west, looking left
    possibleWallPosition[1] = currPos[1]+1;
  }
  //go left
  if ((currentOrientation==0 && dir==3) || //facing north, looking left
  (currentOrientation==1 && dir==2) || //facing east, looking behind
  (currentOrientation==2 && dir==1) || // facing south, looking right
  (currentOrientation==3 && dir==0)){ // facing west, looking in fornt
    possibleWallPosition[0] = currPos[0]-1;
  } 
}
 
void getReachableCells(){
  
}


void updateCurrPosAndVisitedSet(){
  
}

void getWalls(){
  
}

void addWallsToMaze(){
  getWalls(); //this method will be defined somewhere. 
  char wallCoord[2];
  for (int i = 0; i<3; i++){
  
    if (wallsRelativeDirs[i] == 0){  // there's a wall ther
      if (i ==0){
        getAdjacentWall(3);
      }
      if (i==1){
        getAdjacentWall(0);
      }
      else{
        getAdjacentWall(1);
      }
    }
    maze[possibleWallPosition[0]][possibleWallPosition[1]] = Wall;
  }
  
}

void addUnvisitedSurroundingNodesToFrontier(){
  
}
 
 void initializeCurrPos(){
  currPos[0]= 7;
  currPos[1]= 9;
 }


 void addWalls(){
  
 }

 char convertCoordsToChar(char coodinateArray[]){
  if (coodinateArray[0]==1 && coodinateArray[1]==1){
    return (char) 1; 
  }
  if (coodinateArray[0]==3 && coodinateArray[1]==1 ){
    return (char) 2; 
  }
  if (coodinateArray[0]==5 && coodinateArray[1]==1 ){
    return (char) 3; 
  }
  if (coodinateArray[0]==7 && coodinateArray[1]==1 ){
    return (char) 4; 
  }
  if (coodinateArray[0]==1 && coodinateArray[1]==3 ){
    return (char) 5; 
  }
  if (coodinateArray[0]==3 && coodinateArray[1]==3 ){
    return (char) 6; 
  }
  if (coodinateArray[0]==5 && coodinateArray[1]==3 ){
    return (char) 7; 
  }
  if (coodinateArray[0]==7 && coodinateArray[1]==3 ){
    return (char) 8; 
  }
  if (coodinateArray[0]==1 && coodinateArray[1]==5 ){
    return (char) 9; 
  }
  if (coodinateArray[0]==3 && coodinateArray[1]==5 ){
    return (char) 10; 
  }
  if (coodinateArray[0]==5 && coodinateArray[1]==5 ){
    return (char) 11; 
  }
  if (coodinateArray[0]==7 && coodinateArray[1]==5 ){
    return (char) 12; 
  }
  if (coodinateArray[0]==1 && coodinateArray[1]==7 ){
    return (char) 13; 
  }
  if (coodinateArray[0]==3 && coodinateArray[1]==7 ){
    return (char) 14; 
  }
  if (coodinateArray[0]==5 && coodinateArray[1]==7 ){
    return (char) 15; 
  }
  if (coodinateArray[0]==7 && coodinateArray[1]==7 ){
    return (char) 16; 
  }
  if (coodinateArray[0]==1 && coodinateArray[1]==9 ){
    return (char) 17; 
  }
  if (coodinateArray[0]==3 && coodinateArray[1]==9 ){
    return (char) 18; 
  }
  if (coodinateArray[0]==5 && coodinateArray[1]==9 ){
    return (char) 19; 
  }
  if (coodinateArray[0]==7 && coodinateArray[1]==9 ){
    return (char) 20; 
  }
 }

 boolean containsFrontier(char coordinate){
  if (frontier[coordinate-1] ==1){
    return true;
  }
  return false;
 }

 void addToFrontier(char coordinate){
  if (!containsFrontier(coordinate)){
    frontier[coordinate-1] = 1;
  }
 }

 void removeFromFrontier(char coordinate){
  frontier[coordinate-1] = 0;
 }

boolean frontierIsEmpty(){
  for (int i = 0; i<20; i++){
    if (frontier[i] == 1){
      return false;
    }
  }
  return true;
}
  

