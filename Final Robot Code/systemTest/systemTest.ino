//Libraries

#include <QTRSensors.h> //QTR sensor (line sensor) library
#include <Servo.h> // servo library

int micPin = A0; //microphone connected to analog 0, code currently does this in setup in ADMUX

//detectWalls
int wallPinLeft = A5;
int wallPinMid = A4;
int wallPinRight = A3;

int distanceLeft = 0;
int distanceMid = 0;
int distanceRight = 0;

boolean wallLeft = false;
boolean wallRight = false;
boolean wallMid = false;

//Line Following
Servo servoL;
Servo servoR;

// Change the values below to suit your robot's motors, weight, wheel type, etc.
#define KP 0.1
#define KD 0.3
#define Lspeed 180
#define Rspeed 82
#define ML_MAX_SPEED 180
#define MR_MAX_SPEED 0
#define MIDDLE_SENSOR 2
#define NUM_SENSORS  3     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN    QTR_NO_EMITTER_PIN  // emitter is controlled by digital pin 2
#define DEBUG 0 // set to 1 if serial debug output needed

int leftMotorSpeed = Lspeed;
int rightMotorSpeed = Rspeed;
static int LTurnLw = 56; 
static int LTurnRw = 56; 
static int RTurnLw = 180; 
static int RTurnRw = 170;

int lastError = 0;
int position =0;
int error =0;
unsigned int sensors[3];
boolean isJunction;
boolean sameJunct = false;
int junkSensor =0;

QTRSensorsRC qtrrc((unsigned char[]) {2,3,4} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

//for Fourier Transform
#define LOG_OUT 1 // use the log output function
#define FFT_N 128 // set to 256 point fft
#include <FFT.h> // include the FFT library

void setup() {
  //setup for both FFTs
  Serial.begin(9600); // use the serial port
  
  //setup for line following
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);  

  //distance pins
  pinMode(A5, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  //extra line sensor
  pinMode(A0, INPUT);

  servoL.attach(9);
  servoR.attach(6);

  set_motors(90,90);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    //Serial.println("Calibrating");
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe7; // set the adc to free running mode, changed prescalar to 128
  ADMUX = 0x40; // use adc0: analog A0
  DIDR0 = 0x01; // turn off the digital input for adc0
  
}

void loop() {

//readWallSensors();

//readWallBooleans();

//testLineFollowing();
  Serial.println("loop");
  detectStart();
  
}

void readWallSensors() {
  distanceLeft = analogRead(wallPinLeft);
  distanceMid = analogRead(wallPinMid);
  distanceRight = analogRead(wallPinRight);

  Serial.print("Wall left: ");
  Serial.println(distanceLeft);
  Serial.print("Wall forward: ");
  Serial.println(distanceMid);
  Serial.print("Wall right: ");
  Serial.println(distanceRight);
  delay(500);
}

void readWallBooleans() {
  distanceLeft = analogRead(wallPinLeft);
  distanceMid = analogRead(wallPinMid);
  distanceRight = analogRead(wallPinRight);
  
  if (distanceLeft > 150) {
    wallLeft = true;
  }
  else wallLeft = false;
  
  if (distanceMid > 150) {
    wallMid = true;
  }
  else wallMid = false;
  
  if (distanceRight > 150) {
    wallRight = true;
  }
  else wallRight = false;
  
  Serial.print("Wall left: ");
  Serial.println(wallLeft);
  Serial.print("Wall forward: ");
  Serial.println(wallMid);
  Serial.print("Wall right: ");
  Serial.println(wallRight);
  delay(500);
}

void testLineFollowing() {
  position = qtrrc.readLine(sensors);
  junkSensor = analogRead(A0);
  error = position - 1000;
  Serial.println(error);
  goStraight();
}


