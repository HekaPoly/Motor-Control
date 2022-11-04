/**
 * @file
 * main.cpp
 * 
 * @author
 * Équipe Contrôle des moteurs - Heka
 * 
 * @brief
 * This file is meant to define the control loop of the routine tests executed on the motors
 * 
 * @note
 * Pour utiliser : 
	- Brancher le moteur au board (PSU (power supply) + deux câbles allant vers le moteur)
	- Allumer PSU
	- Reset Teensy
 * 
 * @copyright Copyright Heka (c) 2022
 * 
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <stdio.h>

#include "motor_commands.h"

/********** GLOBAL ***********/
int32_t g_position_to_reach = NULL_ENCODER_POSITION;
int32_t *g_position_to_reach_ptr = &g_position_to_reach;
uint8_t g_new_position_required = NO_NEW_POSITION_REQUIRED;

/* Define encoder only in the .cpp */
Encoder encoder_motor_1(PIN_1_ENCODER, PIN_2_ENCODER);

/* Create a motor structure for the 1st motor */
Motor g_motor_1 = {
	.motor_current_speed_reached = MOTOR_ZERO_SPEED,
	.motor_previous_speed_reached = MOTOR_ZERO_SPEED,
	.motor_turn_direction = TURN_CW,
	.motor_error_state = NO_ERROR,
	.motor_encoder = &encoder_motor_1,
	.motor_brake_status = BRAKES_ENABLED,
};

Motor *g_motor_1_ptr = &g_motor_1;


/**
 * @brief Checks the serial buffer read entry for a new encoder position to reach
 * 
 * @return uint8_t A variable that indicates wether or not we want to reach a new position
 */
uint8_t check_serial_buffer(int32_t *position_to_reach)
{
	uint8_t res = NO_NEW_POSITION_REQUIRED;
	if (Serial.available() > 0)
	{
		*position_to_reach = Serial.parseInt();
		Serial.println(*position_to_reach);
		res = NEW_POSITION_REQUIRED;
	}
	else
	{
		/* Do nothing here */
	}

	/* Clear serial read buffer */
	while (Serial.available() > 0)
	{
		Serial.read();
	}

	return res;
}

/* Arduino setup and loop */
void setup()
{
	Serial.begin(9600);
	Serial.println("Starting");

	motor_setup();

	/* Enable device */
	digitalWrite(ENABLE_PIN, HIGH);

	/* Reset encoder position at Teensy reset */
	encoder_motor_1.write(0);

	disable_brakes(g_motor_1_ptr);
}

void loop()
{
	g_new_position_required = check_serial_buffer(g_position_to_reach_ptr);
	
	if (g_new_position_required == NEW_POSITION_REQUIRED)
	{
		move_motor_to_required_position(*g_position_to_reach_ptr, g_motor_1_ptr);
	}
	else
	{
		/* Nothing to do here */
	}
}
