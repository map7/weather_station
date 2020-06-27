<<<<<<< Updated upstream
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
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* E clock=*/ 2, /* R/W data=*/ 10, /* RS CS=*/ 7);
 
void setup(void) {
  u8g2.begin();
}

void display() {

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr);	// choose a suitable font
  u8g2.drawStr(0,20,"12:00");	// write something to the internal memory
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawUTF8(0,60,"☔");  // write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);
}
=======
/*

 Weather Station

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
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* E clock=*/ 2, /* R/W data=*/ 10, /* RS CS=*/ 7);
 
void setup(void) {
  u8g2.begin();
}

void display() {

void loop(void) {
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB14_tr);	// choose a suitable font
  u8g2.drawStr(0,20,"12:00");	// write something to the internal memory
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawUTF8(0,60,"☔");  // write something to the internal memory
  u8g2.sendBuffer();					// transfer internal memory to the display
  delay(1000);
}
>>>>>>> Stashed changes
