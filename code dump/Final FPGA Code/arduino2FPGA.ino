int delayTime = 600; //ms; 
boolean coordinate; 
boolean top; 
boolean right = true; 
boolean bottom;
boolean left;
int tres =0 ;
int x = 0; 
int y = 0;
int yPrev = 0; 

void setup() {
  Serial.begin(9600); 
pinMode(A5, OUTPUT); //most sig: if 1, coord, if 0, wall&tres
pinMode(A4, OUTPUT); // top wall or x-coor
pinMode(7, OUTPUT); // left wall or x-coor
pinMode(6, OUTPUT); // bottom wall or y-coor
pinMode(5, OUTPUT); // right wall or y-coor
pinMode(4, OUTPUT); //tres or y-coor
pinMode(3, OUTPUT); //tres
pinMode(2, OUTPUT); //least sig valid 
coordinate = false; 

}

void loop() {
  
  if (coordinate) {
    digitalWrite(A5,HIGH);
    
    if (x == 0){ //unexplored
      digitalWrite(A4, LOW);
      digitalWrite(7, LOW);
    }
    else if (x == 1){ //unexplored
      digitalWrite(A4, LOW);
      digitalWrite(7, HIGH);
    }
    else if (x == 2){ //unexplored
      digitalWrite(A4, HIGH);
      digitalWrite(7, LOW);
    }
    else { //unexplored
      digitalWrite(A4, HIGH);
      digitalWrite(7, HIGH);
    }
    if (y == 0) {
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
    }
    else if (y == 1) {
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);
    }
    else if (y == 2) {
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
    }
    else if (y == 3) {
      digitalWrite(6, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(3, HIGH);
    }
    else {
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
    } 
    
  }
 
  
  else { 
    digitalWrite(A5,LOW); 
    if (top) {
      digitalWrite(A4,HIGH);
    } else {
      digitalWrite(A4,LOW);
    }

    if (left) {
      digitalWrite(7,HIGH);
    } else {
      digitalWrite(7,LOW);
    }
    if (bottom) {
      digitalWrite(6,HIGH);
    } else {
      digitalWrite(6,LOW);
    }
    if (right) {
      digitalWrite(5,HIGH);
    } else {
      digitalWrite(5,LOW);
    }
    if (tres == 1) {
      digitalWrite(4,LOW);
      digitalWrite(3,HIGH);
    }
    else if (tres == 2) {
      digitalWrite(4,HIGH);
      digitalWrite(3,LOW);
    }
    else if (tres == 3) {
      digitalWrite(4,HIGH);
      digitalWrite(3,HIGH);
    } else {
      digitalWrite(4,LOW);
      digitalWrite(3,LOW);
    }
    }
    
  if (coordinate){
    x ++; 
    if (x == 4) {
      x=0;
      y++;
    }
    if (y == 5) {
      y = 0;
    }
    Serial.println(y);
    coordinate = false; 
  }
  else {
    tres++; 
    if (tres == 0) {
      tres = 1; 
    }
    if (tres == 4) {
      tres = 0;
    }
    coordinate = true;
  }
  



//Serial.println(y);

digitalWrite(2,LOW);
delay(delayTime);
digitalWrite(2,HIGH);

}
