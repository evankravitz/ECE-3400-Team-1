/*
fft_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

char clockFreq = 16E6;
char divisionFactor = 32;
char conversionTime = 13;
char numSamples = 256;
float samplingFrequency = clockFreq/(divisionFactor/conversionTime);
float binWidth = samplingFrequency/numSamples;

#include <FFT.h> // include the library

void setup() {
  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  pinMode(13, OUTPUT);
}

void start(){
  digitalWrite(13, HIGH);
}

boolean detectedFrequency(float freqToDetect, char fftArray[]){
  int peakIndexWidth = 3;
  Serial.print("Freq to detect: %f \n", freqToDetect);
  Serial.print("Bin width = %d \n", binWidth);
  int centralBinIndex = int ((float)freqToDetect)/((float)binWidth);
  Serial.print("central bin index = %d \n", centralBinIndex);
  char maximumMag = -1;
  for (int i = centralBinIndex-peakIndexWidth; i<= centralBinIndex+peakIndexWidth; i++){
    if (fftArray[i]>maximumMag){
      maximumMag = fftArray[i];
    }
  }
  for (int i = 20; i<centralBinIndex-peakIndexWidth; i++){
    if (fftArray[i]>= maximumMag){
      return false;
    }
  }
  for (int i = centralBinIndex+peakIndexWidth+1; i<256; i++){
    if (fftArray[i]>= maximumMag){
      return false;
    }
  }
  return true;

}



void loop() {
  while(1) { // reduces jitter
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
//    Serial.println("start");
//    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
//      Serial.println(fft_log_out[i]); // send out the data
//    }
    
    if (detectedFrequency(7E3, fft_log_out)){
      //Serial.print("7kHz \n");
    }
//    if (detectedFrequency(12E3, fft_log_out)){
//      Serial.print("12kHz \n");
//    }
//    if (detectedFrequency(17E3, fft_log_out)){
//      Serial.print("17kHz \n");
//    }
//    
  }
}
