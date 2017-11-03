#include <QTRSensors.h>
#include <Servo.h>

#include <Servo.h>
Servo servoL;
Servo servoR;

// Change the values below to suit your robot's motors, weight, wheel type, etc.
#define KP 0.1
#define KD 0.3
#define Lspeed 180
#define Rspeed 83
#define ML_MAX_SPEED 180
#define MR_MAX_SPEED 0
#define MIDDLE_SENSOR 2
#define NUM_SENSORS  3     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN    QTR_NO_EMITTER_PIN  // emitter is controlled by digital pin 2
#define DEBUG 0 // set to 1 if serial debug output needed

int leftMotorSpeed = Lspeed;
int rightMotorSpeed = Rspeed;
static int LTurnL = 56; 
static int LTurnR = 56; 
static int RTurnL = 180; 
static int RTurnR = 170;

int lastError = 0;
int position =0;
int error =0;
unsigned int sensors[3];
boolean isJunction;
boolean passed = false;

//IMPROVEMENTS? range of no correction 900-1100? different KP and KD for neg/pos error? percent correction tracking? 

QTRSensorsRC qtrrc((unsigned char[]) {11,10,9} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup()
{
    Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);  

  servoL.attach(12);
  servoR.attach(6);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
 
  set_motors(90,90);
}


void loop()
{
  position = qtrrc.readLine(sensors);
  error = position - 1000;

  junction();
  if(!isJunction) goStraight();
  else{
    turnRight();
  }
}

void stop(){
  set_motors(90,90);
}

void goStraight(){
 
    int motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;
    if (error > 900 && error < 1100) {
      motorSpeed = 0; 
    }
    leftMotorSpeed = Lspeed + motorSpeed*1.2;
    rightMotorSpeed = Rspeed + motorSpeed;
    set_motors(leftMotorSpeed, rightMotorSpeed);
  
}

void junction(){
    if((sensors[0]>800 && sensors[1] >800 &&sensors[2] >800)){
      isJunction = true;
    }
   // else isJunction = false;
}

void turnLeft(){
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
  return;

}

void turnRight(){
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
  return;
}

void set_motors(int motor1speed, int motor2speed)
{
  if (motor1speed > ML_MAX_SPEED ) motor1speed = ML_MAX_SPEED; // limit top speed
  if (motor2speed < MR_MAX_SPEED ) motor2speed = MR_MAX_SPEED; // limit top speed
  servoL.write(motor1speed);     // set motor speed
  servoR.write(motor2speed);     // set motor speed
}
