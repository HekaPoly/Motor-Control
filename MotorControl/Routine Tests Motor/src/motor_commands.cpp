/**
 * @file
 * motor_commands.cpp
 * 
 * @author
 * Équipe Contrôle des moteurs - Heka
 * 
 * @brief
 * This file is meant to define functions to control a motor via the DRV8308 motor driver
 * 
 * @copyright Copyright Heka (c) 2022
 * 
 */

#include <Arduino.h>

#include "motor_commands.h"

/* Define encoder only in the .cpp */
Encoder encoder(PIN_2_ENCODER, PIN_1_ENCODER);

/**
 * @brief 
 * 
 * @param 
 * 
 */
void motor_setup() {
	pinMode(RESET_PIN, OUTPUT);
	pinMode(ENABLE_PIN, OUTPUT);
	pinMode(DIRECTION_PIN, OUTPUT);
	pinMode(BRAKE_PIN, OUTPUT);
	pinMode(MOTOR_PIN, OUTPUT);
	pinMode(LOCK_PIN, INPUT);
	pinMode(FAULT_PIN, INPUT);

	analogWriteResolution(11);
	analogWriteFrequency(MOTOR_PIN, 23437.5); //Values between 0-2047

	digitalWrite(RESET_PIN, LOW);
	digitalWrite(ENABLE_PIN, HIGH);
	digitalWrite(DIRECTION_PIN, HIGH);
	digitalWrite(BRAKE_PIN, LOW);
}

double read_angle(bool fullTurn) {
	double angle = encoder.read() * COUNT_TO_ANGLE_FACTOR;

	return angle;
}

void set_direction(bool direction){
	if (direction)
	{
		digitalWrite(DIRECTION_PIN, HIGH);
	}
	else
	{
		digitalWrite(DIRECTION_PIN, LOW);
	}
}