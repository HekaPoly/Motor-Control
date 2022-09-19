/**
 * @file
 * main.cpp
 * 
 * @author
 * Équipe Contrôle des moteurs - Heka
 * 
 * @brief
 * This file is meant to define the control loop of the routine tests executed on the motors
 * 
 * @note
 * Pour utiliser : 
	- Brancher le moteur au board (PSU (power supply) + deux câbles allant vers le moteur)
	- Allumer PSU
	- Reset Teensy
 * 
 * @copyright Copyright Heka (c) 2022
 * 
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "SPI_helper.h"
#include "motor_commands.h"

unsigned long CURRENT_MILLIS;
uint32_t WAIT_TIME_MILLIS = 2000;

//Fonctions SPI
void writeRegister(byte reg, byte value1, byte value2);
void writeRegisters();
unsigned int readRegister(byte reg);
void SPISetup();
void readRegisters();
void writeDefaultRegisters();
void readSerial(double& angle);

int speed = 100;
bool brakes_enabled_status = true;
uint8_t command_received = TURN_CW;

void setup() 
{
	Serial.begin(9600);
	Serial.println("Starting");
  
	SPISetup();
	motor_setup();

	/* Enable device */
	digitalWrite(ENABLE_PIN, HIGH);

	/* Desactivate brakes */
	digitalWrite(BRAKE_PIN, LOW);
	brakes_enabled_status = false;

	/* Set motor speed for test */
	analogWrite(MOTOR_PIN, speed);
}

void disable_brakes(bool *current_brakes_status)
{
	if (*current_brakes_status == true)
	{
		digitalWrite(BRAKE_PIN, LOW);
		*current_brakes_status = false;
	}
}

inline void control_loop(uint8_t command)
{	
	if (command == TURN_CW)
	{
		disable_brakes(&brakes_enabled_status);
		digitalWrite(DIRECTION_PIN, HIGH);
	}
	else if (command == TURN_CCW)
	{
		disable_brakes(&brakes_enabled_status);
		digitalWrite(DIRECTION_PIN, LOW);
	}
	else if(command == BRAKE)
	{
		digitalWrite(BRAKE_PIN, HIGH);
		brakes_enabled_status = true;
	}
	else
	{
		/* Do nothing */
	}

	delay(1);
}

uint8_t check_serial_buffer()
{
	uint8_t res = IDLE_STATE;

	if (Serial.available() > 0)
	{
		int32_t command = Serial.read();

		if (command == 'A')
		{
			res = TURN_CW;
		}
		else if (command == 'B')
		{
			res = TURN_CCW;
		}
		else if (command == 'Z')
		{
			res = BRAKE;
		}
		else
		{
			/* Do nothing */
		}
	}

	/* Clear serial read buffer */
	while (Serial.available() > 0)
	{
		Serial.read();
	}

	return res;
}

void loop() 
{
	command_received = check_serial_buffer();
	
	control_loop(command_received);
}



void readFault() {
	if (digitalRead(FAULT_PIN) == HIGH) {
		Serial.println("Il y a une faute :(");
	}
	else
		Serial.print("Pas de faute");

	Serial.print("Le registre de faute est : ");
	Serial.println(readRegister(0x2A), BIN);
}

void writeDefaultRegisters() {
	//Recommended setup
	writeRegister(0x00, 0x01, 0x11);
	writeRegister(0x01,0x00,0x00);
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
