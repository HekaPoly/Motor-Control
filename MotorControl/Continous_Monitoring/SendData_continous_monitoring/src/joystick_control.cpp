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

  values_to_send[0] = pos_x;
  values_to_send[1] = pos_x >> 8;
  values_to_send[2] = pos_y;
  values_to_send[3] = pos_y >> 8;

  Serial.write(values_to_send, 4);
  Serial.send_now();

  delay(30);
}