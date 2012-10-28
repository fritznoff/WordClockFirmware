
#define PIN_LATCH 14
#define PIN_DATA 16
#define PIN_CLOCK 15
#define PIN_PWM 10

void segmentClockInit(){
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);
  pinMode(PIN_CLOCK, OUTPUT);
  pinMode(PIN_PWM, OUTPUT);  
  
  //segmentClockSetBrightness(5);
}

void segmentClockSetBrightness(byte brightness){
  analogWrite(PIN_PWM, 255 - brightness);
}

void segmentClockAutoBrightness(){
  int brightness = analogRead(A0) / 4;
  brightness = brightness - 100;
  if(brightness < 2) brightness = 2;
  
  Serial.print(brightness);
  Serial.print("\n\r");
  analogWrite(PIN_PWM, 255 - brightness);
}

void segmentClockDisplayNoTime(){
  
}

void segmentClockDisplayWifi(){
  
}

void segmentClockDisplayNoWifi(){
  
}

void segmentClockDisplayInterwebs(){
  
}

void segmentClockDisplayNoInterwebs(){
  
}


void segmentClockDisplayTime(time_t t){
  byte shiftregsize = 6;
  byte shiftreg[6] = {0,0,0,0,0,0};
  byte segments[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111};
  
  shiftreg[0] = segments[hour(t)/10];
  shiftreg[1] = segments[hour(t)%10];
  shiftreg[2] = segments[minute(t)/10];
  shiftreg[3] = segments[minute(t)%10];
  shiftreg[4] = segments[second(t)/10];
  shiftreg[5] = segments[second(t)%10];
  
  int i = 0;
  digitalWrite(PIN_LATCH, LOW);
  while(i < shiftregsize)
  {
    shiftOut(PIN_DATA, PIN_CLOCK, MSBFIRST, shiftreg[i]);
    i++;
  }
  digitalWrite(PIN_LATCH, HIGH);
}


