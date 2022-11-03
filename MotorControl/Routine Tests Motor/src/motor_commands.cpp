/**
 * @file
 * motor_commands.cpp
 * 
 * @author
 * Équipe Contrôle des moteurs - Heka
 * 
 * @brief
 * This file is meant to define functions to control a motor via the DRV8308 motor driver
 * 
 * @copyright Copyright Heka (c) 2022
 * 
 */

#include <Arduino.h>

#include "motor_commands.h"

/* Define encoder only in the .cpp */
Encoder encoder(PIN_2_ENCODER, PIN_1_ENCODER);

/**
 * @brief motor_setup -> setup des différentes pin du Arduino qui vont chacune contrôler une partie différente du moteur.
 * 
 * @param En passant le curseur sur les différentes PIN, on peut voir à quel numéro ils correspondent sur le Arduino.
 * 
 */

void motor_setup() {
	pinMode(RESET_PIN, OUTPUT);
	pinMode(ENABLE_PIN, OUTPUT);
	pinMode(DIRECTION_PIN, OUTPUT);
	pinMode(BRAKE_PIN, OUTPUT);
	pinMode(MOTOR_PIN, OUTPUT);
	pinMode(LOCK_PIN, INPUT);
	pinMode(FAULT_PIN, INPUT);

	analogWriteResolution(11);
	analogWriteFrequency(MOTOR_PIN, 23437.5); //Values between 0-2047

	digitalWrite(RESET_PIN, LOW);
	digitalWrite(ENABLE_PIN, HIGH);
	digitalWrite(DIRECTION_PIN, HIGH);
	digitalWrite(BRAKE_PIN, LOW);
}
/**
 * @brief "return_angle" = fonction
 * 
 * @param fullTurn : 
 * @return double -> précision (comme un float) va arrondir à 1 décimale après la virgule.
 * 
 */

double read_angle(bool fullTurn) {
	double angle = encoder.read() * COUNT_TO_ANGLE_FACTOR;

	return angle;
}
/**
 * @brief Set the direction object: Si la direction est (what) la DIRECTION_PIN sera activée ou pas. Cela va faire tourner le moteur dans un sens ou dans l'autre
 * 
 * @param direction : (je pense) = Clockwise ou Anticlockwise
 */

void set_direction(bool direction){
	if (direction)
	{
		digitalWrite(DIRECTION_PIN, HIGH);
	}
	else
	{
		digitalWrite(DIRECTION_PIN, LOW);
	}
}