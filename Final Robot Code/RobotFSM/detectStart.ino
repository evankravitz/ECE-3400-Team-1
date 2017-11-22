boolean detectStart() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
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
    //Serial.println("start");
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      //Serial.println(fft_log_out[i]); // send out the data
    }

    //detects input on bin 19 and returns TRUE if signal is detected
    if (fft_log_out[18] > 120) {
      //digitalWrite(13, HIGH);
      return true;
    }
    else {
      return false;
    }
  }
}
