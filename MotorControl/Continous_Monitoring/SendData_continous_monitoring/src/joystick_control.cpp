/**
   * @file joystick_control.cpp
   * @author Équipe Motor Control - Heka
   * @brief 
   * This file contains a small program to recieve data coming from a joystick.
   * This is a test to analyse wether it is possible to control a motor with a joystick.
   * @date 2022-07-16
   * 
   * @copyright Copyright Heka (c) 2022
   * 
   */

#include <Arduino.h>
#include "joystick_control.h"

void setup() {
  Serial.begin(9600u);

  for (int32_t i = 0; i < NUMBER_OF_ENCODERS; i++)
  {
    encoders[i]->write(5000u);
  }

  analogReadResolution(12u);
}

void loop() {
  for (int32_t i = 0; i < NUMBER_OF_ENCODERS; i++)
  {
    values_encoders[i] = abs(encoders[i]->read());
  }

  int32_t counter = 0;
  for (int32_t i =0; i < NUMBER_OF_BYTES_TO_SEND; i++)
  {
    values_to_send[i]     = values_encoders[counter];
    values_to_send[i + 1] = values_encoders[counter] << 8u;
    values_to_send[i + 2] = values_encoders[counter] << 16u;
    values_to_send[i + 3] = values_encoders[counter] << 24u;

    i = i + 3;
    counter = counter + 1;
  }

  Serial.write(values_to_send, NUMBER_OF_BYTES_TO_SEND);
  Serial.send_now();

  delay(30);
}