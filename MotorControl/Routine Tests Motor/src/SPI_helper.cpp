#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Encoder.h>
#include "SPI_helper.h"

/**
 * @brief set default values in driver registers
 * 
 */
void writeRegisters() 
{
	writeRegister(0x00, 0x01, 0x11);
	writeRegister(0x01, 0x00, 0x00);
	writeRegister(0x02, 0x04, 0xFF);
	writeRegister(0x03, 0x68, 0x00);
	writeRegister(0x04, 0x00, 0xD7);
	writeRegister(0x05, 0x00, 0x00);
	writeRegister(0x06, 0x00, 0x00);
	writeRegister(0x07, 0x00, 0x00);
	writeRegister(0x08, 0x00, 0x00);
	writeRegister(0x09, 0x00, 0x00);
	writeRegister(0x0A, 0xF0, 0x00);
	writeRegister(0x0B, 0x00, 0x00);
	writeRegister(0x2A, 0x00, 0x00);
}

/**
 * @brief To read the registers
 * 
 */
void readRegisters() 
{
	Serial.println("Lecture des registres");
	Serial.println(readRegister(0x00), HEX);
	Serial.println(readRegister(0x02), HEX);
	Serial.println(readRegister(0x03), HEX);
	Serial.println(readRegister(0x04), HEX);
	Serial.println(readRegister(0x0A), HEX);
	Serial.println();
	Serial.println();
}

void SPISetup() {
	//SPI Setup
	pinMode(CS, OUTPUT);
	SPI.begin();
	writeRegisters();
	//writeDefaultRegisters();
}

/**
 * @brief read value in a register
 * 
 * @param reg register
 * @return unsigned int 
 */
unsigned int readRegister(byte reg) 
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

void writeRegister(byte reg, byte value1, byte value2) {

	byte toSend = WRITE | reg;

	SPI.beginTransaction(SPISettings(maxSpeed, MSBFIRST, SPI_MODE0));
	digitalWrite(CS, HIGH);
	delayMicroseconds(1);

	//send write command + reg adress
	SPI.transfer(toSend);

	//send first 8 bits (MSB)
	SPI.transfer(value1);
	//send last 8 bits (LSB)
	SPI.transfer(value2);

	digitalWrite(CS, LOW);
	SPI.endTransaction();
}