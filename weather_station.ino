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

void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
}

void ReadSensors(){
  DHT.read11(DHT11_PIN); // Read DHT sensor values for temperature & humidity
}

char* Temperature(){
  int temp_int = DHT.temperature;
  static char temp_str[3];
  dtostrf(temp_int, 2, 0, temp_str);
  return temp_str;
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
  u8g2.drawStr(0, 20, Temperature());  // write Temp to the internal memory
  u8g2.drawUTF8(30,20,"℃");
  u8g2.drawStr(0, 40, Humidity());  // write Humidity to the internal memory
  u8g2.drawStr(20, 40, "% Humidity");
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
  Display();
  ConsoleOutput();
  delay(1000);
}
