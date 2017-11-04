/* Returns 3 bits encoding where walls are located
 * 1 if wall, 0 if no wall
 * Wall: [Left, Center, Right]
 * Index:[ 0  ,   1   ,   2  ]
 */
 
byte detectWalls() {
  byte wallData = 0;
  int wallArray[3];

  for (int i = 0; i < 3; i ++) {    
    wallArray[i] = isWall(wallPinArray[i]);
  }
 
  if (wallArray[2]==1){
    bitSet(wallData,0);
  } if (wallArray[1]==1){
    bitSet(wallData,1);
  } if (wallArray[0]==1){
    bitSet(wallData,2);
  }

  return wallData;
}

/* Returns 1 if wall, 0 if no wall
 * Argument wallPin refers to specified distance sensor pin 
 * that is mapped to analog inputs in wallPinArray
 */
int isWall(int wallPin) {
  int distanceValue = analogRead(wallPin);
  Serial.print(distanceValue);
  if (distanceValue > 100) {
    return 1;
  }
  else return 0;
}

