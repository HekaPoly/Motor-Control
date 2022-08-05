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
  Serial.println(analogRead(VRX));
  Serial.println(analogRead(VRY));

  delay(100);
}