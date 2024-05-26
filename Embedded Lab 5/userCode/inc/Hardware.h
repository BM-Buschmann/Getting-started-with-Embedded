/**
 * @file Hardware.h
 * @brief Header file for hardware-related functionalities.
 *
 * This file contains enumeration and function declarations related to hardware interactions.
 *
 * @date    25.05.2024
 * @authors Bjoern Metzger & Daniel Korobow
 * @version 1.0
 */

#ifndef HARDWARE_H
#define HARDWARE_H

// Enumeration for joystick direction
typedef enum {
    JOYSTICK_UP,         /**< Joystick direction: Up */
    JOYSTICK_DOWN,       /**< Joystick direction: Down */
    JOYSTICK_LEFT,       /**< Joystick direction: Left */
    JOYSTICK_RIGHT,      /**< Joystick direction: Right */
    JOYSTICK_DEADZONE    /**< Joystick direction: Deadzone */
} JOYSTICK_DIRECTION;

/**
 * @brief Enumeration for joystick button state.
 */
typedef enum{
    JOYSTICK_PRESSED,    /**< Joystick button state: Pressed */
    JOYSTICK_RELEASED    /**< Joystick button state: Released */
} JOYSTICK_BUTTON;

/**
 * @brief Initializes the joystick hardware.
 * 
 * This function initializes the hardware components required for using the joystick.
 */
extern void initJoystick();

/**
 * @brief Gets the current direction of the joystick.
 * 
 * @return The current direction of the joystick (JOYSTICK_DIRECTION).
 * 
 * This function reads the joystick input and returns the current direction.
 */
extern JOYSTICK_DIRECTION getJoystickDirection();

/**
 * @brief Checks if the joystick button is pressed.
 * 
 * @return The state of the joystick button (JOYSTICK_BUTTON).
 * 
 * This function checks the state of the joystick button and returns whether
 * it is pressed or released.
 */
extern JOYSTICK_BUTTON isJoystickPressed();

#endif /* HARDWARE_H */
