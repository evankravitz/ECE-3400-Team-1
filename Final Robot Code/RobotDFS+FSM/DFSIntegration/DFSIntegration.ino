
//Definitions
//for Fourier Transform
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

//Libraries
//#include <FFT.h> // include the FFT library
#include <QTRSensors.h> //QTR sensor (line sensor) library
#include <Servo.h> // servo library
#include <StackArray.h>

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

//int distanceLeft = 0;
//int distanceMid = 0;
//int distanceRight = 0;

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

int lastError = 0;
int position =0;
int error =0;
unsigned int sensors[3];
boolean isJunction;
boolean sameJunct = false;
int junkSensor =0;

QTRSensorsRC qtrrc((unsigned char[]) {2,3,4} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

///// Everything above is from the FSM code ^^^^

#include <StackArray.h>
#define Unexplored 0
#define Explored 1
#define Wall 2

#define North 0
#define East 1
#define South 2
#define West 3

#define Left 3 
#define Right 1
#define Straight 0
#define Backwards 2

char currentOrientation; //We'll create some way of computing the current orientation later
char maze [9] [11];
char currPos[2]; 
char frontier[20]; 
StackArray <char> visitedStack;
char reachableCells[4];
char wallsRelativeDirs[2];
char possibleWallPosition[2];
char prevPos[2];
char moveToPerform;

void setup(){
  Serial.begin(9600); // use the serial port
  //wall pins
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  
  pinMode(A0, INPUT); //junction sensor
  //distance pins
  pinMode(A5, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

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
}

void loop(){
  prevPos[0] = currPos[0];
  prevPos[1] = currPos[1];
  resetMaze();
  initializeCurrPos();
  initializeOrientation();
  addToFrontier(convertCoordsToChar(currPos));
  visitedStack.push(convertCoordsToChar(currPos));
  
  
  while (!frontierIsEmpty()){
    maze[currPos[0]][currPos[1]] = Explored;
    removeFromFrontier(convertCoordsToChar(currPos));
    addWallsToMaze();
    getReachableCells();
    addUnvisitedSurroundingNodesToFrontier();
    updateCurrPosAndVisitedSet();
    if (!frontierIsEmpty()){
      addWallsToMaze();
      getReachableCells();
      addUnvisitedSurroundingNodesToFrontier();
    }
   updateMove();
   performMove();
  }
  
  doneWithNavigation();
}

