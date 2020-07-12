/*
 Weather Station
*/

#include <Arduino.h>
#include "lcd_setup.h"
#include "dht_setup.h"
#include "rtc_setup.h"
#include "esp8266_setup.h"

void setup(void) {
  u8g2.begin();                 /* LCD */
  Serial.begin(9600);

  /* RTC */
  Wire.begin();
  //RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // sets clock based on yr PC time
  RTC.begin();
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

/* TODO change to just using unifont  */
void Draw() {
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawStr(0, 20, temperature_str);  // write Temp to the internal memory
  u8g2.drawUTF8(20,20,"℃");
  u8g2.drawStr(0, 40, humidity_str);  // write Humidity to the internal memory
  u8g2.drawStr(20, 40, "% Humidity");

  DateTime now = RTC.now();
  char buffer[10];
  sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  u8g2.setCursor(0,60);
  u8g2.print(buffer);
}

void Display() {
  u8g2.firstPage();
  do {
    Draw();
  } while ( u8g2.nextPage() );
}

void ConsoleOutput() {
  Serial.print("T=");
  Serial.println(int(DHT.temperature));
  Serial.print("H=");
  Serial.println(int(DHT.humidity));
}

void loop(void) {
  GetClock();

  ReadDHT();
  GetTemperature();
  GetHumidity();
  
  Display();
  ConsoleOutput();
  delay(1000);
}
