/**
 * @file
 * motor.h
 * 
 * @copyright Copyright Heka (c) 2022
 */
#ifndef MOTOR_H_
#define MOTOR_H_

#include <Encoder.h>

/* DEFINES */
/* Positioning */
#define NO_NEW_POSITION_REQUIRED    0
#define NEW_POSITION_REQUIRED       1

/* Pins for motors */
#define PIN_RESET       2
#define PIN_ENABLE      3
#define PIN_DIRECTION   4
#define PIN_BRAKES      5
#define PIN_MOTOR       6
#define PIN_LOCK        7
#define PIN_FAULT       8

/* Pins for encoders */
#define PIN_1_ENCODER   0
#define PIN_2_ENCODER   1

/* Other constants */
#define IMPULSE_PER_TURN_RATIO  2
#define MOTOR_TEST_SPEED        200
#define MOTOR_ZERO_SPEED        100
#define NULL_ENCODER_POSITION   0

/* ENUMS */
/**
 * @brief Error state enum used as a code to define what type of error occured in the program
 */
enum Error_State {
    ERROR_NONE              = 0,
    ERROR_CURRENT_SPEED     = 1,
    ERROR_PREVIOUS_SPEED    = 2,
    ERROR_TURN_DIRECTION    = 3,
};

/**
 * @brief Command state enum used as a code to determine the current state of the motor
 */
enum Direction_State {
    DIRECTION_IDLE  = 0,
    DIRECTION_CW    = 1,
    DIRECTION_CCW   = 2,
};

/**
 * @brief Brake status enum used to indicate wether the brakes are enabled or disabled
 */
enum Brake_Status {
    BRAKES_DISABLED = 0,
    BRAKES_ENABLED  = 1,
};

/* STRUCTURES */
/**
 * @brief Contains all the useful characteristics of a Motor
 */
struct Motor {
    uint8_t motor_current_speed_reached;
    uint8_t motor_previous_speed_reached;
    Direction_State motor_turn_direction;
    Error_State motor_error_state;
    Encoder * motor_encoder;
    Brake_Status motor_brake_status;
};

/* FUNCTION PROTOTYPES */
void motor_setup();
void motor_disable_brakes(Motor * current_motor);
void motor_enable_brakes(Motor * current_motor);
void motor_move_to_required_position(int32_t desired_position, Motor * current_motor);
void motor_control_speed(int32_t desired_position, Motor * current_motor);

#endif /* MOTOR_H_ */