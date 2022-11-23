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
	write_register(0x00, 0x01, 0x11);
	write_register(0x01, 0x00, 0x00);
	write_register(0x02, 0x04, 0xFF);
	write_register(0x03, 0x68, 0x00);
	write_register(0x04, 0x00, 0xD7);
	write_register(0x05, 0x00, 0x00);
	write_register(0x06, 0x00, 0x00);
	write_register(0x07, 0x00, 0x00);
	write_register(0x08, 0x00, 0x00);
	write_register(0x09, 0x00, 0x00);
	write_register(0x0A, 0xF0, 0x00);
	write_register(0x0B, 0x00, 0x00);
	write_register(0x2A, 0x00, 0x00);
}

/**
 * @brief To read the registers
 * 
 */
void read_registers() {
	//To Read the registers
	Serial.println("Lecture des registres");
	Serial.println(read_register(0x00), HEX);
	Serial.println(read_register(0x02), HEX);
	Serial.println(read_register(0x03), HEX);
	Serial.println(read_register(0x04), HEX);
	Serial.println(read_register(0x0A), HEX);
	Serial.println();
	Serial.println();
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