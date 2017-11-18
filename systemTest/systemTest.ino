

//Libraries
//#include <FFT.h> // include the FFT library
#include <QTRSensors.h> //QTR sensor (line sensor) library
#include <Servo.h> // servo library

//int micPin = A0; //microphone connected to analog 0, code currently does this in setup in ADMUX

//int treasurePin = A0; // treasure connected to analog 0, code does this in setup in ADMUX
long clockFreq = 16E6;
int divisionFactor = 32;
int conversionTime = 13;
int numSamples = 256;
float samplingFrequency = ((clockFreq/((float)divisionFactor))/conversionTime);
float binWidth = samplingFrequency/numSamples;

int position;
int backSensor;

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

unsigned int sensors[3];

// Change the values below to suit your robot's motors, weight, wheel type, etc.
#define KP 0.1
#define KD 0.3
#define Lspeed 180
#define Rspeed 85
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

QTRSensorsRC qtrrc((unsigned char[]) {2,3,4} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  //setup for both FFTs
  Serial.begin(9600); // use the serial port
  
//  TIMSK0 = 0; // turn off timer0 for lower jitter
//  ADCSRA = 0xe7; // set the adc to free running mode, changed prescalar to 128
//  ADMUX = 0x40; // use adc0: analog A0
//  DIDR0 = 0x01; // turn off the digital input for adc0

  //setup for line following
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  

  //distance pins
  pinMode(A5, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  
  //extra line sensor
  pinMode(A0, INPUT);

  servoL.attach(5);
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
  
  int distanceLeft = analogRead(wallPinLeft);
  int distanceMid = analogRead(wallPinMid);
  int distanceRight = analogRead(wallPinRight);
  
  int backSensor = analogRead(A0);
}

void loop() {
  set_motors(0,180);
//  distanceLeft = analogRead(wallPinLeft);
//  distanceMid = analogRead(wallPinMid);
//  distanceRight = analogRead(A3);
//  Serial.print("Wall left: ");
//  Serial.println(distanceLeft);
//  Serial.print("Wall forward: ");
//  Serial.println(distanceMid);
//  Serial.print("Wall right: ");
//  Serial.println(distanceRight);

//  position = qtrrc.readLine(sensors);
//  backSensor = analogRead(A0);
//  Serial.print("junction sensor: ");
//  Serial.println(backSensor);
//  Serial.print("sensor 1: ");
//  Serial.println(sensors[1]); 
//  Serial.print("sensor 2: ");
//  Serial.println(sensors[2]); 
//  Serial.print("sensor 3: ");
//  Serial.println(sensors[3]); 
  
}

void set_motors(int motor1speed, int motor2speed) {
  if (motor1speed > ML_MAX_SPEED ) motor1speed = ML_MAX_SPEED; // limit top speed
  if (motor2speed < MR_MAX_SPEED ) motor2speed = MR_MAX_SPEED; // limit top speed
  servoL.write(motor1speed);     // set motor speed
  servoR.write(motor2speed);     // set motor speed
}
