/*
 Weather Station
*/

#include <Arduino.h>

/* Setup the LCD screen */
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
#endif

/* 
   Setup using Full Buffer (Defined by the _F_ in the following command) 
   Full Buffering uses too much memory. Leave this here incase we need it.
*/
/* U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /\* SCK green clock=*\/ 13, /\* SID yellow data=*\/ 12, /\* blue CS=*\/ 11, /\* reset=*\/ 8); // GV 79%, Total 68% */

/* Setup using Page Buffer (Defined by the _1_ in the following command) LOW MEMORY */
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 12, /* CS=*/ 11, /* reset=*/ 8);

/* Temperature: Include and setup */
#include <dht.h> // Init for DHT
dht DHT;
#define DHT11_PIN 10 // hooked up to pin 10
char temperature_str[3];

/* RTC */
#include "RTClib.h" // Rtc Lib
#include <Wire.h>   // RTC Lib
RTC_DS1307 RTC;

void setup(void) {
  u8g2.begin();                 /* LCD */
  Serial.begin(9600);

  /* RTC */
  Wire.begin();
  //RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // sets clock based on yr PC time
  RTC.begin();
}

void ReadSensors(){
  DHT.read11(DHT11_PIN); // Read DHT sensor values for temperature & humidity
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

void GetTemperature(){
  int temp_int = DHT.temperature;
  dtostrf(temp_int, 2, 0, temperature_str);
}

char* Humidity(){
  int humidity_int = DHT.humidity;
  static char humidity_str[3];
  dtostrf(humidity_int, 2, 0, humidity_str);
  return humidity_str;
}

/* TODO change to just using unifont  */
void Draw() {
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawStr(0, 20, temperature_str);  // write Temp to the internal memory
  u8g2.drawUTF8(30,20,"℃");
  u8g2.drawStr(0, 40, Humidity());  // write Humidity to the internal memory
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
  ReadSensors();
  GetClock();
  GetTemperature();
  Display();
  ConsoleOutput();
  delay(1000);
}
