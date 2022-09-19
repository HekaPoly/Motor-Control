#include <Arduino.h>

//SPI communication
const byte READ = 0b10000000;
const byte WRITE = 0b00000000;
const byte faultReg = 0x2a;
const int maxSpeed = 480000; //Speed of the SPI (HZ)
const int CS = 10;
const byte registersAdress[] = {0x00,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x2A };

//Fonctions SPI
void writeRegister(byte reg, byte value1, byte value2);
void writeRegisters();
unsigned int readRegister(byte reg);
void SPISetup();
void readRegisters();
void writeDefaultRegisters();
void motorSetup();