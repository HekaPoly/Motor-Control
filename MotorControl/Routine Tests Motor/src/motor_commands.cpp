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

/********** Declare static functions **********/
static void set_motor_speed(uint8_t desired_speed, Motor *current_motor);
static void smooth_motor_slowdown(uint8_t actual_speed, Motor *current_motor);
static void set_motor_direction(Direction_State desired_turn_direction, Motor *current_motor);
static int32_t read_encoder_position(Motor *current_motor);

/********** Function implementation **********/
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

/**
 * @brief Disables motor brakes if they are enabled
 * 
 * @param current_motor Motor that needs its brakes disabled
 */
void disable_brakes(Motor *current_motor)
{
	if (current_motor->motor_brake_status == BRAKES_ENABLED)
	{
		digitalWrite(BRAKE_PIN, LOW);
		current_motor->motor_brake_status = BRAKES_DISABLED;
		Serial.println("Brakes have been disabled");
	}
	else
	{
		/* Do nothing, brakes already disabled */
	}
}

/**
 * @brief Enables motor brakes if they are disabled
 * 
 * @param current_motor Motor that needs its brakes enabled
 */
void enable_brakes(Motor *current_motor)
{
	if (current_motor->motor_brake_status == BRAKES_DISABLED)
	{
		digitalWrite(BRAKE_PIN, HIGH);
		current_motor->motor_brake_status = BRAKES_ENABLED;
		Serial.println("Brakes have been enabled");
	}
	else
	{
		/* Do nothing, brakes already disabled */
	}
}

/**
 * @brief Deccelerates a motor to make it spin at a desired speed
 * 
 * @param desired_speed Desired speed to make the motor reach
 * @param current_motor Motor that needs to be deccelerated
 */
static void smooth_motor_slowdown(uint8_t desired_speed, Motor *current_motor)
{
	if ((current_motor->motor_current_speed_reached == MOTOR_TEST_SPEED) && (desired_speed < MOTOR_TEST_SPEED))
	{
		Serial.println("Start slowdown...");

		uint8_t amount_decreased = 0;
		uint8_t amount_to_decrease = MOTOR_TEST_SPEED - desired_speed;
		for (uint8_t i = 0; i < amount_to_decrease; i++)
		{
			Serial.println(MOTOR_TEST_SPEED - i);
			writeRegister(0x0B, 0x00, (MOTOR_TEST_SPEED - i));
			amount_decreased++;
			delay(1);
		}

		current_motor->motor_previous_speed_reached = current_motor->motor_current_speed_reached;
		current_motor->motor_current_speed_reached = MOTOR_TEST_SPEED - amount_decreased;
		Serial.println(MOTOR_TEST_SPEED - amount_decreased);
	}
	else
	{
		current_motor->motor_error_state = CURRENT_SPEED_ERROR;
		Serial.print("Error in slowdown, current speed is: ");
		Serial.println(current_motor->motor_current_speed_reached);
	}
}

/**
 * @brief Accelerates a motor to make it spin to a desired speed
 * 
 * @param desired_speed Desired speed to make the motor reach
 * @param current_motor Motor that needs to be accelerated
 */
static void set_motor_speed(uint8_t desired_speed, Motor *current_motor)
{
	if (current_motor->motor_current_speed_reached == MOTOR_ZERO_SPEED)
	{
		Serial.println("Start speed up...");

		uint8_t speed_reached = MOTOR_ZERO_SPEED;
		uint8_t amount_to_increase = desired_speed - MOTOR_ZERO_SPEED;

		for (uint8_t i = 0; i < amount_to_increase; i++)
		{
			Serial.println(MOTOR_ZERO_SPEED + i);
			writeRegister(REG_SPEED, 0x00, (MOTOR_ZERO_SPEED + i));
			speed_reached++;
			delay(1);
		}

		current_motor->motor_previous_speed_reached = current_motor->motor_current_speed_reached;
		current_motor->motor_current_speed_reached = speed_reached;
		Serial.println(speed_reached);
	}
	else
	{
		current_motor->motor_error_state = CURRENT_SPEED_ERROR;
		Serial.print("Error in acceleration, current speed is: ");
		Serial.println(current_motor->motor_current_speed_reached);
	}
}

/**
 * @brief Set the direction in which the motor is currently turning
 * 
 * @param desired_turn_direction The direction in which the motor needs to be spinned
 * @param current_motor Motor that needs its direction of spinning to change
 */
static void set_motor_direction(Direction_State desired_turn_direction, Motor *current_motor)
{
	if (desired_turn_direction == TURN_CW)
	{
		digitalWrite(DIRECTION_PIN, HIGH);
	}
	else if (desired_turn_direction == TURN_CCW)
	{
		digitalWrite(DIRECTION_PIN, LOW);
	}
	else
	{
		/* Do nothing here */
	}

	Serial.print("Turn direction is: ");
	Serial.println(current_motor->motor_turn_direction);

	delay(1);
	current_motor->motor_turn_direction = desired_turn_direction;
}

/**
 * @brief Reads the motor current encoder position
 * 
 * @param current_motor The motor from which to read its encoder value
 * @return int32_t The converted encoder value
 */
static int32_t read_encoder_position(Motor *current_motor)
{	
	int32_t current_position = (current_motor->motor_encoder->read() >> IMPULSE_PER_TURN_RATIO);
	//Serial.print("Current motor position: ");
	//Serial.println(current_position);

	return current_position;
}

/**
 * @brief Returns a motor to its zero position according to the encoder value
 * 
 * @param desired_position The desired position for the motor to reach
 * @param current_motor Motor that needs to be turned back to its zero position
 */
void move_motor_to_required_position(int32_t desired_position, Motor *current_motor)
{
	smooth_motor_slowdown(MOTOR_ZERO_SPEED, current_motor);

	int32_t current_encoder_position = read_encoder_position(current_motor);
	Serial.print("Desired position is: ");
	Serial.println(desired_position);

	/**
	 * If the motor is turning in clockwise direction, we assume encoder position is decreasing
	 * If the motor is turning in counter-clockwise direction, we assume encoder position is increasing
	 */
	if (current_encoder_position < desired_position) /* Need to increase encoder position */
	{
		set_motor_direction(TURN_CW, current_motor);
		set_motor_speed(MOTOR_TEST_SPEED, current_motor);

		while (current_encoder_position <= desired_position)
		{
			current_encoder_position = read_encoder_position(current_motor);
		}
	}
	else if (current_encoder_position > desired_position) /* Need to decrease encoder position */
	{
		set_motor_direction(TURN_CCW, current_motor);
		set_motor_speed(MOTOR_TEST_SPEED, current_motor);

		while (current_encoder_position >= desired_position)
		{
			current_encoder_position = read_encoder_position(current_motor);
		}
	}
	else
	{
		/* Do nothing here */
	}
	smooth_motor_slowdown(MOTOR_ZERO_SPEED, current_motor);

	Serial.print("Final encoder position is: ");
	Serial.println(read_encoder_position(current_motor));
}
