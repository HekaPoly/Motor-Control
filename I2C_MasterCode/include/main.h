/**
 * @file
 * main.h
 * 
 * @brief
 * Header file for main file of the master TeensyLC code
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <Wire.h>

/* CONSTANTS */
#define ADDRESS_MOTOR_1 0x1
#define COMMAND_SPEED 0x02
#define COMMAND_POSITION 0x01
#define COMMAND_BRAKE 0x04
#define SLAVE_COMMAND_VALUE_ENCODER 0x01
#define SLAVE_COMMAND_STATE 0x02

#endif /* _MAIN_H_ */