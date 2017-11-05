void stop(){
  set_motors(90,90);
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
    if((sensors[0]>900 && sensors[1] >900 &&sensors[2] >900)){
      isJunction = true;
    }
   else isJunction = false;
}

void turnLeft(){
  //goStraight();
  state = BETWEEN;
  delay(300);
  leftMotorSpeed = LTurnL;  //delay(200);
  rightMotorSpeed = LTurnR;    

  set_motors(leftMotorSpeed, rightMotorSpeed);
  delay(350);
  position = qtrrc.readLine(sensors);
  while(!(sensors[1]>900 && sensors[2]<600 )){
     position = qtrrc.readLine(sensors);
  }
  isJunction=false;
  
  goStraight();
  return;

}

void turnRight(){
  state=BETWEEN;
  delay(300);
  leftMotorSpeed = RTurnL;
  rightMotorSpeed = RTurnR;

  set_motors(leftMotorSpeed, rightMotorSpeed);
  delay(350);
  position = qtrrc.readLine(sensors);
  while(!(sensors[1]>900 && sensors[0]<450 && sensors[2]<450 )){
     position = qtrrc.readLine(sensors);
     
  }
  isJunction=false;
  
  goStraight();
  return;
}

void set_motors(int motor1speed, int motor2speed) {
  if (motor1speed > ML_MAX_SPEED ) motor1speed = ML_MAX_SPEED; // limit top speed
  if (motor2speed < MR_MAX_SPEED ) motor2speed = MR_MAX_SPEED; // limit top speed
  servoL.write(motor1speed);     // set motor speed
  servoR.write(motor2speed);     // set motor speed
}
