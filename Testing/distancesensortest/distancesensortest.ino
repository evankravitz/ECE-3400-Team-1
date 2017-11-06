int value=0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(A3,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  value= analogRead(A3); 
  Serial.println(value);
  if (value>100){ // this would be a little bit closer than 15 cm to any wall.
    //stop at junction 
    //turn at junction 
  } else {
   //go straight
  }
  delay(500);
}
