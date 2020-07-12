/* Temperature: Include and setup */
#include <dht.h> // Init for DHT
dht DHT;
#define DHT11_PIN 10 // hooked up to pin 10 (blue)
char temperature_str[3];
char humidity_str[3];
