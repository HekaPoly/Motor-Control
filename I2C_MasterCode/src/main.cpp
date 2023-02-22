/**
 * @file
 * main.cpp
 * 
 * @brief
 * Main file for the I2C master code
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
int i2c_rx_buffer[2]; // 2 elements int table for wire.read reception

/**
 * @brief Setup function for main loop - Code executes at Teensy startup
 */
void setup(void)
{
  /* Start as master on I2C bus */
  Wire.begin();

  Serial.begin(9600);
}

/**
 * @brief Main loop of I2C master code application
 */
void loop(void) 
{
  Wire.beginTransmission(ADDRESS_MOTOR_1);
  /* TODO: Écrire 3 bytes sur le fil (3 fois Wire.write) */
  Wire.write(3);
  /* TODO: Recevoir la confirmation que le moteur a reçu la vitesse */
  Wire.requestFrom(ADDRESS_MOTOR_1, 2);
  for (byte i=0; i<2; i++)
  {
    i2c_rx_buffer[i] = Wire.read();
  }    
  int address = i2c_rx_buffer[0];
  int state = i2c_rx_buffer[1]; // state = 1 for fail, state = 0 for success
  Serial.print("address : ");
  Serial.println(address);
  Serial.println(state);

  Wire.endTransmission();
  delay(100);
}