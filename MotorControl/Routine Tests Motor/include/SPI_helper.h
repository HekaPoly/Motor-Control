/**
 * @file
 * spi_helper.h
 * 
 * @copyright Heka (c) 2022
 */
#ifndef SPI_HELPER_H_
#define SPI_HELPER_H_

#include <Arduino.h>

/* DEFINES */
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
#define REG_SPEED                                   0x0B
#define REG_FAULT_COLLECTION_UNIT                   0x2A

/* CONSTANTS */
const byte SPI_READ = 0b10000000;
const byte SPI_WRITE = 0b00000000;
const uint32_t SPI_MAX_SPEED = 480000;
const uint8_t CS = 10;

/* FUNCTION PROTOTYPES */
void spi_write_register(byte reg, byte value1, byte value2);
void spi_setup(void);

#endif /* SPI_HELPER_H_ */