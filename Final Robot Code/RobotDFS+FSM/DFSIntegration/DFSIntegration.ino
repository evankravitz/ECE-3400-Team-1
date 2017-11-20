//Radio Stuff:
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0x0000000002LL, 0x0000000003LL };
// The various roles supported by this sketch


// The role of the current running sketch

//for Fourier Transform
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft
#include <FFT.h> // include the FFT library


//Libraries
#include <QTRSensors.h> //QTR sensor (line sensor) library
#include <Servo.h> // servo library
#include <StackArray.h>

//detectStart
//int micPin = A0; //microphone connected to analog 0, code currently does this in setup in ADMUX

//detectTreasure
//int treasurePin = A0; // treasure connected to analog 0, code does this in setup in ADMUX
const long clockFreq = 16E6;
const int divisionFactor = 32;
const int conversionTime = 13;
const int numSamples = 256;
const float samplingFrequency = ((clockFreq/((float)divisionFactor))/conversionTime);
const float binWidth = samplingFrequency/numSamples;

char treasure = 0;

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

QTRSensorsRC qtrrc((unsigned char[]) {2,5,4} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

//start detection things
bool startDFS = false;
const int buttonPin = 8;
int buttonState = 0;

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
#define Stop 4

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
char done = 0;


void setup(){
  
  //radio setup:
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_1MBPS);

  radio.openWritingPipe(pipes[0]);
 radio.openReadingPipe(1,pipes[1]);
   radio.startListening();


  
  Serial.begin(9600); // use the serial port
  Serial.println("setup");
  //servo pins
  pinMode(3, OUTPUT);
  pinMode(6, OUTPUT);  

  //junction sensor
  pinMode(7, INPUT);
  
  //wall detection pins
  pinMode(A5, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);

    //microphone
  pinMode(A0, INPUT);

  servoL.attach(3);
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
  //start on 660 Hz Tone OR Button Press
   while(!startDFS) {
      set_motors(90,90);
      delay(25);
      startDFS = detectStart();
      startDFS |= detectButton();
  }
  //turns light on to tell that we started
  digitalWrite(13, HIGH);
  set_motors(90,90);
  delay(500);
  digitalWrite(13, LOW);
  
  recordAndTransmitData();
  detectWalls();
  detectTreasures();
  prevPos[0] = currPos[0];
  prevPos[1] = currPos[1];
  resetMaze();
  initializeCurrPos();
  initializeOrientation();
  addToFrontier(convertCoordsToChar(currPos));
  visitedStack.push(convertCoordsToChar(currPos));  

  //printMaze();
//  Serial.print("current position x"); Serial.println((int)currPos[0]);
//  Serial.print("current position y"); Serial.println((int)currPos[1]);
  while (true){
    //Serial.print("current position x"); Serial.println((int)currPos[0]);
   // Serial.print("current position y"); Serial.println((int)currPos[1]);
    detectWalls();
    detectTreasures();
    recordAndTransmitData();
 
    maze[currPos[0]][currPos[1]] = Explored;
    removeFromFrontier(convertCoordsToChar(currPos));
    addWallsToMaze();
    //printMaze();
    getReachableCells();
    addUnvisitedSurroundingNodesToFrontier();
   // printFrontier();
    updateCurrPosAndVisitedSet();
      if (frontierIsEmpty()){
        addWallsToMaze();
        getReachableCells();
        addUnvisitedSurroundingNodesToFrontier();
        doneWithNavigation();
     }
   updateMove();
   performMove();
  }

////Figure eight bc lol it's never too late to do milestone 1 
//moveStraight();
//moveRight();
//moveLeft(); 
//moveLeft();
//moveLeft();
//moveLeft();
//moveRight();
//moveRight(); 
//turnRight();

}
