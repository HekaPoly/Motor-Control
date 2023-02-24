/**
 * @file
 * main.cpp
 * 
 * @brief
 * Main file for the I2C slave code
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"

/* GLOBAL VALUES */
int value_received = 0;

// Test values
byte values[2] = {0x03, SUCCESS};

/**
 * @brief Interrupt routine to read incoming I2C data
 * 
 * @param bytes 
 */
void receiveEvent(int bytes)
{
  value_received = Wire.read();
}

void requestEvent() 
{
  Wire.write(values, 2);
}

/**
 * @brief Setup function for main loop - Code executes at Teensy startup
 */
void setup(void)
{
  /* Join I2C bus with motor 1 address */
  Wire.begin(ADDRESS_MOTOR_1);              // join i2c bus with address #4
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.begin(9600);               // start serial for output
}

/**
 * @brief Main loop of I2C master code application
 */
void loop(void) 
{
  Serial.println(value_received);
  delay(100);
}