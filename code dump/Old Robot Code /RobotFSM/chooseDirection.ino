Direction chooseDirection(int wallArray[]){
  Direction directionChosen = UND; //initialized output direction as undefined

  //go straight if no walls in front
  if (wallArray[1] = 0) {
    directionChosen = UP;
  }

  //else go left if no wall to the left
  else if (wallArray[0] = 0) {
    directionChosen = LEFT;
  }

  //else go right if no wall to the right
  else if (wallArray[2] = 0) {
    directionChosen = RIGHT;
  }
  
  //else turn around
  else {
    directionChosen = DOWN;
  }

  return directionChosen;
}

