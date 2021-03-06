#include <Adafruit_Sensor.h> // 388 Lib
#include <SPI.h>    // 388 SPI Lib // using i2c for 388 so not sure if needed just yet
#include "Adafruit_BMP3XX.h"  // 388 LIB from library

#define SEALEVELPRESSURE_HPA (1032.2) // 388 HPA baseline taken from net at a melbourne port 4/6/20. Most like;ly not use, but will leave in for now.
Adafruit_BMP3XX bmp; // I2C */

char press_str[5];
char temp388_str[3];

void BeginBMP388(){
  bmp.begin();
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X); // 388 Set up
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_2X);    // 388 Set up
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
}

void GetBmpData() {
  if (! bmp.performReading()) {
    Serial.println("Fail read");
    return;
  }
  Serial.print(bmp.temperature);
  Serial.println(" *C");
 
  Serial.print(bmp.pressure / 100.0);
  Serial.println(" hPa");
}
