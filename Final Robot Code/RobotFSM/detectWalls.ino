/* Returns 3 bits encoding where walls are located
 * 1 if wall, 0 if no wall
 * Wall: [Left, Center, Right]
 * Index:[ 0  ,   1   ,   2  ]
 */
 
void detectWalls() {

  //Serial.println("Detect walls");

  wallLeft = false;
  wallMid = false;
  wallRight = false;

  int distanceLeft = analogRead(wallPinLeft);
  int distanceMid = analogRead(wallPinMid);
  int distanceRight = analogRead(wallPinRight);
//  Serial.print("left:");
//  Serial.println(distanceLeft);
  if (distanceLeft > 100) {
    wallLeft = true;
  }
//  Serial.print("middle:");
//  Serial.println(distanceMid);
  if (distanceMid > 100) {
    wallLeft = true;
  }
//  Serial.print("right:");
//  Serial.println(distanceRight);
  if (distanceRight > 100) {
    wallRight = true;
  }
  
  Serial.print("left:");
  Serial.println(wallLeft);
  Serial.print("middle:");
  Serial.println(wallMid);
  Serial.print("right:");
  Serial.println(wallRight);
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

