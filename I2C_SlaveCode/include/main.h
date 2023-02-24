/**
 * @file
 * main.h
 * 
 * @brief
 * Header file for main file of the slave TeensyLC code
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>
#include <Wire.h>

/* CONSTANTS */
#define ADDRESS_MOTOR_1 0x01

#define COMMAND_POSITION 0x01
#define COMMAND_VELOCITY 0x02
#define COMMAND_BREAK 0x03
#define COMMAND_EN 0x04

#endif /* _MAIN_H_ */