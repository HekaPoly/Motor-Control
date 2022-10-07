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

/* GLOBAL */
bool g_brakes_enabled_status = true;
uint8_t g_command_received = IDLE_STATE;
Motor g_motor_1;
Motor *g_motor_1_ptr = &g_motor_1;


uint8_t check_serial_buffer()
{
	uint8_t res = IDLE_STATE;

	if (Serial.available() > 0)
	{
		int32_t command = Serial.read();

		if (command == 'A')
		{
			res = TURN_CW;
		}
		else if (command == 'B')
		{
			res = TURN_CCW;
		}
		else
		{
			/* Do nothing */
		}
	}

	/* Clear serial read buffer */
	while (Serial.available() > 0)
	{
		Serial.read();
	}

	return res;
}

void control_loop(uint8_t command)
{
	if (command == TURN_CW)
	{
		smooth_motor_slowdown(MOTOR_ZERO_SPEED, g_motor_1_ptr);

		digitalWrite(DIRECTION_PIN, HIGH);
		delay(1);

		set_motor_speed(MOTOR_TEST_SPEED, g_motor_1_ptr);
	}
	else if (command == TURN_CCW)
	{
		smooth_motor_slowdown(MOTOR_ZERO_SPEED, g_motor_1_ptr);

		digitalWrite(DIRECTION_PIN, LOW);
		delay(1);

		set_motor_speed(MOTOR_TEST_SPEED, g_motor_1_ptr);
	}
	else
	{
		/* Do nothing */
	}

	delay(1);
}

/* Arduino setup and loop */
void setup()
{
	Serial.begin(9600);
	Serial.println("Starting");

	motor_setup();

	/* Enable device */
	digitalWrite(ENABLE_PIN, HIGH);

	/* Set motor speed for test */
	g_motor_1.current_speed_reached = MOTOR_ZERO_SPEED;
	set_motor_speed(MOTOR_TEST_SPEED, g_motor_1_ptr);

	disable_brakes(&g_brakes_enabled_status);
}

void loop()
{
	g_command_received = check_serial_buffer();
	
	control_loop(g_command_received);
}
