
void serialClockDisplayTime(time_t t){
  // digital clock display of the time
  Serial.print(hour(t));
  serialClockPrintDigits(minute(t));
  serialClockPrintDigits(second(t));
  Serial.print(" ");
  Serial.print(day(t));
  Serial.print(".");
  Serial.print(month(t));
  Serial.print(".");
  Serial.print(year(t)); 
  Serial.println(); 
}

void serialClockPrintDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

