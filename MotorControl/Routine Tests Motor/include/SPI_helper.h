#include <Arduino.h>

//SPI communication
const byte READ = 0b10000000;
const byte WRITE = 0b00000000;
const byte faultReg = 0x2a;
const int maxSpeed = 480000; //Speed of the SPI (HZ)
const int CS = 10;

/* Définir les adresses des registres du DRV8308 */
#define REG_INIT_SETTINGS                           0x00
#define REG_COMMUTATION_TIMING                      0x01
#define REG_LOCK_HALL_MIN_PERIOD                    0x02
#define REG_LOCK_SPEED_CHANGE_TOLERANCE             0x03
#define REG_DRIVER_COMPENSATION                     0x04
#define REG_SPEED_COMPENSATOR_GAIN                  0x05
#define REG_FILTER_COEFF_POLE_FREQ                  0x06
#define REG_FILTER_COEFF_ZERO_FREQ                  0x07
#define REG_COMPENSATOR_COEFF_POLE_FREQ             0x08
#define REG_COMPENSATOR_COEFF_ZERO_FREQ             0x09
#define REG_PROTECTION_AND_SPEED_CONTROL_GAIN       0x0A
#define REG_OPEN_LOOP_GAIN                          0x0B
#define REG_FAULT_COLLECTION_UNIT                   0x2A


//Fonctions SPI
void write_register(byte reg, byte value1, byte value2);
void write_registers();
unsigned int read_register(byte reg);
void spi_setup();
void read_registers();
void write_default_registers();
void motor_setup();