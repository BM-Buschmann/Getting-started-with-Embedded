/**
 * @file    Hardware.c
 * @brief   Functions for joystick interactions.
 *
 * This file contains implementations of functions related to joystick interactions.
 * It includes functions to initialize the joystick, read ADC values from the joystick,
 * determine joystick direction, and check the joystick button state.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#include <stdint.h>
#include <math.h>

#include "../inc/TwoWire.h"
#include "../inc/Hardware.h"

#define SLAVE_ADDRESS 0b01001000

#define CONTROLL_BYTE_X 0b01000000
#define CONTROLL_BYTE_Y 0b01000001
#define CONTROLL_BYTE_Z 0b01000010

#define NUM_VALUES 3
#define DEADZONE_THRESHOLD 40 // Define deadzone threshold value

void initJoystick();
JOYSTICK_DIRECTION getJoystickDirection();
JOYSTICK_BUTTON isJoystickPressed();

/**
 * @brief Reads the ADC values from the joystick.
 * 
 * @param values Pointer to an array where the ADC values will be stored.
 * 
 * This function reads the ADC values from the joystick for each axis (X, Y, Z)
 * and stores them in the provided array.
 */
void getADCValues(uint8_t *values)
{
    values[0] = transferByte(SLAVE_ADDRESS, CONTROLL_BYTE_X);
    values[0] = transferByte(SLAVE_ADDRESS, CONTROLL_BYTE_X);
    values[1] = transferByte(SLAVE_ADDRESS, CONTROLL_BYTE_Y);
    values[1] = transferByte(SLAVE_ADDRESS, CONTROLL_BYTE_Y);
    values[2] = transferByte(SLAVE_ADDRESS, CONTROLL_BYTE_Z);
    values[2] = transferByte(SLAVE_ADDRESS, CONTROLL_BYTE_Z);
}

/**
 * @brief Initializes the joystick.
 * 
 * This function initializes the necessary hardware and modules to use the joystick.
 */
void initJoystick()
{
    initI2C();
}

/**
 * @brief Determines the direction of the joystick.
 * 
 * @return The direction of the joystick (JOYSTICK_DIRECTION).
 * 
 * This function reads the ADC values from the joystick and determines the direction
 * based on the differences between the X and Y axis values. It applies a deadzone
 * threshold to filter out noise.
 */
JOYSTICK_DIRECTION getJoystickDirection()
{
    uint8_t adcValues[NUM_VALUES];
    JOYSTICK_DIRECTION currentDirection = JOYSTICK_DEADZONE;

    getADCValues(adcValues);

    int32_t xDiff = adcValues[0] - 128;
    int32_t yDiff = adcValues[1] - 128;

    if (abs(xDiff) > abs(yDiff))
    {
        if (xDiff > DEADZONE_THRESHOLD)
        {
            currentDirection = JOYSTICK_LEFT;
        }
        else if (xDiff < -DEADZONE_THRESHOLD)
        {
            currentDirection = JOYSTICK_RIGHT;
        }
    }
    else
    {
        if (yDiff > DEADZONE_THRESHOLD)
        {
            currentDirection = JOYSTICK_DOWN;
        }
        else if (yDiff < -DEADZONE_THRESHOLD)
        {
            currentDirection = JOYSTICK_UP;
        }
    }

    return currentDirection;
}

/**
 * @brief Checks if the joystick button is pressed.
 * 
 * @return The state of the joystick button (JOYSTICK_BUTTON).
 * 
 * This function reads the ADC value from the joystick button and determines
 * if it is pressed or released based on a threshold value.
 */
JOYSTICK_BUTTON isJoystickPressed() {
    uint8_t adcValues[NUM_VALUES];
    getADCValues(adcValues);

    if (adcValues[2] < 125) {
        return JOYSTICK_RELEASED;
    } else {
        return JOYSTICK_PRESSED;
    }
}
