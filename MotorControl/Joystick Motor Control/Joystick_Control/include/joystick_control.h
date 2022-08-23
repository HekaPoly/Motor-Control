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

const uint8_t VRX = 15;
const uint8_t VRY = 16;

uint32_t pos_x;
uint32_t pos_y;

uint8_t values_to_send[4];