
int delayTime = 600; //ms;
boolean coordinate;
boolean top;
boolean right = true;
boolean bottom;
boolean left;
int tres = 0 ;
int x = 0;
int y = 0;
int yPrev = 0;
word data;
String dataString;

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9, 10);
unsigned char maze[5][5];


//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000002LL, 0x0000000003LL };

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.
//

// The various roles supported by this sketch
typedef enum { role_ping_out = 1, role_pong_back } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

// The role of the current running sketch
role_e role = role_pong_back;


void setup() {
  Serial.begin(9600);
  dataString = "0000000000000000";
  pinMode(A5, OUTPUT); //most sig: if 1, coord, if 0, wall&tres
  pinMode(A4, OUTPUT); // top wall or x-coor
  pinMode(7, OUTPUT); // left wall or x-coor
  pinMode(6, OUTPUT); // bottom wall or y-coor
  pinMode(5, OUTPUT); // right wall or y-coor
  pinMode(4, OUTPUT); //tres or y-coor
  pinMode(3, OUTPUT); //tres or done
  pinMode(2, OUTPUT); //least sig valid


  coordinate = false;




  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_1MBPS);


  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

}

void loop() {
  if ( radio.available() )
  {
    // Dump the payloads until we've gotten everything
    bool done = false;
    while (!done)
    {
      // Fetch the payload, and see if this was the last one.
      done = radio.read( &data, sizeof(word));
      dataString = disassembleWord(data);
      Serial.println(dataString);
      // printf("Got payload %lu...",maze);

      // Delay just a little bit to let the other unit
      // make the transition to receiver
      delay(20);

    }

    // First, stop listening so we can talk
    radio.stopListening();

    Serial.println("sending back...");

    // Send the final one back.
    radio.write(&data, sizeof(word));


    // Now, resume listening so we catch the next packets.
    radio.startListening();

    if ( Serial.available() )
    {
      char c = toupper(Serial.read());
      role = role_pong_back;
      radio.openWritingPipe(pipes[1]);
      radio.openReadingPipe(1, pipes[0]);
    }
  }

    //xcoord
    
    digitalWrite(A5, HIGH);

    if (dataString.charAt(1) == '0' && dataString.charAt(2) == '0') { //unexplored
      digitalWrite(A4, LOW);
      digitalWrite(7, LOW);
    }
    else if (dataString.charAt(1) == '0' && dataString.charAt(2) == '1') { //unexplored
      digitalWrite(A4, LOW);
      digitalWrite(7, HIGH);
    }
    else if (dataString.charAt(1) == '1' && dataString.charAt(2) == '0') { //unexplored
      digitalWrite(A4, HIGH);
      digitalWrite(7, LOW);
    }
    else { //unexplored
      digitalWrite(A4, HIGH);
      digitalWrite(7, HIGH);
    }


    
    if (dataString.charAt(3) == '0' && dataString.charAt(4) == '0' && dataString.charAt(5) == '0') {
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
    }
    else if (dataString.charAt(3) == '0' && dataString.charAt(4) == '0' && dataString.charAt(5) == '1') {
      digitalWrite(6, LOW);
      digitalWrite(5, LOW);
      digitalWrite(4, HIGH);
    }
    else if (dataString.charAt(3) == '0' && dataString.charAt(4) == '1' && dataString.charAt(5) == '0') {
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
    }
    else if (dataString.charAt(3) == '0' && dataString.charAt(4) == '1' && dataString.charAt(5) == '1') {
      digitalWrite(6, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(4, HIGH);
    }
    else if (dataString.charAt(3) == '1' && dataString.charAt(4)=='0' && dataString.charAt(5) == '0') {
      digitalWrite(6, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(4, LOW);
    }
    
    if (dataString.charAt(6) == '1'){
      digitalWrite(3,HIGH);
    }
    digitalWrite(2, LOW);
    delay(delayTime);
    digitalWrite(2, HIGH);




    digitalWrite(A5, LOW);
    if (dataString.charAt(9) == '1') {
      digitalWrite(A4, HIGH);
    } else {
      digitalWrite(A4, LOW);
    }

    if (dataString.charAt(11) == '1') {
      digitalWrite(7, HIGH);
    } else {
      digitalWrite(7, LOW);
    }
    if (dataString.charAt(12) == '1') {
      digitalWrite(6, HIGH);
    } else {
      digitalWrite(6, LOW);
    }
    if (dataString.charAt(10) == '1') {
      digitalWrite(5, HIGH);
    } else {
      digitalWrite(5, LOW);
    }
    if (dataString.charAt(13) == '0' && dataString.charAt(14) == '1') {
      digitalWrite(4, LOW);
      digitalWrite(3, HIGH);
    }
    else if (dataString.charAt(13) == '1' && dataString.charAt(14) == '0') {
      digitalWrite(4, HIGH);
      digitalWrite(3, LOW);
    }
    else if (dataString.charAt(13) == '1' && dataString.charAt(14) == '1') {
      digitalWrite(4, HIGH);
      digitalWrite(3, HIGH);
    } 
    else {
      digitalWrite(4, LOW);
      digitalWrite(3, LOW);
    }
  

//  if (coordinate) {
//    x ++;
//    if (x == 4) {
//      x = 0;
//      y++;
//    }
//    if (y == 5) {
//      y = 0;
//    }
//    printf(y);
//    coordinate = false;
//  }
//  else {
//    tres++;
//    if (tres == 0) {
//      tres = 1;
//    }
//    if (tres == 4) {
//      tres = 0;
//    }
//    coordinate = true;
//  }




  //printf(y);

  digitalWrite(2, LOW);
  delay(delayTime);
  digitalWrite(2, HIGH);



}

String disassembleWord(word data) {
  String stringToReturn;
  for (int i = 0; i < 16; i++) {
    int bitIndex = 15 - i;
    stringToReturn += String(bitRead(data, bitIndex));
  }
  return stringToReturn;
}
