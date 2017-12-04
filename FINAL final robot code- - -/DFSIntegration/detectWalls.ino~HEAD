void detectWalls() {
  int testRead;
  //Serial.println("Detect walls");
   wallLeft = false;
   wallRight = false;
   wallMid = false;

    distanceLeft = analogRead(wallPinLeft);
    distanceMid = analogRead(wallPinMid);
    distanceRight = analogRead(wallPinRight);
  for(i=0; i<4; i++){
    testRead = analogRead(wallPinRight); 
    distanceLeft += analogRead(wallPinLeft);
    distanceMid += analogRead(wallPinMid);
    distanceRight += testRead;
  }
//  Serial.print("left:");
//  Serial.println(distanceLeft);
  if (distanceLeft/5 > 150) {
    wallLeft = true;
  }
//  Serial.print("middle:");
//  Serial.println(distanceMid);
  if (distanceMid/5 > 150) {
    wallMid = true;
  }
//  Serial.print("right:");
//  Serial.println(distanceRight);
  if (distanceRight/5 > 200) {
    wallRight = true;

  }
  
//  Serial.print("left:");
//  Serial.println(wallLeft);
//  Serial.print("middle:");
//  Serial.println(wallMid);
//  Serial.print("right:");
//  Serial.println(wallRight);
}


