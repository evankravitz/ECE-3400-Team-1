//define LOG_OUT 1 // use the log output function
//#define FFT_N 128 // set to 256 point fft
//#include "printf.h"
//
//long clockFreq = 16E6;
//int divisionFactor = 32;
//int conversionTime = 13;
//int numSamples = 256;
//float samplingFrequency = ((clockFreq/((float)divisionFactor))/conversionTime);
//float binWidth = samplingFrequency/numSamples;
//
//#include <FFT.h> // include the library
//
//void setup() {
//  Serial.begin(115200); /
//  / use the serial port
//  TIMSK0 = 0; // turn off timer0 for lower jitter
//  ADCSRA = 0xe5; // set the adc to free running mode
//  ADMUX = 0x40; // use adc0
//  DIDR0 = 0x01; // turn off the digital input for adc0
//  pinMode(13, OUTPUT);
//  printf_begin();
//}
//
//
//boolean detectedFrequency(float freqToDetect, char fftArray[]){
//  int peakIndexWidth = 5;
//  int absoluteMinThreshold = 80;
//  int centralBinIndex = int ((float)freqToDetect)/((float)binWidth);
//  int maximumMag = -1;
//  for (int i = centralBinIndex-peakIndexWidth; i<= centralBinIndex+peakIndexWidth; i++){
//    if (abs((int)fftArray[i])>maximumMag){
//      maximumMag = abs((int)fftArray[i]);
//    }
//  }
//  if (maximumMag<absoluteMinThreshold){
//    return false;
//  }
//  
//  for (int i = 30; i<centralBinIndex-peakIndexWidth; i++){
//    if (abs((int)fftArray[i])>= maximumMag){
//      return false;
//    }
//  }
//  for (int i = centralBinIndex+peakIndexWidth+1; i<256; i++){
//    if (abs((int)fftArray[i])>= maximumMag){
//      return false;
//    }
//  }
//
//  return true;
//
//}
//
//
//
//void detectTreasure() {
//  while(1) { // reduces jitter
//    cli();  // UDRE interrupt slows this way down on arduino1.0
//    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
//      while(!(ADCSRA & 0x10)); // wait for adc to be ready
//      ADCSRA = 0xf5; // restart adc
//      byte m = ADCL; // fetch adc data
//      byte j = ADCH;
//      int k = (j << 8) | m; // form into an int
//      k -= 0x0200; // form into a signed int
//      k <<= 6; // form into a 16b signed int
//      fft_input[i] = k; // put real data into even bins
//      fft_input[i+1] = 0; // set odd bins to 0
//    }
//    fft_window(); // window the data for better frequency response
//    fft_reorder(); // reorder the data before doing the fft
//    fft_run(); // process the data in the fft
//    fft_mag_log(); // take the output of the fft
//    sei();
//  
//    if (detectedFrequency(7E3, fft_log_out)){
//     Serial.print("7kHz \n");
//
//     
//    }
//     if (detectedFrequency(12E3, fft_log_out)){
//     Serial.print("12kHz \n");
//     
//    }
//
//   if (detectedFrequency(17E3, fft_log_out)){
//     Serial.print("17kHz \n");
//     
//    }
//  }
//}