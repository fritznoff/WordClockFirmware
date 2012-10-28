#define EE_WC_BR 0x20

uint16_t currentStates[8] = { 0 };
uint16_t newStates[8] = { 0 };
uint8_t wordClockBrightness = 0;

void wordClockInit() {
 
  Wire.begin();
  
  delay(200);
  
  for(uint8_t i=0; i<8; i++) {
    setLed(i, currentStates[i], 0x00);
  }
  wordClockBrightness = eeprom_read_byte((uint8_t *) EE_WC_BR); eeprom_busy_wait();
  setPwm(wordClockBrightness, 0x00);
}

void testAll() {
  setPwm(255, 0x00);
  for(uint8_t i=0; i<8; i++) {
    setLed(i, 0b1111111111111111, 0x00);
  }
  delay(5000);
}

void wordClockSetBrightness(uint8_t brightness){
  wordClockBrightness = brightness;
  setPwm(wordClockBrightness, 0x00);
}

void wordClockSaveBrightness(){
  eeprom_write_byte((uint8_t *)  EE_WC_BR, wordClockBrightness);  eeprom_busy_wait();
}

void testPwm() {
  setPwm(0, 0x00);
  setPwm(255, 0x01);
  for(uint8_t i=0;i<8;i++) {
    setLed(i, 0xFFFF, 0b1010101010101010);
  }
  for(uint8_t i=0;i<255;i++) {
    setPwm(i,0x00);
    setPwm(254-i, 0x01);
    delay(20);
  }
  delay(2000);
}


/*
setLed sets the states of the leds (led) of a specific ic (ic) 





*/

void setLed (uint8_t ic, uint16_t led, uint16_t pwm) {
  uint8_t pwm0 = 0b00000010;
  uint8_t pwm1 = 0b00000011;
  uint8_t off = 0b00000001;
  
  uint8_t addressMask = 0b01100000;
  uint8_t address = addressMask ^ ic;
  
  uint8_t ls[] = { 0b00000110, 0b00000111, 0b00001000, 0b00001001 };
  
  uint8_t leds[] = { 0b00000000, 0b00000000, 0b00000000, 0b00000000 };
  
  for (uint8_t i=0; i<4; i++) {
    uint16_t regMask = 0b0000000000001111;
    regMask = regMask << (i*4);
    
    uint16_t active = led & regMask;
    active = active >> (i*4);
    
    uint16_t pwmMode = pwm & regMask;
    pwmMode = pwmMode >> (i*4);
    
    for (uint8_t j=0; j<4; j++) {
      uint8_t ledMask = 0b00000001;
      ledMask = ledMask << j;   
      if ((ledMask & active) > 0) {
        if((ledMask & pwmMode) > 0){
          leds[i] = leds[i] ^ (pwm1 << (2*j));
        }
        else{
          leds[i] = leds[i] ^ (pwm0 << (2*j));
        }
      }
      else {
        leds[i] = leds[i] ^ (off << (2*j));
      }
    }
  }
 
  for (uint8_t i=0; i<4; i++) {
    Wire.beginTransmission(address);
    
    Wire.write(ls[i]);
    Wire.write(leds[i]);
    
    Wire.endTransmission();
  }
}

void setPwm (uint8_t brightness, uint8_t pwmNo) {
  uint8_t addressMask = 0b01100000;
  
  uint8_t pwmReg;
  if(pwmNo == 0)
    pwmReg = 0b00000011;
  else if(pwmNo == 1)
    pwmReg = 0b00000101;
  
  for (uint8_t i=0; i<8; i++) {
    uint8_t address = addressMask ^ i;
    
    Wire.beginTransmission(address);
      
    Wire.write(pwmReg);
    Wire.write(255-brightness);
      
    Wire.endTransmission();
  }
}

void wordClockDisplayTime(time_t time) {
  for(uint8_t i=0; i<8; i++) {
    newStates[i] = 0;
  }
  
  showES();
  showIST();
  
  
  uint8_t minutes = minute(time);
  uint8_t hours = hour(time);
  if(minutes >= 25) hours++;
  hours = hours % 12;
  uint8_t dots = minutes % 5;
  minutes -= dots;
  
  switch (dots) {
    case 0: break;
    case 4: newStates[3] ^= 0b1000000000000000;
    case 3: newStates[7] ^= 0b1000000000000000;
    case 2: newStates[4] ^= 0b1000000000000000;
    case 1: newStates[0] ^= 0b1000000000000000; break;
  }
  
  switch (hours) {
    case 0: showZWOELF(); break;
    case 1: {
      if(minutes != 0) showEINS();
      else showEIN();
    } break;
    case 2: showZWEI(); break;
    case 3: showDREI(); break;
    case 4: showVIER(); break;
    case 5: showHourFUENF(); break;
    case 6: showSECHS(); break;
    case 7: showSIEBEN(); break; 
    case 8: showACHT(); break;
    case 9: showNEUN(); break;
    case 10: showHourZEHN(); break;
    case 11: showELF(); break;
  }
  
  switch (minutes) {
    case 0: showUHR(); break;
    case 5: {
      showMinuteFUENF();
      showNACH();
      break;
    }
    case 10: {
      showMinuteZEHN();
      showNACH();
      break;
    }
    case 15: {
      showVIERTEL();
      showNACH();
      break;
    }
    case 20: {
      showZWANZIG();
      showNACH();
      break;
    }
    case 25: {
      showMinuteFUENF();
      showVOR();
      showHALB();
      break;
    }
    case 30: showHALB(); break;
    case 35: {
      showMinuteFUENF();
      showNACH();
      showHALB();
      break;
    }
    case 40: {
      showZWANZIG();
      showVOR();
      break;
    }
    case 45: {
      showVIERTEL();
      showVOR();
      break;
    }
    case 50: {
      showMinuteZEHN();
      showVOR();
      break;
    }
    case 55: {
      showMinuteFUENF();
      showVOR();
      break;
    }
  }
  
  
  uint16_t diff[8] = { 0 };
  uint16_t fadeIn[8] = { 0 };
  uint16_t fadeOut[8] = { 0 };
  bool fading = false;
  
  for(uint8_t i=0; i<8; i++) {
    diff[i] = newStates[i] ^ currentStates[i];
    fadeOut[i] = diff[i] & currentStates[i];
    fadeIn[i] = diff[i] & newStates[i];
    if(diff[i] != 0)
      fading = true;
  }
  
  if(fading) {    
    //Debug-Ausgabe
    /*Serial.print("fucking fading!\r\n");
      for(uint8_t i=0;i<8;i++) {
        Serial.print("currentStates["); Serial.print(i); Serial.print("]:\t"); Serial.print(currentStates[i], BIN); Serial.print("\r\n");
        Serial.print("newStates["); Serial.print(i); Serial.print("]:\t\t"); Serial.print(newStates[i], BIN); Serial.print("\r\n");
        Serial.print("diff["); Serial.print(i); Serial.print("]:\t\t"); Serial.print(diff[i], BIN); Serial.print("\r\n");
        Serial.print("fadeOut["); Serial.print(i); Serial.print("]:\t\t"); Serial.print(fadeOut[i], BIN); Serial.print("\r\n");
        Serial.print("fadeIn["); Serial.print(i); Serial.print("]:\t\t"); Serial.print(fadeIn[i], BIN); Serial.print("\r\n");
      }*/

    for(uint8_t i=0;i<8;i++) {
      setLed(i, currentStates[i], fadeOut[i]);
    }
    for(uint8_t i=wordClockBrightness;i>0;i--) {
      setPwm(i-1, 0x01);
      delay(10);
    }
    for(uint8_t i=0;i<8;i++) {
      setLed(i, newStates[i], fadeIn[i]);
      currentStates[i] = newStates[i];
    }
    for(uint8_t i=0;i<wordClockBrightness;i++) {
      setPwm(i, 0x01);
      delay(10);
    }
  }
  else {
    for(uint8_t i=0;i<8;i++) {
      setLed(i, newStates[i], 0x0000);
    }
  }
}

void showES() {
  newStates[0] ^= 0b0110000000000000;
}

void showIST() {
  newStates[0] ^= 0b0000111000000000;
}

void showVOR() {
  newStates[5] ^= 0b0000000000111000;
}

void showNACH() {
  newStates[1] ^= 0b0000111100000000;
}

void showHALB() {
  newStates[1] ^= 0b0000000000001111;
}

void showUHR() {
  newStates[7] ^= 0b0111000000000000;
}

void showHourFUENF() {
  newStates[6] ^= 0b0000001111000000;
}

void showMinuteFUENF() {
  newStates[4] ^= 0b0000000000001111;
}

void showHourZEHN() {
  newStates[3] ^= 0b0000100000000000;
  newStates[7] ^= 0b0000000000111000;
}

void showMinuteZEHN() {
  newStates[0] ^= 0b0000000011110000;
}

void showVIERTEL() {
  newStates[1] ^= 0b0011000000000000;
  newStates[4] ^= 0b0110000000000000;
  newStates[5] ^= 0b0000000000000111;
}

void showZWANZIG() {
  newStates[0] ^= 0b0000000100001000;
  newStates[4] ^= 0b0000011111000000;
}

void showEIN() {
  newStates[2] ^= 0b0001110000000000;
}

void showEINS() {
  newStates[2] ^= 0b0001111000000000;
}

void showZWEI() {
  newStates[2] ^= 0b0111100000000000;
}

void showDREI() {
  newStates[2] ^= 0b0000000001111000;
}

void showVIER() {
  newStates[6] ^= 0b0100000000000000;
  newStates[7] ^= 0b0000000000000111;
}

void showSECHS() {
  newStates[3] ^= 0b0000000000111110;
}

void showSIEBEN() {
  newStates[2] ^= 0b0000001000000000;
  newStates[6] ^= 0b0000000000011111;
}

void showACHT() {
  newStates[3] ^= 0b0000011101000000;
}

void showNEUN() {
  newStates[3] ^= 0b0111000000000000;
  newStates[6] ^= 0b0010000000000000;
}

void showELF() {
  newStates[2] ^= 0b0000000000000111;
}

void showZWOELF() {
  newStates[1] ^= 0b0000000000100000;
  newStates[5] ^= 0b0011110000000000;
}
