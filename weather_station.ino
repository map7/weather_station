/*

 Weather Station
test
test2
*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

/*
  U8glib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
*/

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected

//U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* E clock=*/ 13, /* R/W data=*/ 11, /* RS CS=*/ 10);

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8); // Look at Pins and change if not same as yours

#include <dht.h> // Init for DHT 
dht DHT;
#define DHT11_PIN 7 // hooked up to pin 7
 
void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
}

void display() {
}

void loop(void) {

  DHT.read11(DHT11_PIN); // Get DHT info and convert ( Maybe put in seperate void?)
  
int temps = DHT.temperature;
char temp_reading_str[3];

int tempsh = DHT.humidity;
char temph_reading_str[3];

   dtostrf(temps, 2, 0, temp_reading_str);
   dtostrf(tempsh, 2, 0, temph_reading_str); // 

   
   u8g2.clearBuffer();					// clear the internal memory
   u8g2.setFont(u8g2_font_ncenB14_tr);   // choose a suitable font
   u8g2.drawStr(0, 20, temp_reading_str);  // write Temp to the internal memory
   u8g2.setFont(u8g2_font_unifont_t_symbols);
   u8g2.drawUTF8(30,20,"℃");
   u8g2.drawStr(0, 40, temph_reading_str);  // write Humidity to the internal memory
   u8g2.drawStr(20, 40, "%  Humidity");
   u8g2.sendBuffer();                    // transfer internal memory to the display
  delay(1000);

  Serial.print("Temperature = "); // had issue unless i sent to serial
  Serial.println(DHT.temperature);
  Serial.print("Humbillity = ");
  Serial.println(DHT.humidity);
  delay(1000);
   
}
