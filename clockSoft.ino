#include <Streaming.h>
#include <avr/eeprom.h>
#include <Time.h>
#include <Timezone.h>
#include <Wire.h>
#include <WiFlySerial.h>

time_t overwriteTime = 0;
time_t global_utc = 0;

WiFlySerial WiFly;


void WiFlyConfigure(){
  WiFly.setSSID("foo"); //set wifi ssid
  WiFly.setPassphrase("bar"); //set wifi passphrase
  WiFly.setAuthMode(4); //set wifi authentication to WPA2-PSK
  WiFly.setJoinMode(1); //set to only join specified SSID Access Point
  WiFly.setDHCPMode(1); //set it to get its IP from the Access Point
  WiFly.setNTP("176.9.253.75");
  WiFly.setNTP_Update_Frequency("0");
}


void setup()
{
  //Do some "display provider" abstraction with pointers to functions like "show time"
  //So segmentClock, serialClock, wordClock and wordClockV2 and whatever else work with a single interface
  wordClockInit();
  
  
  Serial.begin(9600);
  
  //delay only for debugging purposes
  //delay(5000); 

  Serial.print("clockSoft v0.2\n\r");
  
  while(!WiFly.begin()) delay(100);
  Serial.print("WiFly initialized!\n\r");
  
  //Maybe some configuration details here
  //Serial.print(WiFly.getSSID());
  //..
  
  //configure it only once, reflash without this, when configured
  //WiFlyConfigure();
  //WiFly.setDebugChannel(&Serial);
  
  setSyncInterval(10); //small sync interval at the beginning
  setSyncProvider(getTime);
}

void loop()
{
  serial();
  timeLoop();
}


