/**
 * @file
 * main.cpp
 * 
 * @brief
 * This file is meant to define the setup and control loop of the BLDC motors of the exoskeleton
 * 
 * @note
 * Pour utiliser : 
	- Brancher le moteur au board (PSU (power supply) + deux câbles allant vers le moteur)
	- Allumer PSU
	- Reset Teensy
 * 
 * @copyright Copyright Heka (c) 2022
 */

#include <Arduino.h>
#include <stdio.h>

#include "motor.h"

/********** GLOBAL ***********/
uint32_t g_position_to_reach = NULL_ENCODER_POSITION;
uint32_t * g_position_to_reach_ptr = &g_position_to_reach;
uint8_t g_new_position_required = NO_NEW_POSITION_REQUIRED;

/* Motor and encoder combination */
Encoder encoder_motor_1(PIN_1_ENCODER, PIN_2_ENCODER);

Motor g_motor_1 = {
	.motor_current_speed_reached = MOTOR_ZERO_SPEED,
	.motor_previous_speed_reached = MOTOR_ZERO_SPEED,
	.motor_turn_direction = DIRECTION_CW,
	.motor_error_state = ERROR_NONE,
	.motor_encoder = &encoder_motor_1,
	.motor_brake_status = BRAKES_ENABLED,
};

Motor * g_motor_1_ptr = &g_motor_1;

/**
 * @brief Checks the serial buffer read entry for a new encoder position to reach
 * 
 * @return uint32_t Indicates wether or not we want to reach a new position
 */
uint8_t check_serial_buffer(uint32_t * position_to_reach)
{
	uint8_t res = NO_NEW_POSITION_REQUIRED;
	if (Serial.available() > 0)
	{
		*position_to_reach = Serial.parseInt();
		Serial.println(*position_to_reach);
		res = NEW_POSITION_REQUIRED;
	}

	/* Clear serial read buffer */
	while (Serial.available() > 0)
	{
		Serial.read();
	}

	return res;
}

/**
 * @brief Setup function to initialize all Teensy4.0 pins
 */
void setup(void)
{
	Serial.begin(115200);
	Serial.println("Starting");

	motor_setup();

	/* Enable device */
	digitalWrite(PIN_ENABLE, HIGH);

	/* Reset encoder position at Teensy reset */
	encoder_motor_1.write(0);

	motor_disable_brakes(g_motor_1_ptr);
}

/**
 * @brief Loop to move motor to a desired position
 */
void loop(void)
{
	g_new_position_required = check_serial_buffer(g_position_to_reach_ptr);
	
	if (g_new_position_required == NEW_POSITION_REQUIRED)
	{
		//motor_move_to_required_position(*g_position_to_reach_ptr, g_motor_1_ptr);
		motor_control_speed(*g_position_to_reach_ptr, g_motor_1_ptr);
	}
}
