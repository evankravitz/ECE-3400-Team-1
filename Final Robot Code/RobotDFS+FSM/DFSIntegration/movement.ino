void stop(){
  set_motors(90,90);
  delay(500);
}

void goStraight(){
    //Serial.println(position);
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
    if (error > 900 && error < 1100) {
      motorSpeed = 0; 
    }
    leftMotorSpeed = Lspeed + motorSpeed*1.2;
    rightMotorSpeed = Rspeed + motorSpeed;

    //Serial.println(leftMotorSpeed);
    //Serial.println(rightMotorSpeed);
    set_motors(leftMotorSpeed, rightMotorSpeed);
  
}

void junction(){
    if(junkSensor>800 && !sameJunct){
      isJunction = true;
      sameJunct=true;
    }
   else {
    isJunction = false;
   }
}

void turnLeft(){
  //goStraight();
  leftMotorSpeed = LTurnLw;  //delay(200);
  rightMotorSpeed = LTurnRw;    

  set_motors(leftMotorSpeed, rightMotorSpeed);
 
  position = qtrrc.readLine(sensors);
  while(!(analogRead(A0)<650)){
     position = qtrrc.readLine(sensors); 
  }
  while(analogRead(A0)<750){
    position = qtrrc.readLine(sensors); 
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
    if (error > 900 && error < 1100) {
      motorSpeed = 0; 
    }
    leftMotorSpeed = LTurnLw + motorSpeed*1.2;
    rightMotorSpeed = LTurnRw + motorSpeed;

    //Serial.println(leftMotorSpeed);
    //Serial.println(rightMotorSpeed);
    set_motors(leftMotorSpeed, rightMotorSpeed);
    
  }
//  delay(200);
//  position= qtrrc.readLine(sensors);
//    while(!(sensors[1]>900 && analogRead(A0)>800 && sensors[2]<450)){
//     position = qtrrc.readLine(sensors);
//  }
  //isJunction=false;
  
  return;

}

void turnRight(){
 //delay(300);
//  leftMotorSpeed = RTurnLw;
//  rightMotorSpeed = RTurnRw;
//
//  set_motors(leftMotorSpeed, rightMotorSpeed);
//  delay(200);
//  position = qtrrc.readLine(sensors);
//  while(!(sensors[1]>900 && sensors[0]<450 && sensors[2]<450 && analogRead(A0)>800) ){
//     position = qtrrc.readLine(sensors);
//     
//  }
//  

  leftMotorSpeed = RTurnLw;  //delay(200);
  rightMotorSpeed = RTurnRw;  
  isJunction = true;   

  set_motors(leftMotorSpeed, rightMotorSpeed);
 
  position = qtrrc.readLine(sensors);
  while(!(analogRead(A0)<650)){
     position = qtrrc.readLine(sensors); 
  }
  while(analogRead(A0)<750 | sensors[1]<900){
    //Serial.println(sensors[1]);
    position = qtrrc.readLine(sensors); 
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
    if (error > 900 && error < 1100) {
      motorSpeed = 0; 
      }
    
    leftMotorSpeed = RTurnLw + motorSpeed*1.2;
    rightMotorSpeed = RTurnRw + motorSpeed;

    //Serial.println(leftMotorSpeed);
    //Serial.println(rightMotorSpeed);
    set_motors(leftMotorSpeed, rightMotorSpeed);
    
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
  while(analogRead(A0)>700){ 
      goStraight();
   }

  sameJunct = false;
  digitalWrite(13, LOW);
  isJunction = false;
  
  while (!isJunction) {
    position = qtrrc.readLine(sensors);
    junkSensor = analogRead(A0);
    
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
