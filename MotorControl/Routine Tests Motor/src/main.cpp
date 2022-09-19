/*
Cree le 02/09/2019

Code pour tester si un moteur fonctionne.
Moteur va changer de direction a chaque 3 secs.
La lecture de l'encodeur ne fonctionne pas pendant l'exécution

Pour utiliser : 
- Brancher le moteur au board (ribbon cable + molex)
- Open PSU
- Reset Teensy

*/
#define ENCODER_OPTIMIZE_INTERRUPTS

#include <Arduino.h>
#include "SPI_helper.h"
#include <Wire.h>
#include <SPI.h>
#include <Encoder.h>


#pragma region "Prototypes"


//Fonctions qui fonctionnent :
inline double readAngle(bool fullTurn);
void updateAngle(double newAngle, double& angle);
void resetAngle();
void checkIfFullTurn(double& angle);

void readSerial(double& angle);

inline void setDirection(bool);
inline void toggleGoal();

#pragma endregion "Prototypes"

#pragma region "Variables et constantes" //{



//Moteur
const int resetPin = 2;
const int enablePin = 3;
const int directionPin = 4;
const int brakePin = 5;
const int motorPin = 6;
const int lockPin = 7;
const int faultPin = 8;
const int encoderPin1 = 0, encoderPin2 = 1;

//Lecture de l'encoder
const double gearReduction = 1; //100 with gearbox, 1 without
const double encoderResolution = 4;
const double countsPerTurn = 800;

//Controle du moteur
const int pulseMax = 2047;
const int pulseMin = 100;
const int turnCW = 1;
const int turnCCW = 2;
Encoder encoder(encoderPin2, encoderPin1);
bool state = true;

#pragma endregion //}

void setup() {
	Serial.begin(9600);
	Serial.println("Starting");
  
	SPISetup();
	motorSetup();
}

unsigned long prevTime = millis();
unsigned long timeInterval = 3000;  // Change direction every 3 sec
int turnTime = 350;
int speed = 100;

void motorSetup() {
	//Motor Setup
	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT); //Brake
	pinMode(6, OUTPUT); //Motor
	pinMode(7, INPUT);
	pinMode(8, INPUT);

	analogWriteResolution(11);
	analogWriteFrequency(motorPin, 23437.5); //Values between 0-2047

	digitalWrite(2, LOW); //RESET
	digitalWrite(3, HIGH); //Enable
	digitalWrite(4, HIGH); //DIR
	digitalWrite(5, LOW); //Brake
}

inline void timeControl(){
	unsigned long curTime = millis();

	if (curTime - prevTime > timeInterval){
		Serial.println("Changing direction");
		state = !state;
		digitalWrite(enablePin, LOW);
		digitalWrite(brakePin, HIGH);
		delay(10);

		if(state){
			setDirection(HIGH);
		}
		else{
			setDirection(LOW);
		}

		prevTime = curTime;
	}

	Serial.print("Angle : ");
	Serial.println(readAngle(false));

	digitalWrite(enablePin, HIGH);
	digitalWrite(brakePin, LOW);
	analogWrite(motorPin, speed);

	delay(10);
}

void loop() {
	timeControl();
}

#pragma region "Setup and SPI" //{



void readFault() {
	if (digitalRead(faultPin) == HIGH) {
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

#pragma endregion //}

#pragma region "Encoder" //{

double countToAngleFactor = 360 / (countsPerTurn*gearReduction*encoderResolution);

inline double readAngle(bool fullTurn=false) {
	double angle =  encoder.read()*countToAngleFactor ; //To get degrees

	return angle;
}

#pragma endregion //}

#pragma region "Motor" //{

inline void setDirection(bool direction){
	if (direction)
		digitalWrite(directionPin, HIGH);
	else
		digitalWrite(directionPin, LOW);
}

#pragma endregion //}
