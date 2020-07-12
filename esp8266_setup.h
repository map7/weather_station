#include <SoftwareSerial.h>
#include "arduino_secrets.h"

String AP = SECRET_SSID;  
String PASS = SECRET_PASS;

String API = SECRET_WRITE_API;
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;

SoftwareSerial esp8266(2, 3); // RX | TX
