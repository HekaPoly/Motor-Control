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

  analogReadResolution(12u);
}

void loop() {
  pos_x = analogRead(VRX);
  pos_y = analogRead(VRY);

  for (int i = 1; i <= 4; i++)
  {
    values_to_send[i - 1] = VRX >> ((i * 8);
    values_to_send[i + 3] = VRY >> ((i * 8);
  }

  Serial.write(values_to_send, 8);
  Serial.send_now();

  delay(100);
}