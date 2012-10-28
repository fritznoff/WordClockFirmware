
String serialBuffer = "";

void serial(){
  serialRead();
}



void serialRead(){
  while(Serial.available()){
    
    char inChar = Serial.read();
    serialBuffer.concat(inChar);
    
    //Check for 'carriage return' as an end of a command
    if(inChar == '\r') {
      serialParse(serialBuffer);
      serialBuffer = "";
    }
  }
}

void serialParse(String theString){
    if(theString.startsWith("get time")){
      time_t foobar = WiFly.getTime();
      if(foobar > 100000)
        setTime(foobar);
    }
    else if(theString.startsWith("set brightness")){
      char number[3];
      for(int i = 0; i < 3; i++){
        number[i] = theString.charAt(15 + i);
      }
      int brightness = atoi(number);
      wordClockSetBrightness(brightness);
    }
    else if(theString.startsWith("set time")) {
      char u_timestamp[11] = { '0' };
      for(int i = 0; i < 10; i++){
        char c = theString.charAt(9 + i);
        if( c >= '0' && c <= '9'){   
          u_timestamp[i] = c;
        }
      }
      Serial.print(u_timestamp);
      Serial.print("\n\r");
      uint32_t inTime = atol(u_timestamp);
      
      Serial.print(inTime);
      Serial.print("\n\r");
      if(inTime == 0) {
        overwriteTime = 0;
        setTime(getTime());
      }
      else {
        overwriteTime = inTime;
        setTime(inTime);
      }
    }
    else if(theString.startsWith("test leds")) {
      testAll();
    }
    else if(theString.startsWith("test pwm")) {
      testPwm();
    }
    else if(theString.startsWith("save")) {
      wordClockSaveBrightness();
    }
    else if(theString.startsWith("help")){
      Serial.print("\n\rAvailable Commands:\n\r");
      Serial.print("help - show this help message\n\r");
      Serial.print("get time - get and show current time\n\r");
      Serial.print("set time <time_t> - force to display <time_t>; pass 0 to reset\n\r");
      Serial.print("set brightness <0-255> - set the brightness from 0 to 255\n\r");
    }
}


