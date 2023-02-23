/**
 * @file
 * i2c_helper.h
 * 
 * @brief
 * Header file for I2C functions
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <Wire.h>

/* COMMANDS */
#define ADDRESS_MOTOR_1 0x1
#define COMMAND_SPEED 0x02
#define COMMAND_POSITION 0x01
#define COMMAND_BRAKE 0x04
#define SLAVE_COMMAND_VALUE_ENCODER 0x01
#define SLAVE_COMMAND_STATE 0x02

/* FUNCTION PROTOTYPES */
uint32_t create_message(uint8_t address, uint8_t command, uint16_t data);

