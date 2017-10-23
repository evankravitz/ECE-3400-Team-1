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

int lastError = 0;
int  last_proportional = 0;
int integral = 0;


void loop()
{
  unsigned int sensors[3];
  int position = qtrrc.readLine(sensors);
  int error = position - 1000;

  if(sensors[0]>800 && sensors[1] >800 &&sensors[2] >800){
    int leftMotorSpeed = 90;
    int rightMotorSpeed = 90;
    set_motors(leftMotorSpeed, rightMotorSpeed);
    delay(200);
  }
  else{
    int motorSpeed = KP * error + KD * (error - lastError);
  //  Serial.print(position);
  //  Serial.print('\t');
  //  Serial.println(motorSpeed);
    lastError = error;
    if (error > 900 && error < 1100) {
      motorSpeed = 0; 
    }
    
    int leftMotorSpeed = Lspeed + motorSpeed*1.2;
    int rightMotorSpeed = Rspeed + motorSpeed;
     set_motors(leftMotorSpeed, rightMotorSpeed);
  }
  // set motor speeds using the two motor speed variables above
  Serial.print(position);
  Serial.print('\t');
 
}

void set_motors(int motor1speed, int motor2speed)
{
  if (motor1speed > ML_MAX_SPEED ) motor1speed = ML_MAX_SPEED; // limit top speed
  if (motor2speed < MR_MAX_SPEED ) motor2speed = MR_MAX_SPEED; // limit top speed
  if (motor1speed < 90) motor1speed = 90; // keep motor above 0
  if (motor2speed > 90) motor2speed = 90; // keep motor speed above 0
  Serial.print(motor1speed);
  Serial.print('\t');
  Serial.println(motor2speed);
  servoL.write(motor1speed);     // set motor speed
  servoR.write(motor2speed);     // set motor speed
}
