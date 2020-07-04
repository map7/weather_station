/*
 Weather Station
*/

#include <Arduino.h>

/* LCD Library */
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
  #include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
  #include <Wire.h>
#endif

/* Setup the LCD screen */
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* SCK clock=*/ 13, /* SID data=*/ 12, /* CS=*/ 11, /* reset=*/ 8); // Look at Pins and change if not same as yours

/* Temperature: Include and setup */
#include <dht.h> // Init for DHT
dht DHT;
#define DHT11_PIN 10 // hooked up to pin 10

void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
  DHT.read11(DHT11_PIN); // Read DHT sensor values for temperature & humidity
}

char* temperature(){
  int temp_int = DHT.temperature;
  static char temp_str[3];
  dtostrf(temp_int, 2, 0, temp_str);
  return temp_str;
}

char* humidity(){
  int humidity_int = DHT.humidity;
  static char humidity_str[3];
  dtostrf(humidity_int, 2, 0, humidity_str);
  return humidity_str;
}

void display() {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr);   // choose a suitable font
  u8g2.drawStr(0, 20, temperature());  // write Temp to the internal memory
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawUTF8(30,20,"℃");
  u8g2.drawStr(0, 40, humidity());  // write Humidity to the internal memory
  u8g2.drawStr(20, 40, "% Humidity");
  u8g2.sendBuffer();                    // transfer internal memory to the display
  delay(1000);
}

void console_output() {
  Serial.print("T=");
  Serial.println(int(DHT.temperature));
  Serial.print("H=");
  Serial.println(int(DHT.humidity));
}

void loop(void) {
  display();
  console_output();
  delay(1000);
}
