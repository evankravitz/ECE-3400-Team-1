int sevenKHZTreasure; int twelveKHZTreasure; int seventeenKHZTreasure; int noTreasure;

void detectTreasures(){
  readTreasures();
  selectLargestTreasureAndUpdateTreasureVariables();
}


void readTreasures(){
   int noTreasure = 0; int sevenKHZTreasure = 0; int twelveKHZTreasure = 0; int seventeenKHZTreasure = 0;
   while (noTreasure<=10 && sevenKHZTreasure<=10 && twelveKHZTreasure<=10 && seventeenKHZTreasure<=10){
    char freqDetected =  updateFFTArrayAndFindFrequency();

     
     if (freqDetected==7){
       sevenKHZTreasure++;
      }
      
     else if (freqDetected==12){
       twelveKHZTreasure++; 
      }
  
     else if (freqDetected==17){
       seventeenKHZTreasure++; 
      }
  
     else {
      noTreasure++;
     }
   }
}

void selectLargestTreasureAndUpdateTreasureVariables(){
  if  (noTreasure==11){
    treasure = 0;
  }
  else if (sevenKHZTreasure==11){
    treasure = 1;
  }
  else if (twelveKHZTreasure==11){
    treasure = 2;
  }
  else{
    treasure = 3;
  }
  
}



char updateFFTArrayAndFindFrequency(){
    char tempADCSRA = ADCSRA;
    char tempTIMSK0 = TIMSK0; // turn off timer0 for lower jitter
    char tempADMUX = ADMUX; // use adc0: analog A0
    char tempDIDR0 = DIDR0;

    
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
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

boolean detectedFrequency(float freqToDetect, char fftArray[]){
  int peakIndexWidth = 5;
  int absoluteMinThreshold = 0;
  int centralBinIndex = int ((float)freqToDetect)/((float)binWidth);
  int maximumMag = -1;

  for (int i = centralBinIndex-peakIndexWidth; i<= centralBinIndex+peakIndexWidth; i++){
    if (abs((int)fftArray[i])>maximumMag){
      maximumMag = abs((int)fftArray[i]);

      
    }
  }
  if (maximumMag<absoluteMinThreshold){
    return false;
  }
  
  for (int i = 30; i<centralBinIndex-peakIndexWidth; i++){
    if (abs((int)fftArray[i])>= maximumMag){
      return false;
    }
  }
  for (int i = centralBinIndex+peakIndexWidth+1; i<256; i++){
    if (abs((int)fftArray[i])>= maximumMag){
      return false;
    }
  }

  return true;

}





