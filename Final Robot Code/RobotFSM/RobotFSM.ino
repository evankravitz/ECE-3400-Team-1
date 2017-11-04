
/* State machine for all robot functions
 */
enum State {
  START, JUNCTION, BETWEEN, DONE
};

enum Direction {
  UP, DOWN, LEFT, RIGHT, UND
};

//Definitions
//for Fourier Transform
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

//Libraries
//#include <FFT.h> // include the FFT library
#include <QTRSensors.h> //QTR sensor (line sensor) library
#include <Servo.h> // servo library

//Global Variables
State state; //initializes state enum variable
Direction dir; //initializes direction enum variable

//detectStart
//int micPin = A0; //microphone connected to analog 0, code currently does this in setup in ADMUX

//detectTreasure
//int treasurePin = A0; // treasure connected to analog 0, code does this in setup in ADMUX
long clockFreq = 16E6;
int divisionFactor = 32;
int conversionTime = 13;
int numSamples = 256;
float samplingFrequency = ((clockFreq/((float)divisionFactor))/conversionTime);
float binWidth = samplingFrequency/numSamples;

//detectWalls
int wallPinLeft = A5;
int wallPinMid = A4;
int wallPinRight = A3;
int wallPinArray[3] = {wallPinLeft, wallPinMid, wallPinRight}; //array of pins used for wall detection
char maze[9][11];


//Line Following
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

QTRSensorsRC qtrrc((unsigned char[]) {2,3,4} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

void setup() {
  //Initializes the robot into START state
  state = START;
  
  //setup for both FFTs
  Serial.begin(9600); // use the serial port
  Serial.println("setup");
  //pinMode(13, OUTPUT); //sets pin 13 (built in LED) as an output
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe7; // set the adc to free running mode, changed prescalar to 128
  ADMUX = 0x40; // use adc0: analog A0
  DIDR0 = 0x01; // turn off the digital input for adc0

  //setup for line following
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  

  servoL.attach(5);
  servoR.attach(6);

  set_motors(90,90);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
//  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
//  {
//    Serial.println("Calibrating");
//    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
//  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
 

  //instantiateMaze();
 Serial.print("sup");
}

void loop() {
  Serial.println("LOOP");
  //START state: waits until startSignal returns TRUE, then enters JUNCTION state
  if (state == START) {
    Serial.println("START");
    //if (detectStart()) {
      state = JUNCTION;
    //}
  }

  //JUNCTION state: detects walls and treasures, chooses next direction to move
  if (state == JUNCTION) {
    Serial.println("JUNCTION");
    //byte treasure = detectTreasure(); //gets a string for treasure at junction
    byte wallData = detectWalls(); //gets 3 bit of where walls are located
    Serial.println(wallData);

    //updateBaseStation();
    //theMap = updateMap();
    //Direction dir = chooseDirection(Map); //chooses direction to move based on wall array

    if (bitRead(wallData,0) == 0){
      turnRight();
    }
    else if (bitRead(wallData,1) == 0) {
      state == BETWEEN;
    }
    else if (bitRead(wallData,2) == 0) {
      turnLeft();
    }
    else {
      turnRight();
    }
    
  }

  //BETWEEN state: follows a line until it reaches the next junction

  if (state == BETWEEN) {
    Serial.println("BETWEEN");
    position = qtrrc.readLine(sensors);
    error = position - 1000;
    junction();
    if (isJunction) {
      state == JUNCTION;
    }
    else  {
    goStraight();
    }
  }
  
  //DONE state: robot has completed task
}

