/** Returns a string of the frequency detected at that junction
 * Either "7kHz", "12kHz", "17kHz", or "no treasure"
 */
//byte detectTreasure() {
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
//      return(B1);
//    }
//     if (detectedFrequency(12E3, fft_log_out)){
//      return(B10);
//    }
//   if (detectedFrequency(17E3, fft_log_out)){
//     return(B100);
//    }
//   else return (B0);
//}
//
//
///* Returns true if freqToDetect is encoded in the fftArray (frequency-domain) signal;
// * Returns false otherwise. 
// */
//boolean detectedFrequency(float freqToDetect, unsigned char fftArray[]){
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
//  
//  for (int i = centralBinIndex+peakIndexWidth+1; i<256; i++){
//    if (abs((int)fftArray[i])>= maximumMag){
//      return false;
//    }
//  }
//
//  return true;
//}

