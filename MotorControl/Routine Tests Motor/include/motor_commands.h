/**
 * @file
 * motor_commands.h
 * 
 * @author
 * Équipe Contrôle des moteurs - Heka
 * 
 * @brief
 * This file is meant to define constants and function prototypes to control a motor 
 * via the DRV8308 motor driver
 * 
 * @copyright Copyright Heka (c) 2022
 * 
 */
#ifndef MOTOR_COMMANDS_H_
#define MOTOR_COMMANDS_H_

#include <Encoder.h>

/* DEFINES */
#define NO_NEW_POSITION_REQUIRED 0
#define NEW_POSITION_REQUIRED 1

/* CONSTANTS */
/* Pins for motors */
const int RESET_PIN     = 2;
const int ENABLE_PIN    = 3;
const int DIRECTION_PIN = 4;
const int BRAKE_PIN     = 5;
const int MOTOR_PIN     = 6;
const int LOCK_PIN      = 7;
const int FAULT_PIN     = 8;    

/* Pins for encoders */
const int PIN_1_ENCODER = 0; 
const int PIN_2_ENCODER = 1;

/* Other constants */
const uint32_t IMPULSE_PER_TURN_RATIO   = 2;
const uint8_t MOTOR_TEST_SPEED          = 130;
const uint8_t MOTOR_ZERO_SPEED          = 100;
const int32_t NULL_ENCODER_POSITION     = 0;

/* ENUMS */
/**
 * @brief Error state enum used as a code to define what type of error occured in the program
 */
enum Error_State {
    NO_ERROR                    = 0,
    CURRENT_SPEED_ERROR         = 1,
    PREVIOUS_SPEED_ERROR        = 2,
    TURN_DIRECTION_ERROR        = 3,
};

/**
 * @brief Command state enum used as a code to determine the current state of the motor
 */
enum Direction_State {
    IDLE_STATE          = 0,
    TURN_CW             = 1,
    TURN_CCW            = 2,
};

enum Brake_Status {
    BRAKES_DISABLED = 0,
    BRAKES_ENABLED  = 1,
};

/* STRUCTURES */
/**
 * @brief Contains multiple characteristics of a Motor
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
void disable_brakes(Motor *current_motor);
void enable_brakes(Motor *current_motor);
void move_motor_to_required_position(int32_t desired_position, Motor *current_motor);

#endif /* MOTOR_COMMANDS_H_ */