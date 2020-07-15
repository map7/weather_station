/*
 Weather Station
*/

#include <Arduino.h>
#include "options.h"            /* Turn sensors and output on and off. */
#include "lcd_setup.h"
#include "dht_setup.h"
#include "rtc_setup.h"
#include "esp8266_setup.h"
#include "ldr_setup.h"

void setup(void) {
  if (EnableLCD) { u8g2.begin(); }
  Serial.begin(9600);

  if (EnableRTC){
    /* RTC */
    Wire.begin();
    //RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // sets clock based on yr PC time
    RTC.begin();
  }
  
  /* ESP8266 */
  if (EnableESP8266){
    BeginESP8266();
    ConnectWIFI();
  }
}

void BeginESP8266(){
  esp8266.begin(9600);
  Serial.println("");
  Serial.println("Remember to to set Both NL & CR in the serial monitor.");
  Serial.println("Ready");
  Serial.println("");
}

void ConnectWIFI(){
  Display();
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
  sendCommand("AT+CIFSR",5,"OK");
  connected = true;
}

void GetClock() {
  DateTime now = RTC.now();

  Serial.print(now.day(), DEC); // print date and time to Serial
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.year(), DEC); 
  Serial.println();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
} 

void ReadDHT(){
  DHT.read11(DHT11_PIN); // Read DHT sensor values for temperature & humidity
}

void GetTemperature(){
  int temp_int = DHT.temperature;
  dtostrf(temp_int, 2, 0, temperature_str);
}

void GetHumidity(){
  int humidity_int = DHT.humidity;
  dtostrf(humidity_int, 2, 0, humidity_str);
}

void GetLDR(){
  /* LDR Readings */
  ldr_int = analogRead (sensorPin);
  Serial.print("LDR: "); 
  Serial.println (ldr_int, DEC);
}

/* TODO change to just using unifont  */
void Draw() {
  u8g2.setFont(u8g2_font_unifont_t_symbols);

  if (connected == true){

    if (EnableDHT){
      /* Temperature & Humidity */
      u8g2.drawStr(0, 20, temperature_str);  // write Temp to the internal memory
      u8g2.drawUTF8(20,20,"℃");
      u8g2.drawStr(0, 40, humidity_str);  // write Humidity to the internal memory
      u8g2.drawStr(20, 40, "% RH");
    }

    if (EnableRTC) {
      /* Time */
      DateTime now = RTC.now();
      char buffer[10];
      sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
      u8g2.setCursor(0,60);
      u8g2.print(buffer);
    }

    if (EnableLDR) {
      u8g2.drawStr(70,20, "LDR ");
      dtostrf(ldr_int, 4, 0, ldr_str);
      u8g2.drawStr(90,20, ldr_str);
    }
    
  } else {
    u8g2.drawStr(0,20, "Weather Station");
    u8g2.drawStr(0,40, "Connecting WIFI");
  }
}

void Display() {
  u8g2.firstPage();
  do {
    Draw();
  } while ( u8g2.nextPage() );
}

/* Send sensor information to ThingSpeak */
void SendData() {

  /* Setup the URL with our sensor information */
  String temp_field = "&field1="+String(temperature_str);
  String rh_field =   "&field2="+String(humidity_str);

  dtostrf(ldr_int, 2, 0, ldr_str); /* Work around because ThingSpeak was reporting 10,000 */
  String ldr_field =  "&field3="+String(ldr_str);

  String getData = "GET /update?api_key="+ API + temp_field + rh_field + ldr_field;sendCommand("AT+CIPMUX=1",5,"OK");

  /* Send the data */
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
  esp8266.println(getData);delay(1000);countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0",5,"OK");
}

void ConsoleOutput() {
  Serial.print("T=");
  Serial.println(int(DHT.temperature));
  Serial.print("H=");
  Serial.println(int(DHT.humidity));
}

void loop(void) {
  if (EnableRTC) { GetClock(); }

  if (EnableDHT) {
    ReadDHT();
    GetTemperature();
    GetHumidity();
  }

  if (EnableLDR) { GetLDR(); }
  
  if (EnableESP8266){
    SendData();
  }else{
    delay(1000);
  }
  if (EnableLCD) { Display(); }
  ConsoleOutput();
}
