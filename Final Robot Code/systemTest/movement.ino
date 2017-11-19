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

//    Serial.println(leftMotorSpeed);
//    Serial.println(rightMotorSpeed);
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
 
void set_motors(int motor1speed, int motor2speed) {
  if (motor1speed > ML_MAX_SPEED ) motor1speed = ML_MAX_SPEED; // limit top speed
  if (motor2speed < MR_MAX_SPEED ) motor2speed = MR_MAX_SPEED; // limit top speed
  servoL.write(motor1speed);     // set motor speed
  servoR.write(motor2speed);     // set motor speed
}
