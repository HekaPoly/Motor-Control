#ifndef MOTOR_COMMANDS_H_
#define MOTOR_COMMANDS_H_

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

#include <Encoder.h>

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

/* Constants for motor rotation */
const double GEAR_REDUCTION         = 1; //100 with gearbox, 1 without
const double ENCODER_RESOLUTION     = 4;
const double COUNTS_PER_TURN        = 800;
const double COUNT_TO_ANGLE_FACTOR  = 360 / (COUNTS_PER_TURN * GEAR_REDUCTION * ENCODER_RESOLUTION);
const int PULSE_MAX                 = 2047;
const int PULSE_MIN                 = 100;
const uint8_t MOTOR_TEST_SPEED      = 140;
const uint8_t MOTOR_ZERO_SPEED      = 100;

/* ENUMS */
enum Error_State {
    NO_ERROR                    = 0,
    CURRENT_SPEED_NOT_CORRECT   = 1,
};

enum Commands {
    IDLE_STATE  = 0,
    TURN_CW     = 1,
    TURN_CCW    = 2,
    BRAKE       = 3,
};

/* STRUCTURES */
struct Motor {
    uint8_t current_speed_reached;
    uint8_t previous_speed_reached;
    int error_state;
};

/* FUNCTION PROTOTYPES */
void motor_setup();
double read_angle(bool fullTurn);
void check_if_full_turn(double& angle);
void set_direction(bool direction);
void disable_brakes(bool *brakes_status);
void enable_brakes(bool *brakes_status);
void set_motor_speed(uint8_t desired_speed, Motor *current_motor);
void smooth_motor_slowdown(uint8_t actual_speed, Motor *current_motor);

#endif /* MOTOR_COMMANDS_H_ */