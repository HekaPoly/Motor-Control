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

#include "SPI_helper.h"
#include "motor_commands.h"

/* Define encoder only in the .cpp */
Encoder encoder(PIN_2_ENCODER, PIN_1_ENCODER);

void motor_setup() {
	SPISetup();

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

void disable_brakes(bool *brakes_status)
{
	if (*brakes_status == true)
	{
		digitalWrite(BRAKE_PIN, LOW);
		*brakes_status = false;
	}
	else
	{
		/* Do nothing, brakes already disabled */
	}
}

void enable_brakes(bool *brakes_status)
{
	if (*brakes_status == false)
	{
		digitalWrite(BRAKE_PIN, HIGH);
		*brakes_status = true;
	}
	else
	{
		/* Do nothing, brakes already enabled */
	}
}

/* TEST FUNCTION */
void smooth_motor_slowdown(uint8_t desired_speed, Motor *current_motor)
{
	if ((current_motor->current_speed_reached == MOTOR_TEST_SPEED) && (desired_speed < MOTOR_TEST_SPEED))
	{
		Serial.println("Start slowdon...");

		uint8_t amount_decreased = 0;
		uint8_t amount_to_decrease = MOTOR_TEST_SPEED - desired_speed;
		for (uint8_t i = 0; i < amount_to_decrease; i++)
		{
			Serial.println(MOTOR_TEST_SPEED - i);
			writeRegister(0x0B, 0x00, (MOTOR_TEST_SPEED - i));
			amount_decreased++;
			delay(1);
		}

		current_motor->previous_speed_reached = current_motor->current_speed_reached;
		current_motor->current_speed_reached = MOTOR_TEST_SPEED - amount_decreased;
		Serial.println(MOTOR_TEST_SPEED - amount_decreased);
	}
	else
	{
		current_motor->error_state = CURRENT_SPEED_NOT_CORRECT;
		Serial.print("Error in slowdown, current speed is: ");
		Serial.println(current_motor->current_speed_reached);
	}
}

void set_motor_speed(uint8_t desired_speed, Motor *current_motor)
{
	if (current_motor->current_speed_reached == MOTOR_ZERO_SPEED)
	{
		Serial.println("Start speed up...");

		uint8_t speed_reached = MOTOR_ZERO_SPEED;
		uint8_t amount_to_increase = desired_speed - MOTOR_ZERO_SPEED;

		for (uint8_t i = 0; i < amount_to_increase; i++)
		{
			Serial.println(MOTOR_ZERO_SPEED + i);
			writeRegister(0x0B, 0x00, (MOTOR_ZERO_SPEED + i));
			speed_reached++;
			delay(1);
		}

		current_motor->previous_speed_reached = current_motor->current_speed_reached;
		current_motor->current_speed_reached = speed_reached;
		Serial.println(speed_reached);
	}
	else
	{
		current_motor->error_state = CURRENT_SPEED_NOT_CORRECT;
		Serial.print("Error in acceleration, current speed is: ");
		Serial.println(current_motor->current_speed_reached);
	}
}
