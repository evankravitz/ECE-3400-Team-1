void stop(){
  set_motors(90,90);
  delay(500);
}

void goStraight(){
    //Serial.println(position);
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
//    if (error > 900 && error < 1100) {
//      motorSpeed = 0; 
//    }
    leftMotorSpeed = Lspeed + motorSpeed*1.2;
    rightMotorSpeed = Rspeed + motorSpeed;

    //Serial.println(leftMotorSpeed);
    //Serial.println(rightMotorSpeed);
    set_motors(leftMotorSpeed, rightMotorSpeed);
  
}

void junction(){
    if(junkSensor==HIGH && !sameJunct){
      isJunction = true;
      sameJunct=true;
    }
   else {
    isJunction = false;
   }
}

void turnLeft(){

  leftMotorSpeed = LTurnLw;  //delay(200);
  rightMotorSpeed = LTurnRw;    

  set_motors(leftMotorSpeed, rightMotorSpeed);
 
  position = qtrrc.readLine(sensors);
  while(!(digitalRead(7)==LOW)){
     position = qtrrc.readLine(sensors); 
  }
  while(digitalRead(7)==LOW && sensors[1] < 900){
    position = qtrrc.readLine(sensors); 
  }
  set_motors(90,90);
  delay(25);
  if(digitalRead(7)==LOW){
    while(digitalRead(7)==LOW) goStraight();
  }
  
  return;

}

void turnRight(){

  leftMotorSpeed = RTurnLw;  //delay(200);
  rightMotorSpeed = RTurnRw;  
  isJunction = true;   

  set_motors(leftMotorSpeed, rightMotorSpeed);
 
  position = qtrrc.readLine(sensors);
  while(!(digitalRead(7)==LOW)){
     position = qtrrc.readLine(sensors); 
  }
  while(digitalRead(7)==LOW | sensors[1]<900){
    position = qtrrc.readLine(sensors); 
  }

  return;

}
 


void set_motors(int motor1speed, int motor2speed) {
  if (motor1speed > ML_MAX_SPEED ) motor1speed = ML_MAX_SPEED; // limit top speed
  if (motor2speed < MR_MAX_SPEED ) motor2speed = MR_MAX_SPEED; // limit top speed
  servoL.write(motor1speed);     // set motor speed
  servoR.write(motor2speed);     // set motor speed
}


void performMove(){
  currentOrientation = generateNewDirection(currentOrientation, moveToPerform);
  if (moveToPerform == Left){
    //Serial.println("Moving Left!");
    moveLeft();
  }
  if (moveToPerform == Right){
   // Serial.println("Moving Right!");

    moveRight();
  }
  if (moveToPerform == Straight){
   // Serial.println("Moving Straight!");

    moveStraight();
  }
  if (moveToPerform == Backwards){
  // Serial.println("Moving Backwards!");

    moveBackwards();
  }
  if (moveToPerform == Stop){
      stop();
  }
 }

void moveLeft(){
 turnLeft();
 stop();
 moveStraight();
}

void moveRight(){
  turnRight();
  stop();
  moveStraight();
}

void moveBackwards(){
  turnLeft();
  stop();
  turnLeft();
  stop();
  moveStraight();
}

void moveStraight(){
//  if analogRead(A0
  while(digitalRead(7)==HIGH){ 
      goStraight();
   }

  sameJunct = false;
  digitalWrite(13, LOW);
  isJunction = false;
  
  while (!isJunction) {
    position = qtrrc.readLine(sensors);
    junkSensor = digitalRead(7);
    
    error = position - 1000;
    junction();
    if (isJunction) {
      set_motors(90,90);
      digitalWrite(13, HIGH);
      
    }
    else  {
      goStraight();
    }
  }
}
