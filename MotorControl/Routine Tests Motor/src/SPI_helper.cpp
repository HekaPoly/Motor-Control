#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Encoder.h>
#include "SPI_helper.h"

/**
 * @brief set default values in driver registers
 * 
 */
void write_registers() 
{
	write_register(REG_INIT_SETTINGS, 0x01, 0x11);
	write_register(REG_COMMUTATION_TIMING, 0x00, 0x00);
	write_register(REG_LOCK_HALL_MIN_PERIOD, 0x04, 0xFF);
	write_register(REG_LOCK_SPEED_CHANGE_TOLERANCE, 0x68, 0x00);
	write_register(REG_DRIVER_COMPENSATION, 0x00, 0xD7);
	write_register(REG_SPEED_COMPENSATOR_GAIN, 0x00, 0x00);
	write_register(REG_FILTER_COEFF_POLE_FREQ, 0x00, 0x00);
	write_register(REG_FILTER_COEFF_ZERO_FREQ, 0x00, 0x00);
	write_register(REG_COMPENSATOR_COEFF_POLE_FREQ, 0x00, 0x00);
	write_register(REG_COMPENSATOR_COEFF_ZERO_FREQ, 0x00, 0x00);
	write_register(REG_PROTECTION_AND_SPEED_CONTROL_GAIN, 0xF0, 0x00);
	write_register(REG_OPEN_LOOP_GAIN, 0x00, 0x00);
	write_register(REG_FAULT_COLLECTION_UNIT, 0x00, 0x00);


/**
 * @brief To read the registers
 * 
 */
void read_registers() {
	//To Read the registers
	Serial.println("Lecture des registres");
	Serial.println(read_register(REG_INIT_SETTINGS), HEX);
	Serial.println(read_register(REG_LOCK_HALL_MIN_PERIOD), HEX);
	Serial.println(read_register(REG_LOCK_SPEED_CHANGE_TOLERANCE), HEX);
	Serial.println(read_register(REG_DRIVER_COMPENSATION), HEX);
	Serial.println(read_register(REG_PROTECTION_AND_SPEED_CONTROL_GAIN), HEX);
}

/**
 * @brief Cette fonction sert à setup le SPI et à le démarrer.
 */
void spi_setup() 
{
	/* SPI Setup */
	pinMode(CS, OUTPUT);
	SPI.begin();
	write_registers();
}

/**
 * @brief read value in a register
 * 
 * @param reg register
 * @return unsigned int 
 */
unsigned int read_register(byte reg)
{
	byte toRead = (READ | reg);

	SPI.beginTransaction(SPISettings(maxSpeed, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);
	/* send read command + reg adress */
	SPI.transfer(toRead);

	int result;
	byte readValue;

	/* get first 8 bits of data (Most Significant Byte) */
	readValue = SPI.transfer(0x00);
	result = (readValue << 8);

	/* get last 8 bits of data (Less Significant Byte) */
	readValue = SPI.transfer(0x00);

	/* combine into 16 bits result */
	result = (result | readValue);

	digitalWrite(CS, LOW);
	SPI.endTransaction();

	return (result);
}

/**
 * @brief La fonction écrit une valeur dans un registre.
 * 
 * @param reg Registre dans lequel on veut envoyer les byte 1 et 2 combinés.
 * @param value1 byte 1 (MSB)
 * @param value2 byte 2 (LSB)
 */
void write_register(byte reg, byte value1, byte value2) 
{
	byte toSend = (WRITE | reg);

	SPI.beginTransaction(SPISettings(maxSpeed, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);

	/* send write command + reg adress */
	SPI.transfer(toSend);

	/* send first 8 bits (MSB) */
	SPI.transfer(value1);
	/* send last 8 bits (LSB) */
	SPI.transfer(value2);

	digitalWrite(CS, LOW);
	SPI.endTransaction();
}