#include <Streaming.h>
#include <avr/eeprom.h>
#include <Time.h>
#include <Timezone.h>
#include <Wire.h>
#include <WiFlySerial.h>

#define CMD_DELAY 1000

time_t overwriteTime = 0;
time_t global_utc = 0;

WiFlySerial WiFly;


void WiFlyConfigure(){
  delay(CMD_DELAY);
  WiFly.setSSID("foo"); //set wifi ssid
  delay(CMD_DELAY);
  WiFly.setPassphrase("bar"); //set wifi passphrase
  delay(CMD_DELAY);
  WiFly.setAuthMode(4); //set wifi authentication to WPA2-PSK
  delay(CMD_DELAY);
  WiFly.setJoinMode(1); //set to only join specified SSID Access Point
  delay(CMD_DELAY);
  WiFly.setDHCPMode(1); //set it to get its IP from the Access Point
  delay(CMD_DELAY);
  WiFly.setNTP("176.9.253.75");
  delay(CMD_DELAY);
  WiFly.setNTP_Update_Frequency("0");
  delay(CMD_DELAY);
}


void setup()
{
  //Do some "display provider" abstraction with pointers to functions like "show time"
  //So segmentClock, serialClock, wordClock and wordClockV2 and whatever else work with a single interface
  wordClockInit();
  
  
  Serial.begin(9600);

  Serial.print("clockSoft v0.2\n\r");
  
  //testMatrix();
  //while(1);
  
  while(!WiFly.begin()) delay(100);
  Serial.print("WiFly initialized!\n\r");
  
  WiFly.setDebugChannel(&Serial);
  
  //configure it only once, reflash without this, when configured
  WiFlyConfigure();
  WiFly.join();
  
  setSyncInterval(10); //small sync interval at the beginning
  setSyncProvider(getTime);
}

void loop()
{
  serial();
  timeLoop();
}


