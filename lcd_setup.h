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
U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock (green)=*/ 13, /* data (yello)=*/ 12, /* CS (blue)=*/ 11, /* reset=*/ 8);
