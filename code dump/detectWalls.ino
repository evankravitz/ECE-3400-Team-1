void detectWalls() {

  //Serial.println("Detect walls");
   wallLeft = false;
   wallRight = false;
   wallMid = false;

//  for(int i=0; i<3; i++){
     distanceLeft = analogRead(wallPinLeft);
     distanceMid = analogRead(wallPinMid);
     distanceRight = analogRead(wallPinRight);
//  }
 
//  Serial.print("left:");
//  Serial.println(distanceLeft);
  if (distanceLeft > 150) {
    wallLeft = true;
  }
//  Serial.print("middle:");
//  Serial.println(distanceMid);
  if (distanceMid > 150) {
    wallMid = true;
  }
//  Serial.print("right:");
//  Serial.println(distanceRight);
  if (distanceRight > 150) {
    wallRight = true;
  }
  
//  Serial.print("left:");
//  Serial.println(wallLeft);
//  Serial.print("middle:");
//  Serial.println(wallMid);
//  Serial.print("right:");
//  Serial.println(wallRight);
}
