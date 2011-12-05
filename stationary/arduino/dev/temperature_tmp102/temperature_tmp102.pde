
/*
reading temperature from TMP102 sensor breakout from SFE

for ArduinoMega // weather tunnel

ADD0 to GND
ALT unconnected

V+ to 3v
GND to GND
SCL to Analog Input 5 -- WHITE WIRE
SDA to Analog Input 4 -- BLUE WIRE
*/

#include <Wire.h>
byte tmp102comm;
byte msb;
byte lsb;
int val;
float celsius;
float farenheit;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
}

void loop()
{
  tmp102comm = Wire.requestFrom(72,2); 
  if (tmp102comm == 2) {
    msb = Wire.receive(); /* Whole degrees */ 
    lsb = Wire.receive(); /* Fractional degrees */ 
    val = ((msb) << 4);   /* MSB */
    val |= (lsb >> 4);    /* LSB */
    celsius = val*0.0625;
    farenheit = (celsius*1.8) + 32;
    Serial.print("C: ");
    Serial.println(celsius);
    Serial.print("F: ");
    Serial.println(farenheit);
    Serial.println();
    delay(1000);   
  }
}

