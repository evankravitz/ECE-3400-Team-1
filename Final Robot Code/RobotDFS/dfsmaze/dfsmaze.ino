#include <StackArray.h>
#define Unexplored 0
#define Explored 1
#define Wall 2

#define North 0
#define East 1
#define South 2
#define West 3

#define Left 3 
#define Right 1
#define Straight 0
#define Backwards 2



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
char prevPos[2];
char moveToPerform;


void resetMaze() {
  // Resets the arduino's underlying maze to be completely unexplored
  for (int i=0; i<11; i++){
    for (int j= 0; j<9; j++){
      maze[j][i]= Unexplored; 
      
    }
  }
 }




 void loop(){
  prevPos[0] = currPos[0];
  prevPos[1] = currPos[1];
  resetMaze();
  initializeCurrPos();
  addToFrontier(convertCoordsToChar(currPos));
  visitedStack.push(convertCoordsToChar(currPos));
  while (!frontierIsEmpty()){
    maze[currPos[0]][currPos[1]] = Explored;
    removeFromFrontier(convertCoordsToChar(currPos));
    addWallsToMaze();
    getReachableCells();
    addUnvisitedSurroundingNodesToFrontier();
    updateCurrPosAndVisitedSet();
    if (!frontierIsEmpty()){
      addWallsToMaze();
      getReachableCells();
      addUnvisitedSurroundingNodesToFrontier();
    }
   updateMove();
   performMove();
  }
  doneWithNavigation();
  
  
 }

 void performMove(){
  
 }

void doneWithNavigation(){
  
}

void updateMove(){
  int dx = currPos[0] - prevPos[0];
  int dy = currPos[1] - prevPos[1];
  if ((dx == 2 && currentOrientation==East) //displace east, facing east
     || (dy == -2 && currentOrientation==North) //displace north, facing north
     || (dx == -2 && currentOrientation==West) //displace west, facing west
     || (dy == 2 && currentOrientation==South)){      //displace south, facing south
        moveToPerform = Straight;
  }
  else if ((dy == 2  && currentOrientation==East) //displace south, facing east
     || (dx == -2 && currentOrientation==South) //displace west, facing south
     || (dy == -2 && currentOrientation==West) //displace north, facing west
     || (dx == 2 && currentOrientation==North)) //displace east, facing north
     {
        moveToPerform = Right;
  }
    else if ((dy == 2  && currentOrientation==North) //displace south, facing north
     || (dx == -2 && currentOrientation==East) //displace west, facing east
     || (dy == -2 && currentOrientation==South) //displace north, facing south
     || (dx == 2 && currentOrientation==West)) //displace east, facing west
     {
        moveToPerform = Backwards;
  }
  else{
    moveToPerform == Left;
  }

  

  

  
}


char generateNewDirection(char currOrientation, char displacementOrientation){
  //go north
  if ((currOrientation==North && displacementOrientation==Straight) || //facing north, looking in front 
  (currOrientation==East && displacementOrientation==Left) || //facing east, looking left
  (currOrientation==South && displacementOrientation==Backwards) || // facing south, looking behind
  (currOrientation==West && displacementOrientation==Right)){ // facing west, looking right
    return North;
  }
  //go east
  if ((currOrientation==North && displacementOrientation==Right) || //facing north, looking right
  (currOrientation==East && displacementOrientation==Straight) || //facing east, looking in front
  (currOrientation==South && displacementOrientation==Left) || // facing south, looking left
  (currOrientation==West && displacementOrientation==Backwards)){ //facing west, looking behind
    return East;
  }
  //go south
  if ((currOrientation==North && displacementOrientation==Backwards) || //facing north, looking behind
  (currOrientation==East && displacementOrientation==Right) || //facing east, looking right
  (currOrientation==South && displacementOrientation==Straight) || // facing south, looking in front
  (currOrientation==West && displacementOrientation==Left)){ // facing west, looking left
    return South;
  }
  //go west
  if ((currOrientation==North && displacementOrientation==Left) || //facing north, looking left
  (currOrientation==East && displacementOrientation==Backwards) || //facing east, looking behind
  (currOrientation==South && displacementOrientation==Right) || // facing south, looking right
  (currOrientation==West && displacementOrientation==Straight)){ // facing west, looking in fornt
    return West;
  } 
  
}
void getReachableCells(){
  getAdjacentWall(0); //in front
  if (maze[possibleWallPosition[0]][possibleWallPosition[1]] != Wall){
    char reachableCellArray[2]; 
    reachableCellArray[0] = currPos[0] + 2*(possibleWallPosition[0] - currPos[0]);
    reachableCellArray[1] = currPos[0] + 2*(possibleWallPosition[1] - currPos[1]);
    reachableCells[0] = convertCoordsToChar(reachableCellArray);
  }
  getAdjacentWall(1); //right
  if (maze[possibleWallPosition[0]][possibleWallPosition[1]] != Wall){
    char reachableCellArray[2]; 
    reachableCellArray[0] = currPos[0] + 2*(possibleWallPosition[0] - currPos[0]);
    reachableCellArray[1] = currPos[0] + 2*(possibleWallPosition[1] - currPos[1]);
    reachableCells[1] = convertCoordsToChar(reachableCellArray);
  }
  getAdjacentWall(2); //left
  if (maze[possibleWallPosition[0]][possibleWallPosition[1]] != Wall){
    char reachableCellArray[2]; 
    reachableCellArray[0] = currPos[0] + 2*(possibleWallPosition[0] - currPos[0]);
    reachableCellArray[1] = currPos[0] + 2*(possibleWallPosition[1] - currPos[1]);
    reachableCells[2] = convertCoordsToChar(reachableCellArray);
  }
  reachableCells[3] = convertCoordsToChar(prevPos);
  
  
}
void getAdjacentWall(char dir){
  possibleWallPosition[0] = currPos[0];
  possibleWallPosition[1] = currPos[1];
  char directionToGo = generateNewDirection(currentOrientation, dir);
  //go straight
  if (directionToGo == North){ 
    possibleWallPosition[1] = currPos[1] -1;
  }
  //go right
  if (directionToGo == East){
    possibleWallPosition[0] = currPos[0] + 1;
  }
  //go backwards
  if (directionToGo == South ){ 
    possibleWallPosition[1] = currPos[1]+1;
  }
  //go left
  if (directionToGo == West){ 
    possibleWallPosition[0] = currPos[0]-1;
  } 
}
 



void updateCurrPosAndVisitedSet(){
  boolean foundUnexplored = false;
  char newCurrPos[2];
  for (int i = 0; i<4; i++){
    if (reachableCells[i]!=0){
      //conversion between int coordinates to array coordinates
      int ypos = (reachableCells[i]-1)/4;
      int xpos = (reachableCells[i]-1)%4;
      if (maze[xpos][ypos] == Explored){
        foundUnexplored = true;
        newCurrPos[0] = xpos; newCurrPos[1] = ypos;
        break;
      }
    }
  }
  if (!foundUnexplored){
    char lastVisited = visitedStack.pop();
    int newXpos = (lastVisited-1)/4;
    int newYpos = (lastVisited-1)%4;
    newCurrPos[0] = newXpos;
    newCurrPos[1] = newYpos;
  }
  else{
    visitedStack.push(convertCoordsToChar(currPos));
  }
  prevPos[0] = currPos[0];
  prevPos[1] = currPos[1];
  currPos[0] = newCurrPos[0];
  currPos[1] = newCurrPos[1];
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
  for (int i = 0; i<4; i++){
    if (reachableCells[i]!=0){
       int ypos = (reachableCells[i]-1)/4;
       int xpos = (reachableCells[i]-1)%4;
       if (maze[xpos][ypos] == Unexplored){
          addToFrontier(reachableCells[i]);
       }
   }
  }
}
 void initializeCurrPos(){
  currPos[0]= 7;
  currPos[1]= 9;
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
  

