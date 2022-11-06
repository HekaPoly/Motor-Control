/**
 * @file joystick_control.h
 * @author Équipe Motor Control - Heka
 * @brief 
 * @date 2022-07-16
 * 
 * @copyright Copyright Heka (c) 2022
 * 
 */

#include <stdint.h>
#include <Encoder.h>

#define NUMBER_OF_ENCODERS 4
#define NUMBER_OF_BYTES_TO_SEND 16

const uint8_t ENCODER_1_RED     = 2;
const uint8_t ENCODER_1_GREEN   = 3;
const uint8_t ENCODER_2_RED     = 4;
const uint8_t ENCODER_2_GREEN   = 5;
const uint8_t ENCODER_3_RED     = 6;
const uint8_t ENCODER_3_GREEN   = 7;
const uint8_t ENCODER_4_RED     = 8;
const uint8_t ENCODER_4_GREEN   = 9;

Encoder encoder_1(ENCODER_1_RED, ENCODER_1_GREEN);
Encoder encoder_2(ENCODER_2_RED, ENCODER_2_GREEN);
Encoder encoder_3(ENCODER_3_RED, ENCODER_3_GREEN);
Encoder encoder_4(ENCODER_4_RED, ENCODER_4_GREEN);

Encoder* encoders[NUMBER_OF_ENCODERS] = {
    &encoder_1,
    &encoder_2,
    &encoder_3,
    &encoder_4
};

uint32_t values_encoders[NUMBER_OF_ENCODERS];

uint8_t values_to_send[NUMBER_OF_BYTES_TO_SEND];