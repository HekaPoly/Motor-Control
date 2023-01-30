#include <Arduino.h>
#include <Wire.h>

/**
byte angle = 30;
#define ADRESSE 2

void setup() 
{
  Wire.begin();
}

void loop() 
{
  Wire.beginTransmission(ADRESSE);
  Wire.write(angle);
  Wire.endTransmission();

  angle++;
  delay(100);
}
**/

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}