unsigned long started_waiting_at;

void recordAndTrasmitDataAtTurningJunction(){
  if (treasure==0){
    detectTreasures();
    char savedCurrPos[2];
    savedCurrPos[0] = currPos[0];   savedCurrPos[1] = currPos[1];
    currPos[0] = prevPos[0]; currPos[1]=prevPos[1];
    recordAndTransmitData();
    //detectTreasures();
    currPos[0] = savedCurrPos[0];  currPos[1] = savedCurrPos[1];
  }


  
}

void recordAndTransmitData(){
  if (doTransmission){
  //  unsigned long startTime = millis();
    String stringToSend = assembleWordString();
    word wordToSend = assembleWord(stringToSend);
    boolean successfullySent = false;
    while (!successfullySent){
      successfullySent = sendPacket(wordToSend);
    }
  }
  
}

word assembleWord(String stringWord){
  word wordToReturn = 0000000000000000;
  for (int i = 0; i<16; i++){
    int bitIndex = 15-i;
    if (stringWord.charAt(i)=='0'){
      bitWrite(wordToReturn, bitIndex, 0);
    }
    else{
      bitWrite(wordToReturn, bitIndex, 1);
    }
  }
  return wordToReturn;
  
} 

String assembleWordString(){
  String xCoordString = assembleXCoord();
  String yCoordString = assembleYCoord();
  String doneString = assembleDoneString();
  String wallsString = assembleWallsString();
  String treasuresString = assembleTreasuresString();
  String indication1 = "1"; String indication2 = "1";
  String valid1 = "1"; String valid2 = "1";
  String stringToSend = indication1+xCoordString+yCoordString+doneString+valid1+indication2+wallsString+treasuresString+valid2;
  return stringToSend;
}



String assembleTreasuresString(){
  String stringToReturn;
  if (treasure==0){ //no treasure
    stringToReturn = "00";
  }
  else if (treasure ==1){ //lowest frequency
    stringToReturn = "01";
  }
  else if (treasure == 2){ //middle frequency
    stringToReturn = "10";
  }
  else if (treasure == 3){ //highest frequency
    stringToReturn = "11";
  }
  return stringToReturn;
}


String assembleWallsString(){
  String stringToReturn = "0000";
  if (maze[currPos[0]][currPos[1]-1] == Wall){ //north wall
    stringToReturn.setCharAt(0, '1');
  }
  if (maze[currPos[0]+1][currPos[1]] == Wall){ //east wall
    stringToReturn.setCharAt(3, '1');
  }
  if (maze[currPos[0]-1][currPos[1]] == Wall){ //west wall
    stringToReturn.setCharAt(1, '1');
  }
  if (maze[currPos[0]][currPos[1]+1] == Wall){ //south wall
    stringToReturn.setCharAt(2, '1');
  }
  return stringToReturn;
}


String assembleXCoord(){
  int xCoord = (currPos[0]-1)/2;
  if (xCoord==0){
    return "00";
  }
  else if (xCoord==1){
    return "01";
  }
  else if (xCoord ==2){
    return "10";
  }
  else{
    return "11";
  }
}

String assembleYCoord(){
  int yCoord = (currPos[1]-1)/2;
  if (yCoord==0){
    return "000";
  }
  else if (yCoord==1){
    return "001";
  }
  else if (yCoord ==2){
    return "010";
  }
  else if (yCoord ==3){
    return "011";
  }
  else {
    return "100";
  }
}

String assembleDoneString(){
  if (done==1){
    return "1";
  }
  else{
    return "0";
  }
}



boolean sendPacket(word data){
  radio.enableDynamicAck();
  radio.stopListening();
  radio.startFastWrite(&data, sizeof(word), 0 , 1);

//  if (!ok){
//    Serial.println("MOMY");
//    return false;
//
//   }
  radio.startListening();
//  unsigned long started_waiting_at = millis();
//  bool timeout = false;
//  while ( ! radio.available() && ! timeout ){
//    if (millis() - started_waiting_at > 200 ){
//       timeout = true;
//    }
//  }
//  if ( timeout ){
//    return false;
//  }
  word recievedData;
  radio.read(&recievedData, sizeof(word));
  if (!recievedData==data){
     //Serial.println("AHAHAH");
     return false;
   }
  return true;
  

  
  

}

