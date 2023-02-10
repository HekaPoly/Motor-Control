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
  
  /* TODO: Recevoir la confirmation que le moteur a reçu la vitesse */
  Wire.endTransmission();
  
  delay(100);
}