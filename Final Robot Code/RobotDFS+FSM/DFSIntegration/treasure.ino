
void detectTreasures(){
   
//  Serial.println(binWidth);
//  Serial.println("reading treasures");
  boolean successfullyRecordFrequency = false;
  while (!successfullyRecordFrequency){
    noTreasure1 = 0;  sevenKHZTreasure1 = 0;  twelveKHZTreasure1 = 0;  seventeenKHZTreasure1 = 0;
    noTreasure2 = 0;  sevenKHZTreasure2 = 0;  twelveKHZTreasure2 = 0;  seventeenKHZTreasure2 = 0;
    readTreasures();
    successfullyRecordFrequency=selectLargestTreasureAndUpdateTreasureVariables();
  }


}


void readTreasures(){


   for (int k = 0; k<30; k++){
   
    int freqDetected1 =  updateFFTArrayAndFindFrequencyDetector1();
    int freqDetected2 =  updateFFTArrayAndFindFrequencyDetector2();

  //  Serial.println("k= "); Serial.print(k);


     

     
     if (freqDetected1==7){
       sevenKHZTreasure1++;
      }
      
     else if (freqDetected1==12){
       twelveKHZTreasure1++; 
      }
  
     else if (freqDetected1==17){
       seventeenKHZTreasure1++; 
      }
  
     else if (freqDetected1 == 0) {
      noTreasure1++;
     }
     



     if (freqDetected2==7){
       sevenKHZTreasure2++;
      }
     
     else if (freqDetected2==12){
       twelveKHZTreasure2++; 
      }
  
     else if (freqDetected2==17){
       seventeenKHZTreasure2++; 
      }
  
     else if (freqDetected2 == 0) {
      noTreasure2++;
    }

     
   }
}

int selectLargestFrequency(char detector){
  if (detector==1){
    if (noTreasure1>= sevenKHZTreasure1 && noTreasure1>= twelveKHZTreasure1 && noTreasure1>=seventeenKHZTreasure1){
      return 0;
    }
    else if (sevenKHZTreasure1>=noTreasure1 && sevenKHZTreasure1>=twelveKHZTreasure1 && sevenKHZTreasure1>=seventeenKHZTreasure1){
      return 7;
    }
    else if (twelveKHZTreasure1>=noTreasure1 && twelveKHZTreasure1>=sevenKHZTreasure1 && twelveKHZTreasure1>=seventeenKHZTreasure1){
      return 12;
    }
    else{
      return 17;
    }
  }
  else{
    if (noTreasure2>= sevenKHZTreasure2 && noTreasure2>= twelveKHZTreasure2 && noTreasure2>=seventeenKHZTreasure2){
      return 0;
    }
    else if (sevenKHZTreasure2>=noTreasure2 && sevenKHZTreasure2>=twelveKHZTreasure2 && sevenKHZTreasure2>=seventeenKHZTreasure2){
      return 7;
    }
    else if (twelveKHZTreasure2>=noTreasure2 && twelveKHZTreasure2>=sevenKHZTreasure2 && twelveKHZTreasure2>=seventeenKHZTreasure2){
      return 12;
    }
    else{
      return 17;
    }
  }
}

boolean selectLargestTreasureAndUpdateTreasureVariables(){
  int largestFreq1 = selectLargestFrequency(1);
  int largestFreq2 = selectLargestFrequency(2);

//  Serial.println("noTreasure1: "); Serial.print(noTreasure1);
//  Serial.println("sevenKHZTreasure1: "); Serial.print(sevenKHZTreasure1);
//  Serial.println("twelveKHZTreasure1: "); Serial.print(twelveKHZTreasure1);
//  Serial.println("seventeenKHZTreasure1: "); Serial.print(seventeenKHZTreasure1);
//
//  //
//  
//  Serial.println("noTreasure2: "); Serial.print(noTreasure2);
//  Serial.println("sevenKHZTreasure2: "); Serial.print(sevenKHZTreasure2);
//  Serial.println("twelveKHZTreasure2: "); Serial.print(twelveKHZTreasure2);
//  Serial.println("seventeenKHZTreasure2: "); Serial.print(seventeenKHZTreasure2);


  if (largestFreq1==0 && largestFreq2==0){
    treasure = 0;
    return true;
  }
  else if ((largestFreq1 == 7 && largestFreq2==0) || (largestFreq2 == 7 && largestFreq1==0)){
    treasure = 1;
    return true;
  }
  else if ((largestFreq1 == 12 && largestFreq2==0) || (largestFreq2 == 12 && largestFreq1==0)){
    treasure = 2;
    return true;
  }
  else if ((largestFreq1 == 17 && largestFreq2==0) || (largestFreq2 == 17 && largestFreq1==0)){
    treasure = 3;
    return true;
  }
  else{
    return false;
  }

  
}


int updateFFTArrayAndFindFrequencyDetector1(){

  //set_motors(90,90);
    char tempADCSRA = ADCSRA;
    char tempTIMSK0 = TIMSK0; // turn off timer0 for lower jitter
    char tempADMUX = ADMUX; // use adc0: analog A0
    char tempDIDR0 = DIDR0;

    servoL.detach();
    servoR.detach();
    
    ADCSRA = 0xe5;
    set_motors(90,90);
    TIMSK0 = 0; // turn off timer0 for lower jitter
    ADMUX = B01000010; // A2--analog pin 2
    DIDR0 = 0x01; // turn off the digital input for adc0
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 256 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
    
    ADCSRA = tempADCSRA;
    TIMSK0 = tempTIMSK0; // turn off timer0 for lower jitter
    ADMUX = tempADMUX; // use adc0: analog A0
    DIDR0 = tempDIDR0;
  servoL.attach(3);
  servoR.attach(6);

   if (detectedFrequency(7E3, fft_log_out)){
       return 7;
    }
      
     else if (detectedFrequency(12E3, fft_log_out)){
       return 12; 
      }
  
     else if (detectedFrequency(17E3, fft_log_out)){
       return 17; 
      }
  
     else {
      return 0;
     }

  
}
int updateFFTArrayAndFindFrequencyDetector2(){

  //set_motors(90,90);
  
    char tempADCSRA = ADCSRA;
    char tempTIMSK0 = TIMSK0; // turn off timer0 for lower jitter
    char tempADMUX = ADMUX; // use adc0: analog A0
    char tempDIDR0 = DIDR0;

    servoL.detach();
    servoR.detach();
    
    ADCSRA = 0xe5;

    TIMSK0 = 0; // turn off timer0 for lower jitter
    ADMUX = B01000001; // A2--analog pin 2
    DIDR0 = 0x01; // turn off the digital input for adc0

    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 256 ; i += 2) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    sei();
//    
    ADCSRA = tempADCSRA;
    TIMSK0 = tempTIMSK0; // turn off timer0 for lower jitter
    ADMUX = tempADMUX; // use adc0: analog A0
    DIDR0 = tempDIDR0;
   
   servoL.attach(3);
  servoR.attach(6);
   if (detectedFrequency(7E3, fft_log_out)){
       return 7;
    }
      
     else if (detectedFrequency(12E3, fft_log_out)){
       return 12; 
      }
  
     else if (detectedFrequency(17E3, fft_log_out)){
       return 17; 
      }
  
     else {
      return 0;
     }
}

boolean detectedFrequency(float freqToDetect, uint8_t fftArray[]){
  
  int peakIndexWidth = 5;
  int absoluteMinThreshold = 150;
  int centralBinIndex = int ((float)freqToDetect)/((float)binWidth);
  int maximumMag = -1;
  

  for (int i = centralBinIndex-peakIndexWidth; i<= centralBinIndex+peakIndexWidth; i++){
    if ((int)fftArray[i]>maximumMag){
      maximumMag = fftArray[i];

      
    }
  }
//
//  Serial.println("Maximum magnitude: "); Serial.print(maximumMag);
//  Serial.println("Detector 1");
//  for (int i = 0; i<128; i++){
//        Serial.print(" ");
//        Serial.print((int) fftArray[i]);
//     }
//     
  

//  if (maximumMag>158){
//    return true;
//  }
//  return false;
//// //Serial.println("Maximum magnitude: "); Serial.print(maximumMag);
////  
////
////  
  if (maximumMag<absoluteMinThreshold){
    return false;
  }
  
  for (int i = 18; i<centralBinIndex-peakIndexWidth; i++){
    if ((int)fftArray[i]>= maximumMag){
      return false;
    }
  }
  for (int i = centralBinIndex+peakIndexWidth+1; i<62; i++){
    if ((int)fftArray[i]>= maximumMag){
      return false;
    }
  }

  return true;

}





