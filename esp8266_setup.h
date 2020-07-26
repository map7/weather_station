#include <SoftwareSerial.h>
#include "arduino_secrets.h"

String AP = SECRET_SSID;  
String PASS = SECRET_PASS;

String API = SECRET_WRITE_API;
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";

byte countTrueCommand;
byte countTimeCommand; 
boolean found = false; 
boolean connected = false; 
byte valSensor = 1;

SoftwareSerial esp8266(2, 3); // RX | TX

void sendCommand(String command, byte maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at cmd: ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("Ok");
    countTrueCommand++;
    countTimeCommand = 0;
  }else{
    Serial.println("F");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
}
