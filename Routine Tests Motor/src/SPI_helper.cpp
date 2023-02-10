/**
 * @file
 * spi_helper.cpp
 * 
 * @brief
 * This file is meant to define the functions helping the SPI communication the motor driver
 * 	board (DRV8308)
 * 
 * @copyright Heka (c) 2022
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Encoder.h>

#include "spi_helper.h"

/* STATIC FUNCTIONS DECLARATION */
static void spi_write_default_registers(void);

/* FUNCTION IMPLEMENTATION */
/**
 * @brief Set default values in motor driver DRV8308 registers
 */
static void spi_write_default_registers(void)
{
	spi_write_register(REG_INIT_SETTINGS, 						0x01, 0x21);
	spi_write_register(REG_COMMUTATION_TIMING, 					0x00, 0x00);
	spi_write_register(REG_LOCK_HALL_MIN_PERIOD,		 		0x04, 0xFF);

	//To test - LOCK signal takes less rev% to activate
	//spi_write_register(0x03, 0x68, 0x00);
	spi_write_register(REG_LOCK_SPEED_CHANGE_TOLERANCE, 		0x08, 0x00);

	spi_write_register(REG_DRIVER_COMPENSATION, 				0x00, 0xD7);
	spi_write_register(REG_SPEED_COMPENSATOR_GAIN, 				0x00, 0x00);
	spi_write_register(REG_FILTER_COEFF_POLE_FREQ, 				0x00, 0x00);
	spi_write_register(REG_FILTER_COEFF_ZERO_FREQ, 				0x00, 0x00);
	spi_write_register(REG_COMPENSATOR_COEFF_POLE_FREQ, 		0x00, 0x00);
	spi_write_register(REG_COMPENSATOR_COEFF_ZERO_FREQ, 		0x00, 0x00);
	spi_write_register(REG_PROTECTION_AND_SPEED_CONTROL_GAIN, 	0xF0, 0x00);
	spi_write_register(REG_SPEED,	 							0x00, 0x00);
	spi_write_register(REG_FAULT_COLLECTION_UNIT, 				0x00, 0x00);
}

/**
 * @brief Setups the SPI communication between the Teensy4.0 and the motor driver DRV8308
 */
void spi_setup(void) 
{
	pinMode(CS, OUTPUT);
	SPI.begin();

	spi_write_default_registers();
}

/**
 * @brief Writes a given 16 bit value in the desired register
 * 
 * @param reg 		Register to write the value
 * @param value1 	First byte (MSB)
 * @param value2 	Second byte (LSB)
 */
void spi_write_register(byte reg, byte value1, byte value2)
{
	byte reg_address = SPI_WRITE | reg;

	SPI.beginTransaction(SPISettings(SPI_MAX_SPEED, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);

	/* Send (in order): register address, MSB and LSB */
	SPI.transfer(reg_address);
	SPI.transfer(value1);
	SPI.transfer(value2);

	digitalWrite(CS, LOW);
	SPI.endTransaction();
}

uint32_t spi_read_register(byte reg, byte value1, byte value2)
{
	byte reg_address = SPI_READ | reg;

	SPI.beginTransaction(SPISettings(SPI_MAX_SPEED, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);
	SPI.transfer(reg_address);

 	//get first 8 bits of data (MSB)
 	byte readValue = SPI.transfer(0x00);
 	uint32_t result = readValue << 8;
 	//get last 8 bits of data (LSB)
 	readValue = SPI.transfer(0x00);
 	result = result | readValue;

 	digitalWrite(CS, LOW);
 	SPI.endTransaction();

 	return result;
}