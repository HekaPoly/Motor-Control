/**
 * @file
 * motor.cpp
 * 
 * @brief
 * This file is meant to define functions to control a motor via the DRV8308 motor driver
 * 
 * @copyright Heka (c) 2022
 */

#include <Arduino.h>

#include "spi_helper.h"
#include "motor.h"

/* STATIC FUNCTIONS DECLARATION */
static void set_motor_speed(uint8_t desired_speed, Motor * current_motor);
static void smooth_motor_slowdown(uint8_t actual_speed, Motor * current_motor);
static void set_motor_direction(Direction_State desired_turn_direction, Motor * current_motor);
static int32_t read_encoder_position(Motor * current_motor);

/* FUNCTION IMPLEMENTATION */
/**
 * @brief Setups the Teensy4.0 pins to control appropriate motor driver pins
 */
void motor_setup(void) {
	spi_setup();

	pinMode(PIN_RESET, OUTPUT);
	pinMode(PIN_ENABLE, OUTPUT);
	pinMode(PIN_DIRECTION, OUTPUT);
	pinMode(PIN_BRAKES, OUTPUT);
	pinMode(PIN_MOTOR, OUTPUT);
	pinMode(PIN_LOCK, INPUT);
	pinMode(PIN_FAULT, INPUT);

	digitalWrite(PIN_RESET, LOW);
	digitalWrite(PIN_ENABLE, HIGH);
	digitalWrite(PIN_DIRECTION, HIGH);
	digitalWrite(PIN_BRAKES, LOW);

	analogWriteResolution(11);
	analogWriteFrequency(PIN_MOTOR, 23437.5);
}

/**
 * @brief Disables motor brakes if they are enabled
 * 
 * @param current_motor Motor that needs its brakes disabled
 */
void motor_disable_brakes(Motor * current_motor)
{
	if (current_motor->motor_brake_status == BRAKES_ENABLED)
	{
		digitalWrite(PIN_BRAKES, LOW);
		current_motor->motor_brake_status = BRAKES_DISABLED;
		Serial.println("Brakes have been disabled");
	}
}

/**
 * @brief Enables motor brakes if they are disabled
 * 
 * @param current_motor Motor that needs its brakes enabled
 */
void motor_enable_brakes(Motor * current_motor)
{
	if (current_motor->motor_brake_status == BRAKES_DISABLED)
	{
		digitalWrite(PIN_BRAKES, HIGH);
		current_motor->motor_brake_status = BRAKES_ENABLED;
		Serial.println("Brakes have been enabled");
	}
}

void motor_control_speed(int32_t desired_speed, Motor * current_motor)
{
	spi_write_register(REG_SPEED, 0x00, desired_speed);
}

/**
 * @brief Returns a motor to its zero position according to the encoder value
 * 
 * @param desired_position The desired position for the motor to reach
 * @param current_motor Motor that needs to be turned back to its zero position
 */
void motor_move_to_required_position(int32_t desired_position, Motor * current_motor)
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
		set_motor_direction(DIRECTION_CW, current_motor);
		set_motor_speed(MOTOR_TEST_SPEED, current_motor);

		while (current_encoder_position <= desired_position)
		{
			current_encoder_position = read_encoder_position(current_motor);
		}
	}
	else if (current_encoder_position > desired_position) /* Need to decrease encoder position */
	{
		set_motor_direction(DIRECTION_CCW, current_motor);
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

/**
 * @brief Deccelerates a motor to make it spin at a desired speed
 * 
 * @param desired_speed Desired speed of rotation of the motor
 * @param current_motor Motor that needs to be deccelerated
 */
static void smooth_motor_slowdown(uint8_t desired_speed, Motor * current_motor)
{
	if ((current_motor->motor_current_speed_reached == MOTOR_TEST_SPEED) && (desired_speed < MOTOR_TEST_SPEED))
	{
		Serial.println("Start slowdown...");

		spi_write_register(REG_SPEED, 0x00, MOTOR_ZERO_SPEED);
		/*
		uint8_t amount_decreased = 0;
		uint8_t amount_to_decrease = MOTOR_TEST_SPEED - desired_speed;
		for (uint8_t i = 0; i < amount_to_decrease; i++)
		{
			Serial.println(MOTOR_TEST_SPEED - i);
			spi_write_register(REG_SPEED, 0x00, (MOTOR_TEST_SPEED - i));
			amount_decreased++;
			delay(1);
		}
		*/

		current_motor->motor_previous_speed_reached = current_motor->motor_current_speed_reached;
		current_motor->motor_current_speed_reached = MOTOR_ZERO_SPEED;
		//Serial.println(MOTOR_TEST_SPEED - amount_decreased);
	}
	else
	{
		current_motor->motor_error_state = ERROR_CURRENT_SPEED;
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
static void set_motor_speed(uint8_t desired_speed, Motor * current_motor)
{
	if (current_motor->motor_current_speed_reached == MOTOR_ZERO_SPEED)
	{
		Serial.println("Start speed up...");

		uint8_t speed_reached = MOTOR_ZERO_SPEED;
		uint8_t amount_to_increase = desired_speed - MOTOR_ZERO_SPEED;

		for (uint8_t i = 0; i < amount_to_increase; i++)
		{
			Serial.println(MOTOR_ZERO_SPEED + i);
			spi_write_register(REG_SPEED, 0x00, (MOTOR_ZERO_SPEED + i));
			speed_reached++;
			delay(1);
		}

		current_motor->motor_previous_speed_reached = current_motor->motor_current_speed_reached;
		current_motor->motor_current_speed_reached = speed_reached;
		Serial.println(speed_reached);
	}
	else
	{
		current_motor->motor_error_state = ERROR_CURRENT_SPEED;
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
static void set_motor_direction(Direction_State desired_turn_direction, Motor * current_motor)
{
	if (desired_turn_direction == DIRECTION_CW)
	{
		digitalWrite(PIN_DIRECTION, HIGH);
	}
	else if (desired_turn_direction == DIRECTION_CCW)
	{
		digitalWrite(PIN_DIRECTION, LOW);
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
static int32_t read_encoder_position(Motor * current_motor)
{	
	int32_t current_position = (current_motor->motor_encoder->read() >> IMPULSE_PER_TURN_RATIO);
	//Serial.print("Current motor position: ");
	//Serial.println(current_position);

	return current_position;
}