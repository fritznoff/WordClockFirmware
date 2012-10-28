TimeChangeRule MESZ_rule = {"MESZ", Last, Sun, Mar, 2, 120 };
TimeChangeRule MEZ_rule = {"MEZ", Last, Sun, Oct, 3, 60 };
Timezone MEZ(MESZ_rule, MEZ_rule);


time_t getTime(){
  uint32_t old_millis = millis();
  Serial.print("getTime\n\r");
  if(overwriteTime != 0) {
    time_t foo = overwriteTime + 1;
    Serial.print("overwrite: ");
    Serial.print(foo);
    Serial.print("\n\r");
    return foo;
  }
  WiFly.SendInquirySimple("time\r");
  time_t ret_time = WiFly.getTime();
  if(ret_time > 100000000){
    Serial.print("ardui time: ");
    Serial.print(global_utc);
    Serial.print("\n\r");
    Serial.print("wifly time: ");
    Serial.print(ret_time);
    Serial.print("\n\r");
    setSyncInterval(300);
    
    Serial.print("syncTime: ");
    Serial.print(millis() - old_millis);
    Serial.print("\n\r");
    return ret_time;
  }
  else{
    Serial.print("wifly time is not set\n\r");
    delay(1000);
    
    if(WiFly.isAssociated()){
      Serial.print("wifly has got wifi\n\r");
      delay(1000);
      
      /*if(true){
        Serial.print("wifly has got Interwebs\n\r");
        wiflyGetNTP();
        setSyncInterval(10);
        //segmentClockDisplayInterwebs();
        //delay(100);
      }
      else{
        Serial.print("wifly has got no Interwebs\n\r");
        //segmentClockDisplayNoInterwebs();
        //delay(100);
      }*/
    }
    else{
      Serial.print("wifly has got no wifi\n\r");
      delay(1000);
    }
    return 0;
  }
}
  
void timeLoop() {
  time_t utc = now();
  if((utc != global_utc) && (timeStatus() != timeNotSet)){
    global_utc = utc;
    if(overwriteTime != 0)
      overwriteTime = utc;
      
    time_t local = MEZ.toLocal(utc);
    serialClockDisplayTime(global_utc);
    wordClockDisplayTime(local);
  } 
}
