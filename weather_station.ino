/*
  Weather Station
*/

#include <Arduino.h>
#include "options.h"            /* Turn sensors and output on and off. */

#if EnableLCD
#include "lcd_setup.h"
#endif

#include "dht_setup.h"

#include "rtc_setup.h"

#if EnableBMP388
#include "bmp388_setup.h"
#endif

#include "esp8266_setup.h"
#include "ldr_setup.h"
#include "rain_setup.h"

void setup(void) {
#if EnableLCD
  u8g2.begin();
#endif
  Serial.begin(9600);

  if (EnableRTC){ /* RTC */
    //RTC.adjust(DateTime(F(__DATE__), F(__TIME__))); // sets clock based on yr PC time
    RTC.begin();
  }
  
  /* ESP8266 */
  if (EnableESP8266){
    BeginESP8266();
    ConnectWIFI();
  }
  
  /* BMP388 */
#if EnableBMP388
  BeginBMP388();      // 388 Set up
#endif

  /* Rain Sensor */
  if (EnableRAIN){
    pinMode(RAIN_D_PIN, INPUT);
    pinMode(RAIN_A_PIN, INPUT);
    Serial.begin(9600);
  }
}


void BeginESP8266(){
  esp8266.begin(9600);
  /* Serial.println(""); */
  /* Serial.println("Remember to to set Both NL & CR in the serial monitor."); */
  /* Serial.println("Ready"); */
  /* Serial.println(""); */
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
  byte temp_int = DHT.temperature;
  dtostrf(temp_int, 2, 0, temperature_str);
}

void GetHumidity(){
  byte humidity_int = DHT.humidity;
  dtostrf(humidity_int, 2, 0, humidity_str);
}

void GetLDR(){
  /* LDR Readings */
  ldr_int = analogRead (sensorPin);
  Serial.print("LDR: "); 
  Serial.println (ldr_int, DEC);
}

void GetRAIN(){
  rain_val_D = digitalRead(RAIN_D_PIN);
  if(rain_val_D == HIGH) 
    {
      Serial.println("R:wet"); 
      delay(10); 
    }
  else
    {
      Serial.println("R:dry");
      delay(10); 
    }
  rain_val_A=analogRead(RAIN_A_PIN); 
  Serial.print("R: ");
  Serial.println(rain_val_A); 
}

/* TODO change to just using unifont  */
void Draw() {
#if EnableLCD
  u8g2.setFont(u8g2_font_unifont_t_symbols);

  if (connected == true){       // might be better to say if connected = false show a number or sign as display freezes if connection isnt true
    u8g2.drawStr(100, 60, "CT");
  }
  else {
    u8g2.drawStr(100, 60, "NT");
  }
  if (EnableDHT){
    /* Temperature & Humidity */
    if (! EnableBMP388) {
      u8g2.drawStr(0, 20, temperature_str);  // write Temp to the internal memory. Check if bmp is not enabled before displaying dht temp.
      u8g2.drawUTF8(20,20,"℃");
      u8g2.drawStr(0, 40, humidity_str);  // write Humidity to the internal memory
      u8g2.drawStr(20, 40, "% RH");
    }
    else {
      u8g2.drawStr(0, 40, humidity_str);  // write Humidity to the internal memory
      u8g2.drawStr(20, 40, "% RH");
    }
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
    u8g2.drawStr(40,20, "L ");
    dtostrf(ldr_int, 2, 0, ldr_str);
    u8g2.drawStr(50,20, ldr_str);
  }

  if (EnableRAIN) {
    u8g2.drawStr(90,20, "R ");
    dtostrf(rain_val_A, 2, 0, rain_str);
    u8g2.drawStr(100,20, rain_str);
  }

#if EnableBMP388
  float press_int = (bmp.pressure / 100.0) ; // Cant Get proper pressure reading on screen
  dtostrf(press_int, 5, 1, press_str);
  u8g2.drawStr(110,40, "hP");
  u8g2.drawStr(60,40, press_str);

  float temp388_int = (bmp.temperature) ; // Cant Get proper pressure reading on screen
  dtostrf(temp388_int, 3, 1, temp388_str);
  u8g2.drawStr(0, 20, temp388_str);  // write Temp to the internal memory. Check if bmp is not enabled before displaying dht temp.
  u8g2.drawUTF8(35,20,"C");
#endif

#endif
} 
  
/* else {
   if (EnableESP8266){
   u8g2.drawStr(0,20, "Weather Station");
   u8g2.drawStr(0,40, "Connecting WIFI");
   } */
  
//}

void Display() {
#if EnableLCD
  u8g2.firstPage();
  do {
    Draw();
  } while ( u8g2.nextPage() );
#endif
}

/* Send sensor information to ThingSpeak */
void SendData() {

  dtostrf(ldr_int, 2, 0, ldr_str); /* Work around because ThingSpeak was reporting 10,000 */
  
  String getData = "GET /update?api_key="+ API +
    "&field1="+String(temperature_str) +
    "&field2="+String(humidity_str) +
    "&field3="+String(ldr_str) +
    "&field4="+String(rain_str);sendCommand("AT+CIPMUX=1",5,"OK");

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

#if EnableBMP388
  GetBmpData();
#endif

  if (EnableLDR) { GetLDR(); }

  /* Rain */
  if (EnableRAIN) { GetRAIN(); }
  
  if (EnableESP8266){
    SendData();
  }else{
    delay(1000);
  }
  if (EnableLCD) { Display(); }
  ConsoleOutput();
}
