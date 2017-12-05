bool detectStart() {
    char tempADCSRA = ADCSRA;
    char tempTIMSK0 = TIMSK0; // turn off timer0 for lower jitter
    char tempADMUX = ADMUX; // use adc0: analog A0
    char tempDIDR0 = DIDR0;
  while(1) { // reduces jitter
    
    ADCSRA = 0xe7;
    TIMSK0 = 0; // turn off timer0 for lower jitter
    ADMUX = 0x40; // use adc0: analog A0
    DIDR0 = 0x01; // turn off the digital input for adc0
    
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 256 ; i += 2) { // save 128 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf7; // restart adc
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
//  Serial.println("start");
//    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
//      Serial.println(fft_log_out[i]); // send out the data
//    }
    ADCSRA = tempADCSRA;
    TIMSK0 = tempTIMSK0; // turn off timer0 for lower jitter
    ADMUX = tempADMUX; // use adc0: analog A0
    DIDR0 = tempDIDR0;
    //detects input on bin 10 and returns TRUE if signal is detected
    
    if (fft_log_out[9] > 140) {
      return true;
    }
    else {
      return false;
    }
  }
}

bool detectBetterStart() {
  servoL.detach();
  servoR.detach();
  int startFreq = 0;
  for (int k = 0; k<30 ; k++) {
    if (detectStart()) {
      startFreq = startFreq+1;
      //Serial.println(startFreq);
    }
  }
  servoL.attach(3);
  servoR.attach(6);
  if (startFreq > 20) {
    return true;
  }
  else {
    return false;
  }

}

bool detectButton() {
  int buttonState = digitalRead(buttonPin);
  
  if (buttonState == HIGH) {
    return true;
  } else {
    return false;
  } 
}

